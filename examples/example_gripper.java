package org.sdk.lebai.examples;

import org.sdk.lebai.Gripper;
import org.sdk.lebai.Loader;

public final class example_gripper {
  public static void main(String[] args) {
    if (args.length != 1) {
      System.err.println("Usage: example_gripper <serial_port>");
      System.err.println("Example (Windows): example_gripper COM3");
      System.err.println("Example (Linux): example_gripper /dev/ttyUSB0");
      return;
    }

    Loader.loadNativeLibraries();
    Gripper gripper = new Gripper(args[0]);
    gripper.set_position(30);
    gripper.set_position(70);
    gripper.do_calibration();
    System.out.println("Calibrated: " + gripper.is_calibrated());
  }

  private example_gripper() {}
}

