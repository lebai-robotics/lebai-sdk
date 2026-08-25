package org.sdk.lebai.robot;

import org.sdk.lebai.Robot;

public final class ExampleRunTaskAndGetPrintResult {
  public static void main(String[] args) {
    if (args.length < 2 || args.length > 3) {
      System.err.println(
          "Usage: ExampleRunTaskAndGetPrintResult <robot_ip> <task_name> [sim]");
      return;
    }

    if (args.length == 3 && !"sim".equals(args[2])) {
      System.err.println("The optional third argument must be 'sim'");
      return;
    }

    try (Robot robot = new Robot(args[0], args.length == 3)) {
      robot.startSys();

      long taskId = robot.getNative().start_task(args[1]);
      System.out.println("Started task " + taskId);

      String printResult = robot.getNative().wait_task(taskId);
      System.out.println("Task print result:");
      System.out.print(printResult);
      if (printResult.isEmpty() || !printResult.endsWith("\n")) {
        System.out.println();
      }
    }
  }

  private ExampleRunTaskAndGetPrintResult() {}
}
