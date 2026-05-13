# AGENTS.md

Short entry point for coding agents. Keep this file as a map and put concrete
project documentation under `docs/`.

## Working Rules

- Think before coding: state assumptions, surface ambiguity, and ask when the
  safe path is not clear.
- Keep changes simple and surgical. Touch only files needed for the task.
- Preserve user changes. Do not revert unrelated edits or use destructive git
  commands unless explicitly requested.
- Define a verifiable goal for each change and run the narrowest useful build
  or test before reporting completion.

## Purpose

This repository is a CMake-first C++ SDK for controlling Lebai robots, with
optional Python, .NET, and Java bindings generated through SWIG.

## Read First

- `README.md`: supported platforms and top-level build options.
- `docs/agent-guide.md`: repo map, common edit paths, generated areas, and
  practical agent workflow.
- `docs/rpc-protocol.md`: authoritative RPC docs, wire-name mapping rules, and
  the json-rpc-cxx/nlohmann migration policy.
- `docs/develop.md`: canonical "add a new RPC" workflow.
- `docs/architecture.md`: layer boundaries and request flow.
- `docs/build-and-test.md`: local build, simulator endpoint, and test commands.
- `docs/bindings.md`: SWIG and language-package structure.

## High-Value Reminders

- The published `lebai-proto` site is the source of truth for RPC services and
  message definitions, but controller wire names may be snake_case mappings.
- For the local simulated L master controller, use `127.0.0.1:3030` as the SDK
  test target.
- Prefer the new internal RPC pattern for migrated code:
  `json-rpc-cxx` + `nlohmann/json` DTOs under `sdk/src/protos_json`.
- If a public API changes, review C++ declarations, implementation, bindings,
  tests, examples, and `docs/changelog.md`.
