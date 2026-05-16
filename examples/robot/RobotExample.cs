using System;
using lebai;

namespace lebai.app
{
    class RobotExample
    {
        static int Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.Error.WriteLine($"Usage: {AppDomain.CurrentDomain.FriendlyName} <robot_ip> [sim] [--move]");
                return 1;
            }

            bool simulator = Array.IndexOf(args, "sim") >= 0;
            bool runMotion = Array.IndexOf(args, "--move") >= 0;

            using (var robot = new Robot(args[0], simulator))
            {
                Console.WriteLine($"System: {robot.Native.get_system_info().name}");
                Console.WriteLine($"State: {robot.Native.get_robot_state()}");
                Console.WriteLine($"Estop reason: {robot.Native.get_estop_reason()}");

                if (runMotion)
                {
                    robot.StartSys();
                    robot.MoveJ(new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.5 }, 1.0, 0.5, 0.0, 0.0);
                    robot.MoveJ(new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, 1.0, 0.5, 0.0, 0.0);
                    robot.WaitMove();
                }
                else
                {
                    Console.WriteLine("Motion skipped. Pass --move to run the motion demo.");
                }
            }

            return 0;
        }
    }
}
