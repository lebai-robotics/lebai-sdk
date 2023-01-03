package org.sdk.lebai.examples;
import org.sdk.lebai.Loader;
import org.sdk.lebai.Robot;
import org.sdk.lebai.DoubleVector;

public final class example {
  public static void main(String[] args) {
    Loader.loadNativeLibraries();
    System.out.println("Example java robot sdk...");
    Robot robot = new Robot("172.17.0.5", true);
    robot.stop_sys();
    robot.start_sys();
    DoubleVector jp = new DoubleVector();
    jp.add(-0.5);
    jp.add(-0.5);
    jp.add(1.0);
    jp.add(1.5);
    jp.add(2.0);
    jp.add(2.5);
    robot.movej(jp, 0.5, 0.5, 0.0, 0.0);
    jp.set(0, 0.0);
    jp.set(1, 0.0);
    jp.set(2, 0.0);
    jp.set(3, 0.0);
    jp.set(4, 0.0);
    jp.set(5, 0.0);
    robot.movej(jp, 0.5, 0.5, 0.0, 0.0);
    robot.wait_move(0);
  }

  private example() {}
}
