#include "stdafx.h"

#include "Elements/Mesh/Mesh.h"
#include "Elements/Mesh/BoneCollisionData.hpp"
#include "Elements/Mesh/BoneData.hpp"
#include "Elements/Mesh/BoneJointData.hpp"
#include "Elements/Mesh/Material.h"

#include "Utils/zlibUtils.h"

namespace ROC
{

enum MeshSetByte : unsigned char
{
    MSB_Animated = 0x2U,
    MSB_Collision = 0xCBU
};

}

ROC::Mesh::Mesh()
{
    m_elementType = ET_Mesh;

    m_materialCount = 0U;
    m_boundSphereRaduis = 0.f;
    m_loaded = false;
}

ROC::Mesh::~Mesh()
{
    Clear();
}

bool ROC::Mesh::Load(const std::string &p_path)
{
    if(!m_loaded)
    {
        unsigned char l_type = 0;
        std::ifstream l_file;
        l_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            l_file.open(p_path, std::ios::binary);
            std::string l_header(3, '\0');
            l_file.read(&l_header[0], 3);
            if(!l_header.compare("ROC"))
            {
                l_file.read(reinterpret_cast<char*>(&l_type), sizeof(unsigned char));
                l_file.read(reinterpret_cast<char*>(&m_boundSphereRaduis), sizeof(float));

                unsigned int l_materialCount = 0U;
                l_file.read(reinterpret_cast<char*>(&l_materialCount), sizeof(unsigned int));
                m_materialCount = static_cast<size_t>(l_materialCount);

                // Materials
                for(size_t i = 0U; i < m_materialCount; i++)
                {
                    Material *l_material = new Material;
                    m_materials.push_back(l_material);

                    unsigned char l_materialType = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_materialType), sizeof(unsigned char));
                    l_material->SetType(l_materialType);

                    glm::vec4 l_materialParams(1.f);
                    l_file.read(reinterpret_cast<char*>(glm::value_ptr(l_materialParams)), sizeof(glm::vec4));
                    l_material->SetParams(l_materialParams);

                    unsigned char l_diffuseTextureNameLength = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_diffuseTextureNameLength), sizeof(unsigned char));
                    if(l_diffuseTextureNameLength > 0U)
                    {
                        std::string l_diffuseTexture(l_diffuseTextureNameLength, '\0');
                        l_file.read(&l_diffuseTexture[0], l_diffuseTextureNameLength);
                        l_material->LoadTexture(l_diffuseTexture);
                    }

                    unsigned int l_compressedSize = 0U;
                    unsigned int l_sourceSize = 0U;
                    std::vector<char> l_compressedData;

                    // Vertex data
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                    l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                    std::vector<glm::vec3> l_vertexData(l_sourceSize / sizeof(glm::vec3));
                    l_compressedData.resize(l_compressedSize, '\0');
                    l_file.read(l_compressedData.data(), l_compressedSize);
                    if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_vertexData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                    l_material->LoadVertices(l_vertexData);
                    l_vertexData.clear();

                    // Normal data
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                    l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                    std::vector<glm::vec3> l_normalData(l_sourceSize / sizeof(glm::vec3));
                    l_compressedData.resize(l_compressedSize, '\0');
                    l_file.read(l_compressedData.data(), l_compressedSize);
                    if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_normalData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                    l_material->LoadNormals(l_normalData);
                    l_normalData.clear();

                    // UV data
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                    l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                    std::vector<glm::vec2> l_uvData(l_sourceSize / sizeof(glm::vec2));
                    l_compressedData.resize(l_compressedSize, '\0');
                    l_file.read(l_compressedData.data(), l_compressedSize);
                    if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_uvData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                    l_material->LoadUVs(l_uvData);
                    l_uvData.clear();

                    if(l_type == MeshSetByte::MSB_Animated)
                    {
                        // Weight data
                        l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                        l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                        std::vector<glm::vec4> l_weightData(l_sourceSize / sizeof(glm::vec4));
                        l_compressedData.resize(l_compressedSize, '\0');
                        l_file.read(l_compressedData.data(), l_compressedSize);
                        if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_weightData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                        l_material->LoadWeights(l_weightData);
                        l_weightData.clear();

                        // Index data
                        l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                        l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                        std::vector<glm::ivec4> l_indexData(l_sourceSize / sizeof(glm::ivec4));
                        l_compressedData.resize(l_compressedSize, '\0');
                        l_file.read(l_compressedData.data(), l_compressedSize);
                        if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_indexData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                        l_material->LoadIndices(l_indexData);
                        l_indexData.clear();
                    }
                }

                // Skeleton
                if(l_type == MeshSetByte::MSB_Animated)
                {
                    unsigned int l_bonesCount;
                    l_file.read(reinterpret_cast<char*>(&l_bonesCount), sizeof(unsigned int));

                    for(size_t i = 0; i < l_bonesCount; i++)
                    {
                        BoneData *l_boneData = new BoneData();
                        m_bonesData.push_back(l_boneData);

                        unsigned char l_boneNameLength;
                        l_file.read(reinterpret_cast<char*>(&l_boneNameLength), sizeof(unsigned char));
                        if(l_boneNameLength > 0U)
                        {
                            l_boneData->m_name.resize(l_boneNameLength);
                            l_file.read(&l_boneData->m_name[0], l_boneNameLength);
                        }
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_parent), sizeof(int));
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_position), sizeof(glm::vec3));
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_rotation), sizeof(glm::quat));
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_scale), sizeof(glm::vec3));
                    }
                    m_bonesData.shrink_to_fit();
                }

                // Sort materials
                if(m_materialCount > 0U)
                {
                    std::vector<Material*> l_matVecDef;
                    std::vector<Material*> l_matVecDefDouble;
                    std::vector<Material*> l_matVecDefTransp;

                    for(auto l_mat : m_materials)
                    {
                        if(l_mat->IsTransparent() || !l_mat->HasDepth()) l_matVecDefTransp.push_back(l_mat);
                        else l_mat->IsDoubleSided() ? l_matVecDefDouble.push_back(l_mat) : l_matVecDef.push_back(l_mat);
                    }
                    m_materials.clear();
                    m_materials.insert(m_materials.end(), l_matVecDefDouble.begin(), l_matVecDefDouble.end());
                    m_materials.insert(m_materials.end(), l_matVecDef.begin(), l_matVecDef.end());
                    m_materials.insert(m_materials.end(), l_matVecDefTransp.begin(), l_matVecDefTransp.end());
                    m_materials.shrink_to_fit();
                }
            }
            m_loaded = true;
        }
        catch(const std::exception&)
        {
            Clear();
        }

        // Static and dynamic collision, only for animated Mesh
        if(m_loaded && (l_type == MeshSetByte::MSB_Animated))
        {
            try
            {
                unsigned char l_physicsBlock = 0U;
                l_file.read(reinterpret_cast<char*>(&l_physicsBlock), sizeof(unsigned char));
                if(l_physicsBlock == MeshSetByte::MSB_Collision)
                {
                    unsigned int l_scbCount = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_scbCount), sizeof(unsigned int));
                    for(unsigned int i = 0U; i < l_scbCount; i++)
                    {
                        BoneCollisionData *l_colData = new BoneCollisionData();
                        m_collisionData.push_back(l_colData);

                        l_file.read(reinterpret_cast<char*>(&l_colData->m_type), sizeof(unsigned char));
                        l_file.read(reinterpret_cast<char*>(&l_colData->m_size), sizeof(glm::vec3));
                        l_file.read(reinterpret_cast<char*>(&l_colData->m_offset), sizeof(glm::vec3));
                        l_file.read(reinterpret_cast<char*>(&l_colData->m_offsetRotation), sizeof(glm::quat));
                        l_file.read(reinterpret_cast<char*>(&l_colData->m_boneID), sizeof(unsigned int));
                    }
                    m_collisionData.shrink_to_fit();

                    unsigned int l_jointsCount = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_jointsCount), sizeof(unsigned int));

                    for(unsigned int i = 0U; i < l_jointsCount; i++)
                    {
                        unsigned int l_jointParts = 0U;
                        l_file.read(reinterpret_cast<char*>(&l_jointParts), sizeof(unsigned int));

                        if(l_jointParts > 0U)
                        {
                            BoneJointData *l_joint = new BoneJointData();
                            m_jointData.push_back(l_joint);

                            l_file.read(reinterpret_cast<char*>(&l_joint->m_boneID), sizeof(unsigned int));
                            for(unsigned int j = 0; j < l_jointParts; j++)
                            {
                                BoneJointPartData l_jointPart;

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_boneID), sizeof(unsigned int));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_type), sizeof(unsigned char));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_size), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_offset), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_rotation), sizeof(glm::quat));

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_mass), sizeof(float));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_restutition), sizeof(float));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_friction), sizeof(float));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_damping), sizeof(glm::vec2));

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_lowerAngularLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_upperAngularLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_angularStiffness), sizeof(glm::vec3));

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_lowerLinearLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_upperLinearLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_linearStiffness), sizeof(glm::vec3));

                                l_joint->m_jointParts.push_back(l_jointPart);
                            }
                            l_joint->m_jointParts.shrink_to_fit();
                        }
                    }
                    m_jointData.shrink_to_fit();
                }
            }
            catch(const std::exception&)
            {
                for(auto l_colData : m_collisionData) delete l_colData;
                m_collisionData.clear();

                for(auto l_jointData : m_jointData) delete l_jointData;
                m_jointData.clear();
            }
        }
    }
    return m_loaded;
}

void ROC::Mesh::Clear()
{
    for(auto l_mat : m_materials) delete l_mat;
    m_materials.clear();

    m_materialCount = 0U;
    m_boundSphereRaduis = 0.f;

    for(auto l_boneData : m_bonesData) delete l_boneData;
    m_bonesData.clear();

    for(auto l_colData : m_collisionData) delete l_colData;
    m_collisionData.clear();

    for(auto l_jointData : m_jointData) delete l_jointData;
    m_jointData.clear();

    m_loaded = false;
}

void ROC::Mesh::GenerateMaterials()
{
    if(m_loaded)
    {
        for(auto l_mat : m_materials) l_mat->Generate();
    }
}

float ROC::Mesh::GetBoundSphereRadius() const
{
    return m_boundSphereRaduis;
}

size_t ROC::Mesh::GetMaterialsCount() const
{
    return m_materialCount;
}

const std::vector<ROC::Material*>& ROC::Mesh::GetMaterials() const
{
    return m_materials;
}

bool ROC::Mesh::HasBonesData() const
{
    return !m_bonesData.empty();
}

const std::vector<ROC::BoneData*>& ROC::Mesh::GetBonesData() const
{
    return m_bonesData;
}

bool ROC::Mesh::HasBonesCollisionData() const
{
    return !m_collisionData.empty();
}

const std::vector<ROC::BoneCollisionData*>& ROC::Mesh::GetBonesCollisionData() const
{
    return m_collisionData;
}

bool ROC::Mesh::HasJointsData() const
{
    return !m_jointData.empty();
}

const std::vector<ROC::BoneJointData*>& ROC::Mesh::GetJointsData() const
{
    return m_jointData;
}
