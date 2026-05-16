using System;
using lebai;

namespace lebai.app
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine($"Enter example");
            Robot robot = new Robot("127.0.0.1", true);
            robot.StopSys();
            Console.WriteLine($"stop...");
            robot.StartSys();
            Console.WriteLine($"start...");
            double[] jp1 = {
                0.0,
                -60.0 / 180.0 * Math.PI,
                80.0 / 180.0 * Math.PI,
                -10.0 / 180.0 * Math.PI,
                -60.0 / 180.0 * Math.PI,
                0.0
            };
            robot.MoveJ(jp1, 1.0, 1.0, 0.0, 0.0);

            double[] jp2 = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
            robot.MoveJ(jp2, 1.0, 1.0, 0.0, 0.0);
            robot.WaitMove();
            Console.WriteLine($"Exit example");
        }
    }
}
