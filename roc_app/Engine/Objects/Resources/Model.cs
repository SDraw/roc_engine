using GlmSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Model : Resource
    {
        internal struct MeshData
        {
            public byte m_type;
            public vec4 m_params;
            public string m_texturePath;
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

        readonly List<MeshData> m_meshesData = null;
        readonly List<BoneData> m_bonesData = null;

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
        internal void Load(string p_path)
        {
            if(m_loaded)
                return;

            try
            {
                FileStream l_file = new FileStream(p_path, FileMode.Open, FileAccess.Read);
                BinaryReader l_reader = new BinaryReader(l_file);

                string l_header = new string(l_reader.ReadChars(3));
                if(l_header != "ROC")
                    throw new Exception("Not a model file");

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

                    int l_textureNameLength = l_reader.ReadByte();
                    l_subMesh.m_texturePath = new string(l_reader.ReadChars(l_textureNameLength));


                    uint l_compressedSize = 0U;
                    // Vertex
                    l_compressedSize = l_reader.ReadUInt32();
                    l_reader.ReadUInt32(); // Skip
                    {
                        l_reader.ReadBytes(2);
                        byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                        byte[] l_decompressed = Decompress(ref l_compressed);
                        l_subMesh.m_vertexData = new float[l_decompressed.Length / 4];
                        System.Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_vertexData, 0, l_decompressed.Length);
                    }

                    // Normals
                    l_compressedSize = l_reader.ReadUInt32();
                    l_reader.ReadUInt32(); // Skip
                    {
                        l_reader.ReadBytes(2);
                        byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                        byte[] l_decompressed = Decompress(ref l_compressed);
                        l_subMesh.m_normalData = new float[l_decompressed.Length / 4];
                        System.Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_normalData, 0, l_decompressed.Length);
                    }

                    // UV
                    l_compressedSize = l_reader.ReadUInt32();
                    l_reader.ReadUInt32(); // Skip
                    {
                        l_reader.ReadBytes(2);
                        byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                        byte[] l_decompressed = Decompress(ref l_compressed);
                        l_subMesh.m_uvData = new float[l_decompressed.Length / 4];
                        System.Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_uvData, 0, l_decompressed.Length);
                    }

                    if(m_modelType == (byte)ModelType.Rigged)
                    {
                        // Weights
                        l_compressedSize = l_reader.ReadUInt32();
                        l_reader.ReadUInt32(); // Skip
                        {
                            l_reader.ReadBytes(2);
                            byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                            byte[] l_decompressed = Decompress(ref l_compressed);
                            l_subMesh.m_weightData = new float[l_decompressed.Length / 4];
                            System.Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_weightData, 0, l_decompressed.Length);
                        }

                        // Weight indices
                        l_compressedSize = l_reader.ReadUInt32();
                        l_reader.ReadUInt32(); // Skip
                        {
                            l_reader.ReadBytes(2);
                            byte[] l_compressed = l_reader.ReadBytes((int)l_compressedSize - 2);
                            byte[] l_decompressed = Decompress(ref l_compressed);
                            l_subMesh.m_indexData = new int[l_decompressed.Length / 4];
                            System.Buffer.BlockCopy(l_decompressed, 0, l_subMesh.m_indexData, 0, l_decompressed.Length);
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
                l_texture.Load(
                    l_meshData.m_texturePath,
                    (l_meshData.m_type & 4U) != 0U,
                    (l_meshData.m_type & 32U) != 0U,
                    ((l_meshData.m_type & 16U) != 0U) ? Texture.TextureFiltering.Nearest : Texture.TextureFiltering.Linear
                );
                m_textures.Add(l_texture);
            }
        }

        void SetupMaterials()
        {
            foreach(var l_meshData in m_meshesData)
            {
                Material l_material = new Material();
                l_material.Unlit = ((l_meshData.m_type & 1) == 0);
                l_material.DepthWrite = ((l_meshData.m_type & 2) != 0);
                l_material.Transparency = ((l_meshData.m_type & 4) != 0);
                l_material.DoubleSided = ((l_meshData.m_type & 8) != 0);
                l_material.Params = l_meshData.m_params;
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
        public static Model Import(string p_path)
        {
            Model l_model = new Model();
            l_model.Load(p_path);
            return l_model;
        }
    }
}
