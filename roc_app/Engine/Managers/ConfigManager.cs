using GlmSharp;
using System;
using System.Collections.Generic;
using System.Xml;

namespace ROC.Engine.Managers
{
    public sealed class ConfigManager : Manager
    {
        bool m_logging = true;
        bool m_fullscreen = false;
        uint m_antialiasing = 0U;
        uvec2 m_windowSize = new uvec2(640U, 480U);
        uint m_fpsLimit = 60U;
        bool m_vsync = false;
        int m_shadowsSize = 2048;
        bool m_console = false;
        readonly List<string> m_modules = new List<string>();

        internal ConfigManager()
        {
        }

        internal override void Start()
        {
            if(m_active)
                return;

            try
            {
                XmlDocument l_doc = new XmlDocument();
                l_doc.Load("settings.xml");
                foreach(XmlElement l_node in l_doc["settings"].ChildNodes)
                {
                    if(l_node.Name == "setting")
                    {
                        switch(l_node.Attributes["name"].Value)
                        {
                            case "logging":
                                m_logging = bool.Parse(l_node.Attributes["value"].Value);
                                break;

                            case "fullscreen":
                                m_fullscreen = bool.Parse(l_node.Attributes["value"].Value);
                                break;

                            case "antialiasing":
                                m_antialiasing = uint.Parse(l_node.Attributes["value"].Value);
                                break;

                            case "window":
                            {
                                m_windowSize.x = uint.Parse(l_node.Attributes["valueW"].Value);
                                m_windowSize.y = uint.Parse(l_node.Attributes["valueH"].Value);
                            }
                            break;

                            case "fpsLimit":
                                m_fpsLimit = uint.Parse(l_node.Attributes["value"].Value);
                                break;

                            case "vsync":
                                m_vsync = bool.Parse(l_node.Attributes["value"].Value);
                                break;

                            case "shadows":
                                m_shadowsSize = int.Parse(l_node.Attributes["value"].Value);
                                break;

                            case "module":
                                m_modules.Add(l_node.Attributes["value"].Value);
                                break;

                            case "console":
                                m_console = bool.Parse(l_node.Attributes["value"].Value);
                                break;
                        }
                    }
                }
            }
            catch(Exception e)
            {
                System.Console.WriteLine(e.Message);
            }

            m_shadowsSize = Math.Min(Math.Max(2, (int)Math.Pow(2.0, Math.Floor(Math.Log(m_shadowsSize, 2.0)))), 16384);

            m_active = true;
        }

        public bool IsLogEnabled => m_logging;
        public bool IsFullscreen => m_fullscreen;
        public uint AntialiasingLevel => m_antialiasing;
        public uvec2 WindowSize => m_windowSize;
        public uint FpsLimit => m_fpsLimit;
        public bool VSync => m_vsync;
        public int ShadowsSize => m_shadowsSize;
        public List<string> Modules => m_modules;
        public bool Console => m_console;
    }
}
