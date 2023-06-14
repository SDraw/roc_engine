#pragma once
#include "Interfaces/IMesh.h"
#include "Elements/Element.h"

namespace ROC
{

class Material;
struct BoneCollisionData;
struct BoneData;
struct BoneJointData;

class Mesh final : public Element, public IMesh
{
    std::vector<Material*> m_materials;
    size_t m_materialCount;
    float m_boundSphereRaduis;

    std::vector<BoneData*> m_bonesData;
    std::vector<BoneCollisionData*> m_collisionData;
    std::vector<BoneJointData*> m_jointData;

    bool m_loaded;

    Mesh(const Mesh &that) = delete;
    Mesh& operator=(const Mesh &that) = delete;

    void Clear();
public:
    explicit Mesh();
    ~Mesh();

    float GetBoundSphereRadius() const;
    size_t GetMaterialsCount() const;

    bool HasBonesData() const;
    bool HasBonesCollisionData() const;
    bool HasJointsData() const;

    bool Load(const std::string &p_path);
    void GenerateMaterials();

    const std::vector<Material*>& GetMaterials() const;
    const std::vector<BoneData*>& GetBonesData() const;
    const std::vector<BoneCollisionData*>& GetBonesCollisionData() const;
    const std::vector<BoneJointData*>& GetJointsData() const;
};

}
