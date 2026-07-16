using ROC.Engine.Objects;
using ROC.Engine.Objects.Components;
using ROC.Engine.Objects.Resources;
using System.Collections.Generic;

namespace ROC.Engine
{
    public static class InstanceBuilder
    {
        public static GameObject Instantiate(string p_name, Model p_model)
        {
            if(p_model == null || !p_model.IsLoaded)
                return null;

            GameObject l_go = new GameObject(p_name);

            switch(p_model.TypeOfModel)
            {
                case Model.ModelType.Static:
                {
                    MeshRenderer l_renderer = l_go.AddComponent(Component.ComponentType.MeshRenderer) as MeshRenderer;
                    l_renderer.Setup(p_model);
                }
                break;

                case Model.ModelType.Rigged:
                {
                    Animator l_animator = l_go.AddComponent(Component.ComponentType.Animator) as Animator;
                    MeshRenderer l_renderer = l_go.AddComponent(Component.ComponentType.MeshRenderer) as MeshRenderer;

                    List<GameObject> l_bones = new List<GameObject>();
                    for(int i = 0, j = p_model.Bones.Count; i < j; i++)
                    {
                        GameObject l_bone = new GameObject(p_model.Bones[i].m_name);
                        l_bone.LocalPosition = p_model.Bones[i].m_position;
                        l_bone.LocalRotation = p_model.Bones[i].m_rotation;
                        l_bone.LocalScale = p_model.Bones[i].m_scale;
                        l_bones.Add(l_bone);
                    }

                    for(int i = 0, j = p_model.Bones.Count; i < j; i++)
                    {
                        if(p_model.Bones[i].m_parent == -1)
                            l_bones[i].Parent = l_go;
                        else
                            l_bones[i].Parent = l_bones[p_model.Bones[i].m_parent];
                    }

                    l_renderer.Setup(p_model, l_bones);
                    l_animator.MeshRenderer = l_renderer;
                }
                break;
            }

            return l_go;
        }
    }
}
