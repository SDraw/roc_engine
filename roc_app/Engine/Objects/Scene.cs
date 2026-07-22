using GlmSharp;
using ROC.Engine.Objects.Components;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Engine.Objects
{
    public sealed class Scene : Object
    {
        const int c_maxLights = 4;

        static readonly List<Scene> ms_scenes = new List<Scene>();
        internal static List<Scene> AllScenes => ms_scenes;
        static readonly Predicate<GameObject> InvalidGameObjectPredicate = (p_go) => !p_go.IsValid;

        readonly List<GameObject> m_gameObjects = null;

        readonly List<Camera> m_renderLoopCameras = null;
        readonly List<Light> m_renderLoopLights = null;
        readonly mat4[] m_renderLoopLightData = null;
        readonly Frustum m_shadowFrustum = null;

        bool m_valid;

        static Shader ms_shadowShader = null;
        static RenderTarget ms_shadowRenderTarget = null;

        public override bool IsValid => m_valid;

        public Shader Shader
        {
            get;
            set;
        }

        internal Scene()
        {
            m_gameObjects = new List<GameObject>();
            m_renderLoopCameras = new List<Camera>();
            m_renderLoopLights = new List<Light>();
            m_renderLoopLightData = new mat4[c_maxLights];
            m_shadowFrustum = new Frustum();

            m_valid = true;
            ms_scenes.Add(this);
        }
        ~Scene()
        {
            ms_scenes.Remove(this);
        }

        public void AddGameObject(GameObject p_go)
        {
            if(!IsValid || p_go == null || !p_go.IsValid)
                return;

            if(m_gameObjects.Contains(p_go))
                return;

            m_gameObjects.Add(p_go);
        }

        public void RemoveGameObject(GameObject p_go)
        {
            if(!IsValid || p_go == null || !p_go.IsValid)
                return;

            m_gameObjects.Remove(p_go);
        }

        // Destruction
        protected override void DestroyInternal()
        {
            if(m_valid)
            {
                Shader = null;
                m_gameObjects.Clear();
                m_renderLoopCameras.Clear();
                m_renderLoopLights.Clear();

                m_valid = false;
            }

            base.DestroyInternal();
        }

        // Rendering
        internal void OnFramePass(float p_time)
        {
            if(!IsValid)
                return;

            m_gameObjects.RemoveAll(InvalidGameObjectPredicate);
            m_renderLoopCameras.Clear();
            m_renderLoopLights.Clear();

            if(Shader == null || !Shader.IsValid)
            {
                Shader = null;
                return;
            }

            foreach(var l_go in m_gameObjects)
            {
                Component l_component = l_go.GetComponent(Component.ComponentType.Camera);
                if((l_component != null) && l_component.IsValid && l_component.Enabled)
                    m_renderLoopCameras.Add(l_component as Camera);

                l_component = l_go.GetComponent(Component.ComponentType.Light);
                if((l_component != null) && l_component.IsValid && l_component.Enabled)
                    m_renderLoopLights.Add(l_component as Light);
            }

            if(m_renderLoopCameras.Count == 0)
                return;

            m_renderLoopCameras.Sort(SortCameras);
            m_renderLoopLights.Sort(SortLights);

            Camera l_mainCamera = m_renderLoopCameras[0];
            l_mainCamera.UpdateFrustum();

            mat4 l_shadowViewProjection = mat4.Identity;

            // Shadow pass
            Light l_shadowLight = null;
            foreach(var l_light in m_renderLoopLights)
            {
                if(l_light.Enabled && (l_light.TypeOfLight == Light.LightType.Directional) && l_light.Shadows)
                {
                    l_shadowLight = l_light;
                    break;
                }
            }

            if(l_shadowLight != null)
            {
                ms_shadowRenderTarget.Bind();
                ms_shadowShader.Activate();

                // Projection is done with main camera and light
                var l_camPos = l_mainCamera.GameObject.Position;
                var l_lightRot = l_shadowLight.GameObject.Rotation;
                float l_shadowsRadius = l_mainCamera.ShadowsRadius * 0.5f;
                mat4 l_View = mat4.LookAt(l_camPos, l_camPos - l_lightRot * vec3.UnitZ, l_lightRot * vec3.UnitY);
                mat4 l_proj = mat4.Ortho(-l_shadowsRadius, l_shadowsRadius, -l_shadowsRadius, l_shadowsRadius, -l_shadowsRadius, l_shadowsRadius);

                l_shadowViewProjection = l_proj * l_View;
                m_shadowFrustum.UpdatePlanes(l_shadowViewProjection);
                ms_shadowShader.SetViewProjection(l_shadowViewProjection);

                foreach(var l_go in m_gameObjects)
                    l_go.OnShadowRender(ms_shadowShader, m_shadowFrustum);

                RenderTarget.Reset();
            }

            // Regular pass
            for(int i = 0, j = Math.Min(m_renderLoopLights.Count, c_maxLights); i < j; i++)
                m_renderLoopLightData[i] = m_renderLoopLights[i].Data;

            Shader.Activate();
            Shader.SetTime(p_time);
            Shader.SetView(l_mainCamera.ViewMatrix);
            Shader.SetProjection(l_mainCamera.ProjectionMatrix);
            Shader.SetViewProjection(l_mainCamera.ViewProjectionMatrix);
            Shader.SetCamera(l_mainCamera.Position, l_mainCamera.Direction);
            Shader.SetLights(m_renderLoopLightData, m_renderLoopLights.Count);

            if(l_shadowLight != null)
                ms_shadowRenderTarget.BindTexture(OpenGL.TextureUnit.Texture1);
            Shader.SetShadowViewProjection(l_shadowViewProjection);

            foreach(var l_go in m_gameObjects)
                l_go.OnRender(Shader, l_mainCamera);
        }

        static int SortCameras(Camera p_camA, Camera p_camB)
        {
            if(p_camA.Priority == p_camB.Priority)
                return 0;
            if(p_camA.Priority > p_camB.Priority)
                return 1;
            return -1;
        }

        static int SortLights(Light p_lightA, Light p_lightB)
        {
            if(p_lightA.Priority == p_lightB.Priority)
                return 0;
            if(p_lightA.Priority > p_lightB.Priority)
                return 1;
            return -1;
        }

        // Static resources
        internal static void InitResources()
        {
            if(ms_shadowShader == null)
            {
                ms_shadowShader = new Shader();
                ms_shadowShader.Load(
                    Utils.GetEmbeddedResource("shadow_vert.glsl"),
                    Utils.GetEmbeddedResource("shadow_frag.glsl"),
                    true
                );
            }

            if(ms_shadowRenderTarget == null)
            {
                ms_shadowRenderTarget = new RenderTarget();
                ms_shadowRenderTarget.Create(RenderTarget.RenderTargetType.Shadow, new ivec2(Core.Core.Instance.ConfigManager.ShadowsSize));
            }
        }
        internal static void ClearResources()
        {
            if(ms_shadowShader != null)
                Object.Destroy(ms_shadowShader);
            ms_shadowShader = null;

            if(ms_shadowRenderTarget != null)
                Object.Destroy(ms_shadowRenderTarget);
            ms_shadowRenderTarget = null;
        }

        // API
        public static Scene Create()
        {
            return new Scene();
        }
    }
}
