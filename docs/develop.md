# Develop

## RPC Source Of Truth

Lebai RPC definitions are documented at
<https://lebai-robotics.github.io/lebai-proto/>.

Use that site for service methods, requests, and responses. Controller wire
method names are snake_case mappings, for example proto `StartTeachMode` maps
to wire RPC `start_teach_mode`; see `docs/rpc-protocol.md` before adding or
renaming a method.

## Add Or Change A Robot RPC

Use the migrated SDK2-style path:

1. Check the proto docs and confirm the actual wire RPC name.
2. Add or update `nlohmann/json` DTOs in `sdk/src/protos_json`.
3. Add or update the public declaration in `sdk/include/lebai/robot.hh`.
4. Convert public parameters to DTOs in `sdk/src/robot.cc`.
5. Add or update the internal call in `sdk/src/robot_impl.hh` and
   `sdk/src/robot_impl.cc`.
6. Review SWIG bindings under `sdk/python`, `sdk/dotnet`, and `sdk/java` if the
   public API changed.
7. Add focused tests in `sdk/test`.

Do not add new robot RPC DTOs under `sdk/src/protos`; that directory contains
the legacy RapidJSON DTO layer and is no longer linked into the core
`lebai-cpp` library.

## DTO Example

Prefer small aggregate structs with `NLOHMANN_DEFINE_TYPE_INTRUSIVE` when the
JSON shape is direct:

```cpp
namespace protos_json::motion_proto {

struct MotionIndex {
  unsigned int id{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(MotionIndex, id)
};

}  // namespace protos_json::motion_proto
```

Use custom `from_json`/`to_json` only when the controller shape differs from the
C++ field names or has optional fields.

## Implementation Example

Public wrapper in `sdk/src/robot.cc`:

```cpp
int Robot::movej(const std::vector<double>& joint_positions, double a,
                 double v, double t, double r) {
  protos_json::motion_proto::MoveRequest req;
  req.param.acc = a;
  req.param.velocity = v;
  req.param.time = t;
  req.param.radius = r;
  req.pose.kind = 1;
  req.pose.joint.joint = joint_positions;

  const auto resp = impl_->move_joint(req);
  return static_cast<int>(resp.id);
}
```

Internal RPC call in `sdk/src/robot_impl.cc`:

```cpp
protos_json::motion_proto::MotionIndex Robot::RobotImpl::move_joint(
    const protos_json::motion_proto::MoveRequest& req) {
  return rpc_client_->Call<protos_json::motion_proto::MotionIndex>(
      "move_joint", {req});
}
```

## Verification

At minimum, build the C++ test targets and run the DTO test:

```bash
cmake --build build-work --target test_json_protos test_robot
ctest --test-dir build-work -R TestJsonProtos --output-on-failure
```

When a simulator is running, use `127.0.0.1:3030` through
`TEST_ROBOT_IP=127.0.0.1` and run the focused `test_robot` case for the changed
API.
