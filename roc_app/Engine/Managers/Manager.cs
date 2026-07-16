namespace ROC.Engine.Managers
{
    public class Manager
    {
        protected bool m_active = false;

        internal virtual void Start()
        {
            m_active = true;
        }

        internal virtual void Stop()
        {
            m_active = false;
        }

        public bool IsActive => m_active;
    }
}
