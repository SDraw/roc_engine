using ROC.Engine.Core;
using ROC.Engine.Objects.Resources;
using System;

namespace ROC.Engine.Objects.Components
{
    public sealed class Animator : Component
    {
        uint m_loopTime = 0U;
        ulong m_lastFrame = 0UL;

        Animation m_animation = null;

        public Animation Animation
        {
            get => m_animation;
            set
            {
                m_animation = value;
                m_loopTime = 0U;
            }
        }

        public float Speed
        {
            get;
            set;
        }

        public MeshRenderer MeshRenderer
        {
            get;
            set;
        }

        public override int ComponentPriority => 2;

        internal Animator()
        {
            m_componentType = ComponentType.Animator;

            Speed = 1f;
            MeshRenderer = null;
        }

        protected override void DestroyInternal()
        {
            m_loopTime = 0U;

            Animation = null;
            MeshRenderer = null;
            Speed = 1f;

            base.DestroyInternal();
        }

        // GameObject events
        internal override void OnShadowPass(Shader p_shader, Frustum p_frustum)
        {
            if(!Enabled || m_lastFrame == DeltaTick.FrameIndex || m_animation == null || !m_animation.IsValid || MeshRenderer == null || !MeshRenderer.IsValid || MeshRenderer.Bones.Count == 0)
                return;

            m_lastFrame = DeltaTick.FrameIndex;
            Update();
        }

        internal override void OnRenderPass(Shader p_shader, Camera p_camera)
        {
            if(!Enabled || m_lastFrame == DeltaTick.FrameIndex || m_animation == null || !m_animation.IsValid || MeshRenderer == null || !MeshRenderer.IsValid || MeshRenderer.Bones.Count == 0)
                return;

            m_lastFrame = DeltaTick.FrameIndex;
            Update();
        }

        // Animation update
        void Update()
        {
            m_loopTime += (uint)(DeltaTick.Delta * 1000f * Speed);
            m_loopTime %= Animation.Duration;

            var l_frameData = Animation.GetFrameData(m_loopTime);
            if(l_frameData.Length == 0)
                return;

            for(int i = 0, j = Math.Min(Animation.BonesCount, MeshRenderer.Bones.Count); i < j; i++)
            {
                if(l_frameData[i].m_available && (MeshRenderer.Bones[i] != null) && MeshRenderer.Bones[i].IsValid)
                {
                    MeshRenderer.Bones[i].LocalPosition = l_frameData[i].m_keyframe.m_position;
                    MeshRenderer.Bones[i].LocalRotation = l_frameData[i].m_keyframe.m_rotation;
                    MeshRenderer.Bones[i].LocalScale = l_frameData[i].m_keyframe.m_scale;
                }
            }
        }
    }
}
