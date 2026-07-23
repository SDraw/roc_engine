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

        public override bool IsValid => m_loaded;

        public string Log => m_log;
    }
}
