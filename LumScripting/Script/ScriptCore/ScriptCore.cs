﻿using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;

public class ScriptCompiler
{
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
}

public class ScriptManager
{
    private List<IScript> scripts = new List<IScript>();

    public void Load(string assemblyPath)
    {
        string projectName = Path.GetFileName(assemblyPath);
        string fullAssemblyPath = Path.Combine(assemblyPath, "bin", "Debug", "net8.0-windows10.0.26100.0", projectName + ".dll");
        string icyScriptingDllPath = Path.GetFullPath("../IcyScripting/bin/Debug/net8.0-windows10.0.26100.0/win-x64/IcyScripting.dll");

        Console.WriteLine($"Loading user assembly from: {fullAssemblyPath}");
        Console.WriteLine($"Looking for IcyScripting at: {icyScriptingDllPath}");

        if (!File.Exists(fullAssemblyPath))
        {
            Console.WriteLine("User assembly file not found.");
            return;
        }

        if (!File.Exists(icyScriptingDllPath))
        {
            Console.WriteLine("IcyScripting assembly file not found.");
            return;
        }

        try
        {
            Assembly userAssembly = Assembly.LoadFrom(fullAssemblyPath);

            AppDomain.CurrentDomain.AssemblyResolve += (sender, args) =>
            {
                if (args.Name.StartsWith("IcyScripting"))
                {
                    return Assembly.LoadFrom(icyScriptingDllPath);
                }
                return null;
            };

            var loadedAssemblies = AppDomain.CurrentDomain.GetAssemblies();

            var icyScriptingAssembly = loadedAssemblies.FirstOrDefault(a => a.FullName.StartsWith("IcyScripting"))
                                       ?? Assembly.LoadFrom(icyScriptingDllPath);

            // Verifica che icyScriptingAssembly non sia null
            if (icyScriptingAssembly == null)
            {
                Console.WriteLine("Failed to load IcyScripting assembly.");
                return;
            }

            // Ora icyScriptingAssembly contiene l'assembly IcyScripting
            Type? scriptInterfaceType = icyScriptingAssembly.GetType("IcyScripting.Script.Scripting.IScript");

            // A questo punto verifica che il tipo sia stato trovato
            if (scriptInterfaceType == null)
            {
                Console.WriteLine("IScript type not found in IcyScripting assembly.");
                return;
            }

            // Ottieni tutti i tipi che implementano l'interfaccia IScript
            var scriptTypes = userAssembly.GetTypes()
                .Where(t => scriptInterfaceType.IsAssignableFrom(t) && !t.IsAbstract);

            Console.WriteLine(scriptTypes.Count());

            foreach (var scriptType in scriptTypes)
            {
                // Crea un'istanza del tipo di script
                var scriptInstance = Activator.CreateInstance(scriptType);

                // Aggiungi la logica per gestire le proprietà
                var properties = scriptType.GetProperties();
                foreach (var prop in properties)
                {
                    // Verifica se l'attributo [Property] è presente
                    if (Attribute.IsDefined(prop, typeof(PropertyAttribute)))
                    {
                        // Gestisci la proprietà in base alle tue necessità
                        // Esempio: setta una proprietà specifica
                        // prop.SetValue(scriptInstance, /* valore desiderato */);
                    }
                }

                // Se hai bisogno di registrare lo script nel motore
                scripts.Add((dynamic)scriptInstance);
                Console.WriteLine($"Added script: {scriptInstance.GetType().Name}");
            }

        }
        catch (ReflectionTypeLoadException ex)
        {
            Console.WriteLine($"Error loading assembly: {ex.Message}");
            foreach (var loaderException in ex.LoaderExceptions)
            {
                Console.WriteLine(loaderException.Message);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"An unexpected error occurred: {ex.Message}");
        }
    }

    /*

    public void Load(string assemblyPath)
    {
        // Carica l'assembly dell'utente
        string projectName = Path.GetFileName(assemblyPath);
        string fullAssemblyPath = Path.Combine(assemblyPath, "bin", "Debug", "net8.0-windows10.0.26100.0", projectName + ".dll");

        string icyScriptingDllPath = Path.GetFullPath("../IcyScripting/bin/Debug/net8.0-windows10.0.26100.0/win-x64/IcyScripting.dll");

        Console.WriteLine(icyScriptingDllPath);

        if (!File.Exists(fullAssemblyPath))
        {
            Console.WriteLine("Assembly file not found.");
            return;
        }

        try
        {
            // Associa l'evento di risoluzione degli assembly
            AppDomain.CurrentDomain.AssemblyResolve += (sender, args) =>
            {
                if (args.Name.StartsWith("IcyScripting"))
                {
                    return Assembly.LoadFrom(icyScriptingDllPath);
                }
                return null;
            };

            // Carica l'assembly dell'utente
            Assembly userAssembly = Assembly.LoadFrom(fullAssemblyPath);
            var loadedAssemblies = AppDomain.CurrentDomain.GetAssemblies();

            // Carica l'assembly di IcyScripting, se non è già caricato
            var icyScriptingAssembly = loadedAssemblies.FirstOrDefault(a => a.FullName.StartsWith("IcyScripting"))
                                       ?? Assembly.LoadFrom(icyScriptingDllPath);

            foreach (var assembly in loadedAssemblies )
            {
                Console.WriteLine(assembly.FullName);
            }

            // Ora icyScriptingAssembly contiene l'assembly IcyScripting
            Type? scriptInterfaceType = icyScriptingAssembly.GetType("IcyScripting.Script.Scripting.GameBehaviour");

            bool InheritsFromGameBehaviour(Type type)
            {
                while (type != null && type != typeof(object))
                {
                    if (type.FullName == scriptInterfaceType?.FullName)
                    {
                        return true;
                    }
                    type = type.BaseType;
                }
                return false;
            }

            var scriptTypes = userAssembly.GetTypes().ToList();

            Console.WriteLine($"Found {scriptTypes.Count} scripts:");
            
            foreach (var type in scriptTypes)
            {
                Console.WriteLine($"- {type.FullName}, {type.BaseType}");

                if (InheritsFromGameBehaviour(type))
                {
                    // Crea un'istanza dell'oggetto tramite riflessione
                    object scriptInstance = Activator.CreateInstance(type);

                    Console.WriteLine($"Successfully created an instance of {type.FullName} that implements IScript.");

                    var entity = new Entity();

                    // Gestione delle proprietà personalizzate [Property]
                    var customProperties = icyScriptingAssembly.GetTypes()
                        .Where(t => t.GetCustomAttribute<PropertyAttribute>() != null);

                    foreach (var customProperty in customProperties)
                    {
                        entity.AddProperty(customProperty);
                        Console.WriteLine($"Added custom property: {customProperty.FullName}");
                    }

                    // Gestione di [RequireProperty]
                    Type requirePropertyAttributeType = icyScriptingAssembly.GetType("IcyScripting.Script.Scripting.RequirePropertyAttribute");
                    if (requirePropertyAttributeType != null)
                    {
                        var requiredProperties = type.GetCustomAttributes(requirePropertyAttributeType, inherit: true);
                        foreach (var requiredProperty in requiredProperties)
                        {
                            var propertyType = (Type)requiredProperty.GetType().GetProperty("PropertyType")?.GetValue(requiredProperty);
                            if (propertyType != null && !entity.HasProperty(propertyType))
                            {
                                var propertyInstance = Activator.CreateInstance(propertyType);
                                entity.AddProperty(propertyInstance);
                            }
                        }
                    }

                    // Se l'istanza è di tipo GameBehaviour, invoca il metodo SetEntity
                    Type gameBehaviourType = icyScriptingAssembly.GetType("IcyScripting.Script.Scripting.GameBehaviour");
                    if (gameBehaviourType != null && gameBehaviourType.IsInstanceOfType(scriptInstance))
                    {
                        var setEntityMethod = gameBehaviourType.GetMethod("SetEntity");
                        if (setEntityMethod != null)
                        {
                            setEntityMethod.Invoke(scriptInstance, new object[] { entity });
                            Console.WriteLine($"Entity set for {type.FullName}.");
                        }
                    }

                    // Aggiunge lo script alla lista degli script gestiti
                    scripts.Add((dynamic)scriptInstance);
                    Console.WriteLine($"Script added: {type.FullName}");
                }
            }
        }
        catch (ReflectionTypeLoadException ex)
        {
            Console.WriteLine($"Error loading assembly: {ex.Message}");
            foreach (var loaderException in ex.LoaderExceptions)
            {
                Console.WriteLine(loaderException.Message);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"An unexpected error occurred: {ex.Message}");
        }
    }

    */

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