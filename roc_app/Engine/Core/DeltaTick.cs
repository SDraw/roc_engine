namespace ROC.Engine.Core
{
    static public class DeltaTick
    {
        static ulong ms_lastTick = 0UL;
        static float ms_engineDelta = 0f;
        static ulong ms_frameIndex = 0UL;

        static internal void Start()
        {
            ms_lastTick = Utils.GetTickCount64();
            ms_engineDelta = 0f;
            ms_frameIndex = 0UL;
        }

        static internal void Stop()
        {
            ms_lastTick = 0UL;
            ms_engineDelta = 0f;
            ms_frameIndex = 0UL;
        }

        static internal void Update()
        {
            ulong l_tick = Utils.GetTickCount64();
            ms_engineDelta = (l_tick - ms_lastTick) * 0.001f;
            ms_lastTick = l_tick;

            ms_frameIndex++;
        }

        public static float Delta => ms_engineDelta;
        public static ulong FrameIndex => ms_frameIndex;
    }
}
