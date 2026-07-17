# Roadmap

## Phase 0 - Foundation (current)
- Buildable CMake scaffold.
- Core interfaces and modules.
- Documentation baseline.

Exit criteria:
- `cmake -S . -B build` configures successfully.
- Smoke test target is registered.

## Phase 1 - Runtime Core
- Implement runtime config from file + CLI.
- Add structured logging.
- Add health endpoint model in MCP transport.

Exit criteria:
- Runtime config is validated with clear startup errors.
- Health checks report component readiness.

## Phase 2 - CEF Integration
- Wire local libcef package and helper binaries.
- Add persisted profile folders for google/bing/yandex.
- Add first-run workflow for manual user login in embedded browser.

Exit criteria:
- Session state persists across restarts.
- Auth state can be reported by component checks.

## Phase 3 - Provider Adapters
- Implement Google adapter.
- Implement Bing adapter.
- Implement Yandex adapter.
- Add normalization contract and parser test fixtures.

Exit criteria:
- Each adapter can return normalized results independently.
- Provider-specific failures are classified.

## Phase 4 - Orchestration and Reliability
- Timeout and cancellation support.
- Provider failover chain.
- Failure reason propagation for diagnostics.

Exit criteria:
- Fallback works under forced provider failures.
- Latency budget constraints are enforced.

## Phase 5 - MCP Server
- Implement MCP transport and tool schemas.
- Add tools: `search.web`, `search.health`, `search.providers`.
- Add response trace fields (provider_used, fallback_count, duration_ms).

Exit criteria:
- MCP client can call tools and receive valid schema responses.

## Phase 6 - Hardening and Packaging
- Add unit tests and integration tests.
- Add CI checks.
- Linux packaging and release docs.

Exit criteria:
- Stable release candidate with reproducible build instructions.
