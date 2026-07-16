using ROC.Engine.Managers;
using System;

namespace ROC.Engine.Core
{
    public sealed class Core
    {
        public static Core Instance
        {
            get;
            private set;
        }

        readonly ConfigManager m_configManager = null;
        readonly LogManager m_logManager = null;
        readonly SoundManager m_soundManager = null;
        readonly PhysicsManager m_physicsManager = null;
        readonly WindowManager m_windowManager = null;
        readonly RenderManager m_renderManager = null;
        readonly ModuleManager m_moduleManager = null;

        bool m_active = false;

        internal Core()
        {
            if(Instance != null)
                throw new Exception("Only sinlge instance is allowed");
            else
                Instance = this;

            m_configManager = new ConfigManager();
            m_logManager = new LogManager();
            m_soundManager = new SoundManager();
            m_physicsManager = new PhysicsManager();
            m_windowManager = new WindowManager();
            m_renderManager = new RenderManager();
            m_moduleManager = new ModuleManager();
        }

        ~Core()
        {
            if(Instance == this)
                Instance = null;
        }

        internal void Start()
        {
            if(m_active)
                return;

            m_configManager.Start();
            if(m_configManager.Console)
            {
                IntPtr l_handle = Utils.GetConsoleWindow();
                if(l_handle == IntPtr.Zero)
                    Utils.AllocConsole();
                else
                    Utils.ShowWindow(l_handle, 5);
            }

            m_logManager.Start();
            m_soundManager.Start();
            m_physicsManager.Start();
            m_windowManager.Start();
            m_renderManager.Start();
            m_moduleManager.Start();
            DeltaTick.Start();

            m_active = true;
            m_moduleManager.SendEvent(Modules.ModuleEvent.OnEngineStart);
        }

        internal void Stop()
        {
            if(!m_active)
                return;

            m_moduleManager.SendEvent(Modules.ModuleEvent.OnEngineStop);
            m_active = false;

            DeltaTick.Stop();
            m_moduleManager.Stop();
            m_renderManager.Stop();
            m_physicsManager.Stop();
            m_soundManager.Stop();
            m_windowManager.Stop();
            m_logManager.Stop();
            m_configManager.Stop();

            if(m_configManager.Console)
                Utils.ShowWindow(Utils.GetConsoleWindow(), 0);
        }

        internal bool DoPulse()
        {
            if(!m_active)
                return false;

            DeltaTick.Update();

            // Physics
            m_physicsManager.DoPulse();

            // Input
            bool l_loopState = m_windowManager.DoPulse_S1();

            // Update event
            m_moduleManager.SendEvent(Modules.ModuleEvent.OnUpdate);

            // Post-render
            m_renderManager.DoPulse();

            // Buffer swap
            l_loopState = (l_loopState && m_windowManager.DoPulse_S2());
            return l_loopState;
        }

        public ConfigManager ConfigManager => m_configManager;
        public LogManager LogManager => m_logManager;
        public SoundManager SoundManager => m_soundManager;
        public PhysicsManager PhysicsManager => m_physicsManager;
        public WindowManager WindowManager => m_windowManager;
        public RenderManager RenderManager => m_renderManager;
        internal ModuleManager ModuleManager => m_moduleManager;
    }
}
