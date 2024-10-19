using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

public class ScriptManager
{
    private List<IScript> scripts = new List<IScript>();

    public void Load()
    {
        // Trova tutti i tipi che implementano IScript nell'assembly corrente
        var scriptTypes = Assembly.GetExecutingAssembly()
            .GetTypes()
            .Where(t => typeof(IScript).IsAssignableFrom(t) && !t.IsAbstract);

        foreach (var type in scriptTypes)
        {
            // Crea un'istanza dello script e aggiungila alla lista
            IScript script = (IScript)Activator.CreateInstance(type);

            Entity entity = new Entity();

            // Gestione di [Property]
            var customProperties = Assembly.GetExecutingAssembly()
                .GetTypes()
                .Where(t => t.GetCustomAttribute<PropertyAttribute>() != null);

            foreach (var customProperty in customProperties)
            {
                entity.AddProperty(customProperty);
            }

            // Gestione di [RequireProperty]
            var requiredProperties = type.GetCustomAttributes<RequirePropertyAttribute>();
            foreach (var requiredProperty in requiredProperties)
            {
                var propertyType = requiredProperty.PropertyType;

                // Verifica se l'entità ha già la proprietà richiesta
                bool hasProperty = entity.HasProperty(propertyType);

                if (!hasProperty)
                {
                    var propertyInstance = Activator.CreateInstance(propertyType);
                    entity.AddProperty(propertyType);
                }
            }

            if (script is GameBehaviour behaviourScript)
            {
                behaviourScript.SetEntity(entity);
            }

            scripts.Add(script);
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

    public void Initialize()
    {
        // Carica e avvia tutti gli script
        scriptManager.Load();
        scriptManager.Start();
    }

    public void Update()
    {
        // Chiama onRun su tutti gli script per ogni frame
        scriptManager.Run();
    }
}