namespace LumScripting.Script.Scripting
{
    public interface IScript
    {
        void onStart();
        void onRun();
    }

    public abstract class GameBehaviour : IScript
    {
        protected Entity Entity { get; private set; }

        public void SetEntity(Entity entity)
        {
            Entity = entity;
        }
        
        public virtual void onStart() { }
        public virtual void onRun() { }
    }
}