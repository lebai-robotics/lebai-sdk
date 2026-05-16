package org.sdk.lebai.discovery;

import org.sdk.lebai.ControllerInfo;
import org.sdk.lebai.ControllerInfoVector;
import org.sdk.lebai.Discovery;
import org.sdk.lebai.Loader;

public final class DiscoveryExample {
  public static void main(String[] args) {
    Loader.loadNativeLibraries();
    Discovery discovery = new Discovery();
    try {
      ControllerInfoVector controllers = discovery.resolve();
      try {
        System.out.println("Found " + controllers.size() + " controller(s)");
        for (ControllerInfo controller : controllers) {
          System.out.println(
              controller.getIp_address() + " " + controller.getModel() + " " + controller.getHostname());
        }
      } finally {
        controllers.delete();
      }
    } finally {
      discovery.delete();
    }
  }

  private DiscoveryExample() {}
}
