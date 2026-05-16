package org.sdk.lebai.gripper;

import org.sdk.lebai.Gripper;
import org.sdk.lebai.Loader;

public final class GripperExample {
  public static void main(String[] args) {
    if (args.length != 1) {
      System.err.println("Usage: GripperExample <serial_port>");
      System.err.println("Example (Windows): GripperExample COM3");
      System.err.println("Example (Linux): GripperExample /dev/ttyUSB0");
      return;
    }

    Loader.loadNativeLibraries();
    Gripper gripper = new Gripper(args[0]);
    try {
      gripper.set_position(30);
      gripper.set_position(70);
      gripper.do_calibration();
      System.out.println("Calibrated: " + gripper.is_calibrated());
    } finally {
      gripper.delete();
    }
  }

  private GripperExample() {}
}
