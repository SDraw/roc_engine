using GlmSharp;
using ROC.Engine.Objects.Resources;
using SFML.Audio;
using SFML.System;

namespace ROC.Engine.Objects.Components
{
    public sealed class AudioListener : Component
    {
        static vec3 c_forward = -vec3.UnitZ;
        static vec3 c_up = vec3.UnitY;

        internal AudioListener()
        {
            m_componentType = ComponentType.AudioListener;
        }

        internal override void OnRenderPass(Shader p_shader, Camera p_camera)
        {
            vec3 l_pos = GameObject.Position;
            quat l_rot = GameObject.Rotation;
            vec3 l_forward = l_rot * c_forward;
            vec3 l_up = l_rot * c_up;

            Listener.Position = new Vector3f(l_pos.x, l_pos.y, l_pos.z);
            Listener.Direction = new Vector3f(l_forward.x, l_forward.y, l_forward.z);
            Listener.UpVector = new Vector3f(l_up.x, l_up.y, l_up.z);
        }
    }
}
