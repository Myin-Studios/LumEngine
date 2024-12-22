using System.Runtime.InteropServices;

using LumScripting.Script.Timing;
using LumScripting.Script.Properties;
using LumScripting.Script.Log;

namespace LumScriptLoader.Main
{
    public class Program
    {
        static GameCore gameCore = new GameCore();
        static Time time = new Time();

        public static void Example()
        {
            Console.WriteLine(35);
        }

        public static void InitUIManager()
        {
            // UIManagerManaged UIManager = new UIManagerManaged();
        }

        // Definisci un delegato che corrisponde alla firma di LoadScriptPath
        public delegate int LoadScriptPathDelegate(IntPtr pathPtr, int arg1);

        public static int LoadScriptPath(IntPtr pathPtr, int arg1)
        {
            // Verifica assemblies duplicati
            var loadedAssemblies = AppDomain.CurrentDomain.GetAssemblies()
                .GroupBy(a => a.GetName().Name)
                .Where(g => g.Count() > 1)
                .ToDictionary(g => g.Key, g => g.Count());

            if (loadedAssemblies.Any())
            {
                Logger.Warning("Duplicate assemblies detected:");
                foreach (var kvp in loadedAssemblies)
                {
                    Logger.Warning($"{kvp.Key}: loaded {kvp.Value} times");
                }
            }

            string path = Marshal.PtrToStringUni(pathPtr);
            Console.WriteLine("Path from C#: " + path);

            gameCore.Initialize(path);

            return 0;
        }

        public static void Start()
        {
            //propertyManager.RegisterProperties();
            gameCore.Start();
        }

        public static void Run()
        {
            gameCore.Update();
            time.Update();
        }
        
        public static void StartScript()
        {
            
        }
    
        static void Main(string[] args)
        {
        
        }   
    }
}