using System;
using System.Collections.Generic;

namespace CMEngine.Entities
{
    public class Entity
    {
        private Dictionary<Type, object> _properties = new Dictionary<Type, object>();

        // Aggiungi una proprietà generica
        public void AddProperty<T>(T property) where T : class
        {
            _properties[typeof(T)] = property;
        }

        // Ottieni una proprietà generica
        public T GetProperty<T>() where T : class
        {
            if (_properties.TryGetValue(typeof(T), out var property))
            {
                return (T)property;
            }
            return default;
        }

        // Verifica se una proprietà generica è presente
        public bool HasProperty<T>() where T : class
        {
            return _properties.ContainsKey(typeof(T));
        }

        // Aggiungi una proprietà basata su Type
        public void AddProperty(Type propertyType)
        {
            if (!HasProperty(propertyType))
            {
                var propertyInstance = Activator.CreateInstance(propertyType);
                _properties[propertyType] = propertyInstance;
            }
        }

        // Verifica se una proprietà basata su Type è presente
        public bool HasProperty(Type propertyType)
        {
            return _properties.ContainsKey(propertyType);
        }
    }
}