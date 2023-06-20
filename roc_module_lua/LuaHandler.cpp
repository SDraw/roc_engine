#include "stdafx.h"
#include "LuaHandler.h"
#include "Lua/LuaVM.h"

#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaDefs/TransformableDefs.h"
#include "Lua/LuaDefs/CollidableDefs.h"
#include "Lua/LuaDefs/DrawableDefs.h"
#include "Lua/LuaDefs/MeshDefs.h"
#include "Lua/LuaDefs/ModelDefs.h"
#include "Lua/LuaDefs/SceneDefs.h"
#include "Lua/LuaDefs/CameraDefs.h"
#include "Lua/LuaDefs/LightDefs.h"
#include "Lua/LuaDefs/ShaderDefs.h"
#include "Lua/LuaDefs/AnimationDefs.h"
#include "Lua/LuaDefs/RenderTargetDefs.h"
#include "Lua/LuaDefs/TextureDefs.h"
#include "Lua/LuaDefs/ColliderDefs.h"
#include "Lua/LuaDefs/FontDefs.h"
#include "Lua/LuaDefs/SoundDefs.h"
#include "Lua/LuaDefs/PhysicsDefs.h"
#include "Lua/LuaDefs/WindowDefs.h"
#include "Lua/LuaDefs/RenderDefs.h"
#include "Lua/LuaDefs/InputDefs.h"
#include "Lua/LuaDefs/TimeDefs.h"
#include "Lua/LuaDefs/Vector2Defs.h"
#include "Lua/LuaDefs/Vector3Defs.h"
#include "Lua/LuaDefs/Vector4Defs.h"
#include "Lua/LuaDefs/QuaternionDefs.h"
#include "Lua/LuaDefs/MatrixDefs.h"

const std::array<std::string, 16U> g_eventNames
{
    "onEngineStart", "onEngineStop",
    "onRender", "onPreRender",
    "onWindowClose", "onWindowResize", "onWindowFocus",
    "onKeyPress", "onMouseKeyPress", "onMouseScroll",
    "onCursorMove", "onCursorEnter",
    "onJoypadStateChange", "onJoypadButton", "onJoypadAxis",
    "onTextInput"
};

void LuaHandler::InitDefs()
{
    ElementDefs::Init();
    TransformableDefs::Init();
    CollidableDefs::Init();
    MeshDefs::Init();
    ModelDefs::Init();
    SceneDefs::Init();
    CameraDefs::Init();
    LightDefs::Init();
    ShaderDefs::Init();
    AnimationDefs::Init();
    RenderTargetDefs::Init();
    TextureDefs::Init();
    ColliderDefs::Init();
    FontDefs::Init();
    SoundDefs::Init();
    PhysicsDefs::Init();
    WindowDefs::Init();
    RenderDefs::Init();
    InputDefs::Init();
    TimeDefs::Init();
    Vector2Defs::Init();
    Vector3Defs::Init();
    Vector4Defs::Init();
    QuaternionDefs::Init();
    MatrixDefs::Init();
}

LuaHandler::LuaHandler()
{
    m_vm = new LuaVM();

    // Register defs in VM
    ElementDefs::RegisterInVM(m_vm);
    TransformableDefs::RegisterInVM(m_vm);
    CollidableDefs::RegisterInVM(m_vm);
    DrawableDefs::RegisterInVM(m_vm);
    MeshDefs::RegisterInVM(m_vm);
    ModelDefs::RegisterInVM(m_vm);
    SceneDefs::RegisterInVM(m_vm);
    CameraDefs::RegisterInVM(m_vm);
    LightDefs::RegisterInVM(m_vm);
    ShaderDefs::RegisterInVM(m_vm);
    AnimationDefs::RegisterInVM(m_vm);
    RenderTargetDefs::RegisterInVM(m_vm);
    TextureDefs::RegisterInVM(m_vm);
    ColliderDefs::RegisterInVM(m_vm);
    FontDefs::RegisterInVM(m_vm);
    SoundDefs::RegisterInVM(m_vm);
    PhysicsDefs::RegisterInVM(m_vm);
    WindowDefs::RegisterInVM(m_vm);
    RenderDefs::RegisterInVM(m_vm);
    InputDefs::RegisterInVM(m_vm);
    TimeDefs::RegisterInVM(m_vm);
    Vector2Defs::RegisterInVM(m_vm);
    Vector3Defs::RegisterInVM(m_vm);
    Vector4Defs::RegisterInVM(m_vm);
    QuaternionDefs::RegisterInVM(m_vm);
    MatrixDefs::RegisterInVM(m_vm);
}
LuaHandler::~LuaHandler()
{
    delete m_vm;
}

void LuaHandler::LoadScript(const std::string &p_path)
{
    m_vm->LoadScript(p_path);
}

void LuaHandler::ParseEvents()
{
    for(size_t i = ROC::IModule::ModuleEvent::ME_OnEngineStart; i <= ROC::IModule::ModuleEvent::ME_OnTextInput; i++)
    {
        if(m_vm->IsFunctionPresent(g_eventNames[i]))
            m_eventFunctions.insert(std::make_pair(static_cast<ROC::IModule::ModuleEvent>(i), m_vm->GetFunctionReference(g_eventNames[i])));
    }
}

void LuaHandler::CallEvent(ROC::IModule::ModuleEvent p_event, const ROC::ICustomArguments *p_args)
{
    const auto &l_iter = m_eventFunctions.find(p_event);
    if(l_iter != m_eventFunctions.end())
        m_vm->CallFunction(l_iter->second, p_args);
}

void LuaHandler::PerformGC()
{
    m_vm->PerformGC();
}

