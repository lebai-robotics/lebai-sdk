using System;
using lebai.l_master;

namespace lebai.app
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine($"Enter example");
            Robot robot = new Robot("192.168.1.100", true);
            robot.stop_sys();
            Console.WriteLine($"stop...");
            robot.start_sys();
            Console.WriteLine($"start...");
            DoubleVector jp1 = new DoubleVector();
            jp1.Add(0.0);
            jp1.Add(-60.0 / 180.0 * 3.14);
            jp1.Add(80.0 / 180.0 * 3.14);
            jp1.Add(-10.0 / 180.0 * 3.14);
            jp1.Add(-60.0 / 180.0 * 3.14);
            jp1.Add(0.0);
            robot.movej(jp1, 1.0, 1.0, 0.0, 0.0);
            DoubleVector jp2 = new DoubleVector();
            jp2.Add(0.0);
            jp2.Add(0.0);
            jp2.Add(0.0);
            jp2.Add(0.0);
            jp2.Add(0.0);
            jp2.Add(0.0);
            robot.movej(jp2, 1.0, 1.0, 0.0, 0.0);
            robot.wait_move();
            Console.WriteLine($"Exit example");
        }
    }
}
