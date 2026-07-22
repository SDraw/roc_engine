using GlmSharp;
using OpenGL;
using ROC.Engine.Core;
using ROC.Engine.Objects.Resources;
using ROC.Engine.OGL;
using System;
using System.Collections.Generic;

namespace ROC.Engine.Objects.Components
{
    public sealed class MeshRenderer : Component
    {
        const int c_maxBones = 128;

        bool m_rigged = false;
        bool m_ready = false;

        readonly List<Material> m_materials = null;
        readonly List<Mesh> m_meshes = null;
        readonly List<GameObject> m_bones = null;

        mat4[] m_bonesMatricesBinds = null;
        mat4[] m_poseMatrices = null;

        ulong m_lastFrame = 0UL;
        float m_boundsRadius = 0f;

        internal List<GameObject> Bones => m_bones;

        public override int ComponentPriority => 1;

        public bool CastShadows
        {
            get;
            set;
        }

        public bool RecieveShadows
        {
            get;
            set;
        }

        public int MaterialsCount => m_materials.Count;

        internal MeshRenderer()
        {
            m_componentType = ComponentType.MeshRenderer;

            m_materials = new List<Material>();
            m_meshes = new List<Mesh>();
            m_bones = new List<GameObject>();

            CastShadows = true;
            RecieveShadows = true;
        }

        internal void Setup(Model p_model)
        {
            if(m_ready)
                return;

            m_rigged = false;

            foreach(var l_mesh in p_model.Meshes)
                m_meshes.Add(l_mesh);

            foreach(var l_material in p_model.Materials)
                m_materials.Add(new Material(l_material));

            m_boundsRadius = p_model.BoundsRadius;

            m_ready = true;
        }

        internal void Setup(Model p_model, List<GameObject> p_bones)
        {
            if(m_ready)
                return;

            m_rigged = true;

            foreach(var l_mesh in p_model.Meshes)
                m_meshes.Add(l_mesh);

            foreach(var l_material in p_model.Materials)
                m_materials.Add(new Material(l_material));

            m_bonesMatricesBinds = new mat4[Math.Min(c_maxBones, p_bones.Count)];
            m_poseMatrices = new mat4[Math.Min(c_maxBones, p_bones.Count)];

            mat4 l_goMatInv = GameObject.Matrix.Inverse;
            for(int i = 0, j = m_poseMatrices.Length; i < j; i++)
            {
                m_bones.Add(p_bones[i]);
                m_poseMatrices[i] = mat4.Identity;
                m_bonesMatricesBinds[i] = (l_goMatInv * p_bones[i].Matrix).Inverse;
            }

            m_boundsRadius = p_model.BoundsRadius;

            m_ready = true;
        }

        protected override void DestroyInternal()
        {
            if(m_ready)
            {
                m_rigged = false;
                m_meshes.Clear();
                m_materials.Clear();
                m_bones.Clear();
                m_bonesMatricesBinds = null;
                m_poseMatrices = null;
                m_ready = false;
            }

            base.DestroyInternal();
        }

        // GameObject events
        internal override void OnShadowPass(Shader p_shader, Frustum p_frustum)
        {
            if(!Enabled || !m_ready || !CastShadows)
                return;

            if(!p_frustum.IsVisible(GameObject.Position, m_boundsRadius * GameObject.Scale.MaxElement))
                return;

            p_shader.SetModel(GameObject.Matrix);

            if(m_rigged)
            {
                if(m_lastFrame != DeltaTick.FrameIndex)
                {
                    UpdatePoseBones();
                    m_lastFrame = DeltaTick.FrameIndex;
                }

                p_shader.SetAnimated(true);
                p_shader.SetBones(m_poseMatrices);
            }
            else
                p_shader.SetAnimated(false);

            // Render if needed
            GLSettings.Set(EnableCap.Blend, false);
            GLSettings.SetDepthMask(true);

            for(int i = 0, j = Math.Min(m_materials.Count, m_meshes.Count); i < j; i++)
            {
                if(m_materials[i] == null || !m_materials[i].IsValid || m_meshes[i] == null || !m_meshes[i].IsValid)
                    continue;

                if(!m_materials[i].DepthWrite)
                    continue;

                GLSettings.Set(EnableCap.CullFace, !m_materials[i].DoubleSided);
                m_meshes[i].Draw();
            }
        }

        internal override void OnRenderPass(Shader p_shader, Camera p_camera)
        {
            if(!Enabled || !m_ready)
                return;

            if(!p_camera.IsVisible(GameObject.Position, m_boundsRadius * GameObject.Scale.MaxElement))
                return;

            p_shader.SetModel(GameObject.Matrix);

            if(m_rigged)
            {
                if(m_lastFrame != DeltaTick.FrameIndex)
                {
                    UpdatePoseBones();
                    m_lastFrame = DeltaTick.FrameIndex;
                }

                p_shader.SetAnimated(true);
                p_shader.SetBones(m_poseMatrices);
            }
            else
                p_shader.SetAnimated(false);

            for(int i = 0, j = Math.Min(m_materials.Count, m_meshes.Count); i < j; i++)
            {
                if(m_materials[i] == null || !m_materials[i].IsValid || m_meshes[i] == null || !m_meshes[i].IsValid)
                    continue;

                p_shader.SetMaterial(
                    new bvec4(m_materials[i].Unlit, RecieveShadows, false, false),
                    m_materials[i].Params,
                    m_materials[i].Color
                );

                GLSettings.Set(EnableCap.Blend, m_materials[i].Transparency);
                GLSettings.Set(EnableCap.CullFace, !m_materials[i].DoubleSided);
                GLSettings.SetDepthMask(m_materials[i].DepthWrite);

                m_materials[i].DiffuseTexture?.Activate(TextureUnit.Texture0);
                m_meshes[i].Draw();
            }
        }

        // Internal
        void UpdatePoseBones()
        {
            mat4 l_goMatInv = GameObject.Matrix.Inverse;
            for(int i = 0, j = m_poseMatrices.Length; i < j; i++)
                m_poseMatrices[i] = (l_goMatInv * m_bones[i].Matrix) * m_bonesMatricesBinds[i];
        }

        // Public
        public Material GetMaterial(int p_index)
        {
            if(p_index < 0 || p_index >= m_materials.Count)
                return null;

            return m_materials[p_index];
        }

        public void SetMaterial(int p_index, Material p_material)
        {
            if(p_index < 0 || p_index >= m_materials.Count || (p_material != null && !p_material.IsValid))
                return;

            m_materials[p_index] = p_material;
        }
    }
}
