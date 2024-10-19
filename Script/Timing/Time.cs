using System;
using System.Diagnostics;

namespace CMEngine.Script.Timing
{
    public class Time
    {
        private Stopwatch stopwatch;
        private double prevTime;
        
        public static double dt { get; private set; }
        public static float scaleFactor = 1.0f;

        public Time()
        {
            dt = 0.0167f;
            
            stopwatch = new Stopwatch();
            stopwatch.Start();
            prevTime = stopwatch.Elapsed.TotalSeconds;
        }
        
        public void Update()
        {
            // Calcola il tempo corrente
            double currentFrameTime = stopwatch.Elapsed.TotalSeconds;

            // Calcola il delta time
            dt = currentFrameTime - prevTime;

            dt *= scaleFactor;
            
            // Aggiorna lastFrameTime per il prossimo frame
            prevTime = currentFrameTime;
        }
    }
}
