using GlmSharp;

namespace ROC.Engine.Objects.Components
{
    public sealed class Camera : Component
    {
        public enum ProjectionType : int
        {
            Orthogonal = 0,
            Perspective,
            Screen
        }

        static readonly vec3 ms_forward = -vec3.UnitZ;
        static readonly vec3 ms_up = vec3.UnitY;

        readonly Frustum m_frustum = null;

        internal vec3 Position => GameObject.Position;
        internal vec3 Direction => GameObject.Rotation * ms_forward;

        public ProjectionType Projection
        {
            get; set;
        }

        public float FOV
        {
            get; set;
        }

        public float AspectRatio
        {
            get; set;
        }

        public vec4 OrthogonalParams
        {
            get;
            set;
        }

        public vec2 Depth
        {
            get;
            set;
        }

        public int Priority
        {
            get;
            set;
        }

        public mat4 ProjectionMatrix
        {
            get
            {
                switch(Projection)
                {
                    case ProjectionType.Orthogonal:
                        return mat4.Ortho(OrthogonalParams.x, OrthogonalParams.y, OrthogonalParams.z, OrthogonalParams.w, Depth.x, Depth.y);

                    case ProjectionType.Perspective:
                        return mat4.Perspective(FOV, AspectRatio, Depth.x, Depth.y);

                    case ProjectionType.Screen:
                        return mat4.Ortho(OrthogonalParams.x, OrthogonalParams.y, OrthogonalParams.z, OrthogonalParams.w);

                    default:
                        return mat4.Ones;
                }
            }
        }

        public mat4 ViewMatrix
        {
            get
            {
                vec3 l_pos = GameObject.Position;
                quat l_rot = GameObject.Rotation;
                return mat4.LookAt(l_pos, l_pos + l_rot * ms_forward, l_rot * ms_up);
            }
        }

        public mat4 ViewProjectionMatrix => ProjectionMatrix * ViewMatrix;

        public float ShadowsRadius
        {
            get;
            set;
        }

        internal Camera()
        {
            m_componentType = ComponentType.Camera;

            m_frustum = new Frustum();
            Projection = ProjectionType.Orthogonal;
            FOV = (float)System.Math.PI * 0.36f;
            AspectRatio = 640f / 480f;
            OrthogonalParams = new vec4(-5f, 5f, -5f, 5f);
            Depth = vec2.Ones * 5f;
            Priority = 0;
            ShadowsRadius = 50f;
        }

        internal void UpdateFrustum() => m_frustum.UpdatePlanes(ViewProjectionMatrix);

        public bool IsVisible(vec3 l_pos, float p_radius) => m_frustum.IsVisible(l_pos, p_radius);
    }
}
