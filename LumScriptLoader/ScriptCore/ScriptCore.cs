using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;

using LumScripting.Script.Internal;
using LumScripting.Script.Scripting;
using LumScripting.Script.Attributes;

using LumScripting.Script.Log;
using Windows.ApplicationModel.Core;
using LumScripting.Script.Entities;
using Microsoft.CodeAnalysis.Scripting;

public class SharedAssemblyLoadContext : AssemblyLoadContext
{
    private Dictionary<string, Assembly> _loadedAssemblies = new Dictionary<string, Assembly>();
    private string[] _sharedAssemblyNames = new[] { "LumScripting", "LumEngineWrapper" };

    public SharedAssemblyLoadContext() : base("SharedLoader", isCollectible: false)
    {
        // Carica gli assembly condivisi
        foreach (var name in _sharedAssemblyNames)
        {
            string path = Path.Combine("LumScripting", $"{name}.dll");
            var assembly = LoadFromAssemblyPath(Path.GetFullPath(path));
            _loadedAssemblies[name] = assembly;
        }
    }

    protected override Assembly Load(AssemblyName assemblyName)
    {
        // Se è uno degli assembly condivisi, restituisci quello già caricato
        if (_loadedAssemblies.TryGetValue(assemblyName.Name, out var assembly))
        {
            return assembly;
        }

        // Per tutti gli altri, cerca tra gli assembly già caricati nel dominio
        return null;
    }
}

public class ScriptManager
{
    private List<IScript> scripts;
    private SharedAssemblyLoadContext sharedContext;
    private AssemblyLoadContext scriptContext;

    public ScriptManager()
    {
        scripts = new List<IScript>();

        // Crea il contesto condiviso
        sharedContext = new SharedAssemblyLoadContext();

        // Crea il contesto per gli script utente
        scriptContext = new AssemblyLoadContext("ScriptLoader", isCollectible: true);
        scriptContext.Resolving += ScriptAssemblyResolving;
    }

    private Assembly ScriptAssemblyResolving(AssemblyLoadContext context, AssemblyName assemblyName)
    {
        // Per LumScripting e LumEngineWrapper, usa le versioni dal contesto condiviso
        if (assemblyName.Name == "LumScripting" || assemblyName.Name == "LumEngineWrapper")
        {
            return sharedContext.Assemblies.First(a => a.GetName().Name == assemblyName.Name);
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
            // Usa il contesto condiviso per ottenere il tipo IScript
            var lumScriptingAssembly = sharedContext.Assemblies.First(a => a.GetName().Name == "LumScripting");
            Type scriptInterfaceType = lumScriptingAssembly.GetType("LumScripting.Script.Internal.IScript");

            if (scriptInterfaceType == null)
            {
                Logger.Error("IScript type not found in LumScripting.");
                return;
            }

            // Carica l'assembly utente nel contesto degli script
            Assembly userAssembly = scriptContext.LoadFromAssemblyPath(fullAssemblyPath);

            // Verifica assemblies duplicati
            var loadedAssemblies = AppDomain.CurrentDomain.GetAssemblies()
                .GroupBy(a => a.GetName().Name)
                .Where(g => g.Count() > 1)
                .ToDictionary(g => g.Key, g => g.Count());

            if (loadedAssemblies.Any())
            {
                Logger.Warning("Duplicate assemblies detected:");
                foreach (var kvp in loadedAssemblies)
                {
                    Logger.Warning($"{kvp.Key}: loaded {kvp.Value} times");
                }
            }

            var scriptTypes = userAssembly.GetTypes()
                .Where(t => t.GetInterfaces().Any(i => i.FullName == "LumScripting.Script.Internal.IScript") && !t.IsAbstract)
                .ToList();

            foreach (var type in scriptTypes)
            {
                try
                {
                    object instance = Activator.CreateInstance(type);
                    var wrapper = new ScriptWrapper(instance);

                    // Usa il contesto condiviso per ottenere il tipo Entity
                    var wrapperAssembly = sharedContext.Assemblies.First(a => a.GetName().Name == "LumEngineWrapper");
                    var entityType = wrapperAssembly.GetType("LumScripting.Script.Entities.Entity");
                    var entity = Activator.CreateInstance(entityType);

                    ((IEntityContainer)wrapper).SetEntityInstance(entity);
                    scripts.Add(wrapper);
                }
                catch (Exception ex)
                {
                    Logger.Error($"Failed to create instance of {type.FullName}: {ex.Message}");
                    Logger.Error($"Stack trace: {ex.StackTrace}");
                }
            }
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
    }

    public List<IScript> GetScripts()
    {
        return scripts;
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