using System;
using lebai.gripper;

namespace lebai.app
{
    class ExampleGripper
    {
        static int Main(string[] args)
        {
            if (args.Length != 1)
            {
                Console.Error.WriteLine($"Usage: {AppDomain.CurrentDomain.FriendlyName} <serial_port>");
                Console.Error.WriteLine($"Example (Windows): {AppDomain.CurrentDomain.FriendlyName} COM3");
                Console.Error.WriteLine($"Example (Linux): {AppDomain.CurrentDomain.FriendlyName} /dev/ttyUSB0");
                return 0;
            }

            try
            {
                using (var gripper = new Gripper(args[0]))
                {
                    Console.WriteLine("Gripper connected.");
                    gripper.set_position(30);
                    gripper.set_position(70);
                    gripper.do_calibration();
                    Console.WriteLine($"Calibrated: {gripper.is_calibrated()}");
                }
                return 0;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"Error: {ex.Message}");
                return 1;
            }
        }
    }
}
