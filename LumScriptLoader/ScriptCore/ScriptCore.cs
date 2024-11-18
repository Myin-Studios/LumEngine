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
using Microsoft.CodeAnalysis;

public class SharedAssemblyLoadContext : AssemblyLoadContext
{
    private Dictionary<string, Assembly> _loadedAssemblies = new Dictionary<string, Assembly>();
    private string[] _sharedAssemblyNames = new[] { "LumScripting", "LumEngineWrapper" };

    public Dictionary<string, Assembly> GetLoadedAssemblies()
    {
        return _loadedAssemblies;
    }

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
        string scriptBinPath = Path.Combine(assemblyPath, "Build", "Debug");

        string scriptDllPath = Path.Combine(scriptBinPath, $"{projectName}.dll");

        if (!File.Exists(scriptDllPath))
        {
            Logger.Error($"Script DLL not found: {scriptDllPath}. Searching for Release build.");
            scriptBinPath = Path.Combine(assemblyPath, "Build", "Release");
        }

        if (sharedContext.GetLoadedAssemblies().ContainsKey(projectName))
        {
            Logger.Info($"Assembly {projectName} already loaded in shared context.");
            return;
        }
        if (AppDomain.CurrentDomain.GetAssemblies()
            .Any(a => a.FullName.Contains("LumScripting") && a.FullName.Contains("LumEngineWrapper")))
        {
            Logger.Info("LumScripting or LumEngineWrapper already loaded in current domain.");
            return;
        }

        try
        {
            var loadedAssemblies = sharedContext.Assemblies.ToList();
            Logger.Info("Assemblies already loaded in shared context:");
            foreach (var asm in loadedAssemblies)
            {
                Logger.Info($"- {asm.GetName().Name} v{asm.GetName().Version} from {asm.Location}");
            }

            // Carica lo script
            Assembly scriptAssembly = sharedContext.LoadFromAssemblyPath(scriptDllPath);

            // Verifica come lo script vede GameBehaviour
            var scriptTypes = scriptAssembly.GetTypes()
                .Where(t => t.GetInterfaces().Any(i => i.FullName == "LumScripting.Script.Internal.IScript") && !t.IsAbstract)
                .ToList();

            foreach (var type in scriptTypes)
            {
                try
                {
                    Logger.Info($"Creating instance of {type.FullName}");
                    object instance = Activator.CreateInstance(type);

                    // Otteniamo il riferimento a un'entity esistente
                    int entityCount = NativeEntityFactory.GetEntityCount();
                    if (entityCount > 0)
                    {
                        IntPtr nativeEntityPtr = NativeEntityFactory.GetExistingEntity(entityCount - 1); // O l'indice appropriato
                        if (nativeEntityPtr != IntPtr.Zero)
                        {
                            var entity = new Entity(nativeEntityPtr);
                            var wrapper = new ScriptWrapper(instance);
                            ((IEntityContainer)wrapper).SetEntityInstance(entity);
                            scripts.Add(wrapper);
                        }
                    }
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
            Logger.Error($"Failed to load script assembly: {ex.Message}");
            Logger.Error($"Stack trace: {ex.StackTrace}");
            if (ex.InnerException != null)
            {
                Logger.Error($"Inner exception: {ex.InnerException.Message}");
                Logger.Error($"Inner stack trace: {ex.InnerException.StackTrace}");
            }
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