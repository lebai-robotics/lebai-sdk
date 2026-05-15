# Architecture

## Overview

`lebai-sdk` is a C++ SDK that talks to a Lebai controller over HTTP JSON-RPC
using `json-rpc-cxx`. The C++ library is the core implementation, and Python,
.NET, and Java bindings are layered on top through SWIG.

## Main Layers

### Public API

- `sdk/include/lebai/robot.hh`
- `sdk/include/lebai/discovery.hh`
- `sdk/include/lebai/gripper.hh`
- `sdk/include/lebai/lua_robot.hh`

These headers define the public SDK surface that downstream users consume.

### Public Method Adapters

- `sdk/src/robot.cc`
- `sdk/src/discovery.cc`
- `sdk/src/gripper.cc`
- `sdk/src/lua_robot.cc`

These files convert public API calls into internal request objects and return simplified SDK-facing results.

### Internal RPC Layer

- `sdk/src/robot_impl.hh`
- `sdk/src/robot_impl.cc`
- `sdk/src/discovery_impl.hh`
- `sdk/src/discovery_impl.cc`
- `sdk/src/gripper_impl.hh`
- `sdk/src/lua_robot_impl.hh`

This layer owns the controller-facing operations. For robot calls, it passes
`nlohmann/json` DTOs to the RPC client and receives typed DTO responses.

### Transport

- `sdk/src/http_jsonrpc_connector.hh`
- `sdk/src/rpc_client.hh`

This is the HTTP transport and JSON-RPC plumbing used by the robot SDK
implementation.

### Request/Response DTOs

- `sdk/src/protos_json/*.hh`

These files define the `nlohmann/json` request/response objects used by the
internal robot RPC layer. Despite the directory name, these are not generated
protobuf sources.

## Typical Robot Call Flow

For a typed robot API such as `movej`:

1. A public method is declared in `sdk/include/lebai/robot.hh`.
2. `sdk/src/robot.cc` builds a `protos_json` DTO from public parameters.
3. `sdk/src/robot_impl.cc` calls a JSON-RPC method name such as `move_joint`.
4. `sdk/src/rpc_client.hh` serializes parameters through `json-rpc-cxx`.
5. `sdk/src/http_jsonrpc_connector.hh` posts the request to `/jsonrpc`.
6. The JSON response is parsed back into a `protos_json` DTO and returned up
   the stack.

There is also a generic escape hatch on the robot API for raw method calls, which is useful when the typed wrapper for a controller RPC does not exist yet.

## Subsystems

### Discovery

- Public surface: `sdk/include/lebai/discovery.hh`
- Implementation: `sdk/src/discovery.cc`, `sdk/src/discovery_impl.cc`

Discovery is separate from the robot command path and is used to locate devices on the local network.

### Gripper

- Public surface: `sdk/include/lebai/gripper.hh`
- Implementation: `sdk/src/gripper.cc`, `sdk/src/gripper_impl.hh`,
  `sdk/src/modbus_rtu_client.hh`, `sdk/src/modbus_rtu_client.cc`

The gripper code path is distinct from the main robot control path and should
be treated as its own subsystem. Direct RS485 gripper support uses a small
internal Modbus RTU client built on Asio.

### Lua Robot Compatibility

- Public surface: `sdk/include/lebai/lua_robot.hh`
- Implementation: `sdk/src/lua_robot.cc`, `sdk/src/lua_robot_impl.cc`

This path mirrors parts of the Lua-facing API design used by the broader Lebai ecosystem.

## Where To Edit

### Add or change a typed robot RPC

Edit all of the following together:

1. `sdk/src/protos_json` for request/response objects
2. `sdk/include/lebai/robot.hh` for the public declaration
3. `sdk/src/robot.cc` for parameter-to-DTO conversion
4. `sdk/src/robot_impl.hh` and `sdk/src/robot_impl.cc` for the JSON-RPC call

Use `docs/develop.md` as the primary repo-specific reference for this workflow.

### Change transport behavior

Start with:

- `sdk/src/http_jsonrpc_connector.hh`
- `sdk/src/rpc_client.hh`

Be careful here because transport changes affect all bindings and all controller operations.

### Change binding exposure

Review:

- `sdk/python/*.i`
- `sdk/dotnet/*.i`
- `sdk/java/*.i`
- `docs/bindings.md`

Public API changes in C++ often require matching wrapper updates.

## Generated And External Sources

- `sdk/include/lebai/config.hh` is generated from `sdk/config.hh.in`
- the authoritative RPC schema lives outside this repository; see
  `docs/rpc-protocol.md`
- `sdk/src/protos_json` is a maintained translation layer for that external
  schema, not the source of truth
