package org.sdk.lebai;

/** Java-friendly facade for the Lebai robot controller. */
public final class Robot implements AutoCloseable {
  private final NativeRobot nativeRobot;

  public Robot(String ip, boolean simulator) {
    Loader.loadNativeLibraries();
    nativeRobot = new NativeRobot(ip, simulator);
  }

  public Robot(String ip) {
    Loader.loadNativeLibraries();
    nativeRobot = new NativeRobot(ip);
  }

  /** Return the low-level SWIG object for APIs not yet wrapped by this facade. */
  public NativeRobot getNative() {
    return nativeRobot;
  }

  @Override
  public void close() {
    nativeRobot.delete();
  }

  public void startSys() {
    nativeRobot.start_sys();
  }

  public void stopSys() {
    nativeRobot.stop_sys();
  }

  public void stop() {
    nativeRobot.stop();
  }

  public void eStop() {
    nativeRobot.estop();
  }

  public void startTeachMode() {
    nativeRobot.start_teach_mode();
  }

  public void endTeachMode() {
    nativeRobot.end_teach_mode();
  }

  public void pauseMove() {
    nativeRobot.pause_move();
  }

  public void resumeMove() {
    nativeRobot.resume_move();
  }

  public void reboot() {
    nativeRobot.reboot();
  }

  public void waitMove() {
    nativeRobot.wait_move();
  }

  public void waitMove(long id) {
    nativeRobot.wait_move(id);
  }

  public int moveJoint(double[] jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = new DoubleVector(jointPositions);
    try {
      return nativeRobot.move_joint(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveJoint(Iterable<Double> jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = toDoubleVector(jointPositions);
    try {
      return nativeRobot.move_joint(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveJ(double[] jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = new DoubleVector(jointPositions);
    try {
      return nativeRobot.movej(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveJ(Iterable<Double> jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = toDoubleVector(jointPositions);
    try {
      return nativeRobot.movej(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveLinear(double[] jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = new DoubleVector(jointPositions);
    try {
      return nativeRobot.move_linear(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveLinear(Iterable<Double> jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = toDoubleVector(jointPositions);
    try {
      return nativeRobot.move_linear(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveL(double[] jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = new DoubleVector(jointPositions);
    try {
      return nativeRobot.movel(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveL(Iterable<Double> jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = toDoubleVector(jointPositions);
    try {
      return nativeRobot.movel(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int moveCircular(
      double[] jointVia, double[] joint, double rad, double a, double v, double t, double r) {
    DoubleVector via = new DoubleVector(jointVia);
    DoubleVector target = new DoubleVector(joint);
    try {
      return nativeRobot.move_circular(via, target, rad, a, v, t, r);
    } finally {
      target.delete();
      via.delete();
    }
  }

  public int moveC(
      double[] jointVia, double[] joint, double rad, double a, double v, double t, double r) {
    DoubleVector via = new DoubleVector(jointVia);
    DoubleVector target = new DoubleVector(joint);
    try {
      return nativeRobot.movec(via, target, rad, a, v, t, r);
    } finally {
      target.delete();
      via.delete();
    }
  }

  public int speedJoint(double a, double[] speed, double t) {
    DoubleVector values = new DoubleVector(speed);
    try {
      return nativeRobot.speed_joint(a, values, t);
    } finally {
      values.delete();
    }
  }

  public int speedJoint(double a, double[] speed) {
    DoubleVector values = new DoubleVector(speed);
    try {
      return nativeRobot.speed_joint(a, values);
    } finally {
      values.delete();
    }
  }

  public int speedJ(double a, double[] speed, double t) {
    DoubleVector values = new DoubleVector(speed);
    try {
      return nativeRobot.speedj(a, values, t);
    } finally {
      values.delete();
    }
  }

  public int speedJ(double a, double[] speed) {
    DoubleVector values = new DoubleVector(speed);
    try {
      return nativeRobot.speedj(a, values);
    } finally {
      values.delete();
    }
  }

  public int towardJoint(double[] jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = new DoubleVector(jointPositions);
    try {
      return nativeRobot.toward_joint(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public int towardJ(double[] jointPositions, double a, double v, double t, double r) {
    DoubleVector joints = new DoubleVector(jointPositions);
    try {
      return nativeRobot.towardj(joints, a, v, t, r);
    } finally {
      joints.delete();
    }
  }

  public void movePvat(double[] p, double[] v, double[] a, double t) {
    DoubleVector positions = new DoubleVector(p);
    DoubleVector speeds = new DoubleVector(v);
    DoubleVector accelerations = new DoubleVector(a);
    try {
      nativeRobot.move_pvat(positions, speeds, accelerations, t);
    } finally {
      accelerations.delete();
      speeds.delete();
      positions.delete();
    }
  }

  public double[] getActualJointPositions() {
    return toArray(nativeRobot.get_actual_joint_positions());
  }

  public double[] getTargetJointPositions() {
    return toArray(nativeRobot.get_target_joint_positions());
  }

  public double[] getActualJointSpeed() {
    return toArray(nativeRobot.get_actual_joint_speed());
  }

  public double[] getTargetJointSpeed() {
    return toArray(nativeRobot.get_target_joint_speed());
  }

  public double[] getActualJointTorques() {
    return toArray(nativeRobot.get_actual_joint_torques());
  }

  public double[] getTargetJointTorques() {
    return toArray(nativeRobot.get_target_joint_torques());
  }

  public long[] getDos(String device, long pin, long count) {
    return toArray(nativeRobot.get_dos(device, pin, count));
  }

  public void setDos(String device, long pin, long[] values) {
    UintVector vector = new UintVector(values);
    try {
      nativeRobot.set_dos(device, pin, vector);
    } finally {
      vector.delete();
    }
  }

  public long[] getDis(String device, long pin, long count) {
    return toArray(nativeRobot.get_dis(device, pin, count));
  }

  public double[] getAos(String device, long pin, long count) {
    return toArray(nativeRobot.get_aos(device, pin, count));
  }

  public void setAos(String device, long pin, double[] values) {
    DoubleVector vector = new DoubleVector(values);
    try {
      nativeRobot.set_aos(device, pin, vector);
    } finally {
      vector.delete();
    }
  }

  public double[] getAis(String device, long pin, long count) {
    return toArray(nativeRobot.get_ais(device, pin, count));
  }

  public void setLed(long mode, long speed, long[] color) {
    UintVector vector = new UintVector(color);
    try {
      nativeRobot.set_led(mode, speed, vector);
    } finally {
      vector.delete();
    }
  }

  public int[] getSignals(long index, long length) {
    return toArray(nativeRobot.get_signals(index, length));
  }

  public void setSignals(long index, int[] values) {
    IntVector vector = new IntVector(values);
    try {
      nativeRobot.set_signals(index, vector);
    } finally {
      vector.delete();
    }
  }

  public String[] getBoxDevices(String prefix) {
    return toArray(nativeRobot.get_box_devices(prefix));
  }

  public String[] loadTcpList(String dir) {
    return toArray(nativeRobot.load_tcp_list(dir));
  }

  public String[] loadTcpList() {
    return toArray(nativeRobot.load_tcp_list());
  }

  public String[] loadPoseList(String dir) {
    return toArray(nativeRobot.load_pose_list(dir));
  }

  public String[] loadPoseList() {
    return toArray(nativeRobot.load_pose_list());
  }

  public String[] loadFrameList(String dir) {
    return toArray(nativeRobot.load_frame_list(dir));
  }

  public String[] loadFrameList() {
    return toArray(nativeRobot.load_frame_list());
  }

  public String[] loadTrajectoryList(String dir) {
    return toArray(nativeRobot.load_trajectory_list(dir));
  }

  public String[] loadTrajectoryList() {
    return toArray(nativeRobot.load_trajectory_list());
  }

  private static DoubleVector toDoubleVector(Iterable<Double> values) {
    if (values == null) {
      throw new NullPointerException("values");
    }
    return new DoubleVector(values);
  }

  private static double[] toArray(DoubleVector values) {
    try {
      double[] out = new double[values.size()];
      for (int i = 0; i < values.size(); ++i) {
        out[i] = values.get(i);
      }
      return out;
    } finally {
      values.delete();
    }
  }

  private static long[] toArray(UintVector values) {
    try {
      long[] out = new long[values.size()];
      for (int i = 0; i < values.size(); ++i) {
        out[i] = values.get(i);
      }
      return out;
    } finally {
      values.delete();
    }
  }

  private static int[] toArray(IntVector values) {
    try {
      int[] out = new int[values.size()];
      for (int i = 0; i < values.size(); ++i) {
        out[i] = values.get(i);
      }
      return out;
    } finally {
      values.delete();
    }
  }

  private static String[] toArray(StrVector values) {
    try {
      String[] out = new String[values.size()];
      for (int i = 0; i < values.size(); ++i) {
        out[i] = values.get(i);
      }
      return out;
    } finally {
      values.delete();
    }
  }
}
