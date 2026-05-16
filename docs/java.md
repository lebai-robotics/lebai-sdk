# Java

## Install

When the Java artifacts are published to Maven Central, users can depend on the
main package:

```xml
<dependency>
  <groupId>io.github.liufang-robot</groupId>
  <artifactId>lebai-java</artifactId>
  <version>2.0.0</version>
</dependency>
```

The Java source package remains `org.sdk.lebai` for compatibility with the
existing package namespace.

Use `org.sdk.lebai.Robot` for application code. It is a Java facade over the
SWIG binding, accepts primitive arrays for common vector parameters, returns
primitive arrays for common vector results, loads native libraries
automatically, and implements `AutoCloseable`.

```java
import org.sdk.lebai.Robot;

try (Robot robot = new Robot("172.17.0.5", true)) {
  robot.startSys();
  robot.moveJ(new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 0.5, 0.5, 0.0, 0.0);
  robot.waitMove(0);
}
```

The lower-level SWIG-generated robot class is `org.sdk.lebai.NativeRobot` in
v2.x. Access it through `Robot.getNative()` when a method has not yet been
wrapped by the facade.

## Local Build

```bash
cmake -S . -B build-java \
  -DBUILD_JAVA=ON \
  -DBUILD_DOCUMENTATION=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_TESTING=OFF \
  -DSKIP_GPG=ON
cmake --build build-java --target java_package
```

The build generates:

- native runtime jar: `build-java/java/lebai-<platform-id>/target/*.jar`
- main Java jar: `build-java/java/lebai-java/target/*.jar`

## Maven Central Release

The release workflow is `.github/workflows/java_release.yml`.

It builds native runtime jars for:

- `lebai-linux-x86-64`
- `lebai-linux-aarch64`
- `lebai-win32-x86-64`

Then it builds the aggregate `lebai-java` package with runtime dependencies on
those native jars. All jars are uploaded to the GitHub Actions artifact named
`lebai-java-jars` and included in the top-level GitHub Release.

If these GitHub repository secrets are configured, the workflow also uploads
signed Maven deployments to the Maven Central Portal:

- `MAVEN_CENTRAL_USERNAME`
- `MAVEN_CENTRAL_PASSWORD`
- `MAVEN_GPG_PRIVATE_KEY`
- `MAVEN_GPG_PASSPHRASE`

The Maven server id is `central`.

The Central Portal plugin is enabled only for deploy steps with
`JAVA_ENABLE_MAVEN_CENTRAL=ON`. Normal local Java package builds do not load
the Central Portal plugin.

`central.autoPublish` defaults to `false`, so the workflow uploads deployments
to the Central Portal but does not automatically publish them. After a release
run, review the deployments in `https://central.sonatype.com/` and publish them
manually when all Java, Python, .NET, and C++ artifacts are correct.
