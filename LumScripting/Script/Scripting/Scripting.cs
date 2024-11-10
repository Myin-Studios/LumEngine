namespace LumScripting.Script.Scripting
{
    public interface IScript
    {
        void onStart();
        void onRun();

        void SetEntity(Entity entity);
    }

    public abstract class GameBehaviour : IScript
    {
        protected Entity? Entity { get; private set; }

        public void SetEntity(Entity entity)
        {
            this.Entity = entity;
        }

        public virtual void onStart() { }
        public virtual void onRun() { }
    }
}