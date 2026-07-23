using ROC.Engine.Core;

namespace ROC.App
{
    public static class App
    {
        static int Main(string[] args)
        {
            Core l_core = new Core();
            l_core.Start();
            while(l_core.DoPulse());
            l_core.Stop();
            return 0;
        }
    }
}
