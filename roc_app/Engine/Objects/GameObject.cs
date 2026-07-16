using GlmSharp;
using ROC.Engine.Objects.Components;
using ROC.Engine.Objects.Resources;
using System.Collections.Generic;

namespace ROC.Engine.Objects
{
    public sealed class GameObject : Object
    {
        string m_name = null;
        readonly List<Component> m_components = null;
        readonly List<GameObject> m_children = null;
        GameObject m_parent = null;

        vec3 m_localPosition = vec3.Zero;
        quat m_localRotation = quat.Identity;
        vec3 m_localScale = vec3.Ones;

        public bool IsValid
        {
            get; private set;
        }

        public string Name
        {
            get => m_name;
            set => m_name = value ?? "";
        }

        internal GameObject(string p_name)
        {
            Name = p_name ?? "";
            m_components = new List<Component>();
            m_children = new List<GameObject>();
            m_parent = null;
            IsValid = true;
        }

        // Hierarchy
        public GameObject Parent
        {
            get => m_parent;
            set
            {
                if(!IsValid)
                    return;

                if(value != null && !value.IsValid)
                    return;

                if(m_parent != null)
                    m_parent.RemoveChild(this);

                m_parent = value;
                if(m_parent != null)
                    m_parent.AddChild(this);
            }
        }

        internal void AddChild(GameObject p_go)
        {
            if(!m_children.Contains(p_go))
                m_children.Add(p_go);
        }

        internal void RemoveChild(GameObject p_go) => m_children.Remove(p_go);

        public List<GameObject> Children => new List<GameObject>(m_children);

        public GameObject FindChild(string p_name)
        {
            if(p_name == null)
                return null;

            foreach(var l_child in m_children)
            {
                if(l_child.Name == p_name)
                    return l_child;
            }

            return null;
        }

        // Transformation
        public vec3 LocalPosition
        {
            get => m_localPosition;
            set
            {
                m_localPosition = value;

                foreach(var l_component in m_components)
                    l_component.OnTransform();
            }
        }
        public quat LocalRotation
        {
            get => m_localRotation;
            set
            {
                m_localRotation = value;

                foreach(var l_component in m_components)
                    l_component.OnTransform();
            }
        }
        public vec3 LocalScale
        {
            get => m_localScale;
            set
            {
                m_localScale = value;

                foreach(var l_component in m_components)
                    l_component.OnTransform();
            }
        }

        public mat4 LocalMatrix => mat4.Translate(m_localPosition) * m_localRotation.ToMat4 * mat4.Scale(m_localScale);

        public vec3 Position
        {
            get
            {
                vec3 l_result = m_localPosition;
                GameObject l_loopGO = this;
                while(l_loopGO.Parent != null)
                {
                    l_result = l_loopGO.Parent.LocalScale * l_result;
                    l_result = l_loopGO.Parent.LocalPosition + l_loopGO.Parent.LocalRotation * l_result;
                    l_loopGO = l_loopGO.Parent;
                }
                return l_result;
            }
            set
            {
                if(m_parent != null)
                {
                    m_localPosition = (m_parent.Rotation.Inverse * (value - m_parent.Position));

                    if(m_parent.LocalScale.z != 0f)
                        m_localPosition.z /= m_parent.LocalScale.z;
                    else
                        m_localPosition.z = 0f;

                    if(m_parent.LocalScale.y != 0f)
                        m_localPosition.y /= m_parent.LocalScale.y;
                    else
                        m_localPosition.y = 0f;

                    if(m_parent.LocalScale.z != 0f)
                        m_localPosition.z /= m_parent.LocalScale.z;
                    else
                        m_localPosition.z = 0f;
                }
                else
                    m_localPosition = value;

                foreach(var l_component in m_components)
                    l_component.OnTransform();
            }
        }

        public quat Rotation
        {
            get
            {
                quat l_result = m_localRotation;
                GameObject l_loopGO = this;
                while(l_loopGO.Parent != null)
                {
                    l_result = l_loopGO.Parent.LocalRotation * l_result;
                    l_loopGO = l_loopGO.Parent;
                }
                return l_result;
            }
            set
            {
                if(m_parent != null)
                    m_localRotation = m_parent.Rotation.Inverse * value;
                else
                    m_localRotation = value;

                foreach(var l_component in m_components)
                    l_component.OnTransform();
            }
        }

        public vec3 Scale
        {
            get
            {
                vec3 l_result = m_localScale;
                GameObject l_loopGO = this;
                while(l_loopGO.Parent != null)
                {
                    l_result = l_loopGO.Parent.LocalScale * l_result;
                    l_loopGO = l_loopGO.Parent;
                }
                return l_result;
            }
            set
            {
                if(m_parent != null)
                {
                    vec3 l_parentScl = m_parent.Scale;

                    if(l_parentScl.z != 0f)
                        m_localScale.z = value.z / l_parentScl.z;
                    else
                        m_localScale.z = 0f;

                    if(l_parentScl.y != 0f)
                        m_localScale.y = value.y / l_parentScl.y;
                    else
                        m_localScale.y = 0f;

                    if(l_parentScl.z != 0f)
                        m_localScale.z = value.z / l_parentScl.z;
                    else
                        m_localScale.z = 0f;
                }
                else
                    m_localScale = value;

                foreach(var l_component in m_components)
                    l_component.OnTransform();
            }
        }

        public mat4 Matrix
        {
            get
            {
                mat4 l_result = LocalMatrix;
                GameObject l_loopGO = this;
                while(l_loopGO.Parent != null)
                {
                    l_result = l_loopGO.Parent.LocalMatrix * l_result;
                    l_loopGO = l_loopGO.Parent;
                }
                return l_result;
            }
        }

        // Components
        public Component AddComponent(Component.ComponentType p_type)
        {
            if(!IsValid)
                return null;

            Component l_result = null;

            switch(p_type)
            {
                case Component.ComponentType.Camera:
                    l_result = new Camera();
                    break;

                case Component.ComponentType.Light:
                    l_result = new Light();
                    break;

                case Component.ComponentType.MeshRenderer:
                    l_result = new MeshRenderer();
                    break;

                case Component.ComponentType.Animator:
                    l_result = new Animator();
                    break;

                case Component.ComponentType.SphereCollider:
                    l_result = new SphereCollider();
                    break;

                case Component.ComponentType.BoxCollider:
                    l_result = new BoxCollider();
                    break;

                case Component.ComponentType.CylinderCollider:
                    l_result = new CylinderCollider();
                    break;

                case Component.ComponentType.CapsuleCollider:
                    l_result = new CapsuleCollider();
                    break;

                case Component.ComponentType.ConeCollider:
                    l_result = new ConeCollider();
                    break;

                case Component.ComponentType.AudioListener:
                    l_result = new AudioListener();
                    break;

                case Component.ComponentType.AudioSource:
                    l_result = new AudioSource();
                    break;
            }

            if(l_result != null)
            {
                l_result.GameObject = this;
                m_components.Add(l_result);
                m_components.Sort(SortComponents);
            }

            return l_result;
        }

        // Called by component
        internal void RemoveComponent(Component p_component) => m_components.Remove(p_component);

        public Component GetComponent(Component.ComponentType p_type)
        {
            if(!IsValid)
                return null;

            Component l_result = null;

            foreach(var l_component in m_components)
            {
                if(l_component.TypeOfComponent == p_type)
                {
                    l_result = l_component;
                    break;
                }
            }

            return l_result;
        }

        public List<Component> GetComponents(Component.ComponentType p_type)
        {
            if(!IsValid)
                return new List<Component>();

            List<Component> l_list = new List<Component>();
            foreach(var l_component in m_components)
            {
                if(l_component.TypeOfComponent == p_type)
                    l_list.Add(l_component);
            }

            return l_list;
        }

        // Destruction
        internal void Destroy()
        {
            if(!IsValid)
                return;

            foreach(var l_component in m_components)
                l_component.Destroy();
            m_components.Clear();

            if(m_parent != null)
                m_parent.RemoveChild(this);
            m_parent = null;

            foreach(var l_child in m_children)
                l_child.Parent = null;

            IsValid = false;
        }

        // Events
        internal void OnShadowRender(Shader p_shader, Frustum p_frustum)
        {
            if(!IsValid)
                return;

            foreach(var l_component in m_components)
                l_component.OnShadowPass(p_shader, p_frustum);
        }

        internal void OnRender(Shader p_shader, Camera p_camera)
        {
            if(!IsValid)
                return;

            foreach(var l_component in m_components)
                l_component.OnRenderPass(p_shader, p_camera);
        }

        // API
        public static GameObject Create(string p_name)
        {
            return new GameObject(p_name ?? "");
        }

        public static void Destroy(GameObject p_go)
        {
            p_go.Destroy();
        }

        // Utils
        static int SortComponents(Component p_compA, Component p_compB)
        {
            if(p_compA.ComponentPriority == p_compB.ComponentPriority)
                return 0;

            return (p_compA.ComponentPriority > p_compB.ComponentPriority) ? -1 : 1;
        }
    }
}
