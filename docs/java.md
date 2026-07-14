# Java平台应用文档

## 安装

Java 构件发布到 Maven Central 后，可以在应用项目中依赖主包：

```xml
<dependency>
  <groupId>io.github.liufang-robot</groupId>
  <artifactId>lebai-java</artifactId>
  <version>2.0.4</version>
</dependency>
```

Java 源码包名仍为 `org.sdk.lebai`，用于保持现有 SWIG API 命名空间兼容。

应用代码推荐使用 `org.sdk.lebai.Robot`。它是 SWIG 绑定外层的 Java 封装：
常见向量参数使用基本类型数组传入，常见向量结果返回基本类型数组，会自动加载原生库，
并实现 `AutoCloseable`。

```java
import org.sdk.lebai.Robot;

try (Robot robot = new Robot("172.17.0.5", true)) {
  robot.startSys();
  robot.moveJ(new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 0.5, 0.5, 0.0, 0.0);
  robot.waitMove(0);
}
```

较底层的 SWIG 生成机器人类在 v2.x 中是 `org.sdk.lebai.NativeRobot`。
当某个方法尚未由 `Robot` 封装时，可以通过 `Robot.getNative()` 调用底层对象。

## 夹爪

直接 RS485 夹爪接口由 SWIG 生成的 `org.sdk.lebai.Gripper` 类提供。
它独立于主 `Robot` JSON-RPC 连接，需要传入本机串口设备路径。

```java
import org.sdk.lebai.Gripper;
import org.sdk.lebai.Loader;

Loader.loadNativeLibraries();

Gripper gripper = new Gripper("/dev/serial/by-id/usb-1a86_USB_Serial-if00-port0");
try {
  gripper.set_position(50);
} finally {
  gripper.delete();
}
```

当前维护的 Java 示例是 `examples/gripper/GripperExample.java`，运行方式为：

```bash
GripperExample <serial_port> [position]
```

`position` 是可选参数，默认值为 `50`，取值范围为 `0-100`。
Linux 上优先使用 `/dev/serial/by-id/` 下的稳定路径，而不是 `/dev/ttyUSB0`，
因为 USB 串口设备重连后可能重新编号。

## 本地构建

```bash
cmake -S . -B build-java \
  -DBUILD_JAVA=ON \
  -DBUILD_DOCUMENTATION=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_TESTING=OFF \
  -DSKIP_GPG=ON
cmake --build build-java --target java_package
```

构建会生成：

- 原生运行时 jar：`build-java/java/lebai-<platform-id>/target/*.jar`
- 主 Java jar：`build-java/java/lebai-java/target/*.jar`

## Maven Central 发布

发布工作流为 `.github/workflows/java_release.yml`。

它会为以下平台构建原生运行时 jar：

- `lebai-linux-x86-64`
- `lebai-linux-aarch64`
- `lebai-win32-x86-64`

随后构建聚合包 `lebai-java`，并声明对这些原生 jar 的运行时依赖。
所有 jar 会上传到名为 `lebai-java-jars` 的 GitHub Actions 构件，
并包含在顶层 GitHub Release 中。

如果配置了以下 GitHub 仓库 secrets，工作流还会将签名后的 Maven 部署上传到
Maven Central Portal：

- `MAVEN_CENTRAL_USERNAME`
- `MAVEN_CENTRAL_PASSWORD`
- `MAVEN_GPG_PRIVATE_KEY`
- `MAVEN_GPG_PASSPHRASE`

Maven server id 为 `central`。

与 `MAVEN_GPG_PRIVATE_KEY` 对应的公钥必须在发布前公开。
Maven Central 会使用公钥服务器校验签名，支持 `keyserver.ubuntu.com`、
`keys.openpgp.org` 和 `pgp.mit.edu`。发布任务会先导入配置的私钥、读取指纹，
然后确认能从这些服务器之一获取对应公钥，再开始上传到 Maven Central。

Central Portal 插件只在 `JAVA_ENABLE_MAVEN_CENTRAL=ON` 的部署步骤中启用。
普通本地 Java 包构建不会加载 Central Portal 插件。

`central.autoPublish` 默认值为 `true`，所以发布 CI 会通过 Central Portal
自动发布已校验的部署。CI 完成后，在 `https://central.sonatype.com/` 查看发布运行和已发布构件。
