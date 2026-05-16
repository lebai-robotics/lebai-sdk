# Bindings

## Overview

The C++ library is the source implementation. Python, .NET, and Java bindings are produced from SWIG interface files under `sdk/python`, `sdk/dotnet`, and `sdk/java`.

In practice, a public API change is not fully integrated until the C++ headers, implementation, and the relevant SWIG wrappers all agree.

The current binding interface files prioritize compatibility with the C++ SDK.
They are good enough for generated packages, but they are not the final
idiomatic API design for every language. Prefer incremental cleanup:

- keep the C++ public API as the source of truth for behavior
- add small language-specific aliases or helper wrappers only when they remove
  real friction
- avoid broad renames in SWIG files unless examples, docs, and compatibility
  notes are updated together
- keep generated names stable for existing users unless there is a clear 2.x
  migration reason

## Binding Inputs

### Shared C++ source of truth

- public headers: `sdk/include/lebai/*.hh`
- implementation: `sdk/src/*.cc`, `sdk/src/*.hh`
- internal robot DTOs: `sdk/src/protos_json/*.hh`

### SWIG interface files

- Python: `sdk/python/l_master.i`, `sdk/python/zeroconf.i`, `sdk/python/gripper.i`
- .NET: `sdk/dotnet/l_master.i`, `sdk/dotnet/zeroconf.i`, `sdk/dotnet/gripper.i`
- Java: `sdk/java/l_master.i`, `sdk/java/zeroconf.i`, `sdk/java/gripper.i`

## Python

### Build path

- CMake logic: `cmake/python.cmake`
- SWIG setup: `sdk/python/CMakeLists.txt`
- packaging templates: `python/setup.py.in`, `python/pyproject.toml.in`

### Generated modules

The Python binding generates wrapper modules for:

- `l_master`
- `zeroconf`
- `gripper`

The packaged Python project name is `pylebai`.

The public Python API should prefer `pylebai.Robot` from
`python/pylebai/robot.py`. The facade accepts normal Python `list`/`dict`
arguments and converts common vector returns to `list`, while the SWIG-generated
`pylebai.l_master` module remains available as the lower-level bridge.

### Packaging notes

- the package is assembled in the build tree under `build/python`
- wheels are emitted under `build/python/dist`
- the build copies generated wrapper binaries plus the native runtime pieces needed by the package
- release CI builds Linux x64, Linux arm64, and Windows x64 wheels as artifacts
  first; `.github/workflows/python_release.yml` then validates all wheels with
  `twine check` and publishes once to PyPI using the `PYLEBAI` secret

## .NET

### Build path

- CMake logic: `cmake/dotnet.cmake`
- SWIG setup: `sdk/dotnet/CMakeLists.txt`
- packaging templates: `dotnet/*.csproj.in`, `dotnet/Directory.Build.props.in`

### Generated structure

The `.NET` build creates:

- a native runtime package project named like `lebai.runtime.<rid>`
- a managed package project named `lebai`

The public C# API should prefer the handwritten `lebai.Robot` facade in
`dotnet/RobotFacade.cs`. The facade exposes ordinary C# collections such as
`double[]`, `uint[]`, `int[]`, and `string[]`, while the SWIG-generated
`lebai.l_master` namespace remains the lower-level bridge to the C++ API.

The runtime identifier is selected from the current platform, for example `linux-x64`, `linux-arm64`, or `win-x64`.

The main `lebai` NuGet package directly packs staged native assets from
`runtimes/<rid>/native`. It does not declare `PackageReference` dependencies on
every `lebai.runtime.<rid>` package, because that causes wrong-platform runtime
packages such as `lebai.runtime.linux-x64` to be installed on Windows.

Per-RID runtime packages may still be built as compatibility artifacts. Product
release CI must aggregate native assets from all release platforms before
publishing a single multi-RID `lebai` package to NuGet; otherwise the published
main package only contains the RID built by that job.

`.github/workflows/dotnet_release.yml` is the aggregate .NET release workflow.
It builds native assets for Linux x64, Linux arm64, and Windows x64, packs one
multi-RID `lebai` NuGet package, smoke-tests it, and publishes to NuGet using
the `LEBAI_NUGET_KEY` secret when that secret is configured.

`multi-RID` means the package contains native assets for multiple .NET Runtime
Identifiers, for example:

- `runtimes/linux-x64/native`
- `runtimes/linux-arm64/native`
- `runtimes/win-x64/native`

.NET selects the matching native asset folder for the consumer's target runtime.

### Target frameworks

When `USE_DOTNET_8=ON`, the generated managed package targets:

- `net48`
- `net8.0`

### Important packaging note

SWIG targets exist for:

- `dotnet_l_master`
- `dotnet_zeroconf`
- `dotnet_gripper`

All three targets are linked into `lebai-native`. If you are adding or fixing
binding support, verify the full package path and a local install smoke test,
not just the SWIG compilation step.

SWIG can parse Doxygen comments with `-doxygen`, but the resulting C# XML
documentation is not as polished as hand-authored C# docs. The generated .NET
projects already enable XML documentation output; useful inline docs require
either enabling SWIG Doxygen processing for C# or adding targeted SWIG
documentation features for the public wrapper surface.

## Java

### Build path

- CMake logic: `cmake/java.cmake`
- SWIG setup: `sdk/java/CMakeLists.txt`
- Maven templates: `java/pom-native.xml.in`, `java/pom-local.xml.in`, `java/pom-full.xml.in`, `java/pom-example.xml.in`
- user/developer notes: `docs/java.md`

### Generated structure

The Java build creates:

- a native artifact named like `lebai-<platform-id>`
- a Java package project named `lebai-java`

The configured Java package namespace is `org.sdk.lebai`. The Maven Central
`groupId` is intentionally separate and defaults to `io.github.liufang-robot`
through `JAVA_MAVEN_GROUP_ID`, because the verified Sonatype namespace controls
Maven coordinates while the Java package namespace controls source-level imports.

The public Java API should prefer `org.sdk.lebai.Robot` from `java/Robot.java`.
The SWIG-generated robot class is renamed to `org.sdk.lebai.NativeRobot` so the
public facade can own the `Robot` name; lower-level access remains available via
`Robot.getNative()`.

`.github/workflows/java_release.yml` builds native runtime jars for Linux x64,
Linux arm64, and Windows x64, then builds the aggregate `lebai-java` package.
It uploads all jars as the `lebai-java-jars` GitHub artifact. If
`MAVEN_CENTRAL_USERNAME`, `MAVEN_CENTRAL_PASSWORD`, `MAVEN_GPG_PRIVATE_KEY`, and
`MAVEN_GPG_PASSPHRASE` are configured, the workflow also uploads signed Maven
deployments to the Maven Central Portal using server id `central`. The public
key for `MAVEN_GPG_PRIVATE_KEY` must already be published on a Maven
Central-supported keyserver, such as `keyserver.ubuntu.com`, `keys.openpgp.org`,
or `pgp.mit.edu`; the workflow checks this before upload.

### Important packaging note

SWIG targets exist for:

- `jniLMaster`
- `jniZeroconf`
- `jniGripper`

All three targets are linked into the top-level JNI shared library. If a task
touches Java binding support, validate the full Maven packaging path and not
only the generated wrapper sources.

## What To Update When API Changes

For a public API addition or signature change:

1. update the C++ public header in `sdk/include/lebai`
2. update the C++ implementation in `sdk/src`
3. update request/response DTOs in `sdk/src/protos_json` if the RPC shape
   changed
4. update the relevant SWIG interface files under `sdk/python`, `sdk/dotnet`, and `sdk/java`
5. rebuild the affected package targets
6. verify at least one example in `examples`

## Safe Agent Workflow

- start by deciding whether the change is C++-only or must be visible in bindings
- do not assume all three bindings expose the exact same surface without checking the SWIG files
- prefer validating the packaged build target, because successful SWIG generation alone does not prove the final package is correct
- when changing language-facing behavior, check the corresponding docs in `docs/python.md` and `docs/dotnet.md`
