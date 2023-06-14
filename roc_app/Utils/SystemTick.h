#pragma once

namespace ROC
{

class SystemTick final
{
    static unsigned long long ms_tick;
    static unsigned long long ms_delta;

    SystemTick() = delete;
    ~SystemTick() = delete;
public:
    static unsigned long long GetTick();
    static unsigned long long GetDelta();

    static void Init();
    static void UpdateTick();
};

}
