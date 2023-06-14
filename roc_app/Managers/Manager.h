#pragma once

namespace ROC
{

class Core;

class Manager
{
    Core *m_core;
    bool m_started;

    Manager(const Manager &that) = delete;
    Manager& operator=(const Manager &that) = delete;
protected:
    explicit Manager(Core *p_core);
    virtual ~Manager() = default;
public:
    virtual void Start();
    virtual void Stop();

    bool IsActive() const;
    Core* GetCore();
};

}
