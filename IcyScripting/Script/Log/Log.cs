namespace IcyScripting.Script.Log
{
    public static class Logger
    {
        public static void Debug(object msg)
        {
            Console.WriteLine(msg.ToString());
        }
        public static void Succeed(object msg)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine(msg.ToString());
            Console.ResetColor();
        }
        public static void Warning(object msg)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine(msg.ToString());
            Console.ResetColor();
        }
        public static void Error(object msg)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(msg.ToString());
            Console.ResetColor();
        }
        
        public static void Debug(string format, params object[] args)
        {
            string msg = string.Format(format, args);
            Console.WriteLine(msg);
        }
        public static void Succeed(string format, params object[] args)
        {
            string msg = string.Format(format, args);
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine(msg);
            Console.ResetColor();
        }
        public static void Warning(string format, params object[] args)
        {
            string msg = string.Format(format, args);
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine(msg);
            Console.ResetColor();
        }
        public static void Error(string format, params object[] args)
        {
            string msg = string.Format(format, args);
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(msg);
            Console.ResetColor();
        }
    }
}

// public class Debug
// {
//     public Debug(string msg)
//     {
//         Console.WriteLine(msg);
//     }
// }
// 
// public class Succeed
// {
//     public Succeed(string msg)
//     {
//         Console.ForegroundColor = ConsoleColor.Green;
//         Console.WriteLine(msg);
//         Console.ResetColor();
//     }
// }
// 
// public class Warning
// {
//     public Warning(string msg)
//     {
//         Console.ForegroundColor = ConsoleColor.Yellow;
//         Console.WriteLine(msg);
//         Console.ResetColor();
//     }
// }
// 
// public class Error
// {
//     public Error(string msg)
//     {
//         Console.ForegroundColor = ConsoleColor.Red;
//         Console.WriteLine(msg);
//         Console.ResetColor();
//     }
// }