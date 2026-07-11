# Repository Correctness Repairs Implementation Plan

> **For agentic workers:** REQUIRED: Use superpowers:subagent-driven-development (if subagents available) or superpowers:executing-plans to implement this plan. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Correct the reviewed RPC schemas, public APIs, transports, discovery state, gripper command, and build/package behavior with deterministic regression coverage.

**Architecture:** Keep the current public-wrapper, DTO, `RobotImpl`, and transport layers. Add only three focused internal units: a timed Asio operation runner, an order-independent DNS-SD accumulator, and a standalone Python-module CMake helper. Every behavioral repair starts with a failing focused test and lands as an independently testable commit.

**Tech Stack:** C++17, CMake/CTest, GoogleTest, nlohmann/json, json-rpc-cxx, cpp-httplib 0.20-compatible APIs, standalone Asio 1.24, SWIG, Python 3.

**Design:** `docs/superpowers/specs/2026-07-11-repository-correctness-repair-design.md`

---

## File Structure

**Create:**

- `sdk/src/timed_operation.hh`: reusable Asio operation/deadline race.
- `sdk/src/timed_operation.cc`: deadline runner implementation.
- `sdk/src/discovery_accumulator.hh`: internal DNS-SD record association API.
- `sdk/src/discovery_accumulator.cc`: normalization, completeness, and result assembly.
- `sdk/src/gripper_protocol.hh`: named register and auto-calibration command values.
- `cmake/python_modules.cmake`: reusable Python module/version/install checks.
- `sdk/test/test_robot_api.cc`: compile-time public signatures and local argument validation.
- `sdk/test/test_http_jsonrpc_connector.cc`: deterministic local HTTP transport tests.
- `sdk/test/test_timed_operation.cc`: deterministic deadline/cancellation tests.
- `sdk/test/test_lua_robot_client.cc`: local TCP framing and timeout tests.
- `sdk/test/test_discovery_accumulator.cc`: network-independent discovery tests.
- `sdk/test/test_install_headers.cmake`: staged public-header install assertions.
- `sdk/test/test_python_module_versions.cmake`: offline old-module/fake-pip fixture.
- `sdk/test/test_build_without_cxx.cmake`: isolated `BUILD_CXX=OFF` configure test.

**Modify:**

- `sdk/include/lebai/robot.hh`: approved breaking force-mode and box-test signatures.
- `sdk/src/robot.cc`: corrected adapters and `move_pvat` validation.
- `sdk/src/protos_json/{motion,claw,io,led,quality}_proto.hh`: protocol field names and enum mapping.
- `sdk/src/http_jsonrpc_connector.hh`: request-scoped clients and method-aware timeouts.
- `sdk/src/lua_robot_impl.{hh,cc}`: completed connect and CRLF-framed timed reads.
- `sdk/src/discovery_impl.{hh,cc}`: per-resolution callback state and accumulator integration.
- `sdk/src/gripper_impl.cc`: disable value `2` from named protocol constants.
- `sdk/CMakeLists.txt`: new internal sources and installed Lua header.
- `sdk/test/CMakeLists.txt`: focused C++ and CMake test registration.
- `sdk/test/test_json_protos.cc`: exact request JSON assertions.
- `sdk/test/test_robot.cc`: protocol-correct force-mode smoke call and same-object signal wait.
- `sdk/test/test_modbus_rtu_client.cc`: gripper disable frame regression.
- `cmake/python.cmake`: include module helper and require `setuptools>=61.0`.
- `examples/CMakeLists.txt`: guard C++ examples behind `BUILD_CXX`.
- `docs/changelog.md`: breaking APIs and correctness fixes.

## Chunk 1: RPC Schemas And Public API

### Task 1: Correct request DTO wire shapes

**Files:**

- Modify: `sdk/test/test_json_protos.cc`
- Modify: `sdk/src/protos_json/motion_proto.hh`
- Modify: `sdk/src/protos_json/claw_proto.hh`
- Modify: `sdk/src/protos_json/io_proto.hh`
- Modify: `sdk/src/protos_json/led_proto.hh`
- Modify: `sdk/src/protos_json/quality_proto.hh`

- [ ] **Step 1: Add exact-object serialization tests**

Add tests that construct every affected request and compare the whole JSON
object. Use representative non-default values so a dropped field is visible:

```cpp
EXPECT_EQ(nlohmann::json(force_mode),
          (nlohmann::json{{"damping", 0.1},
                          {"mass", 2.0},
                          {"force_threshold", 3.0},
                          {"torque_threshold", 4.0}}));
EXPECT_EQ(nlohmann::json(init_claw), nlohmann::json{{"force", true}});
EXPECT_EQ(nlohmann::json(dio_mode),
          (nlohmann::json{{"device", "EXTRA"},
                          {"pin", 4},
                          {"mode", "OUTPUT"}}));
EXPECT_EQ(nlohmann::json(fan), nlohmann::json{{"mode", 2}});
EXPECT_EQ(nlohmann::json(box_test),
          (nlohmann::json{{"auth", {{"time", "123"},
                                     {"auth", "signature"}}}}));
```

- [ ] **Step 2: Run the focused test and verify RED**

Run:

```bash
cmake --build build --target test_json_protos -j2
./build/bin/test_json_protos --gtest_filter='JsonMotionProtoTest.*:JsonClawProtoTest.*:JsonIoProtoTest.*:JsonLedProtoTest.*:JsonQualityProtoTest.*'
```

Expected: compilation fails because the corrected members/enums do not yet
exist, or the assertions fail against legacy keys.

- [ ] **Step 3: Implement protocol-aligned DTOs**

Implement these exact shapes:

```cpp
struct SetForceModeParamRequest {
  double damping{};
  double mass{};
  double force_threshold{};
  double torque_threshold{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetForceModeParamRequest, damping, mass,
                                 force_threshold, torque_threshold)
};

struct InitClawRequest {
  bool force{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitClawRequest, force)
};

enum class DigitalMode { INPUT = 0, OUTPUT = 1 };
```

Add explicit `to_json`/`from_json` for `DigitalMode` using `INPUT` and
`OUTPUT`, make `SetDioModeRequest::mode` a `DigitalMode`, rename
`FanData::fan` to `mode`, and define `quality_proto::EmptyRequest` with an
`Auth auth` member after moving `Auth` above it.

- [ ] **Step 4: Run the DTO tests and verify GREEN**

Run:

```bash
cmake --build build --target test_json_protos -j2
./build/bin/test_json_protos --gtest_filter='JsonMotionProtoTest.*:JsonClawProtoTest.*:JsonIoProtoTest.*:JsonLedProtoTest.*:JsonQualityProtoTest.*'
```

Expected: selected tests pass with no legacy keys in any expected object.

- [ ] **Step 5: Commit the DTO correction**

```bash
git add sdk/src/protos_json sdk/test/test_json_protos.cc
git commit -m "fix: align request DTOs with lebai protocol"
```

### Task 2: Replace the approved public API signatures

**Files:**

- Create: `sdk/test/test_robot_api.cc`
- Modify: `sdk/test/CMakeLists.txt`
- Modify: `sdk/include/lebai/robot.hh`
- Modify: `sdk/src/robot.cc`
- Modify: `sdk/test/test_robot.cc`
- Modify: `docs/changelog.md`

- [ ] **Step 1: Add compile-time signature tests**

Register `test_robot_api` and assert the unambiguous member pointer types:

```cpp
using ForceModeSignature =
    void (lebai::l_master::Robot::*)(double, double, double, double);
using BoxTestSignature =
    int (lebai::l_master::Robot::*)(const std::string&, const std::string&);

static_assert(std::is_same_v<decltype(&lebai::l_master::Robot::set_force_mode_param),
                             ForceModeSignature>);
static_assert(std::is_same_v<decltype(&lebai::l_master::Robot::box_test),
                             BoxTestSignature>);
```

- [ ] **Step 2: Build the test and verify RED**

Run:

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON -DBUILD_DOCUMENTATION=OFF
cmake --build build --target test_robot_api -j2
```

Expected: compilation fails because both old signatures are still present.

- [ ] **Step 3: Replace declarations and adapters**

Change `Robot::set_force_mode_param` to four doubles and assign all four
correct DTO members. Change `Robot::box_test` to accept `time` and `auth`, then
fill `req.auth.time` and `req.auth.auth`. Rename the public `init_claw`
parameter to `force` and map it to `req.force`. Keep `set_dio_mode`'s boolean
surface and map it to `DigitalMode::OUTPUT`/`INPUT`; map `set_fan` to
`FanData::mode`.

Update the force-mode simulator smoke call to four scalar values. Add a 2.0.0
changelog paragraph explicitly listing both breaking signatures and the wire
field corrections. The SWIG files include `robot.hh` directly, so do not add
duplicated signature declarations there.

- [ ] **Step 4: Build API and live-test targets and verify GREEN**

Run:

```bash
cmake --build build --target test_robot_api test_robot -j2
./build/bin/test_robot_api
```

Expected: `test_robot_api` passes and the live test target compiles with the new
signature.

- [ ] **Step 5: Commit the breaking API update**

```bash
git add sdk/include/lebai/robot.hh sdk/src/robot.cc sdk/test/CMakeLists.txt \
  sdk/test/test_robot_api.cc sdk/test/test_robot.cc docs/changelog.md
git commit -m "fix: replace invalid force and quality APIs"
```

### Task 3: Reject mismatched PVAT vectors before indexing

**Files:**

- Modify: `sdk/test/test_robot_api.cc`
- Modify: `sdk/src/robot.cc`

- [ ] **Step 1: Add mismatched-vector regression cases**

Construct `Robot("127.0.0.1", true)` and assert `std::invalid_argument` when
velocity or acceleration is shorter or longer than position. Do not run a
server; validation must happen before `impl_->move_pvat`. Assert the exception
message names `p`, `v`, and `a`.

- [ ] **Step 2: Run and verify RED**

```bash
cmake --build build --target test_robot_api -j2
./build/bin/test_robot_api --gtest_filter='RobotApiTest.MovePvatRejectsMismatchedVectors'
```

Expected: the process fails or throws a non-validation error because indexing
occurs without equal-length checks.

- [ ] **Step 3: Add one equal-length guard**

At the top of `move_pvat`, require `p.size() == v.size()` and
`p.size() == a.size()`. Throw `std::invalid_argument` naming `p`, `v`, and `a`.
Do not add a six-joint restriction.

- [ ] **Step 4: Run and verify GREEN**

```bash
cmake --build build --target test_robot_api -j2
./build/bin/test_robot_api
```

Expected: all API and validation tests pass without network access.

- [ ] **Step 5: Commit the memory-safety fix**

```bash
git add sdk/src/robot.cc sdk/test/test_robot_api.cc
git commit -m "fix: validate move pvat vector lengths"
```

## Chunk 2: HTTP And Lua Transports

### Task 4: Make HTTP JSON-RPC requests concurrent and method-aware

**Files:**

- Create: `sdk/test/test_http_jsonrpc_connector.cc`
- Modify: `sdk/test/CMakeLists.txt`
- Modify: `sdk/src/http_jsonrpc_connector.hh`
- Modify: `sdk/test/test_robot.cc`

- [ ] **Step 1: Add an in-process HTTP connector test target**

Use `httplib::Server::bind_to_any_port` and `listen_after_bind`. Cover:

- `wait_task` blocks on a condition variable while `cancel_task` reaches a
  second server worker through the same connector;
- an injected 50 ms ordinary timeout fails a delayed ordinary request;
- an injected 500 ms wait timeout allows the same delay for any `wait_` method;
- malformed JSON uses the ordinary timeout policy;
- HTTP 503 and an unused port retain `JsonRpcException(-32003, ...)` mapping,
  including the underlying cpp-httplib error text.

Use promises/condition variables to prove request arrival; use durations only
for timeout assertions and deadlock containment. The concurrency case must
install a bounded watchdog that always releases the blocked wait handler,
regardless of whether the second request arrives. Assert the second-request
promise becomes ready before the watchdog, then release and collect both
futures before stopping and joining the server. The promise is the correctness
proof; the watchdog only guarantees cleanup under the legacy implementation.

- [ ] **Step 2: Run and verify RED**

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --target test_http_jsonrpc_connector -j2
./build/bin/test_http_jsonrpc_connector
```

Expected: concurrency test times out or the target cannot compile because
timeout injection does not exist.

- [ ] **Step 3: Implement request-scoped clients**

Add internal `HttpJsonRpcTimeouts` storing microsecond durations with defaults:
five seconds for connect/read/write and 24 hours for wait reads. Store `host_`,
`port_`, and the timeout struct in the connector. In `Send`:

1. parse the request with nlohmann/json;
2. select wait timeout when the string method starts with `wait_`;
3. construct a local `httplib::Client`;
4. split each microsecond duration into `(seconds, microseconds)` for the
   long-established cpp-httplib setters;
5. issue `Post` and preserve current error construction.

Remove the connector mutex and shared client. Treat malformed request JSON as
an ordinary request for timeout purposes.

- [ ] **Step 4: Update the simulator concurrency smoke**

Change `TestSignal` so its async lambda captures and calls the fixture's
`robot_` instead of constructing a second `Robot`. Keep the promise bounded by
the existing five-second test deadline.

- [ ] **Step 5: Run and verify GREEN**

```bash
cmake --build build --target test_http_jsonrpc_connector test_robot -j2
./build/bin/test_http_jsonrpc_connector
```

Expected: all connector tests pass and the same-object simulator test compiles.

- [ ] **Step 6: Commit the HTTP transport repair**

```bash
git add sdk/src/http_jsonrpc_connector.hh sdk/test/CMakeLists.txt \
  sdk/test/test_http_jsonrpc_connector.cc sdk/test/test_robot.cc
git commit -m "fix: allow concurrent blocking robot RPCs"
```

### Task 5: Add a reusable timed Asio operation runner

**Files:**

- Create: `sdk/src/timed_operation.hh`
- Create: `sdk/src/timed_operation.cc`
- Create: `sdk/test/test_timed_operation.cc`
- Modify: `sdk/CMakeLists.txt`
- Modify: `sdk/test/CMakeLists.txt`

- [ ] **Step 1: Add deterministic timeout-race tests**

Define the runner API around a completion callback and cancellation callback:

```cpp
struct TimedOperationResult {
  std::error_code error;
  bool timed_out{};
};

TimedOperationResult run_timed_operation(
    asio::io_context& io, std::chrono::milliseconds timeout,
    const std::function<void(std::function<void(std::error_code)>)>& start,
    const std::function<void()>& cancel);
```

Test operation-first success, timer-first cancellation, operation error, and a
late completion ignored after timeout. Drive the pending operation with an
`asio::steady_timer`, not a network address.

- [ ] **Step 2: Build and verify RED**

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --target test_timed_operation -j2
```

Expected: target fails because the runner is absent.

- [ ] **Step 3: Implement the operation/deadline race**

Use one local steady timer, `io.restart()`, a single completion flag, and a
timer callback that invokes `cancel` once. Operation completion cancels the
deadline. Continue `io.run()` until cancellation handlers drain, but preserve
the first terminal result. The runner contract requires `cancel` to cause the
pending operation's completion handler to run; assert this in the timer-first
test so `io.run()` cannot retain an abandoned operation. Do not throw inside
the runner.

- [ ] **Step 4: Run and verify GREEN**

```bash
cmake --build build --target test_timed_operation -j2
./build/bin/test_timed_operation
```

Expected: all race and cancellation cases pass without sleeps.

- [ ] **Step 5: Commit the timing primitive**

```bash
git add sdk/src/timed_operation.hh sdk/src/timed_operation.cc \
  sdk/test/test_timed_operation.cc sdk/CMakeLists.txt sdk/test/CMakeLists.txt
git commit -m "test: add deterministic asio deadline runner"
```

### Task 6: Complete and frame the Lua transport

**Files:**

- Create: `sdk/test/test_lua_robot_client.cc`
- Modify: `sdk/test/CMakeLists.txt`
- Modify: `sdk/src/lua_robot_impl.hh`
- Modify: `sdk/src/lua_robot_impl.cc`

- [ ] **Step 1: Add a local TCP fake and failing tests**

Expose an internal-only `LuaRobotImpl::Config` with port, millisecond timeout,
and maximum response bytes. Test through `LuaRobotImpl` on an ephemeral port:

- unused loopback port throws during construction;
- `"abc"` and `"\r\n"` in separate writes returns `"abc"`;
- `"one\r\ntwo\r\n"` in one write is returned across two calls;
- accepted connection with no response times out;
- EOF before CRLF throws;
- data beyond the configured maximum throws.

Coordinate server state with promises and join every server thread. For the
silent-server case, a bounded watchdog closes the accepted peer even when the
legacy client never times out; assert that the repaired client reports its
configured timeout before that escape fires. Apply the same unconditional
peer-close cleanup to unframed oversized input. Set a CTest `TIMEOUT` property
on the executable as a final process-level guard; no test may leave a blocked
thread for fixture teardown to join.

- [ ] **Step 2: Run and verify RED**

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --target test_lua_robot_client -j2
./build/bin/test_lua_robot_client
```

Expected: connection/framing/timeout cases fail under the current implementation.

- [ ] **Step 3: Implement timed construction and framed reads**

Replace the unused flags and `io_service` pointer with owned `asio::io_context`,
resolver, socket, config, and bounded `asio::streambuf`. Resolve and connect via
`run_timed_operation`; cancel resolver and close socket on timeout. Wrap errors
as descriptive `std::runtime_error`.

For `call`, use `async_read_until(socket_, response_buffer_, "\r\n")` through
the runner. Capture `bytes_transferred`, copy exactly that many bytes, consume
exactly that many, verify the CRLF suffix, remove it, and leave any extra bytes
in the stream buffer. Keep the public port at 5180 and the public constructor
unchanged.

- [ ] **Step 4: Run focused and existing Lua tests**

```bash
cmake --build build --target test_lua_robot_client test_lua_robot -j2
./build/bin/test_lua_robot_client
```

Expected: all deterministic client tests pass; the controller-dependent Lua
target compiles.

- [ ] **Step 5: Commit the Lua transport repair**

```bash
git add sdk/src/lua_robot_impl.hh sdk/src/lua_robot_impl.cc \
  sdk/test/CMakeLists.txt sdk/test/test_lua_robot_client.cc
git commit -m "fix: frame and time out lua robot transport"
```

## Chunk 3: Discovery And Gripper

### Task 7: Build an order-independent discovery accumulator

**Files:**

- Create: `sdk/src/discovery_accumulator.hh`
- Create: `sdk/src/discovery_accumulator.cc`
- Create: `sdk/test/test_discovery_accumulator.cc`
- Modify: `sdk/CMakeLists.txt`
- Modify: `sdk/test/CMakeLists.txt`

- [ ] **Step 1: Add raw-record accumulator tests**

Define methods `add_ptr(service, instance)`, `add_srv(instance, target)`,
`add_txt(instance, key, value)`, `add_ipv4(target, address)`, `add_ipv6(...)`,
and `controllers()`. Tests must cover:

- two complete controllers with all records interleaved;
- reverse record order;
- case and trailing-dot differences across raw names;
- a PTR owner outside `_lebai._tcp.local` being ignored;
- IPv4 preferred over IPv6 and IPv6-only fallback;
- duplicate records yielding one result;
- omission for each missing mandatory record/TXT key;
- separate instances sharing no state.

- [ ] **Step 2: Build and verify RED**

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --target test_discovery_accumulator -j2
```

Expected: target fails because the accumulator is absent.

- [ ] **Step 3: Implement normalized association**

Use `std::map` for deterministic output. Normalize keys by lowercasing ASCII and
removing trailing dots. `add_ptr` accepts a raw owner but records an instance
only when the normalized owner is `_lebai._tcp.local`. Keep display instance
text from PTR and derive public hostname from its first label. Require PTR
membership, SRV target, TXT
`model/ds/rc/id/mac`, and at least one target address. Prefer IPv4, otherwise
IPv6. Assemble only in `controllers()`, never while ingesting a record.

- [ ] **Step 4: Run and verify GREEN**

```bash
cmake --build build --target test_discovery_accumulator -j2
./build/bin/test_discovery_accumulator
```

Expected: all order, normalization, completeness, and family tests pass.

- [ ] **Step 5: Commit the accumulator**

```bash
git add sdk/src/discovery_accumulator.hh sdk/src/discovery_accumulator.cc \
  sdk/test/test_discovery_accumulator.cc sdk/CMakeLists.txt sdk/test/CMakeLists.txt
git commit -m "test: add deterministic discovery accumulator"
```

### Task 8: Route mDNS callbacks through per-resolution state

**Files:**

- Modify: `sdk/src/discovery_impl.hh`
- Modify: `sdk/src/discovery_impl.cc`
- Modify: `sdk/test/test_discovery.cc`

- [ ] **Step 1: Add a no-stale-state public regression**

Keep the host-network smoke non-fatal, but add a test that creates two
`Discovery` instances and calls `resolve` concurrently, asserting only that
both calls complete without exception. Accumulator tests remain the source of
record correctness.

- [ ] **Step 2: Run the current test before implementation**

```bash
cmake --build build --target test_discovery -j2
./build/bin/test_discovery
```

Expected: the smoke may pass, demonstrating why internal state tests are
required; record this as a characterization run rather than RED.

- [ ] **Step 3: Remove all mutable file globals**

Make `send_mdns_query` return `std::vector<ControllerInfo>`. Create one local
accumulator in it and pass its address as `user_data`. Make parser buffers and
TXT arrays callback-local. Feed raw PTR owner/target, TXT owner/key/value, SRV
owner/target, and A/AAAA owner/address strings to the accumulator. Use a local
byte vector for packet storage. Return `{}` immediately when sockets cannot
open and return `accumulator.controllers()` after collection.

Do not retain partial state in `DiscoveryImpl`; `resolve()` returns the result
of this one query directly.

- [ ] **Step 4: Run accumulator and discovery tests**

```bash
cmake --build build --target test_discovery_accumulator test_discovery -j2
./build/bin/test_discovery_accumulator
./build/bin/test_discovery
```

Expected: deterministic tests pass and the network smoke completes.

- [ ] **Step 5: Commit discovery integration**

```bash
git add sdk/src/discovery_impl.hh sdk/src/discovery_impl.cc sdk/test/test_discovery.cc
git commit -m "fix: isolate mdns discovery response state"
```

### Task 9: Correct gripper auto-calibration disable value

**Files:**

- Create: `sdk/src/gripper_protocol.hh`
- Modify: `sdk/src/gripper_impl.cc`
- Modify: `sdk/test/test_modbus_rtu_client.cc`

- [ ] **Step 1: Add a command-frame regression**

Use the new protocol constants with
`ModbusRtuClient::build_write_single_register_request`. Assert register 40090,
value `2`, function code `0x06`, and the complete disable frame
`{0x01, 0x06, 0x9C, 0x9A, 0x00, 0x02, 0x06, 0x74}`. Also assert enable value
is `3` so the commands cannot collapse again.

- [ ] **Step 2: Build and verify RED**

```bash
cmake --build build --target test_modbus_rtu_client -j2
```

Expected: compilation fails because named values are absent.

- [ ] **Step 3: Add constants and use them in both methods**

Define `kAutoCalibrationRegister = 40090`,
`kAutoCalibrationEnableValue = 3`, and
`kAutoCalibrationDisableValue = 2` in `gripper_protocol.hh`. Remove the local
register constant and make on/off use the named values while retaining their
existing Modbus function codes.

- [ ] **Step 4: Run and verify GREEN**

```bash
cmake --build build --target test_modbus_rtu_client -j2
./build/bin/test_modbus_rtu_client
```

Expected: all Modbus tests pass, including the value-2 disable frame.

- [ ] **Step 5: Commit the gripper fix**

```bash
git add sdk/src/gripper_protocol.hh sdk/src/gripper_impl.cc \
  sdk/test/test_modbus_rtu_client.cc
git commit -m "fix: disable gripper auto calibration correctly"
```

## Chunk 4: Build, Install, Bindings, And Final Verification

### Task 10: Install every documented public header

**Files:**

- Create: `sdk/test/test_install_headers.cmake`
- Modify: `sdk/CMakeLists.txt`
- Modify: `sdk/test/CMakeLists.txt`

- [ ] **Step 1: Add an install-tree CTest**

Pass the active build directory, a temporary prefix, and `$<CONFIG>` to a CMake
script. The script removes only its temporary prefix, runs `cmake --install`,
and asserts these files exist under `include/lebai`:
`config.hh`, `discovery.hh`, `gripper.hh`, `lebai.hh`, `lua_robot.hh`, and
`robot.hh`.

- [ ] **Step 2: Run and verify RED**

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DBUILD_DOCUMENTATION=OFF
cmake --build build --target lebai-cpp -j2
ctest --test-dir build -R InstallHeaders --output-on-failure
```

Expected: failure reports missing `include/lebai/lua_robot.hh`.

- [ ] **Step 3: Add `lua_robot.hh` to `PUBLIC_HEADER`**

Keep the existing install destination and append only the omitted header.

- [ ] **Step 4: Run and verify GREEN**

```bash
cmake --build build --target lebai-cpp -j2
ctest --test-dir build -R InstallHeaders --output-on-failure
```

Expected: the staged prefix contains all six headers.

- [ ] **Step 5: Commit the install repair**

```bash
git add sdk/CMakeLists.txt sdk/test/CMakeLists.txt sdk/test/test_install_headers.cmake
git commit -m "fix: install lua robot public header"
```

### Task 11: Validate and upgrade Python module versions hermetically

**Files:**

- Create: `cmake/python_modules.cmake`
- Create: `sdk/test/test_python_module_versions.cmake`
- Modify: `cmake/python.cmake`
- Modify: `sdk/test/CMakeLists.txt`

- [ ] **Step 1: Add an offline CMake fixture test**

The parent CMake script creates a temporary `setuptools.py` at version 59.6 and
a fake `pip` package containing both `pip/__init__.py` and `pip/__main__.py`.
Run child `cmake -P` drivers through
`cmake -E env PYTHONNOUSERSITE=1 PYTHONPATH=<fixture>` and the real
`Python3_EXECUTABLE`, ensuring `python -m pip` resolves only the fixture package.

Assert:

- fetch OFF fails and reports found `59.6`, required `61.0`;
- fetch ON invokes fake pip with `install --user --upgrade setuptools>=61.0`,
  rewrites only the fixture module, rechecks, and succeeds;
- a fake pip nonzero exit is reported as an install failure;
- a no-op fake pip makes the post-install recheck fail.

- [ ] **Step 2: Run and verify RED**

```bash
cmake -S . -B build -DBUILD_TESTING=ON
ctest --test-dir build -R PythonModuleVersions --output-on-failure
```

Expected: failure because minimum-version and recheck support are absent.

- [ ] **Step 3: Extract and harden `search_python_module`**

Move the helper to `cmake/python_modules.cmake`. Add `MIN_VERSION` parsing.
Probe import/version, treat `VERSION_LESS MIN_VERSION` as unavailable, build a
constrained package string, run pip with `--upgrade` when fetching, check pip's
result, probe again, and fail if the module is still absent or old. Preserve
`NO_VERSION` behavior for callers that only require importability.

Include the helper from `cmake/python.cmake` after `find_package(Python3 ...)`
and call:

```cmake
search_python_module(
  NAME setuptools
  PACKAGE setuptools
  MIN_VERSION 61.0)
```

- [ ] **Step 4: Run and verify GREEN**

```bash
cmake -S . -B build -DBUILD_TESTING=ON
ctest --test-dir build -R PythonModuleVersions --output-on-failure
```

Expected: all four offline scenarios pass without changing user site-packages.

- [ ] **Step 5: Commit the dependency check**

```bash
git add cmake/python.cmake cmake/python_modules.cmake sdk/test/CMakeLists.txt \
  sdk/test/test_python_module_versions.cmake
git commit -m "fix: enforce python build dependency versions"
```

### Task 12: Make `BUILD_CXX=OFF` configure with default examples

**Files:**

- Create: `sdk/test/test_build_without_cxx.cmake`
- Modify: `sdk/test/CMakeLists.txt`
- Modify: `examples/CMakeLists.txt`

- [ ] **Step 1: Add an isolated nested-configure test**

Configure a temporary build with `BUILD_CXX=OFF`, `BUILD_EXAMPLES=ON`, and
`BUILD_PYTHON/DOTNET/JAVA/DOCUMENTATION/DEB/TESTING=OFF`. Pass populated
FetchContent source directories from the outer build when available. Assert
configure exits zero and does not report unknown `add_cpp_example`.

- [ ] **Step 2: Run and verify RED**

```bash
cmake -S . -B build -DBUILD_TESTING=ON
ctest --test-dir build -R BuildWithoutCxx --output-on-failure
```

Expected: nested configure fails at `examples/CMakeLists.txt` with unknown
`add_cpp_example`.

- [ ] **Step 3: Guard only the C++ example loop**

Wrap the C++ glob and `add_cpp_example` loop in `if(BUILD_CXX)`. Leave .NET,
Java, and Python example branches controlled by their existing options.

- [ ] **Step 4: Run and verify GREEN**

```bash
ctest --test-dir build -R BuildWithoutCxx --output-on-failure
```

Expected: nested configure succeeds with examples left ON.

- [ ] **Step 5: Commit the option repair**

```bash
git add examples/CMakeLists.txt sdk/test/CMakeLists.txt \
  sdk/test/test_build_without_cxx.cmake
git commit -m "fix: support configuring without cxx targets"
```

### Task 13: Run bindings and full integration verification

**Files:**

- Modify only if verification exposes a spec-related defect.

- [ ] **Step 1: Run formatting and source checks**

```bash
git diff --check 3af3eae..HEAD
git diff --check
cmake --build build --target clang-format-check -j2
```

If `clang-format-check` is not generated unless configured, reconfigure with
`-DCLANG_FORMAT_CHECK=ON` and run the generated target. Fix only touched files.

- [ ] **Step 2: Run a fresh C++ build and hermetic suite**

```bash
cmake -E remove_directory build-final
cmake -S . -B build-final -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON \
  -DBUILD_DOCUMENTATION=OFF
cmake --build build-final -j2
ctest --test-dir build-final --output-on-failure \
  -E '^(TestRobot|TestLuaRobot|TestDiscovery)$'
```

Expected: configure/build succeeds and every hermetic test passes.

- [ ] **Step 3: Verify install and C++-disabled configuration explicitly**

```bash
cmake -E remove_directory /tmp/lebai-sdk-correctness-install
cmake --install build-final --prefix /tmp/lebai-sdk-correctness-install
test -f /tmp/lebai-sdk-correctness-install/include/lebai/lua_robot.hh
cmake -E remove_directory build-no-cxx
cmake -S . -B build-no-cxx -DBUILD_CXX=OFF -DBUILD_EXAMPLES=ON \
  -DBUILD_PYTHON=OFF -DBUILD_DOTNET=OFF -DBUILD_JAVA=OFF \
  -DBUILD_DOCUMENTATION=OFF -DBUILD_DEB=OFF -DBUILD_TESTING=OFF
```

Expected: install includes Lua header and no-C++ configure succeeds.

- [ ] **Step 4: Build binding targets where toolchains are available**

Use separate, freshly removed build directories. Build Python with an isolated
venv and explicit interpreter selection:

```bash
cmake -E remove_directory /tmp/lebai-sdk-python-venv
cmake -E remove_directory build-python-final
python3 -m venv /tmp/lebai-sdk-python-venv
/tmp/lebai-sdk-python-venv/bin/python -m pip install --upgrade \
  'setuptools>=61.0' wheel build
cmake -S . -B build-python-final -DBUILD_PYTHON=ON \
  -DPython3_EXECUTABLE=/tmp/lebai-sdk-python-venv/bin/python \
  -DFETCH_PYTHON_DEPS=OFF -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF \
  -DBUILD_DOCUMENTATION=OFF
cmake --build build-python-final --target python_package -j2
```

When SWIG and `dotnet` are available, run:

```bash
if command -v swig >/dev/null && command -v dotnet >/dev/null; then
  cmake -E remove_directory build-dotnet-final
  cmake -S . -B build-dotnet-final -DBUILD_DOTNET=ON \
    -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_DOCUMENTATION=OFF
  cmake --build build-dotnet-final --target dotnet_package -j2
else
  echo 'SKIP: SWIG or dotnet unavailable'
fi
```

When SWIG, Java, and Maven are available, run:

```bash
if command -v swig >/dev/null && command -v java >/dev/null && \
   command -v mvn >/dev/null; then
  cmake -E remove_directory build-java-final
  cmake -S . -B build-java-final -DBUILD_JAVA=ON \
    -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_DOCUMENTATION=OFF
  cmake --build build-java-final --target java_package -j2
else
  echo 'SKIP: SWIG, Java, or Maven unavailable'
fi
```

Expected: generated wrappers compile with both breaking signatures and each
available package target succeeds. Include every emitted `SKIP` line in the
final residual-gap report rather than silently omitting a binding.

- [ ] **Step 5: Run focused simulator smoke tests**

```bash
./build-final/bin/test_http_jsonrpc_connector
./build-final/bin/test_lua_robot_client
./build-final/bin/test_robot \
  --gtest_filter='RobotTest.TestForceModeConfigSmoke:RobotTest.TestSignal'
./build-final/bin/test_lua_robot
```

Expected: deterministic transport tests pass; simulator tests pass against
`127.0.0.1:3030` and Lua port 5180. Report controller capability failures with
their exact test names.

- [ ] **Step 6: Inspect the final change set**

```bash
git status --short --branch
git diff --stat 3af3eae..HEAD
git log --oneline 3af3eae..HEAD
```

Expected: no untracked/generated files and commits correspond to the tasks
above.
