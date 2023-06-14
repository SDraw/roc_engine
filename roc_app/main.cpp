#include "stdafx.h"

#include "Core/Core.h"

#ifdef _DEBUG
int main(int argc, char *argv[])
{
    SetConsoleTitleA("Debug Console");
#else
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
#endif
    if(ROC::Core::Init())
    {
        ROC::Core *l_core = ROC::Core::GetInstance();
        while(l_core->DoPulse());
        ROC::Core::Terminate();
        l_core = nullptr;
    }

    return EXIT_SUCCESS;
}
