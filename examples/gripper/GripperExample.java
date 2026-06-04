package org.sdk.lebai.gripper;

import org.sdk.lebai.Gripper;
import org.sdk.lebai.Loader;

public final class GripperExample {
  public static void main(String[] args) {
    if (args.length < 1 || args.length > 2) {
      System.err.println("Usage: GripperExample <serial_port> [position]");
      System.err.println("Example (Windows): GripperExample COM3 50");
      System.err.println("Example (Linux): GripperExample /dev/ttyUSB0 50");
      return;
    }

    long position = 50;
    if (args.length == 2) {
      try {
        if (args[1].startsWith("-")) {
          throw new NumberFormatException();
        }
        position = Long.parseLong(args[1]);
        if (position > 100) {
          throw new NumberFormatException();
        }
      } catch (NumberFormatException ex) {
        System.err.println("Invalid position: " + args[1]);
        return;
      }
    }

    Loader.loadNativeLibraries();
    Gripper gripper = new Gripper(args[0]);
    try {
      System.out.println("Setting gripper position to: " + position);
      gripper.set_position(position);
    } finally {
      gripper.delete();
    }
  }

  private GripperExample() {}
}
