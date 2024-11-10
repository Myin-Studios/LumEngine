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

    public void Load(string assemblyPath)
    {
        string projectName = Path.GetFileName(assemblyPath);
        string fullAssemblyPath = Path.Combine(assemblyPath, "bin", "Debug", "net8.0-windows10.0.26100.0", projectName + ".dll");
        Assembly? lumScriptingAssembly = AppDomain.CurrentDomain.GetAssemblies()
            .FirstOrDefault(a => a.FullName.StartsWith("LumScripting"));

        var loadContext = new AssemblyLoadContext("ScriptLoadContext", isCollectible: true);

        Assembly userAssembly;
        try
        {
            userAssembly = loadContext.LoadFromAssemblyPath(fullAssemblyPath);
        }
        catch (Exception ex)
        {
            Logger.Error($"Error loading user assembly: {ex.Message}");
            return;
        }

        Type scriptInterfaceType = lumScriptingAssembly.GetType("LumScripting.Script.Scripting.IScript");
        if (scriptInterfaceType == null)
        {
            Logger.Error("IScript type not found in LumScripting.");
            return;
        }

        var scripts = userAssembly.GetTypes()
            .Where(t => scriptInterfaceType.IsAssignableFrom(t) && !t.IsAbstract)
            .Select(t => Activator.CreateInstance(t))
            .OfType<IScript>();

        foreach (var script in scripts)
        {
            Logger.Succeed($"Found script: {script.GetType().FullName}");
            // this.scripts.Add(script);
        }

        // 
        // var scriptTypes = userAssembly.GetTypes()
        //     .Where(t => scriptInterfaceType.IsAssignableFrom(t) && !t.IsAbstract);
        // 
        // foreach (var scriptType in scriptTypes)
        // {
        //     Logger.Succeed($"Found script: {scriptType.FullName}");
        // 
        //     var scriptInstance = Activator.CreateInstance(scriptType);
        //     if (scriptInstance is IScript validScript)
        //     {
        //         scripts.Add(validScript);
        //         Logger.Succeed($"Added script: {validScript.GetType().Name}");
        //     }
        //     else
        //     {
        //         Logger.Error($"Failed to cast {scriptType.FullName} to IScript.");
        //     }
        // }
        // 
        // if (!scriptTypes.Any())
        // {
        //     Logger.Warning("No scripts implementing IScript were found.");
        // }
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