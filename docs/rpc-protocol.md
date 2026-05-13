# RPC Protocol Guidance

## Authoritative Sources

- Main protocol docs: <https://lebai-robotics.github.io/lebai-proto/>
- Motion service method list:
  <https://lebai-robotics.github.io/lebai-proto/#lebai.motion.MotionService>

The published `lebai-proto` spec is the source of truth for which RPCs,
services, requests, and responses exist.

## Wire Name Rule

The controller wire method name is not always written exactly like the proto
service method spelling. Proto method names often map to snake_case wire names.

Example:

```text
StartTeachMode -> start_teach_mode
```

Do not guess the final wire name from old wrappers alone. Check the proto docs
first, then confirm the actual controller-facing wire name from existing SDK
code, controller docs, or verified runtime behavior.

## Migration Policy

For the SDK2-style refactor, prefer:

- `json-rpc-cxx` for JSON-RPC request/response handling
- `cpp-httplib` for HTTP `/jsonrpc` transport
- `nlohmann/json` DTOs under `sdk/src/protos_json`

If SDK2 already has a similar API or model, use that style as the priority. If
it does not, keep the public API stable and migrate the internals in the same
pattern.

For newly migrated C++ internals, name implementation functions after the real
wire RPC name where practical. Keep public API names stable unless the task
explicitly changes them.

## Simulator Endpoint

For the local simulated L master controller, use:

```text
127.0.0.1:3030
```

The controller container may report an internal Docker IP such as
`172.17.0.3`, but SDK tests should target the forwarded localhost port. Do not
use `3031` for this simulator setup unless the container explicitly exposes it.
