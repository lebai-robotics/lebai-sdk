# Build And Test

## Purpose

This file is a quick build and verification guide for contributors and AI agents. It complements `README.md`, `docs/python.md`, `docs/dotnet.md`, and the CI workflows under `.github/workflows`.

## Prerequisites

### Linux

The existing docs and CI workflows expect:

- a C++14-capable compiler
- CMake
- Python and `pip`
- SWIG when building bindings
- `python3-venv` when running the local Python wheel smoke check
- Doxygen and Graphviz when building docs
- Maven and a JDK when building Java bindings
- the `.NET` SDK when building `.NET` bindings

On Ubuntu 22.04, a practical development setup is:

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  git \
  python3 \
  python3-pip \
  python3-venv \
  doxygen \
  graphviz \
  default-jdk \
  maven \
  dotnet-sdk-8.0
```

For Python packaging, keep `build`, `wheel`, and `twine` available in the
environment used to build wheels:

```bash
python3 -m pip install --user build wheel twine
```

The CI workflow in `.github/workflows/linux_cpp_build.yml` is the best reference for a known-good Linux build sequence.

If the distro SWIG package is unavailable or too old, install a user-level SWIG
executable with `uv` instead of writing into system Python directories:

```bash
uv tool install swig
swig -version
```

Avoid `uv pip install --system swig` for local development unless the shell has
permission to write to the system site-packages directory.

### Windows

The project is set up for CMake with Visual Studio generators as well as command-line CMake builds. The top-level `README.md` notes that Windows binding support has had limitations, so verify language-specific behavior before claiming Windows parity.

Install these tools for local Windows development:

- Visual Studio 2022 with the "Desktop development with C++" workload
- CMake, either from Visual Studio or a standalone installer
- Python 3.8 or newer when building Python wheels
- SWIG 4.x on `PATH` when building bindings
- .NET SDK 8.x when building the C# package
- JDK and Maven when building Java bindings

Windows C# native package builds use the static MSVC runtime (`/MT`) so C#
consumers should not need to install the Visual C++ redistributable only to load
`lebai-native.dll`.

### Tool Notes

The main build surfaces are:

- C++: compiler + CMake
- Python: CMake + SWIG + Python development headers/modules
- C#: CMake + SWIG + .NET SDK 8
- Java: CMake + SWIG + JDK + Maven

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

### Local simulator endpoint

For the local simulated L master controller, use host endpoint `127.0.0.1:3030`.
The controller container may report an internal Docker IP such as `172.17.0.3`,
but the useful SDK test target is the forwarded localhost port. Do not use
`3031` for this simulator setup unless the container explicitly exposes it.

## Python Build

```bash
cmake -S . -B build -DBUILD_PYTHON=ON
cmake --build build --target python_package
```

### Wheel smoke check

After building `python_package`, install the generated wheel into a clean venv
and import the SWIG modules:

```bash
scripts/python_wheel_smoke.sh build python3.10
```

On Windows, use the PowerShell equivalent:

```powershell
.\scripts\python_wheel_smoke.ps1 -WheelDir build\python\dist -Python python.exe
```

Notes:

- Python packaging is driven by `cmake/python.cmake`
- templates come from `python/setup.py.in` and `python/pyproject.toml.in`
- wheels are built from the build tree and written under `build/python/dist`
- the package build depends on SWIG-generated wrapper modules for `l_master`, `zeroconf`, and `gripper`
- Python wrappers are generated with SWIG `autodoc` and `doxygen` enabled so
  public methods expose inline Python docstrings where SWIG can translate the
  C++ declarations and comments

## .NET Build

```bash
cmake -S . -B build -DBUILD_DOTNET=ON
cmake --build build --target dotnet_package
```

Notes:

- `.NET` packaging is driven by `cmake/dotnet.cmake`
- generated projects live under `build/dotnet`
- package projects are built and packed with the `dotnet` CLI
- the main `lebai` package includes the generated managed assembly and native
  assets staged under `runtimes/<rid>/native`
- split packages named `lebai.runtime.<rid>` may still be generated for
  compatibility, but the main package does not depend on all runtime packages
- use `DOTNET_CLI_HOME=/tmp/dotnet-cli-home` in restricted environments where
  the .NET SDK cannot write to the default home directory
- current configuration targets `net48` and `net8.0` when `USE_DOTNET_8=ON`

To assemble a local package from staged native assets, use:

```bash
bash scripts/dotnet_pack_multirid.sh build-dotnet-multirid dotnet-native-artifacts
bash scripts/dotnet_smoke_local_package.sh build-dotnet-multirid/dotnet/packages 2.0.0
```

The release workflow uses the same scripts after downloading native assets from
Linux x64, Linux arm64, and Windows x64 jobs.

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
- `examples/test_gripper_diagnostics.cc` is special-cased to include the
  internal Modbus RTU helper from `sdk/src`, but it links only against
  `lebai-cpp`

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
