using System;
using lebai.l_master;

namespace lebai.app {
  class Program {
    static void Main(string[] args) {
      Console.WriteLine($"Enter example");
      Robot robot = new Robot("172.17.0.5", true);
      robot.stop_sys();
      Console.WriteLine($"stop...");
      robot.start_sys();
      Console.WriteLine($"start...");
      JointMap joint_map = new JointMap();
      joint_map.Add("j1", 0.0);
      joint_map.Add("j2", -60.0 / 180.0 * 3.14);
      joint_map.Add("j3", 80.0 / 180.0 * 3.14);
      joint_map.Add("j4", -10.0 / 180.0 * 3.14);
      joint_map.Add("j5", -60.0 / 180.0 * 3.14);
      joint_map.Add("j6", 0.0);
      robot.movej(joint_map,1.0,1.0,0.0,0.0);
      robot.wait_move();
      Console.WriteLine($"Exit example");
    }
  }
}
