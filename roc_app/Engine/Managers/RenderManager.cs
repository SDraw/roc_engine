using GlmSharp;
using OpenGL;
using ROC.Engine.Objects;
using ROC.Engine.OGL;
using System;

namespace ROC.Engine.Managers
{
    public sealed class RenderManager : Manager
    {
        public enum RenderMode
        {
            Triangles = 0,
            Lines,
            Points
        }

        bool m_renderStage = false;
        static ivec2 ms_renderWindow = ivec2.Ones;
        RenderMode m_renderMode = RenderMode.Triangles;

        static readonly Predicate<Scene> InvalidScenePredicate = (p_go) => !p_go.IsValid;

        internal override void Start()
        {
            if(m_active)
                return;

            GLSettings.Set(EnableCap.DepthTest, true);
            GLSettings.Set(EnableCap.CullFace, true);
            GLSettings.Set(EnableCap.Texture2d, true);
            GLSettings.SetDepthMask(true);
            GLSettings.SetPixelUnpackAlignment(1);

            GLViewport.ClearColor = new vec4(0.223529f, 0.223529f, 0.223529f, 0f);
            GLViewport.SetBlending(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha);

            GLViewport.SetClearDepth(1f);

            Scene.InitResources();

            m_active = true;
        }

        internal override void Stop()
        {
            if(!m_active)
                return;

            Scene.ClearResources();
            m_active = false;
        }

        internal void DoPulse()
        {
            if(!m_active || m_renderStage)
                return;

            ms_renderWindow.x = (int)Core.Core.Instance.WindowManager.WindowSize.x;
            ms_renderWindow.y = (int)Core.Core.Instance.WindowManager.WindowSize.y;
            m_renderStage = true;

            GLViewport.SetArea(ivec2.Zero, ms_renderWindow);
            GLViewport.Clear(true, true);

            // Iterate over all scenes and draw everything
            Scene.AllScenes.RemoveAll(InvalidScenePredicate);

            float l_time = Core.Core.Instance.WindowManager.Time;
            foreach(var l_scene in Scene.AllScenes)
                l_scene.OnFramePass(l_time);

            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnRender, l_time);

            m_renderStage = false;
        }

        internal static ivec2 RenderSize => ms_renderWindow;

        public RenderMode Mode
        {
            get => m_renderMode;
            set
            {
                m_renderMode = value;
                switch(m_renderMode)
                {
                    case RenderMode.Triangles:
                        GLSettings.SetFillMode(PolygonMode.Fill);
                        break;

                    case RenderMode.Lines:
                        GLSettings.SetFillMode(PolygonMode.Line);
                        break;

                    case RenderMode.Points:
                        GLSettings.SetFillMode(PolygonMode.Point);
                        break;
                }
            }
        }
    }
}
