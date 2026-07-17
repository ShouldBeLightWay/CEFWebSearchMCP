# Architecture

## High-Level Design
The server is split into a small set of modular components:

- Core Application Layer: process lifecycle, config wiring, startup checks.
- Session Layer (CEF): managed browser contexts and authenticated profile persistence.
- Search Orchestration Layer: provider routing, fallback policy, timeout budget.
- Provider Adapters: Google, Bing, Yandex query execution and result normalization.
- MCP Transport Layer: exposes search tools and structured responses to AI agents.
- Observability Layer: health, metrics, diagnostics, and structured logs.

## Component Responsibilities

### Core Application
- Parse runtime options.
- Initialize dependency graph.
- Start MCP transport.
- Trigger graceful shutdown.

### CEF Session Manager
- Initialize CEF runtime once.
- Create and maintain persisted per-provider profiles.
- Track authentication state.
- Expose browser contexts to provider adapters.

### Search Orchestrator
- Accept normalized search requests.
- Apply timeout policy and provider order.
- Execute failover chain based on configurable conditions.
- Return normalized result set and trace metadata.

### Provider Adapter Interface
All providers must implement the same behavior contract:
- Build provider-specific query URL and settings.
- Execute request with browser/session context.
- Parse and normalize title/snippet/url/site metadata.
- Report provider-specific failure reasons.

### MCP Transport
- Define tools such as `search.web` and `search.health`.
- Validate input schemas.
- Convert internal responses to MCP output format.

## Data Flow
1. Agent sends MCP search request.
2. MCP layer validates payload and creates internal request object.
3. Orchestrator selects provider order and timeout budget.
4. Adapter executes query using authenticated CEF context.
5. Results are normalized and returned.
6. On failure, orchestrator switches to next provider until success or chain exhaustion.

## Failure Strategy
- Hard timeout per provider.
- Retry budget (optional, disabled by default for phase 1).
- Failover to next provider on parser, network, or anti-bot failure.
- Final structured error response if all providers fail.

## Security and Privacy
- Profiles stored locally only.
- No credential extraction from browser stores.
- Logs must avoid sensitive data.
- Optional redaction for query strings in logs.

## Directory Mapping
- `include/cefwebsearchmcp/*`: public interfaces and shared headers.
- `src/core`: app bootstrap and runtime wiring.
- `src/cef`: CEF runtime and profile/session management.
- `src/search`: orchestration and provider-level domain logic.
- `src/net`: transport and protocol code.
- `src/config`: config parsing and defaults.
- `tests/unit`: fast unit and smoke tests.
