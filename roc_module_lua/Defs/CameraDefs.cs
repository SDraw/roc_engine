using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class CameraDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_matrix4Type = typeof(Matrix4);
        static readonly Type ms_vector2Type = typeof(Vector2);
        static readonly Type ms_vector4Type = typeof(Vector4);

        static CameraDefs()
        {
            Definition.m_name = nameof(Camera);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),

                ("isValid", ComponentDefs.IsValid, null),
                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("aspectRatio", GetAspectRatio, SetAspectRatio),
                ("depth", GetDepth, SetDepth),
                ("fov", GetFOV, SetFOV),
                ("orthoParams", GetOrthoParams, SetOrthoParams),
                ("priority", GetPriority, SetPriority),
                ("projection", GetProjectionType, SetProjectionType),
                ("shadowsRadius", GetShadowsRadius, SetShadowsRadius),
                ("projectionMatrix", GetProjectionMatrix, null),
                ("viewMatrix", GetViewMatrix, null),
                ("viewProjectionMatrix", GetViewProjectionMatrix, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Destroy", ComponentDefs.Destroy),

                ("IsVisible", IsVisible)
            };
        }

        static int GetAspectRatio(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_camera.AspectRatio);
            return 1;
        }
        static int SetAspectRatio(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_camera.AspectRatio = l_val;
            return 0;
        }

        static int GetDepth(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Vector2(l_camera.Depth), ms_vector2Type);
            return 1;
        }
        static int SetDepth(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadObject(out Vector2 l_vec))
                return 0;

            l_camera.Depth = l_vec.m_vector;
            return 0;
        }

        static int GetFOV(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_camera.FOV);
            return 1;
        }
        static int SetFOV(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_camera.FOV = l_val;
            return 0;
        }

        static int GetOrthoParams(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Vector4(l_camera.OrthogonalParams), ms_vector4Type);
            return 1;
        }
        static int SetOrthoParams(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadObject(out Vector4 l_vec))
                return 0;

            l_camera.OrthogonalParams = l_vec.m_vector;
            return 0;
        }

        static int GetPriority(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_camera.Priority);
            return 1;
        }
        static int SetPriority(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadInteger(out int l_val))
                return 0;

            l_camera.Priority = l_val;
            return 0;
        }

        static int GetProjectionType(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushString(l_camera.Projection.ToString());
            return 1;
        }
        static int SetProjectionType(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadEnum(out Camera.ProjectionType l_val))
                return 0;

            l_camera.Projection = l_val;
            return 0;
        }

        static int GetShadowsRadius(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_camera.ShadowsRadius);
            return 1;
        }
        static int SetShadowsRadius(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_camera.ShadowsRadius = l_val;
            return 0;
        }

        static int GetProjectionMatrix(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Matrix4(l_camera.ProjectionMatrix), ms_matrix4Type);
            return 1;
        }

        static int GetViewMatrix(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Matrix4(l_camera.ViewMatrix), ms_matrix4Type);
            return 1;
        }

        static int GetViewProjectionMatrix(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Matrix4(l_camera.ViewProjectionMatrix), ms_matrix4Type);
            return 1;
        }

        static int IsVisible(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Camera l_camera) || !l_reader.ReadObject(out Vector3 l_vec) || !l_reader.ReadNumber(out float l_rad))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(l_camera.IsVisible(l_vec.m_vector, l_rad));
            return 1;
        }
    }
}
