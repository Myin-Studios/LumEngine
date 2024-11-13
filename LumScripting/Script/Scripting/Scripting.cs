using LumScripting.Script.Internal;
using System.Reflection;

namespace LumScripting.Script.Internal
{
    public interface IScript
    {
        void onStart();
        void onRun();
    }

    internal interface IScriptInternal : IScript
    {
        void SetEntity(Entity entity);
    }

    public class ScriptWrapper : IScriptInternal  // Classe public
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
            _startMethod = type.GetMethod("onStart");
            _runMethod = type.GetMethod("onRun");
        }

        void IScriptInternal.SetEntity(Entity entity)  // Implementazione esplicita
        {
            _entity = entity;

            // Se l'istanza implementa IScriptInternal, chiamiamo SetEntity direttamente
            if (_instance is IScriptInternal scriptInternal)
            {
                scriptInternal.SetEntity(entity);
            }
        }

        public void InitializeEntity(Entity entity)  // Metodo pubblico per inizializzare l'entity
        {
            ((IScriptInternal)this).SetEntity(entity);
        }

        public void onStart()
        {
            _startMethod?.Invoke(_instance, null);
        }

        public void onRun()
        {
            _runMethod?.Invoke(_instance, null);
        }
    }
}

namespace LumScripting.Script.Scripting
{
    public abstract class GameBehaviour : IScriptInternal
    {
        Entity _entity;

        public Entity Entity { get => _entity; }

        protected GameBehaviour()
        {
            _entity = new Entity();
        }

        void IScriptInternal.SetEntity(Entity entity)
        {
            _entity = entity ?? throw new ArgumentNullException(nameof(entity));
        }

        public abstract void onStart();
        public abstract void onRun();
    }

}
