using LuaSharp.Lua;
using ROC.Engine.Core;
using ROC.Module.Defs;
using ROC.Modules;
using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;

namespace ROC.Module
{
    public sealed class LuaModule : IModule
    {
        readonly LuaVM m_vm = null;
        readonly Dictionary<ModuleEvent, int> m_eventsMap = null;

        public LuaModule()
        {
            m_eventsMap = new Dictionary<ModuleEvent, int>();

            m_vm = new LuaVM();
            m_vm.ErrorMessage.AddListener(this.OnAnyMessage);
            m_vm.WarningMessage.AddListener(this.OnAnyMessage);

            BindDefinitions();
        }

        void IModule.OnEvent(ModuleEvent p_event)
        {
            switch(p_event)
            {
                case ModuleEvent.OnEngineStart:
                {
                    LoadScripts();

                    if(m_eventsMap.TryGetValue(p_event, out int l_ref))
                        m_vm.CallFunction(l_ref);
                }
                break;

                case ModuleEvent.OnEngineStop:
                {
                    if(m_eventsMap.TryGetValue(p_event, out int l_ref))
                        m_vm.CallFunction(l_ref);

                    m_vm.PerformFullGC();

                    m_eventsMap.Clear();
                    m_vm.Dispose();
                }
                break;

                case ModuleEvent.OnUpdate:
                {
                    m_vm.PerformStepGC();

                    if(m_eventsMap.TryGetValue(p_event, out int l_ref))
                        m_vm.CallFunction(l_ref);
                }
                break;

                default:
                {
                    if(m_eventsMap.TryGetValue(p_event, out int l_ref))
                        m_vm.CallFunction(l_ref);
                }
                break;
            }
        }

        void IModule.OnEvent(ModuleEvent p_event, params object[] p_args)
        {
            if(m_eventsMap.TryGetValue(p_event, out int l_ref))
                m_vm.CallFunction(l_ref, p_args);
        }

        void OnAnyMessage(string p_msg) => Core.Instance.LogManager.Log(p_msg);

        void BindDefinitions()
        {
            // Bind
            m_vm.RegisterClass(Vector2Defs.Definition);
            m_vm.RegisterClass(Vector3Defs.Definition);
            m_vm.RegisterClass(Vector4Defs.Definition);
            m_vm.RegisterClass(QuaternionDefs.Definition);
            m_vm.RegisterClass(Matrix2Defs.Definition);
            m_vm.RegisterClass(Matrix3Defs.Definition);
            m_vm.RegisterClass(Matrix4Defs.Definition);

            m_vm.RegisterClass(ObjectDefs.Definition);
            m_vm.RegisterClass(SceneDefs.Definition);
            m_vm.RegisterClass(GameObjectDefs.Definition);
            m_vm.RegisterClass(MaterialDefs.Definition);

            m_vm.RegisterClass(ResourceDefs.Definition);
            m_vm.RegisterClass(ModelDefs.Definition);
            m_vm.RegisterClass(AnimationDefs.Definition);
            m_vm.RegisterClass(ShaderDefs.Definition);
            m_vm.RegisterClass(AudioClipDefs.Definition);
            m_vm.RegisterClass(TextureDefs.Definition);

            m_vm.RegisterClass(ComponentDefs.Definition);
            m_vm.RegisterClass(CameraDefs.Definition);
            m_vm.RegisterClass(LightDefs.Definition);
            m_vm.RegisterClass(AnimatorDefs.Definition);
            m_vm.RegisterClass(MeshRendererDefs.Definition);
            m_vm.RegisterClass(AudioListenerDefs.Definition);
            m_vm.RegisterClass(AudioSourceDefs.Definition);

            m_vm.RegisterClass(ColliderDefs.Definition);
            m_vm.RegisterClass(SphereColliderDefs.Definition);
            m_vm.RegisterClass(BoxColliderDefs.Definition);
            m_vm.RegisterClass(CapsuleColliderDefs.Definition);
            m_vm.RegisterClass(ConeColliderDefs.Definition);
            m_vm.RegisterClass(CylinderColliderDefs.Definition);

            m_vm.RegisterClass(PhysicsDef.Definition);
            m_vm.RegisterClass(TimeDefs.Definition);
            m_vm.RegisterClass(WindowDefs.Definition);
            m_vm.RegisterClass(InputDefs.Definition);
            m_vm.RegisterClass(RenderDefs.Definition);

            m_vm.RegisterClass(UtilitiesDefs.Definition);
        }

        void LoadScripts()
        {
            // Get scripts from config
            List<string> l_scripts = new List<string>();
            try
            {
                XmlDocument l_doc = new XmlDocument();
                l_doc.Load("module_lua.xml");
                foreach(XmlElement l_node in l_doc["module"].ChildNodes)
                {
                    if(l_node.Name != "script")
                        continue;

                    l_scripts.Add(l_node.Attributes["src"].Value);
                }
            }
            catch(Exception e)
            {
                Core.Instance.LogManager.Log(e.Message);
            }

            foreach(var l_script in l_scripts)
            {
                try
                {
                    m_vm.Execute(l_script, File.ReadAllBytes(l_script));
                    Core.Instance.LogManager.Log(string.Format("Script '{0}' is loaded", l_script));
                }
                catch(Exception e)
                {
                    Core.Instance.LogManager.Log(e.Message);
                }
            }

            foreach(ModuleEvent l_event in Enum.GetValues(typeof(ModuleEvent)))
            {
                string l_eventName = l_event.ToString();
                if(m_vm.IsFunctionPresent(l_eventName))
                    m_eventsMap.Add(l_event, m_vm.GetFunctionRef(l_eventName));
            }
        }
    }
}
