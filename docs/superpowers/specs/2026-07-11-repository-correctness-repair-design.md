# Repository Correctness Repair Design

## Goal

Repair the confirmed protocol, transport, discovery, device, and packaging
defects found during the repository review. Each repair must have a focused
regression test and must preserve existing behavior except for the two public
API breaks explicitly approved for protocol correctness.

## Source Of Truth

The published `lebai-proto` documentation is authoritative for RPC method
names, request fields, response fields, and enum values:

- https://lebai-robotics.github.io/lebai-proto/

Repository documentation under `docs/` remains authoritative for build,
binding, architecture, and test workflow.

## Scope

The work is divided into four independently testable areas:

1. RPC DTO and public API correctness
2. HTTP and Lua transport behavior
3. Discovery and gripper correctness
4. Build, install, and Python packaging behavior

The changes remain surgical. They do not replace the current RPC stack,
generate DTOs from protobuf, or redesign unrelated public APIs.

## Approved Breaking API Changes

### Force Mode Parameters

Replace:

```cpp
void set_force_mode_param(double damping, double gain,
                          const std::vector<double>& max_vel);
```

with:

```cpp
void set_force_mode_param(double damping, double mass,
                          double force_threshold,
                          double torque_threshold);
```

The request DTO and JSON payload must contain exactly:

- `damping`
- `mass`
- `force_threshold`
- `torque_threshold`

No legacy overload or compatibility translation will remain. C++, Python,
.NET, and Java consumers will receive the new signature through the shared
SWIG inputs.

### Box Test Authentication

Replace:

```cpp
int box_test();
```

with:

```cpp
int box_test(const std::string& time, const std::string& auth);
```

The wrapper must construct the protocol `Auth` object and serialize it under
the request's `auth` field. No unauthenticated overload will remain.

Both breaking changes must be recorded in `docs/changelog.md` and reflected in
tests and examples that call the affected methods.

## RPC DTO Corrections

The migrated nlohmann DTOs must use protocol field names rather than legacy
public names:

- `InitClawRequest` serializes `force`.
- `SetDioModeRequest` serializes `mode`, using protocol enum strings `INPUT`
  and `OUTPUT`. The public boolean remains an ergonomic adapter where `false`
  maps to `INPUT` and `true` maps to `OUTPUT`.
- `FanData` serializes `mode`.
- the quality request serializes `auth` with `time` and `auth` members.
- `SetForceModeParamRequest` uses the four approved protocol fields.

Focused JSON tests must assert complete object equality, not only that a field
exists. This prevents unknown legacy fields from remaining beside corrected
fields.

## Motion Input Validation

`Robot::move_pvat` accepts three independent vectors. Before constructing any
request or making a network call, it must verify that position, velocity, and
acceleration vectors have equal lengths. A mismatch throws
`std::invalid_argument` with a message naming the three inputs.

The repair does not add a fixed six-joint requirement because the reviewed
defect is memory safety, and changing accepted joint counts would be a separate
behavioral decision.

## HTTP JSON-RPC Transport

### Concurrency

`HttpJsonRpcConnector` will store endpoint and timeout configuration, not one
shared `httplib::Client`. Every `Send` operation creates and configures its own
client before issuing the POST.

This is intentionally simple and does not lose current connection reuse:
cpp-httplib keep-alive is disabled by default in the pinned version, so the
current shared client already closes its socket after each request.

Independent clients allow a blocking `wait_move`, `wait_task`, `wait_signal`,
or `wait_claw_ai` request and a stop, cancel, or status request to proceed at
the same time through one `Robot` object. Concurrent requests consequently use
concurrent controller connections; simulator coverage must confirm that the
controller accepts this transport pattern.

### Timeout Policy

Connection, write, and normal RPC read timeouts are five seconds, matching the
pre-migration connector behavior. The connector parses its internally
generated JSON request structurally and treats any method whose name begins
with `wait_` as blocking. Blocking calls use a separately configurable 24-hour
read timeout.

cpp-httplib does not expose a portable infinite read timeout. The production
default is therefore a documented large finite duration rather than a
negative or zero sentinel. Internal timeout configuration will be injectable
for deterministic tests but will not become part of the public `Robot` API.

Malformed request JSON uses the normal timeout policy. Transport failures
continue to surface as `jsonrpccxx::JsonRpcException` with code `-32003` and
the underlying cpp-httplib error in the message.

### HTTP Tests

An in-process threaded HTTP server will provide deterministic tests for:

- a wait request blocked on the server while a second request reaches the
  server through the same connector;
- a wait request surviving the injected normal read timeout;
- an ordinary request failing at its injected read timeout;
- non-200 and connection failure error mapping.

Tests must coordinate with condition variables or promises, not timing-only
sleeps.

## Lua Transport

### Connection

`LuaRobotImpl` will complete connection establishment before its constructor
returns. It will run asynchronous resolution and connection together with a
steady timer on its `io_context`; timeout cancels resolution, closes the socket,
and produces a descriptive `std::runtime_error`. Connection refusal and
resolution errors also surface to the constructor.

Connect and read operations will share a small internal deadline runner. The
runner owns the race between operation completion and a steady timer and takes
an explicit cancellation callback. This is a production abstraction, not a
test-only branch: resolution, connection, and framed reads all use the same
completion rules.

### Framed Reads

`call()` will:

1. send the existing `print(...)` command;
2. asynchronously read until the protocol CRLF delimiter;
3. enforce a finite read deadline;
4. return one response without the CRLF delimiter;
5. preserve any bytes read beyond the first delimiter for the next call.

A bounded `asio::streambuf` prevents an unframed peer from growing memory
without limit. Timeout, disconnect, oversized response, and malformed framing
all throw descriptive `std::runtime_error` exceptions.

The default controller port remains 5180. Port, timeout, and maximum response
size may be injected only through the internal implementation constructor so
unit tests can use an ephemeral local server without changing the public API.

### Lua Tests

An in-process TCP server will verify:

- connection failure is reported by construction;
- the internal deadline runner deterministically selects timeout, invokes its
  cancellation callback, and ignores a later operation completion;
- a response split across multiple TCP writes is reassembled;
- two responses received together are returned one at a time;
- a silent server triggers the configured deadline;
- a response exceeding the configured limit is rejected.

The deadline-runner test uses an Asio timer as the pending operation, avoiding
unreliable unroutable addresses or firewall behavior. Connection refusal on an
unused loopback port separately verifies the real socket error path.

## Discovery

### Per-Resolution State

All mutable parser buffers, partial records, and results become local to one
`resolve()` operation. The mdns callback receives this state through
`user_data`. Separate `Discovery` objects and concurrent calls therefore share
no writable state.

If no client socket opens, the operation returns an empty vector. No result can
survive from a prior resolution.

### Record Association

Introduce a small internal discovery accumulator with order-independent input
methods. It associates DNS-SD records as follows:

- PTR maps the Lebai service to a service-instance name.
- TXT metadata is keyed by that service-instance name.
- SRV maps the service instance to its target hostname.
- A and AAAA addresses are keyed by the SRV target hostname.

DNS keys are normalized for case and trailing-dot differences inside the
accumulator. Callbacks pass the raw parsed owner and target strings; callers do
not normalize before ingestion. Display values retain their original content.
Results are assembled only after response collection completes, so callback
order cannot mix fields from separate controllers.

A complete controller requires all of the following:

- a PTR record naming the service instance;
- an SRV record mapping that instance to a target hostname;
- TXT values for `model`, `ds`, `rc`, `id`, and `mac` on that instance;
- at least one A or AAAA record for the SRV target.

The public `hostname` remains the first label of the PTR service-instance name,
matching current behavior. Any instance missing one of the required records or
TXT values is incomplete and is omitted.

IPv4 is preferred when both address families are present. A valid IPv6 address
is used when no IPv4 address exists. Duplicate records produce one
`ControllerInfo` per service instance. Incomplete service instances are not
returned.

### Discovery Tests

The accumulator is separated from socket I/O so tests can inject raw parsed
record data directly. Tests cover:

- two controllers with deliberately interleaved records;
- records arriving in reverse order;
- raw owner and target names that differ only by case or a trailing dot;
- IPv4 preference and IPv6 fallback;
- duplicate and incomplete records;
- isolation between separate accumulator instances.

The existing host-network smoke test remains, but correctness does not depend
on an mDNS-capable test environment.

## Gripper Auto-Calibration

Turning auto-calibration on writes value `3` to register 40090. Turning it off
writes value `2`, matching the register behavior documented in repository
history.

The command values should be named constants to make the distinction explicit.
The narrowest practical regression test must verify the disable transaction's
register and value without requiring physical hardware. If direct `GripperImpl`
testing would require invasive dependency injection, the test may target a
small internal command descriptor used by the implementation.

## Build And Packaging

### Installed Headers

Add `include/lebai/lua_robot.hh` to the `lebai-cpp` public header install set.
An install-tree test must verify every documented public SDK header exists in
the staged prefix.

### Python Build Dependencies

Extend the CMake Python-module lookup helper with an optional minimum version.
`setuptools` must be checked against the same `>=61.0` requirement declared in
`python/pyproject.toml.in`.

When `FETCH_PYTHON_DEPS=ON`, a missing or too-old dependency is installed or
upgraded with the required version constraint and then rechecked. A failed
install or failed recheck is a configure error. When dependency fetching is
off, configuration fails immediately with the found and required versions.

The package target continues to use `python -m build --no-isolation`; the
configure step must guarantee that its declared build dependencies are usable.

Move the reusable module lookup into a standalone CMake helper so its version
and install/recheck behavior can be tested without configuring or building all
bindings. Hermetic CMake tests use the real selected Python interpreter with a
temporary `PYTHONPATH` containing:

- a fake old `setuptools` module;
- a fake `pip` package whose `__main__` records the requested constraint and
  rewrites only that temporary module to the required version.

This fixture verifies the fetch-off failure, constrained upgrade command, and
post-install recheck without network access, user-site changes, or host package
modification. A no-op fake installer separately verifies that a failed recheck
is fatal.

### C++-Disabled Configuration

When `BUILD_CXX=OFF`, `examples/CMakeLists.txt` must not enumerate or call the
C++-only `add_cpp_example` helper. Language-specific example handling remains
controlled by its own build options.

A configure regression test will use the documented default
`BUILD_EXAMPLES=ON` together with `BUILD_CXX=OFF` and assert successful
generation. It explicitly sets `BUILD_PYTHON=OFF`, `BUILD_DOTNET=OFF`,
`BUILD_JAVA=OFF`, `BUILD_DOCUMENTATION=OFF`, `BUILD_DEB=OFF`, and
`BUILD_TESTING=OFF`, so success cannot depend on unrelated language toolchains
or test dependencies.

## Error And Compatibility Policy

- The two approved public signature changes are intentionally breaking.
- Other public signatures remain unchanged.
- Invalid local arguments throw `std::invalid_argument` before network I/O.
- Network and protocol failures retain existing exception families.
- No compatibility overloads or silent field aliases are added.
- SWIG bindings derive the new signatures from the public C++ headers; all
  enabled binding builds must still compile.

## Non-Goals

This repair does not:

- generate DTOs from protobuf definitions;
- change controller RPC method names;
- redesign response-ID handling in the pinned json-rpc-cxx dependency;
- add the deprecated `TaskState::BEGIN` protocol value;
- guarantee concurrent calls on one `LuaRobot` instance;
- replace the mDNS or Modbus libraries;
- fix simulator capabilities or unrelated state leakage in live tests.

## Validation

Implementation is complete only when all applicable checks pass from a clean
build directory:

1. focused DTO, validation, connector, Lua, discovery, Modbus, and CMake tests;
2. the full hermetic CTest suite;
3. C++ SDK and examples build;
4. staged installation contains `lua_robot.hh`;
5. `BUILD_CXX=OFF` configures with default examples enabled;
6. Python package configuration handles an old setuptools version correctly,
   and the package target succeeds after dependency upgrade;
7. enabled Python, .NET, and Java SWIG compilation where toolchains are
   available;
8. focused simulator smoke tests against `127.0.0.1:3030` and Lua port 5180.

Hardware-specific gripper behavior and multi-controller network discovery are
reported as residual validation gaps when the required hardware or network
environment is unavailable.
