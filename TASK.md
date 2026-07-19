# Current Task

Phase 1 - Runtime Core

## Goal
Bring the runtime layer up to the first production-shaped milestone: configuration validation, structured logging, and a health model that can later be exposed through MCP transport.

## Current Status
In progress.

## Phase 1 Subtasks

### P0 - Configuration loading and validation
- Add file-based runtime config loading.
- Merge file config with CLI overrides in a predictable order.
- Validate required paths, numeric ranges, and provider lists before startup.

### Done criteria
- Startup fails fast with clear errors for invalid config values.
- CLI override precedence is documented and tested.
- Current defaults remain usable when no config file is provided.

### P1 - Structured logging
- Replace ad hoc startup prints with structured log output.
- Include severity and component context in startup/runtime messages.
- Keep logs usable in both normal run and smoke-run paths.

### Done criteria
- Startup and initialization messages use a consistent log format.
- Errors are emitted with enough context to identify the failing component.
- Smoke-run remains readable and does not regress runtime behavior.

### P2 - Health model for future MCP transport
- Define a health/status model for core runtime components.
- Represent CEF readiness, configuration validity, and provider readiness.
- Keep the model transport-agnostic so MCP wiring can consume it later.

### Done criteria
- Health state can be queried from code without side effects.
- The model distinguishes ok / degraded / not ready states.
- The structure is ready to map into `search.health` later.

## Done
- CLI parsing is implemented with `cxxopts` in no-exceptions/no-RTTI mode.
- Runtime options are wired into `RuntimeConfig`.
- CEF bootstrap works with absolute `cache_path` and `root_cache_path`.
- CEF subprocess handling is fixed so child processes do not re-enter the main application flow.
- Smoke run works and exits successfully.
- CMake configure and smoke test registration are passing.

## Not Done Yet
- Runtime config is not loaded from file.
- Runtime config validation is still minimal and mostly implicit.
- Structured logging is not implemented.
- Health endpoint / health model is not implemented.
- MCP transport is not implemented yet.

## Notes
- The current codebase is beyond Phase 0 and is ready to start Phase 1 work.
- Keep this file updated as the active implementation target moves forward.
