#include "stdafx.h"
#include "Core.h"

BOOL APIENTRY DllMain(HMODULE, DWORD  ul_reason_for_call, LPVOID)
{
    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) ROC::IModule* ModuleInit(ROC::ICore *p_core)
{
    Core::Init(p_core);
    return Core::GetInstance();
}

