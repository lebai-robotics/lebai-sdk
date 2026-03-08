# AGENTS.md

## Purpose

This repository is a CMake-first C++ SDK for controlling Lebai robots, with optional Python, .NET, and Java bindings generated through SWIG.

## Repo Map

- `sdk/include/lebai`: public C++ API surface (`robot.hh`, `discovery.hh`, `gripper.hh`, `lua_robot.hh`, `lebai.hh`)
- `sdk/src`: core implementations, transport, and internal helpers
- `sdk/src/protos`: hand-maintained JSON DTOs used by RPC calls; these are not generated protobuf files
- `sdk/python`, `sdk/dotnet`, `sdk/java`: SWIG interface layers
- `python`, `dotnet`, `java`: packaging templates (`*.in`) and language-specific project metadata
- `sdk/test`: C++ tests
- `examples`: usage examples across C++, Python, C#, and Java
- `doc`: human documentation, especially `doc/develop.md` for adding new RPCs
- `cmake`: build logic for C++, Python, .NET, Java, docs, packaging
- `third`: vendored third-party dependencies; avoid changing this unless the task explicitly requires it

## Read First

- `README.md` for supported platforms and top-level build options
- `doc/develop.md` for the canonical "add a new RPC" workflow
- `doc/architecture.md` for layer boundaries and request flow
- `doc/build-and-test.md` for local build, test, and packaging commands
- `doc/bindings.md` for SWIG and language-package structure

## Common Edit Paths

### Add or change a robot RPC

1. Check the external proto/API definition referenced by `doc/develop.md`.
2. Add or update DTOs in `sdk/src/protos`.
3. Add or update the public API in `sdk/include/lebai/robot.hh`.
4. Implement the public wrapper in `sdk/src/robot.cc`.
5. Add the JSON-RPC call in `sdk/src/robot_impl.hh` and `sdk/src/robot_impl.cc`.
6. If the API is intended for bindings, review SWIG files under `sdk/python`, `sdk/dotnet`, and `sdk/java`.
7. Add or update tests in `sdk/test` and examples in `examples` when appropriate.

### Change discovery or gripper behavior

- discovery: `sdk/include/lebai/discovery.hh`, `sdk/src/discovery.cc`, `sdk/src/discovery_impl.cc`, `sdk/src/discovery_impl.hh`
- gripper: `sdk/include/lebai/gripper.hh`, `sdk/src/gripper.cc`, `sdk/src/gripper_impl.hh`

## Build And Test

### C++ build

```bash
cmake -S . -B build
cmake --build build
```

### C++ tests

Set `TEST_ROBOT_IP` when tests need a reachable robot or simulator:

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DTEST_ROBOT_IP=127.0.0.1
cmake --build build
cmake --build build --target test
```

### Python package

```bash
cmake -S . -B build -DBUILD_PYTHON=ON
cmake --build build --target python_package
```

### .NET package

```bash
cmake -S . -B build -DBUILD_DOTNET=ON
cmake --build build --target dotnet_package
```

### Java package

```bash
cmake -S . -B build -DBUILD_JAVA=ON
cmake --build build --target java_package
```

Key CMake toggles live in `CMakeLists.txt`: `BUILD_CXX`, `BUILD_PYTHON`, `BUILD_DOTNET`, `BUILD_JAVA`, `BUILD_TESTING`, `BUILD_EXAMPLES`, `BUILD_DEB`, `CLANG_FORMAT_CHECK`, `BUILD_DOCUMENTATION`, `ENABLE_TSAN`.

## Generated And Sensitive Areas

- `sdk/include/lebai/config.hh` is generated from `sdk/config.hh.in` by `cmake/cpp.cmake`
- the repo does not contain authoritative `.proto` files; `sdk/src/protos` is a hand-written JSON serialization layer matching an external schema
- package metadata and build outputs are produced from templates under `python`, `dotnet`, and `java`
- prefer editing source and template files, not build artifacts under `build`

## Practical Guidance For Agents

- start with the docs listed in `Read First`
- keep public API changes aligned with implementation, DTOs, bindings, tests, and examples
- prefer focused edits in `sdk/include/lebai`, `sdk/src`, `sdk/src/protos`, and `sdk/test`
- avoid broad formatting-only changes across the vendored `third` directory
- if a task changes public behavior, review whether `doc/changelog.md` should be updated
