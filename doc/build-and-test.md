# Build And Test

## Purpose

This file is a quick build and verification guide for contributors and AI agents. It complements `README.md`, `doc/python.md`, `doc/dotnet.md`, and the CI workflows under `.github/workflows`.

## Prerequisites

### Linux

The existing docs and CI workflows expect:

- a C++14-capable compiler
- CMake
- Python and `pip`
- SWIG when building bindings
- Doxygen and Graphviz when building docs
- Maven and a JDK when building Java bindings
- the `.NET` SDK when building `.NET` bindings

The CI workflow in `.github/workflows/linux_cpp_build.yml` is the best reference for a known-good Linux build sequence.

### Windows

The project is set up for CMake with Visual Studio generators as well as command-line CMake builds. The top-level `README.md` notes that Windows binding support has had limitations, so verify language-specific behavior before claiming Windows parity.

## Common Configure Options

Key cache options from `CMakeLists.txt`:

- `BUILD_CXX=ON|OFF`
- `BUILD_PYTHON=ON|OFF`
- `BUILD_DOTNET=ON|OFF`
- `BUILD_JAVA=ON|OFF`
- `BUILD_TESTING=ON|OFF`
- `BUILD_EXAMPLES=ON|OFF`
- `BUILD_DEB=ON|OFF`
- `BUILD_DOCUMENTATION=ON|OFF`
- `CLANG_FORMAT_CHECK=ON|OFF`
- `ENABLE_TSAN=ON|OFF`
- `TEST_ROBOT_IP=<ip>`

## Baseline C++ Build

```bash
cmake -S . -B build
cmake --build build
```

This is the shortest path for building the core C++ SDK.

## C++ Tests

### Configure and run

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DTEST_ROBOT_IP=127.0.0.1
cmake --build build
cmake --build build --target test
```

### Defined test executables

- `sdk/test/test_protos.cc`
- `sdk/test/test_robot.cc`
- `sdk/test/test_lua_robot.cc`
- `sdk/test/test_discovery.cc`

CTest names are:

- `TestProtos`
- `TestRobot`
- `TestLuaRobot`
- `TestDiscovery`

`TEST_ROBOT_IP` is compiled into the robot-facing tests; if not set, they default to `127.0.0.1`.

## Python Build

```bash
cmake -S . -B build -DBUILD_PYTHON=ON
cmake --build build --target python_package
```

Notes:

- Python packaging is driven by `cmake/python.cmake`
- templates come from `python/setup.py.in` and `python/pyproject.toml.in`
- wheels are built from the build tree and written under `build/python/dist`
- the package build depends on SWIG-generated wrapper modules for `l_master`, `zeroconf`, and `gripper`

## .NET Build

```bash
cmake -S . -B build -DBUILD_DOTNET=ON
cmake --build build --target dotnet_package
```

Notes:

- `.NET` packaging is driven by `cmake/dotnet.cmake`
- generated projects live under `build/dotnet`
- package projects are built and packed with the `dotnet` CLI
- current configuration targets `net48` and `net8.0` when `USE_DOTNET_8=ON`
- existing `doc/dotnet.md` says the supported path is Linux-first and Windows should be verified carefully

## Java Build

```bash
cmake -S . -B build -DBUILD_JAVA=ON
cmake --build build --target java_package
```

Notes:

- Java packaging is driven by `cmake/java.cmake`
- Maven is required
- generated Java projects live under `build/java`
- artifacts are built through Maven `compile`, `package`, and `install`

## Examples

Examples are enabled by default with `BUILD_EXAMPLES=ON` and live in `examples`.

- C++ examples are built through `examples/CMakeLists.txt`
- `.NET` and Java examples are only configured when their respective bindings are enabled
- `examples/test_gripper_diagnostics.cc` is special-cased and links directly against `modbus`

## Formatting And Docs

### Apply formatting

```bash
cmake --build build --target clang-format
```

### Check formatting in CI style

```bash
cmake -S . -B build -DCLANG_FORMAT_CHECK=ON
cmake --build build --target clang-format
```

### Build API docs

```bash
cmake -S . -B build -DBUILD_DOCUMENTATION=ON
cmake --build build --target doc
```

## Generated Files And Cautions

- `sdk/include/lebai/config.hh` is generated from `sdk/config.hh.in`
- files under `build/` are generated outputs and should usually not be edited directly
- language package metadata is produced from templates in `python/`, `dotnet/`, and `java/`
- `sdk/src/protos` files are source files, even though the directory name suggests generated content

## Good Validation Order

When changing public behavior, prefer this sequence:

1. configure with only the targets you need
2. build the core C++ library
3. run C++ tests if the change affects runtime behavior
4. build the relevant binding package if the public API changed
5. run or inspect a matching example when the change is user-facing
