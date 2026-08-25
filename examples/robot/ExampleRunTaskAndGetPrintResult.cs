using System;
using lebai;

namespace lebai.app
{
    class ExampleRunTaskAndGetPrintResult
    {
        static int Main(string[] args)
        {
            if (args.Length < 2 || args.Length > 3)
            {
                Console.Error.WriteLine(
                    $"Usage: {AppDomain.CurrentDomain.FriendlyName} <robot_ip> <task_name> [sim]");
                return 1;
            }

            if (args.Length == 3 && args[2] != "sim")
            {
                Console.Error.WriteLine("The optional third argument must be 'sim'");
                return 1;
            }

            using (var robot = new Robot(args[0], args.Length == 3))
            {
                robot.StartSys();

                uint taskId = robot.Native.start_task(args[1]);
                Console.WriteLine($"Started task {taskId}");

                string printResult = robot.Native.wait_task(taskId);
                Console.WriteLine("Task print result:");
                Console.Write(printResult);
                if (printResult.Length == 0 || !printResult.EndsWith("\n", StringComparison.Ordinal))
                {
                    Console.WriteLine();
                }
            }

            return 0;
        }
    }
}
