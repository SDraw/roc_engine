using System.Collections.Generic;

namespace ROC.Engine.Objects
{
    public class Object
    {
        static ulong ms_idPool = 0UL;
        static readonly List<Object> ms_objects = new List<Object>();

        readonly ulong m_id = 1UL;

        protected Object()
        {
            m_id = ms_idPool;

            ms_idPool++;
            ms_objects.Add(this);
        }

        ~Object()
        {
            DestroyInternal();
            ms_objects.Remove(this);
        }

        public ulong InstanceID => m_id;

        protected virtual void DestroyInternal()
        {
        }

        public virtual bool IsValid => true;

        // API
        public static void Destroy(Object p_obj) => p_obj.DestroyInternal();

        // Cleanup
        internal static void DestroyAll()
        {
            foreach(var l_obj in ms_objects)
                l_obj.DestroyInternal();

            ms_objects.Clear();
        }
    }
}
