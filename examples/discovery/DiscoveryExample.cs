using System;
using lebai.zeroconf;

namespace lebai.app
{
    class DiscoveryExample
    {
        static void Main()
        {
            using (var discovery = new Discovery())
            using (var controllers = discovery.resolve())
            {
                Console.WriteLine($"Found {controllers.Count} controller(s)");
                foreach (var controller in controllers)
                {
                    Console.WriteLine($"{controller.ip_address} {controller.model} {controller.hostname}");
                }
            }
        }
    }
}
