using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class CameraDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_matrix4Type = typeof(Matrix4);
        static readonly Type ms_vector2Type = typeof(Vector2);
        static readonly Type ms_vector4Type = typeof(Vector4);

        static CameraDefs()
        {
            Definition.m_name = nameof(Camera);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

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
                ("IsVisible", IsVisible)
            };
        }

        static int GetAspectRatio(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_camera.AspectRatio);
            return 1;
        }
        static int SetAspectRatio(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_camera.AspectRatio = l_val;
            return 0;
        }

        static int GetDepth(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector2(l_camera.Depth), ms_vector2Type);
            return 1;
        }
        static int SetDepth(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadObject(out Vector2 l_vec))
                return 0;

            l_camera.Depth = l_vec.m_vector;
            return 0;
        }

        static int GetFOV(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_camera.FOV);
            return 1;
        }
        static int SetFOV(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_camera.FOV = l_val;
            return 0;
        }

        static int GetOrthoParams(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_camera.OrthogonalParams), ms_vector4Type);
            return 1;
        }
        static int SetOrthoParams(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_camera.OrthogonalParams = l_vec.m_vector;
            return 0;
        }

        static int GetPriority(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_camera.Priority);
            return 1;
        }
        static int SetPriority(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadInteger(out int l_val))
                return 0;

            l_camera.Priority = l_val;
            return 0;
        }

        static int GetProjectionType(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_camera.Projection.ToString());
            return 1;
        }
        static int SetProjectionType(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadEnum(out Camera.ProjectionType l_val))
                return 0;

            l_camera.Projection = l_val;
            return 0;
        }

        static int GetShadowsRadius(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_camera.ShadowsRadius);
            return 1;
        }
        static int SetShadowsRadius(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_camera.ShadowsRadius = l_val;
            return 0;
        }

        static int GetProjectionMatrix(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Matrix4(l_camera.ProjectionMatrix), ms_matrix4Type);
            return 1;
        }

        static int GetViewMatrix(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Matrix4(l_camera.ViewMatrix), ms_matrix4Type);
            return 1;
        }

        static int GetViewProjectionMatrix(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Matrix4(l_camera.ViewProjectionMatrix), ms_matrix4Type);
            return 1;
        }

        static int IsVisible(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Camera l_camera) || !l_argReader.ReadObject(out Vector3 l_vec) || !l_argReader.ReadNumber(out float l_rad))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_camera.IsVisible(l_vec.m_vector, l_rad));
            return 1;
        }
    }
}
