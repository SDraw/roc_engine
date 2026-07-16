using GlmSharp;

namespace ROC.Engine
{
    internal sealed class Frustum
    {
        const int c_planesCount = 6;

        readonly vec4[] m_planes = null;

        public Frustum()
        {
            m_planes = new vec4[c_planesCount];
        }

        public void UpdatePlanes(mat4 p_viewProj)
        {
            m_planes[0] = p_viewProj.Row3 + p_viewProj.Row0;
            m_planes[1] = p_viewProj.Row3 - p_viewProj.Row0;
            m_planes[2] = p_viewProj.Row3 + p_viewProj.Row1;
            m_planes[3] = p_viewProj.Row3 - p_viewProj.Row1;
            m_planes[4] = p_viewProj.Row3 + p_viewProj.Row2;
            m_planes[5] = p_viewProj.Row3 + p_viewProj.Row2;
            for(int i = 0; i < c_planesCount; i++)
                m_planes[i] /= m_planes[i].xyz.Length;
        }

        internal bool IsVisible(vec3 l_pos, float p_radius)
        {
            bool l_result = true;
            foreach(var l_plane in m_planes)
            {
                if(l_plane.x * l_pos.x + l_plane.y * l_pos.y + l_plane.z * l_pos.z + l_plane.w < -p_radius)
                {
                    l_result = false;
                    break;
                }
            }
            return l_result;
        }
    }
}
