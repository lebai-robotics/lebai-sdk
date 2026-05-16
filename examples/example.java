package org.sdk.lebai.examples;
import org.sdk.lebai.Robot;

public final class example {
  public static void main(String[] args) {
    System.out.println("Example java robot sdk...");
    try (Robot robot = new Robot("172.17.0.5", true)) {
      robot.stopSys();
      robot.startSys();
      robot.moveJ(new double[] {-0.5, -0.5, 1.0, 1.5, 2.0, 2.5}, 0.5, 0.5, 0.0, 0.0);
      robot.moveJ(new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 0.5, 0.5, 0.0, 0.0);
      robot.waitMove(0);
    }
  }

  private example() {}
}
