using GlmSharp;

namespace ROC.Engine.Objects.Components
{
    public sealed class Light : Component
    {
        public enum LightType
        {
            Directional = 0,
            Point,
            Spotlight
        }

        static readonly vec3 c_forward = -vec3.UnitZ;

        mat4 m_data;

        public LightType TypeOfLight
        {
            get;
            set;
        }

        public vec4 Color
        {
            get;
            set;
        }

        public vec2 Cutoff
        {
            get;
            set;
        }

        public vec3 Falloff
        {
            get;
            set;
        }

        public int Priority
        {
            get;
            set;
        }

        public bool Shadows
        {
            get;
            set;
        }

        public mat4 Data
        {
            get
            {
                m_data.Column0 = new vec4(GameObject.Position, Cutoff.x);
                m_data.Column1 = new vec4(GameObject.Rotation * c_forward, Cutoff.y);
                m_data.Column2 = Color;
                m_data.Column3 = new vec4(Falloff, (float)TypeOfLight);
                return m_data;
            }
        }

        internal Light()
        {
            m_componentType = ComponentType.Light;

            TypeOfLight = LightType.Directional;
            Color = vec4.Ones;
            Cutoff = vec2.Zero;
            Falloff = vec3.UnitX;
            Priority = 0;
            Shadows = true;
        }
    }
}
