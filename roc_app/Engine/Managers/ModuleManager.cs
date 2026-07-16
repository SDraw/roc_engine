using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace ROC.Engine.Managers
{
    internal sealed class ModuleManager : Manager
    {
        readonly List<Modules.IModule> m_modules = null;
        readonly List<Assembly> m_libraries = null;
        static readonly Type ms_moduleType = typeof(Modules.IModule);
        static readonly string ms_appPath = AppDomain.CurrentDomain.BaseDirectory;

        internal ModuleManager()
        {
            m_modules = new List<Modules.IModule>();
            m_libraries = new List<Assembly>();
        }

        internal override void Start()
        {
            if(m_active)
                return;

            foreach(string l_moduleFile in Core.Core.Instance.ConfigManager.Modules)
            {
                try
                {
                    var l_library = Assembly.LoadFile(Path.Combine(ms_appPath, l_moduleFile));
                    foreach(Type l_type in l_library.GetExportedTypes())
                    {
                        if(ms_moduleType.IsAssignableFrom(l_type))
                        {
                            m_modules.Add(Activator.CreateInstance(l_type) as Modules.IModule);
                            m_libraries.Add(l_library);
                            break;
                        }
                    }
                }
                catch(Exception e)
                {
                    Core.Core.Instance.LogManager.Log(string.Format("Error loading {0}: {1}", l_moduleFile, e.Message));
                }
            }

            m_active = true;
        }

        internal override void Stop()
        {
            if(!m_active)
                return;

            m_modules.Clear();
            m_libraries.Clear();

            m_active = false;
        }

        internal void SendEvent(Modules.ModuleEvent p_event)
        {
            foreach(var l_module in m_modules)
                l_module.OnEvent(p_event);
        }

        internal void SendEvent(Modules.ModuleEvent p_event, params object[] p_args)
        {
            foreach(var l_module in m_modules)
                l_module.OnEvent(p_event, p_args);
        }
    }
}
