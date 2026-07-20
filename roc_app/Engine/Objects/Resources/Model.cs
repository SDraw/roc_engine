using GlmSharp;
using glTFLoader;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Model : Resource
    {
        const byte c_unlitBit = 0b1;
        const byte c_depthWriteBit = 0b10;
        const byte c_transparencyBit = 0b100;
        const byte c_doubleSidedBit = 0b1000;
        const byte c_filteringBit = 0b10000;
        const byte c_compressionBit = 0b100000;

        public enum ModelFileType : int
        {
            RMF = 0,
            glTF
        }

        internal struct MeshData
        {
            public byte m_type;
            public vec4 m_params;
            public string m_texturePath;
            public byte[] m_textureData;
            public vec4 m_materialColor;
            public float[] m_vertexData;
            public float[] m_normalData;
            public float[] m_uvData;
            public float[] m_weightData;
            public int[] m_indexData;
        }

        internal struct BoneData
        {
            public int m_parent;
            public string m_name;
            public vec3 m_position;
            public quat m_rotation;
            public vec3 m_scale;
        }

        public enum ModelType : byte
        {
            Empty = 0,
            Static = 1,
            Rigged = 2,
        }

        byte m_modelType = 0b0;
        float m_boundsRadius = 0f;

        // Parsed
        readonly List<MeshData> m_meshesData = null;
        readonly List<BoneData> m_bonesData = null;

        // Generated
        readonly List<Mesh> m_meshes = null;
        readonly List<Texture> m_textures = null;
        readonly List<Material> m_materials = null;

        // Properties
        public ModelType TypeOfModel => (ModelType)m_modelType;
        public float BoundsRadius => m_boundsRadius;

        internal List<BoneData> Bones => m_bonesData;
        internal List<Mesh> Meshes => m_meshes;
        internal List<Texture> Textures => m_textures;
        internal List<Material> Materials => m_materials;

        // Class
        internal Model()
        {
            m_modelType = (byte)ModelType.Empty;

            m_meshesData = new List<MeshData>();
            m_bonesData = new List<BoneData>();

            m_meshes = new List<Mesh>();
            m_textures = new List<Texture>();
            m_materials = new List<Material>();
        }

        // Resource
        internal void Load(string p_path, ModelFileType p_type)
        {
            if(m_loaded)
                return;

            try
            {
                switch(p_type)
                {
                    case ModelFileType.RMF:
                        LoadRMF(p_path);
                        break;

                    case ModelFileType.glTF:
                        LoadGLTF(p_path);
                        break;
                }

                SetupMeshes();
                SetupTextures();
                SetupMaterials();

                m_loaded = true;
            }
            catch(Exception e)
            {
                m_log = e.Message;
            }
        }

        void LoadRMF(string p_path)
        {
            FileStream l_file = new FileStream(p_path, FileMode.Open, FileAccess.Read);
            BinaryReader l_reader = new BinaryReader(l_file);

            string l_header = new string(l_reader.ReadChars(3));
            if(l_header != "ROC")
                throw new Exception("Not RMF model file");

            m_modelType = l_reader.ReadByte();
            m_boundsRadius = l_reader.ReadSingle();

            uint l_materialsCount = l_reader.ReadUInt32();
            if(l_materialsCount == 0U)
                throw new Exception("Model contains no materials");

            for(uint i = 0; i < l_materialsCount; i++)
            {
                MeshData l_subMesh = new MeshData();
                l_subMesh.m_type = l_reader.ReadByte();

                l_subMesh.m_params.x = l_reader.ReadSingle();
                l_subMesh.m_params.y = l_reader.ReadSingle();
                l_subMesh.m_params.z = l_reader.ReadSingle();
                l_subMesh.m_params.w = l_reader.ReadSingle();

                l_subMesh.m_materialColor = vec4.Ones;

                int l_textureNameLength = l_reader.ReadByte();
                l_subMesh.m_texturePath = new string(l_reader.ReadChars(l_textureNameLength));

                // Vertex
                uint l_compressedSize = l_reader.ReadUInt32();
                l_reader.ReadUInt32(); // Decompressed size, skip
                {
                    l_reader.ReadBytes(2);
                    byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                    byte[] l_decompressed = Decompress(ref l_compressed);
                    l_subMesh.m_vertexData = new float[l_decompressed.Length / 4];
                    Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_vertexData, 0, l_decompressed.Length);
                }

                // Normals
                l_compressedSize = l_reader.ReadUInt32();
                l_reader.ReadUInt32(); // Decompressed size, skip
                {
                    l_reader.ReadBytes(2);
                    byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                    byte[] l_decompressed = Decompress(ref l_compressed);
                    l_subMesh.m_normalData = new float[l_decompressed.Length / 4];
                    Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_normalData, 0, l_decompressed.Length);
                }

                // UV
                l_compressedSize = l_reader.ReadUInt32();
                l_reader.ReadUInt32(); // Decompressed size, skip
                {
                    l_reader.ReadBytes(2);
                    byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                    byte[] l_decompressed = Decompress(ref l_compressed);
                    l_subMesh.m_uvData = new float[l_decompressed.Length / 4];
                    Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_uvData, 0, l_decompressed.Length);
                }

                if(m_modelType == (byte)ModelType.Rigged)
                {
                    // Weights
                    l_compressedSize = l_reader.ReadUInt32();
                    l_reader.ReadUInt32(); // Decompressed size, skip
                    {
                        l_reader.ReadBytes(2);
                        byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                        byte[] l_decompressed = Decompress(ref l_compressed);
                        l_subMesh.m_weightData = new float[l_decompressed.Length / 4];
                        Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_weightData, 0, l_decompressed.Length);
                    }

                    // Weight indices
                    l_compressedSize = l_reader.ReadUInt32();
                    l_reader.ReadUInt32(); // Decompressed size, skip
                    {
                        l_reader.ReadBytes(2);
                        byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                        byte[] l_decompressed = Decompress(ref l_compressed);
                        l_subMesh.m_indexData = new int[l_decompressed.Length / 4];
                        Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_indexData, 0, l_decompressed.Length);
                    }
                }
                else
                {
                    l_subMesh.m_weightData = null;
                    l_subMesh.m_indexData = null;
                }

                m_meshesData.Add(l_subMesh);
            }

            if(m_modelType == (byte)ModelType.Rigged)
            {
                uint l_bonesCount = l_reader.ReadUInt32();

                for(uint i = 0U; i < l_bonesCount; i++)
                {
                    BoneData l_boneData = new BoneData();

                    byte l_boneNameLength = l_reader.ReadByte();
                    l_boneData.m_name = new string(l_reader.ReadChars(l_boneNameLength));

                    l_boneData.m_parent = l_reader.ReadInt32();

                    l_boneData.m_position.x = l_reader.ReadSingle();
                    l_boneData.m_position.y = l_reader.ReadSingle();
                    l_boneData.m_position.z = l_reader.ReadSingle();

                    l_boneData.m_rotation.x = l_reader.ReadSingle();
                    l_boneData.m_rotation.y = l_reader.ReadSingle();
                    l_boneData.m_rotation.z = l_reader.ReadSingle();
                    l_boneData.m_rotation.w = l_reader.ReadSingle();

                    l_boneData.m_scale.x = l_reader.ReadSingle();
                    l_boneData.m_scale.y = l_reader.ReadSingle();
                    l_boneData.m_scale.z = l_reader.ReadSingle();

                    m_bonesData.Add(l_boneData);
                }
            }

            l_reader.Close();
            l_reader.Dispose();

            l_file.Close();
            l_file.Dispose();
        }

        void LoadGLTF(string p_path)
        {
            var l_gltfModel = Interface.LoadModel(p_path);

            List<byte[]> l_binaryBuffers = new List<byte[]>();
            foreach(var l_buffer in l_gltfModel.Buffers)
            {
                if(l_buffer.Uri != null)
                    l_binaryBuffers.Add(Convert.FromBase64String(l_buffer.Uri.Split(',')[1]));
                else
                    l_binaryBuffers.Add(Interface.LoadBinaryBuffer(l_gltfModel, l_binaryBuffers.Count, p_path));
            }

            var l_mainScene = l_gltfModel.Scenes[l_gltfModel.Scene.Value];
            var l_firstNode = l_gltfModel.Nodes[l_mainScene.Nodes[0]];

            glTFLoader.Schema.Mesh l_mesh = null;
            glTFLoader.Schema.Skin l_skin = null;
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

            if(l_mesh == null)
                throw new Exception("No mesh found");

            vec3 l_boundsSize = vec3.Zero;
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
                MeshData l_meshData = new MeshData();
                l_meshData.m_vertexData = new float[l_trianglesCount * 9];
                l_meshData.m_normalData = new float[l_trianglesCount * 9];
                l_meshData.m_uvData = new float[l_trianglesCount * 6];
                l_meshData.m_weightData = (l_weightsFlat != null) ? new float[l_trianglesCount * 12] : null;
                l_meshData.m_indexData = (l_indexFlat != null) ? new int[l_trianglesCount * 12] : null;
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
                        Array.Copy(l_weightsFlat, l_faceIndex[i * 3] * 4, l_meshData.m_weightData, i * 12, 4);
                        Array.Copy(l_weightsFlat, l_faceIndex[i * 3 + 1] * 4, l_meshData.m_weightData, i * 12 + 4, 4);
                        Array.Copy(l_weightsFlat, l_faceIndex[i * 3 + 2] * 4, l_meshData.m_weightData, i * 12 + 8, 4);

                        Array.Copy(l_indexFlat, l_faceIndex[i * 3] * 4, l_meshData.m_indexData, i * 12, 4);
                        Array.Copy(l_indexFlat, l_faceIndex[i * 3 + 1] * 4, l_meshData.m_indexData, i * 12 + 4, 4);
                        Array.Copy(l_indexFlat, l_faceIndex[i * 3 + 2] * 4, l_meshData.m_indexData, i * 12 + 8, 4);
                    }
                }

                if(l_primitive.Material.HasValue)
                {
                    var l_material = l_gltfModel.Materials[l_primitive.Material.Value];
                    l_meshData.m_type = 0;
                    switch(l_material.AlphaMode)
                    {
                        case glTFLoader.Schema.Material.AlphaModeEnum.OPAQUE:
                            l_meshData.m_type |= c_depthWriteBit;
                            break;

                        case glTFLoader.Schema.Material.AlphaModeEnum.MASK:
                        case glTFLoader.Schema.Material.AlphaModeEnum.BLEND:
                            l_meshData.m_type |= (c_depthWriteBit | c_transparencyBit);
                            break;
                    }
                    if(l_material.DoubleSided)
                        l_meshData.m_type |= c_doubleSidedBit;

                    l_meshData.m_materialColor = new vec4(l_material.PbrMetallicRoughness.BaseColorFactor);
                    l_meshData.m_materialColor.a = 1f;

                    if(new vec3(l_material.EmissiveFactor).MaxElement.IsEpsilonEqual(0f))
                        l_meshData.m_type |= c_unlitBit;

                    if(l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Sampler.HasValue)
                    {
                        var l_sampler = l_gltfModel.Samplers[l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Sampler.Value];
                        switch(l_sampler.MagFilter)
                        {
                            case glTFLoader.Schema.Sampler.MagFilterEnum.NEAREST:
                                l_meshData.m_type |= c_filteringBit;
                                break;
                        }
                    }

                    if(l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Source.HasValue)
                    {
                        var l_image = l_gltfModel.Images[l_gltfModel.Textures[l_material.PbrMetallicRoughness.BaseColorTexture.Index].Source.Value];
                        if(l_image.Uri != null)
                        {
                            if(l_image.Uri.StartsWith("data"))
                            {
                                l_meshData.m_textureData = Convert.FromBase64String(l_image.Uri.Split(',')[1]);
                                l_meshData.m_texturePath = null;
                            }
                            else
                            {
                                l_meshData.m_texturePath = l_image.Uri;
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
                }
                else
                {
                    l_meshData.m_type = (c_unlitBit | c_depthWriteBit);
                    l_meshData.m_texturePath = "";
                    l_meshData.m_textureData = null;
                }

                // Bounds
                l_boundsSize = vec3.Max(new vec3(l_gltfModel.Accessors[l_primitive.Attributes["POSITION"]].Max), l_boundsSize);

                m_meshesData.Add(l_meshData);
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
                    l_boneData.m_position = new vec3(l_jointNode.Translation);
                    l_boneData.m_rotation = new quat();
                    l_boneData.m_rotation.x = l_jointNode.Rotation[0];
                    l_boneData.m_rotation.y = l_jointNode.Rotation[1];
                    l_boneData.m_rotation.z = l_jointNode.Rotation[2];
                    l_boneData.m_rotation.w = l_jointNode.Rotation[3];
                    l_boneData.m_scale = new vec3(l_jointNode.Scale);
                    m_bonesData.Add(l_boneData);
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
                        for(int i = 0, j = m_bonesData.Count; i < j; i++)
                        {
                            if(m_bonesData[i].m_name == l_childNode.Name)
                            {
                                var l_boneData = m_bonesData[i];
                                l_boneData.m_parent = l_parentIndex;
                                m_bonesData[i] = l_boneData;
                                break;
                            }
                        }
                    }
                }
            }

            m_boundsRadius = l_boundsSize.Length;
            m_modelType = (l_skin != null) ? (byte)ModelType.Rigged : (byte)ModelType.Static;
        }

        protected override void DestroyInternal()
        {
            if(m_loaded)
            {
                m_modelType = (byte)ModelType.Empty;
                m_meshesData.Clear();
                m_bonesData.Clear();

                foreach(var l_mesh in m_meshes)
                    Object.Destroy(l_mesh);
                m_meshes.Clear();

                foreach(var l_texture in m_textures)
                    Object.Destroy(l_texture);
                m_textures.Clear();

                m_materials.Clear();

                m_loaded = false;
            }

            base.DestroyInternal();
        }

        // Arbitrary
        void SetupMeshes()
        {
            bool l_isRigged = (m_modelType == (byte)ModelType.Rigged);

            foreach(var l_meshData in m_meshesData)
            {
                Mesh l_mesh = new Mesh();
                l_mesh.Generate(
                    l_meshData.m_vertexData, l_meshData.m_normalData, l_meshData.m_uvData,
                    l_isRigged ? l_meshData.m_weightData : null,
                    l_isRigged ? l_meshData.m_indexData : null
                );
                m_meshes.Add(l_mesh);
            }
        }

        void SetupTextures()
        {
            foreach(var l_meshData in m_meshesData)
            {
                Texture l_texture = new Texture();

                if(l_meshData.m_texturePath != null)
                    l_texture.Load(
                        l_meshData.m_texturePath,
                        (l_meshData.m_type & c_transparencyBit) != 0U,
                        (l_meshData.m_type & c_compressionBit) != 0U,
                        ((l_meshData.m_type & c_filteringBit) != 0U) ? Texture.TextureFiltering.Nearest : Texture.TextureFiltering.Linear
                    );

                if(l_meshData.m_textureData != null)
                    l_texture.Load(
                        l_meshData.m_textureData,
                        (l_meshData.m_type & c_transparencyBit) != 0U,
                        (l_meshData.m_type & c_compressionBit) != 0U,
                        ((l_meshData.m_type & c_filteringBit) != 0U) ? Texture.TextureFiltering.Nearest : Texture.TextureFiltering.Linear
                    );

                m_textures.Add(l_texture);
            }
        }

        void SetupMaterials()
        {
            foreach(var l_meshData in m_meshesData)
            {
                Material l_material = new Material();
                l_material.Unlit = ((l_meshData.m_type & c_unlitBit) == 0);
                l_material.DepthWrite = ((l_meshData.m_type & c_depthWriteBit) != 0);
                l_material.Transparency = ((l_meshData.m_type & c_transparencyBit) != 0);
                l_material.DoubleSided = ((l_meshData.m_type & c_doubleSidedBit) != 0);
                l_material.Params = l_meshData.m_params;
                l_material.Color = l_meshData.m_materialColor;
                m_materials.Add(l_material);
            }

            if(m_materials.Count == m_textures.Count)
            {
                for(int i = 0, j = m_materials.Count; i < j; i++)
                    m_materials[i].DiffuseTexture = m_textures[i];
            }
        }

        // Utils
        static byte[] Decompress(ref byte[] p_in)
        {
            var l_memStreamIn = new MemoryStream(p_in);
            var l_deflate = new DeflateStream(l_memStreamIn, CompressionMode.Decompress);
            var l_memStreamOut = new MemoryStream();
            l_deflate.CopyTo(l_memStreamOut);
            return l_memStreamOut.ToArray();
        }

        // API
        public static Model Import(string p_path, ModelFileType p_type)
        {
            Model l_model = new Model();
            l_model.Load(p_path, p_type);
            return l_model;
        }
    }
}
