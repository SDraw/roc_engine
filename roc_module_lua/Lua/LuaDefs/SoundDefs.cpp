#include "stdafx.h"
#include "Lua/LuaDefs/SoundDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec2Name;
extern const std::string g_vec3Name;
const std::string g_soundName("Sound");
const std::string g_soundInvalid("Invalid object/Sound");

const std::vector<std::string> g_soundStates
{
    "stopped", "paused", "playing"
};

std::vector<LuaPropDef> SoundDefs::ms_staticProps;
std::vector<LuaMethodDef> SoundDefs::ms_metaMethods;
std::vector<LuaPropDef> SoundDefs::ms_instanceProps;
std::vector<LuaMethodDef> SoundDefs::ms_instanceMethods;

void SoundDefs::Init()
{
    ms_instanceProps.emplace_back("listenerPosition", GetListenerPosition, SetListenerPosition);
    ms_instanceProps.emplace_back("listenerDirection", GetListenerDirection, SetListenerDirection);
    ms_instanceProps.emplace_back("listenerUp", GetListenerUp, SetListenerUp);
    ms_instanceProps.emplace_back("globalVolume", GetGlobalVolume, SetGlobalVolume);

    ms_instanceProps.emplace_back("loop", GetLoop, SetLoop);
    ms_instanceProps.emplace_back("duration", GetDuration, nullptr);
    ms_instanceProps.emplace_back("speed", GetSpeed, SetSpeed);
    ms_instanceProps.emplace_back("volume", GetVolume, SetVolume);
    ms_instanceProps.emplace_back("time", GetTime, SetTime);
    ms_instanceProps.emplace_back("3d", Get3D, Set3D);
    ms_instanceProps.emplace_back("position", GetPosition, SetPosition);
    ms_instanceProps.emplace_back("distance", GetDistance, SetDistance);
    ms_instanceProps.emplace_back("state", GetState, nullptr);

    ms_instanceMethods.emplace_back("play", Play);
    ms_instanceMethods.emplace_back("pause", Pause);
    ms_instanceMethods.emplace_back("stop", Stop);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
}
void SoundDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_soundName, Create, &ms_staticProps, nullptr, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isSound", IsSound);
}

int SoundDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_path;
    l_argReader.Skip();
    l_argReader.ReadString(l_path);
    if(!l_argReader.HasError())
    {
        ROC::ISound* l_sound = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateISound(l_path.c_str());
        if(l_sound)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_sound), g_soundName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SoundDefs::GetListenerPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(Core::GetInstance()->GetEngineCore()->GetISoundManager()->GetListenerPosition()), g_vec3Name, false);
    return 1;
}
int SoundDefs::SetListenerPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISoundManager()->SetListenerPosition(*l_vec);

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetListenerDirection(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(Core::GetInstance()->GetEngineCore()->GetISoundManager()->GetListenerDirection()), g_vec3Name, false);
    return 1;
}
int SoundDefs::SetListenerDirection(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISoundManager()->SetListenerDirection(*l_vec);

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetListenerUp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(Core::GetInstance()->GetEngineCore()->GetISoundManager()->GetListenerUp()), g_vec3Name, false);
    return 1;
}
int SoundDefs::SetListenerUp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISoundManager()->SetListenerUp(*l_vec);

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetGlobalVolume(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushNumber(Core::GetInstance()->GetEngineCore()->GetISoundManager()->GetGlobalVolume());
    return 1;
}
int SoundDefs::SetGlobalVolume(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    float l_val;
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISoundManager()->SetGlobalVolume(l_val);

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetLoop(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushBoolean(l_sound->IsLooped());
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SoundDefs::SetLoop(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    bool l_val;
    l_argReader.ReadObject(l_obj, g_soundName);
    l_argReader.ReadBoolean(l_val);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_sound->SetLoop(l_val);
        else
            l_argReader.SetError(g_soundInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetDuration(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushNumber(l_sound->GetDuration());
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int SoundDefs::GetSpeed(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushNumber(l_sound->GetSpeed());
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SoundDefs::SetSpeed(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    float l_val;
    l_argReader.ReadObject(l_obj, g_soundName);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_sound->SetSpeed(l_val);
        else
            l_argReader.SetError(g_soundInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetVolume(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushNumber(l_sound->GetVolume());
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SoundDefs::SetVolume(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    float l_val;
    l_argReader.ReadObject(l_obj, g_soundName);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_sound->SetVolume(l_val);
        else
            l_argReader.SetError(g_soundInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetTime(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushNumber(l_sound->GetTime());
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SoundDefs::SetTime(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    float l_val;
    l_argReader.ReadObject(l_obj, g_soundName);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_sound->SetTime(l_val);
        else
            l_argReader.SetError(g_soundInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int SoundDefs::Get3D(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushBoolean(l_sound->Get3DPositionEnabled());
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SoundDefs::Set3D(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    bool l_val;
    l_argReader.ReadObject(l_obj, g_soundName);
    l_argReader.ReadBoolean(l_val);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_sound->Set3DPositionEnabled(l_val);
        else
            l_argReader.SetError(g_soundInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushObject(new glm::vec3(l_sound->Get3DPosition()), g_vec3Name, false);
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SoundDefs::SetPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_val;
    l_argReader.ReadObject(l_obj, g_soundName);
    l_argReader.ReadObject(l_val, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_sound->Set3DPosition(*l_val);
        else
            l_argReader.SetError(g_soundInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetDistance(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushObject(new glm::vec2(l_sound->Get3DDistance()), g_vec2Name, false);
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SoundDefs::SetDistance(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec2 *l_val;
    l_argReader.ReadObject(l_obj, g_soundName);
    l_argReader.ReadObject(l_val, g_vec2Name);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_sound->Set3DDistance(*l_val);
        else
            l_argReader.SetError(g_soundInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int SoundDefs::GetState(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
            l_argReader.PushString(g_soundStates[l_sound->GetState()]);
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int SoundDefs::Play(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
        {
            l_sound->Play();
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int SoundDefs::Pause(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
        {
            l_sound->Pause();
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int SoundDefs::Stop(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_soundName);
    if(!l_argReader.HasError())
    {
        ROC::ISound *l_sound;
        if(Utils::IsValid(l_obj) && (l_sound = Utils::Cast<ROC::ISound*>(l_obj)))
        {
            l_sound->Stop();
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_soundInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int SoundDefs::IsSound(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_soundName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::ISound*>(l_obj));
    return l_argReader.GetReturnValue();
}
