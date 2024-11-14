using LumScripting.Script.Internal;
using System.Reflection;
using LumScripting.Script.Properties;

namespace LumScripting.Script.Internal
{
    public interface IScript
    {
        void onStart();
        void onRun();
    }

    internal interface IScriptInternal : IScript
    {
        Entity Entity { get; set; }

        void SetEntity(Entity entity);
    }

    public interface IEntityContainer
    {
        void SetEntityInstance(object entity);
    }

    public class ScriptWrapper : IScriptInternal, IEntityContainer
    {
        private readonly object _instance;
        private readonly MethodInfo _startMethod;
        private readonly MethodInfo _runMethod;
        private Entity _entity;

        public Entity Entity
        {
            get => _entity;
            set => _entity = value;
        }

        public ScriptWrapper(object instance)
        {
            _instance = instance;
            var type = instance.GetType();

            // Invece di controllare l'ereditarietà, controlliamo solo che i metodi necessari esistano
            _startMethod = type.GetMethod("onStart", BindingFlags.Public | BindingFlags.Instance);
            _runMethod = type.GetMethod("onRun", BindingFlags.Public | BindingFlags.Instance);

            if (_startMethod == null || _runMethod == null)
            {
                throw new InvalidOperationException($"Type {type.FullName} must implement onStart and onRun methods");
            }
        }

        void IScriptInternal.SetEntity(Entity entity)
        {
            if (entity == null)
                throw new ArgumentNullException(nameof(entity));

            _entity = entity;

            // Cerca il metodo SetEntity direttamente sul tipo
            var setEntityMethod = _instance.GetType()
                .GetMethod("SetEntity",
                    BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.FlattenHierarchy);

            if (setEntityMethod != null)
            {
                setEntityMethod.Invoke(_instance, new object[] { entity });
            }
        }

        public void onStart()
        {
            try
            {
                _startMethod.Invoke(_instance, null);
            }
            catch (TargetInvocationException ex)
            {
                throw ex.InnerException ?? ex;
            }
        }

        public void onRun()
        {
            try
            {
                _runMethod.Invoke(_instance, null);
            }
            catch (TargetInvocationException ex)
            {
                throw ex.InnerException ?? ex;
            }
        }

        public void SetEntityInstance(object entity)
        {
            if (entity == null)
                throw new ArgumentNullException(nameof(entity));

            if (entity is Entity entityTyped)
            {
                ((IScriptInternal)this).SetEntity(entityTyped);
            }
            else
            {
                throw new ArgumentException($"Entity must be of type {typeof(Entity).FullName}", nameof(entity));
            }
        }
    }
}

namespace LumScripting.Script.Scripting
{
    public abstract class GameBehaviour : IScriptInternal
    {
        private Entity _entity;

        // Proprietà pubblica per l'accesso degli utenti
        public Entity Entity
        {
            get { return _entity; }
        }

        Entity IScriptInternal.Entity
        {
            get => _entity;
            set => _entity = value;
        }

        protected GameBehaviour()
        {
            _entity = new Entity();
        }

        // Implementazione esplicita per l'interfaccia interna
        void IScriptInternal.SetEntity(Entity entity)
        {
            _entity = entity ?? throw new ArgumentNullException(nameof(entity));
        }

        public abstract void onStart();
        public abstract void onRun();
    }
}

