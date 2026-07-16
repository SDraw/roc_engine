namespace ROC.Engine.Objects.Resources
{
    public class Resource : Object
    {
        protected bool m_loaded = false;
        protected string m_log;

        protected Resource()
        {
            m_log = "";
        }

        virtual public void Unload()
        {
            if(!m_loaded)
                return;

            m_loaded = false;
        }

        public bool IsLoaded => m_loaded;
        public string Log => m_log;
    }
}
