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

public class ScriptManager
{
    private List<IScript> scripts;
    private AssemblyLoadContext loadContext;
    private Assembly wrapperAssembly; // Aggiungi questo campo

    public ScriptManager()
    {
        scripts = new List<IScript>();

        // Prima carica LumEngineWrapper globalmente
        string engineWrapperPath = Path.Combine("LumScripting", "LumEngineWrapper.dll");
        wrapperAssembly = Assembly.LoadFrom(Path.GetFullPath(engineWrapperPath));
        Logger.Debug($"Loaded LumEngineWrapper globally: {wrapperAssembly.FullName}");

        // Poi crea l'AssemblyLoadContext
        loadContext = new AssemblyLoadContext("ScriptLoader");
        loadContext.Resolving += AssemblyResolving;
    }

    private Assembly AssemblyResolving(AssemblyLoadContext context, AssemblyName assemblyName)
    {
        if (assemblyName.Name == "LumEngineWrapper")
        {
            // Ritorna l'assembly già caricato globalmente
            return wrapperAssembly;
        }
        if (assemblyName.Name == "LumScripting")
        {
            string engineAssemblyPath = Path.Combine("LumScripting", "LumScripting.dll");
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

            Type scriptInterfaceType = lumScriptingAssembly.GetType("LumScripting.Script.Internal.IScript");
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
                .Where(t => t.GetInterfaces().Any(i => i.FullName == "LumScripting.Script.Internal.IScript") && !t.IsAbstract)
                .ToList();

            Logger.Debug($"Found {scriptTypes.Count} script types");

            foreach (var type in scriptTypes)
            {
                try
                {
                    Logger.Debug($"Creating instance of: {type.FullName}");

                    // Aggiungi più informazioni di debug
                    Logger.Debug($"Base type: {type.BaseType?.FullName}");
                    Logger.Debug($"Assembly: {type.Assembly.FullName}");

                    object instance = Activator.CreateInstance(type);
                    Logger.Debug($"Instance created successfully");

                    var wrapper = new ScriptWrapper(instance);

                    // Crea e imposta l'Entity
                    // Usa reflection per chiamare InitializeEntity
                    var entityType = wrapperAssembly.GetType("LumScripting.Script.Entities.Entity");
                    Logger.Debug($"Entity type found: {entityType != null}");

                    var entity = Activator.CreateInstance(entityType);
                    Logger.Debug($"Entity instance created");

                    // Ottieni il tipo di ScriptWrapper dall'assembly corretto
                    var scriptWrapperType = wrapper.GetType();
                    var initializeEntityMethod = scriptWrapperType.GetMethod("InitializeEntity");

                    // Assicurati che il parametro sia del tipo corretto
                    var parameterType = initializeEntityMethod.GetParameters()[0].ParameterType;
                    var convertedEntity = Convert.ChangeType(entity, parameterType);

                    initializeEntityMethod.Invoke(wrapper, new[] { convertedEntity });
                    Logger.Debug($"Entity initialized");

                    scripts.Add(wrapper);
                    Logger.Succeed($"Successfully loaded script: {type.FullName}");
                    Logger.Debug($"Script added to list. Current script count: {scripts.Count}");
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