using ROC.Engine.Objects.Resources;

namespace ROC.Engine.Objects.Components
{
    public class Component : Object
    {
        public enum ComponentType : int
        {
            Invalid = -1,
            Camera = 0,
            Light,
            MeshRenderer,
            Animator,
            SphereCollider,
            BoxCollider,
            CylinderCollider,
            CapsuleCollider,
            ConeCollider,
            AudioListener,
            AudioSource
        }

        virtual public int ComponentPriority => 0;

        GameObject m_gameObject = null;
        protected ComponentType m_componentType = ComponentType.Invalid;

        public ComponentType TypeOfComponent => m_componentType;
        public bool IsValid => (m_componentType != ComponentType.Invalid);
        bool m_enabled;

        public GameObject GameObject
        {
            get => m_gameObject;
            internal set => m_gameObject = value;
        }

        public bool Enabled
        {
            get => m_enabled;
            set
            {
                if(!IsValid || m_enabled == value)
                    return;

                m_enabled = value;
                if(m_enabled)
                    OnEnable();
                else
                    OnDisable();
            }
        }

        protected Component()
        {
            m_gameObject = null;
            m_enabled = true;
        }

        internal virtual void Destroy()
        {
            if(!IsValid)
                return;

            m_gameObject = null;
            m_componentType = ComponentType.Invalid;
        }


        // GameObject events
        internal virtual void OnShadowPass(Shader p_shader, Frustum p_frustum)
        {
        }

        internal virtual void OnRenderPass(Shader p_shader, Camera p_camera)
        {
        }

        internal virtual void OnTransform()
        {
        }

        internal virtual void OnEnable()
        {
        }

        internal virtual void OnDisable()
        {
        }

        // API
        public static void Destroy(Component p_component)
        {
            if(p_component == null || !p_component.IsValid)
                return;

            p_component.GameObject.RemoveComponent(p_component);
            p_component.Destroy();
        }
    }
}
