using glTFLoader;
using System;
using System.Collections.Generic;
using System.IO;
using System.Numerics;
using System.Text;

namespace ROC.Converter
{
    internal class GLTFModel
    {
        const byte c_unlitBit = 0b1;
        const byte c_depthWriteBit = 0b10;
        const byte c_transparencyBit = 0b100;
        const byte c_doubleSidedBit = 0b1000;
        const byte c_filteringBit = 0b10000;

        bool m_loaded = false;
        string m_name = "";
        float m_boundsRadius = 0f;

        struct SubMeshData
        {
            public float[] m_vertexData;
            public float[] m_normalData;
            public float[] m_uvData;
            public float[] m_weightsData;
            public int[] m_weightIndiciesData;
            public bool m_unlit;
            public bool m_transparent;
            public bool m_depthWrite;
            public bool m_doubleSided;
            public bool m_nearestFilter;
            public Vector4 m_color;
            public Vector4 m_params;
            public byte[] m_textureData;
            public string m_textureName;
        }
        readonly List<SubMeshData> m_submeshes = new List<SubMeshData>();

        struct BoneData
        {
            public string m_name;
            public int m_parent;
            public Vector3 m_localPosition;
            public Quaternion m_localRotation;
            public Vector3 m_localScale;
        }
        readonly List<BoneData> m_bones = new List<BoneData>();

        readonly List<GLTFAnimation> m_animations = new List<GLTFAnimation>();

        public void Load(string p_filePath)
        {
            if(m_loaded)
                return;

            try
            {
                m_name = Path.GetFileNameWithoutExtension(p_filePath);

                var l_gltfModel = Interface.LoadModel(p_filePath);

                List<byte[]> l_binaryBuffers = new List<byte[]>();
                foreach(var l_buffer in l_gltfModel.Buffers)
                {
                    if(l_buffer.Uri != null)
                        l_binaryBuffers.Add(Convert.FromBase64String(l_buffer.Uri.Split(',')[1]));
                    else
                        l_binaryBuffers.Add(Interface.LoadBinaryBuffer(l_gltfModel, l_binaryBuffers.Count, p_filePath));
                }

                var l_mainScene = l_gltfModel.Scenes[l_gltfModel.Scene.Value];
                var l_firstNode = l_gltfModel.Nodes[l_mainScene.Nodes[0]];

                glTFLoader.Schema.Mesh l_mesh = (l_firstNode.Mesh.HasValue) ? l_gltfModel.Meshes[l_firstNode.Mesh.Value] : null;
                glTFLoader.Schema.Skin l_skin = (l_firstNode.Skin.HasValue) ? l_gltfModel.Skins[l_firstNode.Skin.Value] : null;

                if((l_mesh == null) && (l_firstNode.Children != null))
                {
                    foreach(var l_nodeID in l_firstNode.Children)
                    {
                        // Find only first node with mesh
                        if(l_gltfModel.Nodes[l_nodeID].Mesh.HasValue)
                        {
                            l_mesh = l_gltfModel.Meshes[l_gltfModel.Nodes[l_nodeID].Mesh.Value];

                            if(l_gltfModel.Nodes[l_nodeID].Skin.HasValue)
                                l_skin = l_gltfModel.Skins[l_gltfModel.Nodes[l_nodeID].Skin.Value];

                            break;
                        }
                    }
                }

                if(l_mesh == null)
                    throw new Exception("No mesh found");

                Vector3 l_boundsSize = Vector3.Zero;
                foreach(var l_primitive in l_mesh.Primitives)
                {
                    if(l_primitive.Mode != glTFLoader.Schema.MeshPrimitive.ModeEnum.TRIANGLES)
                        continue;

                    // POSITION, NORMAL, TEXCOORD_0, WEIGHTS_0, JOINTS_0
                    float[] l_vertexFlat = Utils.GetAccessorData<float>(l_gltfModel, l_gltfModel.Accessors[l_primitive.Attributes["POSITION"]], l_binaryBuffers);
                    float[] l_normalFlat = Utils.GetAccessorData<float>(l_gltfModel, l_gltfModel.Accessors[l_primitive.Attributes["NORMAL"]], l_binaryBuffers);
                    float[] l_uvFlat = Utils.GetAccessorData<float>(l_gltfModel, l_gltfModel.Accessors[l_primitive.Attributes["TEXCOORD_0"]], l_binaryBuffers);

                    float[] l_weightsFlat = (l_skin != null) ? Utils.GetAccessorData<float>(l_gltfModel, l_gltfModel.Accessors[l_primitive.Attributes["WEIGHTS_0"]], l_binaryBuffers) : null;
                    int[] l_indexFlat = (l_skin != null) ? Array.ConvertAll(Utils.GetAccessorData<byte>(l_gltfModel, l_gltfModel.Accessors[l_primitive.Attributes["JOINTS_0"]], l_binaryBuffers), (val) => (int)val) : null;

                    int[] l_faceIndex = null;
                    if(l_gltfModel.Accessors[l_primitive.Indices.Value].ComponentType == glTFLoader.Schema.Accessor.ComponentTypeEnum.UNSIGNED_SHORT)
                        l_faceIndex = Array.ConvertAll(Utils.GetAccessorData<ushort>(l_gltfModel, l_gltfModel.Accessors[l_primitive.Indices.Value], l_binaryBuffers), (val) => (int)val);
                    else if(l_gltfModel.Accessors[l_primitive.Indices.Value].ComponentType == glTFLoader.Schema.Accessor.ComponentTypeEnum.UNSIGNED_INT)
                        l_faceIndex = Array.ConvertAll(Utils.GetAccessorData<uint>(l_gltfModel, l_gltfModel.Accessors[l_primitive.Indices.Value], l_binaryBuffers), (val) => (int)val);
                    if(l_faceIndex == null)
                        return;

                    int l_trianglesCount = l_faceIndex.Length / 3;
                    SubMeshData l_meshData = new SubMeshData();
                    l_meshData.m_vertexData = new float[l_trianglesCount * 9];
                    l_meshData.m_normalData = new float[l_trianglesCount * 9];
                    l_meshData.m_uvData = new float[l_trianglesCount * 6];
                    l_meshData.m_weightsData = (l_weightsFlat != null) ? new float[l_trianglesCount * 12] : null;
                    l_meshData.m_weightIndiciesData = (l_indexFlat != null) ? new int[l_trianglesCount * 12] : null;
                    for(int i = 0, j = l_trianglesCount; i < j; i++)
                    {
                        Array.Copy(l_vertexFlat, l_faceIndex[i * 3] * 3, l_meshData.m_vertexData, i * 9, 3);
                        Array.Copy(l_vertexFlat, l_faceIndex[i * 3 + 1] * 3, l_meshData.m_vertexData, i * 9 + 3, 3);
                        Array.Copy(l_vertexFlat, l_faceIndex[i * 3 + 2] * 3, l_meshData.m_vertexData, i * 9 + 6, 3);

                        Array.Copy(l_normalFlat, l_faceIndex[i * 3] * 3, l_meshData.m_normalData, i * 9, 3);
                        Array.Copy(l_normalFlat, l_faceIndex[i * 3 + 1] * 3, l_meshData.m_normalData, i * 9 + 3, 3);
                        Array.Copy(l_normalFlat, l_faceIndex[i * 3 + 2] * 3, l_meshData.m_normalData, i * 9 + 6, 3);

                        Array.Copy(l_uvFlat, l_faceIndex[i * 3] * 2, l_meshData.m_uvData, i * 6, 2);
                        Array.Copy(l_uvFlat, l_faceIndex[i * 3 + 1] * 2, l_meshData.m_uvData, i * 6 + 2, 2);
                        Array.Copy(l_uvFlat, l_faceIndex[i * 3 + 2] * 2, l_meshData.m_uvData, i * 6 + 4, 2);

                        if(l_weightsFlat != null)
                        {
                            Array.Copy(l_weightsFlat, l_faceIndex[i * 3] * 4, l_meshData.m_weightsData, i * 12, 4);
                            Array.Copy(l_weightsFlat, l_faceIndex[i * 3 + 1] * 4, l_meshData.m_weightsData, i * 12 + 4, 4);
                            Array.Copy(l_weightsFlat, l_faceIndex[i * 3 + 2] * 4, l_meshData.m_weightsData, i * 12 + 8, 4);
                        }
                        if(l_indexFlat != null)
                        {
                            Array.Copy(l_indexFlat, l_faceIndex[i * 3] * 4, l_meshData.m_weightIndiciesData, i * 12, 4);
                            Array.Copy(l_indexFlat, l_faceIndex[i * 3 + 1] * 4, l_meshData.m_weightIndiciesData, i * 12 + 4, 4);
                            Array.Copy(l_indexFlat, l_faceIndex[i * 3 + 2] * 4, l_meshData.m_weightIndiciesData, i * 12 + 8, 4);
                        }
                    }

                    if(l_primitive.Material.HasValue)
                    {
                        var l_material = l_gltfModel.Materials[l_primitive.Material.Value];
                        switch(l_material.AlphaMode)
                        {
                            case glTFLoader.Schema.Material.AlphaModeEnum.OPAQUE:
                            {
                                l_meshData.m_depthWrite = true;
                                l_meshData.m_transparent = false;
                            }
                            break;

                            case glTFLoader.Schema.Material.AlphaModeEnum.MASK:
                            {
                                l_meshData.m_depthWrite = true;
                                l_meshData.m_transparent = true;
                            }
                            break;

                            case glTFLoader.Schema.Material.AlphaModeEnum.BLEND:
                            {
                                l_meshData.m_depthWrite = false;
                                l_meshData.m_transparent = true;
                            }
                            break;
                        }

                        l_meshData.m_doubleSided = l_material.DoubleSided;
                        switch(l_material.PbrMetallicRoughness.BaseColorFactor.Length)
                        {
                            case 3:
                                l_meshData.m_color = new Vector4(l_material.PbrMetallicRoughness.BaseColorFactor[0], l_material.PbrMetallicRoughness.BaseColorFactor[1], l_material.PbrMetallicRoughness.BaseColorFactor[2], 1f);
                                break;

                            case 4:
                                l_meshData.m_color = new Vector4(l_material.PbrMetallicRoughness.BaseColorFactor[0], l_material.PbrMetallicRoughness.BaseColorFactor[1], l_material.PbrMetallicRoughness.BaseColorFactor[2], l_material.PbrMetallicRoughness.BaseColorFactor[3]);
                                break;

                            default:
                                l_meshData.m_color = Vector4.One;
                                break;
                        }

                        if(Math.Max(Math.Max(l_material.EmissiveFactor[0], l_material.EmissiveFactor[1]), l_material.EmissiveFactor[2]).IsEpsilonEqual(0f))
                            l_meshData.m_unlit = false;
                        else
                            l_meshData.m_unlit = true;

                        l_meshData.m_nearestFilter = false;
                        if(l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Sampler.HasValue)
                        {
                            var l_sampler = l_gltfModel.Samplers[l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Sampler.Value];
                            switch(l_sampler.MagFilter)
                            {
                                case glTFLoader.Schema.Sampler.MagFilterEnum.NEAREST:
                                    l_meshData.m_nearestFilter = true;
                                    break;
                            }
                        }

                        if(l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Source.HasValue)
                        {
                            var l_image = l_gltfModel.Images[l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Source.Value];
                            l_meshData.m_textureName = l_image.Name + Utils.GetExtensionByType(l_image.MimeType);
                            if(l_image.Uri != null)
                            {
                                if(l_image.Uri.StartsWith("data"))
                                    l_meshData.m_textureData = Convert.FromBase64String(l_image.Uri.Split(',')[1]);
                                else
                                {
                                    l_meshData.m_textureName = l_image.Uri;
                                    l_meshData.m_textureData = null;
                                }
                            }
                            else if(l_image.BufferView.HasValue)
                            {
                                var l_imageBV = l_gltfModel.BufferViews[l_image.BufferView.Value];
                                l_meshData.m_textureData = new byte[l_imageBV.ByteLength];
                                Array.Copy(l_binaryBuffers[l_imageBV.Buffer], l_imageBV.ByteOffset, l_meshData.m_textureData, 0, l_imageBV.ByteLength);
                            }
                        }
                        else
                        {
                            l_meshData.m_textureName = "white.png";
                            l_meshData.m_textureData = null;
                        }
                    }
                    else
                    {
                        l_meshData.m_depthWrite = true;
                        l_meshData.m_unlit = false;
                        l_meshData.m_transparent = false;
                        l_meshData.m_doubleSided = false;
                        l_meshData.m_nearestFilter = false;
                        l_meshData.m_color = Vector4.One;
                        l_meshData.m_textureName = "white.png";
                        l_meshData.m_textureData = null;
                    }

                    l_meshData.m_params = Vector4.One;

                    // Bounds
                    var l_posAccessor = l_gltfModel.Accessors[l_primitive.Attributes["POSITION"]];
                    l_boundsSize = Utils.Max(new Vector3(l_posAccessor.Max[0], l_posAccessor.Max[1], l_posAccessor.Max[2]), l_boundsSize);

                    m_submeshes.Add(l_meshData);
                }

                if(l_skin != null)
                {
                    // Add
                    foreach(int l_jointNodeIndex in l_skin.Joints)
                    {
                        var l_jointNode = l_gltfModel.Nodes[l_jointNodeIndex];
                        BoneData l_boneData = new BoneData();
                        l_boneData.m_parent = -1;
                        l_boneData.m_name = l_jointNode.Name;
                        l_boneData.m_localPosition = new Vector3(l_jointNode.Translation[0], l_jointNode.Translation[1], l_jointNode.Translation[2]);
                        l_boneData.m_localRotation = new Quaternion(l_jointNode.Rotation[0], l_jointNode.Rotation[1], l_jointNode.Rotation[2], l_jointNode.Rotation[3]);
                        l_boneData.m_localScale = new Vector3(l_jointNode.Scale[0], l_jointNode.Scale[1], l_jointNode.Scale[2]);
                        m_bones.Add(l_boneData);
                    }

                    // Set parent
                    int l_parentIndex = -1;
                    foreach(int l_parentNodeIndex in l_skin.Joints)
                    {
                        l_parentIndex++;

                        var l_parentNode = l_gltfModel.Nodes[l_parentNodeIndex];
                        if(l_parentNode.Children == null)
                            continue;

                        foreach(int l_childNodeIndex in l_parentNode.Children)
                        {
                            var l_childNode = l_gltfModel.Nodes[l_childNodeIndex];
                            for(int i = 0, j = m_bones.Count; i < j; i++)
                            {
                                if(m_bones[i].m_name == l_childNode.Name)
                                {
                                    var l_boneData = m_bones[i];
                                    l_boneData.m_parent = l_parentIndex;
                                    m_bones[i] = l_boneData;
                                    break;
                                }
                            }
                        }
                    }
                }

                m_boundsRadius = l_boundsSize.Length();

                if(l_gltfModel.Animations != null && l_gltfModel.Animations.Length > 0)
                {
                    for(int i = 0, j = l_gltfModel.Animations.Length; i < j; i++)
                    {
                        GLTFAnimation l_animation = new GLTFAnimation();
                        l_animation.Parse(l_gltfModel, i, l_binaryBuffers);
                        m_animations.Add(l_animation);
                    }
                }

                m_loaded = true;

                Console.WriteLine("Model '{0}' is loaded", p_filePath);
            }
            catch(Exception e)
            {
                Console.WriteLine("Model load exception: {0} at {1}", e.Message, e.StackTrace);
            }
        }

        public void Save(string p_dir)
        {
            if(!m_loaded)
                return;

            try
            {
                string l_dir = Path.Combine(p_dir, "models");
                if(!Directory.Exists(l_dir))
                    Directory.CreateDirectory(l_dir);

                FileStream l_file = new FileStream(Path.Combine(l_dir, m_name + ".rmf"), FileMode.Create, FileAccess.ReadWrite);
                BinaryWriter l_writer = new BinaryWriter(l_file);

                l_writer.Write((byte)'R'); // Header
                l_writer.Write((byte)'M');
                l_writer.Write((byte)'F');

                if(m_bones.Count > 0)
                    l_writer.Write((byte)2); // Rigged
                else
                    l_writer.Write((byte)1); // Static

                l_writer.Write(m_boundsRadius); // Bounds radius

                l_writer.Write(m_submeshes.Count); // Materials/submeshes count
                foreach(var l_submesh in m_submeshes)
                {
                    byte l_materialType = 0b0;
                    if(!l_submesh.m_unlit)
                        l_materialType |= c_unlitBit; // Shaded
                    if(l_submesh.m_depthWrite)
                        l_materialType |= c_depthWriteBit; // Depth write
                    if(l_submesh.m_transparent)
                        l_materialType |= c_transparencyBit; // Transparent
                    if(l_submesh.m_doubleSided)
                        l_materialType |= c_doubleSidedBit; // Doublesided
                    if(!l_submesh.m_nearestFilter)
                        l_materialType |= c_filteringBit; // 0 - nearest filtering, 1 - linear

                    l_writer.Write(l_materialType); // Material type

                    l_writer.Write(l_submesh.m_color.X); // Material color R
                    l_writer.Write(l_submesh.m_color.Y); // Material color G
                    l_writer.Write(l_submesh.m_color.Z); // Material color B
                    l_writer.Write(l_submesh.m_color.W); // Material color A

                    l_writer.Write(l_submesh.m_params.X); // Material param 0
                    l_writer.Write(l_submesh.m_params.Y); // Material param 1
                    l_writer.Write(l_submesh.m_params.Z); // Material param 2
                    l_writer.Write(l_submesh.m_params.W); // Material param 3

                    string l_path = "textures/" + l_submesh.m_textureName;
                    if(l_submesh.m_textureData != null)
                    {
                        string l_textureDir = Path.Combine(p_dir, "textures");
                        if(!Directory.Exists(l_textureDir))
                            Directory.CreateDirectory(l_textureDir);

                        File.WriteAllBytes(Path.Combine(l_textureDir, l_submesh.m_textureName), l_submesh.m_textureData);

                        Console.WriteLine("Saved texture '{0}", Path.Combine(l_textureDir, l_submesh.m_textureName));
                    }
                    byte[] l_pathData = Encoding.UTF8.GetBytes(l_path);
                    l_writer.Write((byte)l_pathData.Length); // Texture path length
                    l_writer.Write(l_pathData); // Texture path

                    l_writer.Write(l_submesh.m_vertexData.Length * sizeof(float)); // Vertex data length
                    l_writer.Write(l_submesh.m_vertexData.AsBytes());  // Vertex data

                    l_writer.Write(l_submesh.m_normalData.Length * sizeof(float)); // Normal data length
                    l_writer.Write(l_submesh.m_normalData.AsBytes());// Normal data

                    l_writer.Write(l_submesh.m_uvData.Length * sizeof(float)); // UV data length
                    l_writer.Write(l_submesh.m_uvData.AsBytes());// UV data

                    if(m_bones.Count > 0)
                    {
                        l_writer.Write(l_submesh.m_weightsData.Length * sizeof(float)); // Weights data length
                        l_writer.Write(l_submesh.m_weightsData.AsBytes()); // Weights data

                        l_writer.Write(l_submesh.m_weightIndiciesData.Length * sizeof(int)); // Weights index data length
                        l_writer.Write(l_submesh.m_weightIndiciesData.AsBytes()); // Weights index data
                    }
                }

                if(m_bones.Count > 0)
                {
                    l_writer.Write(m_bones.Count); // Bones count

                    foreach(var l_bone in m_bones)
                    {
                        byte[] l_nameData = Encoding.UTF8.GetBytes(l_bone.m_name);
                        l_writer.Write((byte)l_nameData.Length); // Bone name length
                        l_writer.Write(l_nameData); // Bone name
                        l_writer.Write(l_bone.m_parent); // Parent index
                        l_writer.Write(l_bone.m_localPosition.X);
                        l_writer.Write(l_bone.m_localPosition.Y);
                        l_writer.Write(l_bone.m_localPosition.Z);
                        l_writer.Write(l_bone.m_localRotation.X);
                        l_writer.Write(l_bone.m_localRotation.Y);
                        l_writer.Write(l_bone.m_localRotation.Z);
                        l_writer.Write(l_bone.m_localRotation.W);
                        l_writer.Write(l_bone.m_localScale.X);
                        l_writer.Write(l_bone.m_localScale.Y);
                        l_writer.Write(l_bone.m_localScale.Z);
                    }
                }

                l_writer.Flush();
                l_writer.Close();
                l_writer.Dispose();

                l_file.Close();
                l_file.Dispose();

                Console.WriteLine("Model saved to {0}", Path.Combine(l_dir, m_name + ".rmf"));
            }
            catch(Exception e)
            {
                Console.WriteLine("Model save exception: {0} at {1}", e.Message, e.StackTrace);
            }

            foreach(var l_anim in m_animations)
                l_anim.Save(p_dir);
        }
    }
}
