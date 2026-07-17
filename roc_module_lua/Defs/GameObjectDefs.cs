using LuaSharp.Lua;
using ROC.Engine.Objects;
using ROC.Engine.Objects.Components;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class GameObjectDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_gameObjectType = typeof(GameObject);
        static readonly Type ms_vector3Type = typeof(Wrappers.Vector3);
        static readonly Type ms_quaternionType = typeof(Wrappers.Quaternion);
        static readonly Type ms_matrix4Type = typeof(Wrappers.Matrix4);

        static GameObjectDefs()
        {
            Definition.m_name = nameof(GameObject);
            Definition.m_constructor = Create;

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Instantiate", Instantiate)
            };

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("name", GetName, SetName),
                ("isValid", IsValid, null),
                ("parent", GetParent, SetParent),
                ("children", GetChildren, null),
                ("localPosition", GetLocalPosition, SetLocalPosition),
                ("localRotation", GetLocalRotation, SetLocalRotation),
                ("localScale", GetLocalScale, SetLocalScale),
                ("localMatrix", GetLocalMatrix, null),
                ("position", GetPosition, SetPosition),
                ("rotation", GetRotation, SetRotation),
                ("scale", GetScale, SetScale),
                ("matrix", GetMatrix, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("AddComponent", AddComponent),
                ("GetComponent", GetComponent),
                ("GetComponents", GetComponents),
                ("FindChild", FindChild)
            };
        }

        // Constructor
        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.Skip();
            if(!l_argReader.ReadString(out var l_string))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(GameObject.Create(l_string), ms_gameObjectType);
            return 1;
        }

        // Instancing
        static int Instantiate(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadString(out string l_name) || !l_argReader.ReadObject(out Model l_model))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            var l_go = Engine.InstanceBuilder.Instantiate(l_name, l_model);
            if(l_go != null)
                l_argReader.PushObject(l_go, ms_gameObjectType);
            else
                l_argReader.PushBoolean(false);
            return 1;
        }

        // Validation
        static int IsValid(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_go.IsValid);
            return 1;
        }

        // Name
        static int GetName(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_go.Name);
            return 1;
        }

        static int SetName(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
                return 0;

            if(!l_argReader.ReadString(out string p_name))
                return 0;
            l_go.Name = p_name;
            return 0;
        }

        // Parent
        static int GetParent(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            if(l_go.Parent != null)
                l_argReader.PushObject(l_go.Parent, ms_gameObjectType);
            else
                l_argReader.PushBoolean(false);

            return 1;
        }

        static int SetParent(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
                return 0;

            if(l_argReader.IsNextNil())
            {
                l_go.Parent = null;
                return 0;
            }

            if(!l_argReader.ReadObject(out GameObject l_parent))
                return 0;

            l_go.Parent = l_parent;
            return 0;
        }

        static int GetChildren(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushTable(l_go.Children);
            return 1;
        }

        // Local transform
        static int GetLocalPosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Vector3(l_go.LocalPosition), ms_vector3Type);
            return 1;
        }
        static int SetLocalPosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadObject(out Wrappers.Vector3 l_vec))
                return 0;

            l_go.LocalPosition = l_vec.m_vector;
            return 0;
        }

        static int GetLocalRotation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Quaternion(l_go.LocalRotation), ms_quaternionType);
            return 1;
        }
        static int SetLocalRotation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadObject(out Wrappers.Quaternion l_quat))
                return 0;

            l_go.LocalRotation = l_quat.m_quat;
            return 0;
        }

        static int GetLocalScale(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Vector3(l_go.LocalScale), ms_vector3Type);
            return 1;
        }
        static int SetLocalScale(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadObject(out Wrappers.Vector3 l_vec))
                return 0;

            l_go.LocalScale = l_vec.m_vector;
            return 0;
        }

        static int GetLocalMatrix(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Matrix4(l_go.LocalMatrix), ms_matrix4Type);
            return 1;
        }

        // Gloabl transform
        static int GetPosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Vector3(l_go.Position), ms_vector3Type);
            return 1;
        }
        static int SetPosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadObject(out Wrappers.Vector3 l_vec))
                return 0;

            l_go.Position = l_vec.m_vector;
            return 0;
        }

        static int GetRotation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Quaternion(l_go.Rotation), ms_quaternionType);
            return 1;
        }
        static int SetRotation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadObject(out Wrappers.Quaternion l_quat))
                return 0;

            l_go.Rotation = l_quat.m_quat;
            return 0;
        }

        static int GetScale(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Vector3(l_go.Scale), ms_vector3Type);
            return 1;
        }
        static int SetScale(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadObject(out Wrappers.Vector3 l_vec))
                return 0;

            l_go.Scale = l_vec.m_vector;
            return 0;
        }

        static int GetMatrix(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Wrappers.Matrix4(l_go.Matrix), ms_matrix4Type);
            return 1;
        }

        // Components
        static int AddComponent(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadEnum(out Component.ComponentType l_type))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            Component l_component = l_go.AddComponent(l_type);
            if(l_component == null)
                l_argReader.PushBoolean(false);
            else
                l_argReader.PushObject(l_component);

            return 1;
        }

        static int GetComponent(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadEnum(out Component.ComponentType l_type))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            Component l_component = l_go.GetComponent(l_type);
            if(l_component == null)
                l_argReader.PushBoolean(false);
            else
                l_argReader.PushObject(l_component);

            return 1;
        }

        static int GetComponents(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadEnum(out Component.ComponentType l_type))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushTable(l_go.GetComponents(l_type));
            return 1;
        }

        static int FindChild(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out GameObject l_go) || !l_argReader.ReadString(out string l_name))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            var l_child = l_go.FindChild(l_name);
            if(l_child != null)
                l_argReader.PushObject(l_child, ms_gameObjectType);
            else
                l_argReader.PushBoolean(false);

            return 1;
        }
    }
}
