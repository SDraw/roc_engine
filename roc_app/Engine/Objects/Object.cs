namespace ROC.Engine.Objects
{
    public class Object
    {
        static ulong ms_idPool = 0UL;

        readonly ulong m_id = 1UL;

        protected Object()
        {
            m_id = ms_idPool;
            ms_idPool++;
        }

        public ulong InstanceID => m_id;
    }
}
