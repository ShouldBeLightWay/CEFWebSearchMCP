# Requirements

## Functional Requirements
- FR-1: Server must expose an MCP-compatible web search tool.
- FR-2: Server must support provider order configuration (google, bing, yandex).
- FR-3: Server must support authenticated browser sessions through CEF.
- FR-4: Server must fail over to the next provider when current provider fails.
- FR-5: Server must enforce configurable timeout limits.
- FR-6: Server must return normalized results (`title`, `snippet`, `url`, `site_name`, `provider`).
- FR-7: Server must expose health status and provider readiness.

## Non-Functional Requirements
- NFR-1: Startup time under 5 seconds without CEF profile bootstrap.
- NFR-2: P95 request latency target under 6 seconds in healthy path.
- NFR-3: Structured logs with correlation id for each request.
- NFR-4: No plaintext credential logging.
- NFR-5: Graceful shutdown with in-flight request cancellation.

## Operational Requirements
- OR-1: Must run on Linux localhost as first target platform.
- OR-2: Must support configuration via CLI flags and optional config file.
- OR-3: Must provide deterministic exit codes for startup failures.
