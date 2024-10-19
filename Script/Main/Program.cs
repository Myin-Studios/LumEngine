public class Program
{
    public delegate void ExampleDelegate();
    
    public static void Example()
    {
        Console.WriteLine(35);
    }
    
    public static void ScriptRunner()
    {
        try
        {
            GameCore gameCore = new GameCore();
            Time time = new Time();
            var propertyManager = new Property();
            propertyManager.RegisterProperties();

            gameCore.Initialize();

            while (true)
            {
                gameCore.Update();
                time.Update();
                System.Threading.Thread.Sleep(1000 / 60); // Simula un frame time di circa 60 FPS
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Exception occurred: {ex.Message}");
            return; // Ritorna un errore
        }
    }
    
    static void Main(string[] args)
    {
        
    }   
}