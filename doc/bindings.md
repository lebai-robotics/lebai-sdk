# Bindings

## Overview

The C++ library is the source implementation. Python, .NET, and Java bindings are produced from SWIG interface files under `sdk/python`, `sdk/dotnet`, and `sdk/java`.

In practice, a public API change is not fully integrated until the C++ headers, implementation, and the relevant SWIG wrappers all agree.

## Binding Inputs

### Shared C++ source of truth

- public headers: `sdk/include/lebai/*.hh`
- implementation: `sdk/src/*.cc`, `sdk/src/*.hh`
- DTOs: `sdk/src/protos/*.hh`, `sdk/src/protos/*.cc`

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

### Packaging notes

- the package is assembled in the build tree under `build/python`
- wheels are emitted under `build/python/dist`
- the build copies generated wrapper binaries plus the native runtime pieces needed by the package

## .NET

### Build path

- CMake logic: `cmake/dotnet.cmake`
- SWIG setup: `sdk/dotnet/CMakeLists.txt`
- packaging templates: `dotnet/*.csproj.in`, `dotnet/Directory.Build.props.in`

### Generated structure

The `.NET` build creates:

- a native runtime package project named like `lebai.runtime.<rid>`
- a managed package project named `lebai`

The runtime identifier is selected from the current platform, for example `linux-x64`, `linux-arm64`, or `win-x64`.

### Target frameworks

When `USE_DOTNET_8=ON`, the generated managed package targets:

- `net48`
- `net8.0`

### Important packaging note

SWIG targets exist for:

- `dotnet_l_master`
- `dotnet_zeroconf`
- `dotnet_gripper`

But the top-level native package target currently links only `dotnet_l_master` and `dotnet_zeroconf` into `lebai-native`. If you are adding or fixing gripper support for `.NET`, verify the full package path, not just the SWIG compilation step.

## Java

### Build path

- CMake logic: `cmake/java.cmake`
- SWIG setup: `sdk/java/CMakeLists.txt`
- Maven templates: `java/pom-native.xml.in`, `java/pom-local.xml.in`, `java/pom-full.xml.in`, `java/pom-example.xml.in`

### Generated structure

The Java build creates:

- a native artifact named like `lebai-<platform-id>`
- a Java package project named `lebai-java`

The configured Java package namespace is `org.sdk.lebai`.

### Important packaging note

SWIG targets exist for:

- `jniLMaster`
- `jniZeroconf`
- `jniGripper`

But the top-level JNI shared library currently links only `jniLMaster` and `jniZeroconf`. If a task touches gripper support for Java, validate the full Maven packaging path and not only the generated wrapper sources.

## What To Update When API Changes

For a public API addition or signature change:

1. update the C++ public header in `sdk/include/lebai`
2. update the C++ implementation in `sdk/src`
3. update request/response DTOs in `sdk/src/protos` if the RPC shape changed
4. update the relevant SWIG interface files under `sdk/python`, `sdk/dotnet`, and `sdk/java`
5. rebuild the affected package targets
6. verify at least one example in `examples`

## Safe Agent Workflow

- start by deciding whether the change is C++-only or must be visible in bindings
- do not assume all three bindings expose the exact same surface without checking the SWIG files
- prefer validating the packaged build target, because successful SWIG generation alone does not prove the final package is correct
- when changing language-facing behavior, check the corresponding docs in `doc/python.md` and `doc/dotnet.md`
