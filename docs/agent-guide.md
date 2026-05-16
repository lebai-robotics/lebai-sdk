# Agent Guide

Project-specific guidance for coding agents working in this repository.

## Repo Map

- `sdk/include/lebai`: public C++ API surface (`robot.hh`, `discovery.hh`,
  `gripper.hh`, `lua_robot.hh`, `lebai.hh`)
- `sdk/src`: core implementations, transport, and internal helpers
- `sdk/src/protos_json`: nlohmann/json DTOs used by robot RPC calls
- `sdk/python`, `sdk/dotnet`, `sdk/java`: SWIG interface layers
- `python`, `dotnet`, `java`: packaging templates and language metadata
- `sdk/test`: C++ tests
- `examples`: usage examples across C++, Python, C#, and Java
- `docs`: human documentation
- `cmake`: build logic for C++, Python, .NET, Java, docs, packaging
- `third`: vendored third-party dependencies; avoid changing this unless the
  task explicitly requires it

## Common Edit Paths

### Add or change a robot RPC

1. Check `docs/rpc-protocol.md` and the external proto/API definition.
2. Add or update DTOs in `sdk/src/protos_json`.
3. Add or update the public API in `sdk/include/lebai/robot.hh`.
4. Implement the public wrapper in `sdk/src/robot.cc`.
5. Add the JSON-RPC call in `sdk/src/robot_impl.hh` and `sdk/src/robot_impl.cc`.
6. If the API is intended for bindings, review SWIG files under `sdk/python`,
   `sdk/dotnet`, and `sdk/java`.
7. Add or update focused tests in `sdk/test` and examples when appropriate.

### Change discovery or gripper behavior

- discovery: `sdk/include/lebai/discovery.hh`, `sdk/src/discovery.cc`,
  `sdk/src/discovery_impl.cc`, `sdk/src/discovery_impl.hh`
- gripper: `sdk/include/lebai/gripper.hh`, `sdk/src/gripper.cc`,
  `sdk/src/gripper_impl.hh`, `sdk/src/modbus_rtu_client.hh`,
  `sdk/src/modbus_rtu_client.cc`

## Generated And Sensitive Areas

- `sdk/include/lebai/config.hh` is generated from `sdk/config.hh.in` by
  `cmake/cpp.cmake`.
- Build outputs belong under ignored build directories, not source control.
- Package metadata and build outputs are produced from templates under
  `python`, `dotnet`, and `java`.
- Avoid broad formatting-only changes across `third`.

## Practical Workflow

- Start with the docs listed in `AGENTS.md`.
- Keep public API changes aligned with implementation, DTOs, bindings, tests,
  and examples.
- Prefer focused edits in `sdk/include/lebai`, `sdk/src`, `sdk/src/protos_json`,
  and `sdk/test`.
- If a task changes public behavior, review whether `docs/changelog.md` should
  be updated.
