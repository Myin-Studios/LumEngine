using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;

using LumScripting.Script.Scripting;
using LumScripting.Script.Attributes;

using LumScripting.Script.Log;
using Windows.ApplicationModel.Core;

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
    private List<IScript> scripts = new List<IScript>();
    private AssemblyLoadContext loadContext;

    public ScriptManager()
    {
        // Crea un nuovo context con un resolver personalizzato
        loadContext = new AssemblyLoadContext("ScriptLoadContext", isCollectible: true);
        loadContext.Resolving += AssemblyResolving;
    }

    private Assembly AssemblyResolving(AssemblyLoadContext context, AssemblyName assemblyName)
    {
        // Se viene richiesto LumScripting, carica quello dell'engine
        if (assemblyName.Name == "LumScripting")
        {
            string engineLumScriptingPath = Path.Combine("LumScripting", "LumScripting.dll");
            Logger.Debug($"Redirecting LumScripting load to: {engineLumScriptingPath}");
            return context.LoadFromAssemblyPath(Path.GetFullPath(engineLumScriptingPath));
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
            Logger.Debug($"Loading assembly from: {fullAssemblyPath}");

            // Carica prima LumScripting per essere sicuri che sia disponibile
            string engineLumScriptingPath = Path.Combine("LumScripting", "LumScripting.dll");
            Logger.Debug($"Loading LumScripting from: {Path.GetFullPath(engineLumScriptingPath)}");

            Assembly lumScriptingAssembly = loadContext.LoadFromAssemblyPath(Path.GetFullPath(engineLumScriptingPath));
            Logger.Debug($"Loaded LumScripting assembly: {lumScriptingAssembly.FullName}");

            Type scriptInterfaceType = lumScriptingAssembly.GetType("LumScripting.Script.Scripting.IScript");
            if (scriptInterfaceType == null)
            {
                Logger.Error("IScript type not found in LumScripting.");
                return;
            }
            Logger.Debug($"Found IScript type: {scriptInterfaceType.FullName}");

            // Ora carica l'assembly utente
            Assembly userAssembly = loadContext.LoadFromAssemblyPath(fullAssemblyPath);
            Logger.Debug($"Loaded assembly: {userAssembly.FullName}");

            // Log dei tipi disponibili nell'assembly utente
            Logger.Debug($"Types in user assembly:");
            foreach (var t in userAssembly.GetTypes())
            {
                Logger.Debug($" - {t.FullName}");
                foreach (var iface in t.GetInterfaces())
                {
                    Logger.Debug($"   Implements: {iface.FullName} from {iface.Assembly.Location}");
                }
            }

            var scriptTypes = userAssembly.GetTypes()
                .Where(t => scriptInterfaceType.IsAssignableFrom(t) && !t.IsAbstract)
                .ToList();

            Logger.Debug($"Found {scriptTypes.Count} script types");

            foreach (var type in scriptTypes)
            {
                try
                {
                    Logger.Debug($"Creating instance of: {type.FullName}");
                    var instance = Activator.CreateInstance(type);
                    Logger.Debug($"Instance created, attempting cast to IScript");

                    // Usa reflection per il cast invece del cast diretto
                    bool isScript = scriptInterfaceType.IsInstanceOfType(instance);
                    if (isScript)
                    {
                        Logger.Succeed($"Found script: {type.FullName}");
                        // Usa reflection per creare un delegate che chiama i metodi dell'interfaccia
                        dynamic script = instance;
                        scripts.Add((IScript)script);
                        Logger.Debug($"Script added to list. Current script count: {scripts.Count}");
                    }
                    else
                    {
                        Logger.Error($"Instance is not of type IScript: {type.FullName}");
                    }
                }
                catch (Exception ex)
                {
                    Logger.Error($"Failed to create instance of {type.FullName}: {ex.Message}");
                    Logger.Error($"Stack trace: {ex.StackTrace}");
                }
            }

            Logger.Debug($"Total scripts loaded: {scripts.Count}");
            foreach (var script in scripts)
            {
                Logger.Debug($"Loaded script type: {script.GetType().FullName}");
            }
        }
        catch (Exception ex)
        {
            Logger.Error($"Failed to load assembly {projectName}: {ex.Message}");
            Logger.Error($"Stack trace: {ex.StackTrace}");
            return;
        }
    }

    public void Start()
    {
        foreach (var script in scripts)
        {
            script.onStart();
        }

        Console.WriteLine(scripts.Count);
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