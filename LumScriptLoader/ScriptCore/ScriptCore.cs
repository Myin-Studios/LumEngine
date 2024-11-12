using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;

using LumScripting.Script.Scripting;
using LumScripting.Script.Attributes;

using LumScripting.Script.Log;
using Windows.ApplicationModel.Core;
using LumScripting.Script.Entities;

public class ScriptCompiler
{
    /*
    public static string CompileScriptsToDll(string scriptDirectory, string outputDllPath)
    {
        List<SyntaxTree> syntaxTrees = new List<SyntaxTree>();

        // Cerca tutti i file .cs nelle cartelle e sottocartelle
        var scriptFiles = Directory.GetFiles(scriptDirectory, "*.cs", SearchOption.AllDirectories);

        foreach (var scriptFile in scriptFiles)
        {
            string scriptCode = File.ReadAllText(scriptFile);
            SyntaxTree syntaxTree = CSharpSyntaxTree.ParseText(scriptCode);
            syntaxTrees.Add(syntaxTree);
        }

        var references = AppDomain.CurrentDomain.GetAssemblies()
            .Where(a => !a.IsDynamic)
            .Select(a => MetadataReference.CreateFromFile(a.Location))
            .Cast<MetadataReference>()
            .ToList();

        // Compilazione in un'assembly DLL
        CSharpCompilation compilation = CSharpCompilation.Create(
            Path.GetFileNameWithoutExtension(outputDllPath), // Nome dell'assembly
            syntaxTrees,
            references,
            new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary) // DLL come output
        );

        EmitResult result;
        using (var fs = new FileStream(outputDllPath, FileMode.Create, FileAccess.Write))
        {
            result = compilation.Emit(fs); // Scrive l'output della compilazione in una DLL
        }

        if (!result.Success)
        {
            foreach (var diagnostic in result.Diagnostics)
            {
                Console.WriteLine($"Error: {diagnostic.GetMessage()}");
            }
            return null; // Restituisce null se la compilazione fallisce
        }
        else
        {
            Console.WriteLine($"DLL salvata correttamente in: {outputDllPath}");
            return outputDllPath; // Restituisce il percorso della DLL se la compilazione ha successo
        }
    }
    */
}

public class ScriptManager
{
    private List<IScript> scripts;
    private AssemblyLoadContext loadContext;

    public ScriptManager()
    {
        scripts = new List<IScript>();

        // Crea un nuovo AssemblyLoadContext con il nostro handler per il resolving
        loadContext = new AssemblyLoadContext("ScriptLoader");
        loadContext.Resolving += AssemblyResolving;
    }

    // MODIFICATO: Aggiunto supporto per LumEngineWrapper
    private Assembly AssemblyResolving(AssemblyLoadContext context, AssemblyName assemblyName)
    {
        // Gestisce sia LumScripting che LumEngineWrapper
        if (assemblyName.Name == "LumScripting" || assemblyName.Name == "LumEngineWrapper")
        {
            string engineAssemblyPath = Path.Combine("LumScripting", $"{assemblyName.Name}.dll");
            Logger.Debug($"Redirecting {assemblyName.Name} load to: {engineAssemblyPath}");
            return context.LoadFromAssemblyPath(Path.GetFullPath(engineAssemblyPath));
        }
        return null;
    }

    public void Load(string assemblyPath)
    {
        string projectName = Path.GetFileName(assemblyPath);
        string fullAssemblyPath = Path.Combine(assemblyPath, "bin", "Debug", "net8.0-windows10.0.26100.0", projectName + ".dll");

        if (!File.Exists(fullAssemblyPath))
        {
            Logger.Error($"Assembly path: {fullAssemblyPath} doesn't exist!");
            return;
        }

        try
        {
            // MODIFICATO: Carica entrambe le dipendenze dell'engine
            string engineLumScriptingPath = Path.Combine("LumScripting", "LumScripting.dll");
            string engineWrapperPath = Path.Combine("LumScripting", "LumEngineWrapper.dll");

            // Carica LumScripting
            Logger.Debug($"Loading LumScripting from: {Path.GetFullPath(engineLumScriptingPath)}");
            Assembly lumScriptingAssembly = loadContext.LoadFromAssemblyPath(Path.GetFullPath(engineLumScriptingPath));
            Logger.Debug($"Loaded LumScripting assembly: {lumScriptingAssembly.FullName}");

            // NUOVO: Carica LumEngineWrapper
            Logger.Debug($"Loading LumEngineWrapper from: {Path.GetFullPath(engineWrapperPath)}");
            Assembly wrapperAssembly = loadContext.LoadFromAssemblyPath(Path.GetFullPath(engineWrapperPath));
            Logger.Debug($"Loaded LumEngineWrapper assembly: {wrapperAssembly.FullName}");

            Type scriptInterfaceType = lumScriptingAssembly.GetType("LumScripting.Script.Scripting.IScript");
            if (scriptInterfaceType == null)
            {
                Logger.Error("IScript type not found in LumScripting.");
                return;
            }
            Logger.Debug($"Found IScript type: {scriptInterfaceType.FullName}");

            Logger.Debug("Loaded assemblies before user assembly loading:");
            var assemblies = AppDomain.CurrentDomain.GetAssemblies();
            foreach (Assembly assembly in assemblies)
            {
                Logger.Succeed($"{assembly.FullName}");
            }

            // Carica l'assembly utente
            Assembly userAssembly = loadContext.LoadFromAssemblyPath(fullAssemblyPath);
            Logger.Debug($"Loaded assembly: {userAssembly.FullName}");

            Logger.Debug("Loaded assemblies after user assembly loading:");
            var ass = AppDomain.CurrentDomain.GetAssemblies();
            foreach (Assembly assembly in ass)
            {
                Logger.Succeed($"{assembly.FullName}");
            }

            var scriptTypes = userAssembly.GetTypes()
                .Where(t => t.GetInterfaces().Any(i => i.FullName == "LumScripting.Script.Scripting.IScript") && !t.IsAbstract)
                .ToList();

            Logger.Debug($"Found {scriptTypes.Count} script types");

            foreach (var type in scriptTypes)
            {
                try
                {
                    Logger.Debug($"Creating instance of: {type.FullName}");
                    object instance = Activator.CreateInstance(type);
                    Logger.Debug($"Instance created");

                    // Verifica che i metodi dell'interfaccia esistano
                    var startMethod = type.GetMethod("onStart");
                    var runMethod = type.GetMethod("onRun");

                    if (startMethod != null && runMethod != null)
                    {
                        // Crea un wrapper che implementa IScript
                        var wrapper = new ScriptWrapper(instance);

                        // Chiama SetEntity direttamente sull'istanza
                        var setEntityMethod = type.GetMethod("SetEntity", new[] { typeof(LumScripting.Script.Entities.Entity) });
                        setEntityMethod?.Invoke(wrapper, new object[] { new Entity() });

                        scripts.Add(wrapper);
                        Logger.Succeed($"Successfully loaded script: {type.FullName}");
                        Logger.Debug($"Script added to list. Current script count: {scripts.Count}");
                    }
                    else
                    {
                        Logger.Error($"Type {type.FullName} is missing required methods");
                    }
                }
                catch (Exception ex)
                {
                    Logger.Error($"Failed to create instance of {type.FullName}: {ex.Message}");
                    Logger.Error($"Stack trace: {ex.StackTrace}");
                }
            }

            Logger.Debug($"Total scripts loaded: {scripts.Count}");
        }
        catch (Exception ex)
        {
            Logger.Error($"Failed to load assembly {projectName}: {ex.Message}");
            Logger.Error($"Stack trace: {ex.StackTrace}");
        }
    }

    public void Unload()
    {
        scripts.Clear();
        loadContext.Unload();
    }

    public List<IScript> GetScripts()
    {
        return scripts;
    }

    public class ScriptWrapper : IScript
    {
        private readonly object _instance;
        private readonly MethodInfo _startMethod;
        private readonly MethodInfo _runMethod;
        private readonly MethodInfo _setEntityMethod;

        public ScriptWrapper(object instance)
        {
            _instance = instance;
            var type = instance.GetType();
            _startMethod = type.GetMethod("onStart");
            _runMethod = type.GetMethod("onRun");
            _setEntityMethod = type.GetMethod("SetEntity", new[] { typeof(LumScripting.Script.Entities.Entity) });
        }

        public void onStart()
        {
            try
            {
                Console.WriteLine($"Starting invoke of {_startMethod} on {_instance}");
                Console.WriteLine($"Method declaring type: {_startMethod?.DeclaringType}");
                Console.WriteLine($"Instance type: {_instance?.GetType()}");
                _startMethod?.Invoke(_instance, null);
                Console.WriteLine("Invoke completed successfully");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception during invoke: {ex.Message}");
                Console.WriteLine($"Stack: {ex.StackTrace}");
                if (ex.InnerException != null)
                {
                    Console.WriteLine($"Inner: {ex.InnerException.Message}");
                    Console.WriteLine($"Inner Stack: {ex.InnerException.StackTrace}");
                }
                throw;
            }
        }
        
        public void onRun()
        {
            _runMethod?.Invoke(_instance, null);
        }

        public void SetEntity(Entity entity)
        {
            _setEntityMethod?.Invoke(_instance, new object[] { entity });
        }
    }

    public void Start()
    {
        foreach (var script in scripts)
        {
            script.onStart();
        }
    }

    public void Run()
    {
        foreach (var script in scripts)
        {
            script.onRun();
        }
    }
}

public class GameCore
{
    private ScriptManager scriptManager = new ScriptManager();

    public void Initialize(string assemblyPath)
    {
        scriptManager.Load(assemblyPath);
    }

    public void Start()
    {
        // Carica e avvia tutti gli script
        scriptManager.Start();
    }

    public void Update()
    {
        // Chiama onRun su tutti gli script per ogni frame
        scriptManager.Run();
    }
}