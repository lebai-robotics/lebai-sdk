using System;
using lebai.gripper;

namespace lebai.app
{
    class GripperExample
    {
        static int Main(string[] args)
        {
            if (args.Length < 1 || args.Length > 2)
            {
                Console.Error.WriteLine($"Usage: {AppDomain.CurrentDomain.FriendlyName} <serial_port> [position]");
                Console.Error.WriteLine($"Example (Windows): {AppDomain.CurrentDomain.FriendlyName} COM3 50");
                Console.Error.WriteLine($"Example (Linux): {AppDomain.CurrentDomain.FriendlyName} /dev/ttyUSB0 50");
                return 1;
            }

            uint position = 50;
            if (args.Length == 2 &&
                (!uint.TryParse(args[1], out position) || position > 100))
            {
                Console.Error.WriteLine($"Invalid position: {args[1]}");
                return 1;
            }

            try
            {
                using (var gripper = new Gripper(args[0]))
                {
                    Console.WriteLine("Gripper connected.");
                    Console.WriteLine($"Setting gripper position to: {position}");
                    gripper.set_position(position);
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
