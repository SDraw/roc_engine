#pragma once
#include "Interfaces/ISoundManager.h"
#include "Managers/Manager.h"

namespace ROC
{

class SoundManager final : public ISoundManager, public Manager
{
    glm::vec3 m_listenerPosition;
    glm::vec3 m_listenerDirection;
    glm::vec3 m_listenerUp;
    float m_globalVolume;

    SoundManager(const SoundManager &that) = delete;
    SoundManager& operator=(const SoundManager &that) = delete;
public:
    SoundManager(Core *p_core);
    ~SoundManager() = default;

    void SetListenerPosition(const glm::vec3 &p_pos);
    const glm::vec3& GetListenerPosition() const;

    void SetListenerDirection(const glm::vec3 &p_dir);
    const glm::vec3& GetListenerDirection() const;

    void SetListenerUp(const glm::vec3 &p_up);
    const glm::vec3& GetListenerUp() const;

    void SetGlobalVolume(float p_val);
    float GetGlobalVolume() const;
};

}
