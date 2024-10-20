using System.Reflection;

namespace LumScripting.Script.Properties
{
    public class Property
    {
        private static Dictionary<Type, object> properties = new Dictionary<Type, object>();
        
        // Metodi per aggiungere proprietà, ecc.
        public void RegisterProperties()
        {
            var types = Assembly.GetExecutingAssembly().GetTypes();
        
            foreach (var type in types)
            {
                var attribute = type.GetCustomAttribute<PropertyAttribute>();
            
                if (attribute != null)
                {
                    // Console.WriteLine($"{type.Name} has AddProperty attribute.");
                    // Gestisci la classe come proprietà
                    // E.g., registra la classe come proprietà base
                    RegisterProperty(type);
                }
            }
        }

        private void RegisterProperty(Type type)
        {
            var instance = Activator.CreateInstance(type);
            properties[type] = instance;
        }
        
        public static T GetProperty<T>() where T : class
        {
            if (properties.TryGetValue(typeof(T), out var value))
            {
                return value as T;
            }
            return null;
        }
    }
}
