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

        public virtual int ComponentPriority => 0;

        GameObject m_gameObject = null;
        protected ComponentType m_componentType = ComponentType.Invalid;

        public ComponentType TypeOfComponent => m_componentType;
        public override bool IsValid => (m_componentType != ComponentType.Invalid);
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

        protected override void DestroyInternal()
        {
            if(m_componentType == ComponentType.Invalid)
                return;

            m_gameObject?.RemoveComponent(this);
            m_gameObject = null;

            m_componentType = ComponentType.Invalid;

            base.DestroyInternal();
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
    }
}
