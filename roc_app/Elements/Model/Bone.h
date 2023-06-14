#pragma once
namespace ROC
{

class BoneFrameData;
class Transformation;

class Bone final
{
#ifdef _DEBUG
    std::string m_name;
#endif
    Transformation *m_localTransform;
    glm::mat4 m_fullMatrix;
    glm::mat4 m_bindMatrix;
    glm::mat4 m_poseMatrix;
    bool m_updated;

    Bone *m_parent;
    std::vector<Bone*> m_children;

    Bone(const Bone &that) = delete;
    Bone& operator=(const Bone &that) = delete;
public:
    Bone(const std::string &p_name, const glm::quat &p_rot, const glm::vec3 &p_pos, const glm::vec3 &p_scl);
    ~Bone();

    Bone* GetParent() const;
    const std::vector<Bone*>& GetChildren() const;

    const Transformation* GetLocalTransformation() const;
    const glm::mat4& GetBindMatrix() const;
    const glm::mat4& GetFullMatrix() const;
    const glm::mat4& GetPoseMatrix() const;

    bool IsUpdated() const;

    void GenerateBindPose();
    void SetFrameData(BoneFrameData *p_data);
    void SetFrameData(BoneFrameData *p_data, float p_blend);

    void SetParent(Bone *p_bone);
    void AddChild(Bone *p_bone);

    void SetFullMatrix(const btTransform &p_transform);
    void SetPoseMatrix(const btTransform &p_transform);

    void Update();
};

}
