#pragma once

namespace ROC
{

class IElementManager;
class ILogManager;
class IPhysicsManager;
class IRenderManager;
class ISfmlManager;
class ISoundManager;

class ICore
{
protected:
    ~ICore() = default;
public:
    virtual float GetEngineDelta() const = 0;
    virtual IElementManager* GetIElementManager() const = 0;
    virtual ILogManager* GetILogManager() const = 0;
    virtual IPhysicsManager* GetIPhysicsManager() const = 0;
    virtual IRenderManager* GetIRenderManager() const = 0;
    virtual ISfmlManager* GetISfmlManager() const = 0;
    virtual ISoundManager* GetISoundManager() const = 0;
};

}
