using IcyEngine.Script.FileLoader;
using System.Runtime.InteropServices;

namespace IcyScripting.Script.Main
{
    public class Program
    {
        static GameCore gameCore = new GameCore();
        static Time time = new Time();
        static Property propertyManager = new Property();
        static ScriptPathLoader pathLoader = new ScriptPathLoader();

        public static void Example()
        {
            Console.WriteLine(35);
        }

        // Definisci un delegato che corrisponde alla firma di LoadScriptPath
        public delegate int LoadScriptPathDelegate(IntPtr pathPtr, int arg1);

        public static int LoadScriptPath(IntPtr pathPtr, int arg1)
        {
            string path = Marshal.PtrToStringUni(pathPtr);
            Console.WriteLine("Path from C#: " + path);

            gameCore.Initialize(path);

            return 0;
        }

        public static void Start()
        {
            propertyManager.RegisterProperties();
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