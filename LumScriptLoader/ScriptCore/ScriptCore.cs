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
using System.Diagnostics;

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

    public ScriptManager()
    {
        scripts = new List<IScript>();
        sharedContext = new SharedAssemblyLoadContext();
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
            byte[] assemblyBytes = File.ReadAllBytes(fullAssemblyPath);
            Assembly userAssembly = sharedContext.LoadFromStream(new MemoryStream(assemblyBytes));

            // Verifica che l'assembly sia caricato correttamente
            Logger.Info($"Assembly loaded: {userAssembly.FullName}");

            var allTypes = userAssembly.GetTypes();
            Logger.Info($"Total types found: {allTypes.Length}");

            var scriptTypes = allTypes
                .Where(t => {
                    try
                    {
                        return t.GetInterfaces().Any(i => i.FullName == "LumScripting.Script.Internal.IScript") && !t.IsAbstract;
                    }
                    catch (Exception ex)
                    {
                        Logger.Error($"Error checking type {t.FullName}: {ex.Message}");
                        return false;
                    }
                })
                .ToList();

            Logger.Info($"Script types found: {scriptTypes.Count}");

            foreach (var type in scriptTypes)
            {
                try
                {
                    Logger.Info($"Creating instance of {type.FullName}");
                    object instance = Activator.CreateInstance(type);

                    Logger.Info("Creating wrapper");
                    var wrapper = new ScriptWrapper(instance);

                    Logger.Info("Creating entity");
                    var entity = new Entity();

                    Logger.Info("Setting entity instance");
                    ((IEntityContainer)wrapper).SetEntityInstance(entity);

                    Logger.Info("Adding script to list");
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

    public void Run()
    {
        foreach (var script in scripts)
        {
            try
            {
                script.onRun();
            }
            catch (Exception ex)
            {
                // Log dettagliato dell'eccezione
                Logger.Error($"Exception in script execution: {ex.GetType().FullName}");
                Logger.Error($"Message: {ex.Message}");
                Logger.Error($"Stack trace: {ex.StackTrace}");

                // Se c'è un'inner exception, logga anche quella
                if (ex.InnerException != null)
                {
                    Logger.Error($"Inner exception: {ex.InnerException.Message}");
                    Logger.Error($"Inner stack trace: {ex.InnerException.StackTrace}");
                }
            }
        }
    }

    public void Start()
    {
        foreach (var script in scripts)
        {
            try
            {
                script.onStart();
            }
            catch (Exception ex)
            {
                // Log dettagliato dell'eccezione
                Logger.Error($"Exception in script start: {ex.GetType().FullName}");
                Logger.Error($"Message: {ex.Message}");
                Logger.Error($"Stack trace: {ex.StackTrace}");

                if (ex.InnerException != null)
                {
                    Logger.Error($"Inner exception: {ex.InnerException.Message}");
                    Logger.Error($"Inner stack trace: {ex.InnerException.StackTrace}");
                }
            }
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