using System;
using System.IO;

namespace ROC.Converter
{
    public static class App
    {
        static int Main(string[] args)
        {
            if(args.Length == 0)
            {
                Console.WriteLine("No input file specified.");
                return 0;
            }

            string l_directory = Path.GetDirectoryName(args[0]);
            GLTFModel l_model = new GLTFModel();
            l_model.Load(args[0]);
            l_model.Save(l_directory);

            Console.WriteLine("Press any key ...");
            Console.ReadKey();
            return 0;
        }
    }
}
