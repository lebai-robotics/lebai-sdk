package org.sdk.lebai.robot;
import org.sdk.lebai.Robot;

public final class RobotExample {
  public static void main(String[] args) {
    if (args.length < 1) {
      System.err.println("Usage: RobotExample <robot_ip> [sim] [--move]");
      return;
    }

    boolean simulator = false;
    boolean runMotion = false;
    for (int i = 1; i < args.length; ++i) {
      if ("sim".equals(args[i])) {
        simulator = true;
      } else if ("--move".equals(args[i])) {
        runMotion = true;
      }
    }

    System.out.println("Example java robot sdk...");
    try (Robot robot = new Robot(args[0], simulator)) {
      System.out.println("System: " + robot.getNative().get_system_info().getName());
      System.out.println("State: " + robot.getNative().get_robot_state());
      System.out.println("Estop reason: " + robot.getNative().get_estop_reason());

      if (!runMotion) {
        System.out.println("Motion skipped. Pass --move to run the motion demo.");
        return;
      }

      robot.startSys();
      robot.moveJ(new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.5}, 0.5, 0.5, 0.0, 0.0);
      robot.moveJ(new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 0.5, 0.5, 0.0, 0.0);
      robot.waitMove(0);
    }
  }

  private RobotExample() {}
}
