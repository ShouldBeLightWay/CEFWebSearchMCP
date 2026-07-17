# CEFWebSearchMCP

Reliable localhost Web Search MCP server scaffold based on CEF.

## Why this project

Common localhost search servers are often blocked by Google/Bing/Yandex because requests are anonymous and session state is not preserved. This project targets a more reliable approach:

- Authenticated provider sessions through CEF profiles.
- Provider fallback (Google -> Bing -> Yandex).
- Configurable timeout/reliability controls.
- MCP-native tool interface for AI agents.

## Current status

Scaffold phase is complete:

- C++20 + CMake project layout.
- Core modules (`core`, `cef`, `search`, `config`, `net`).
- Basic executable target and smoke test.
- Project documentation in `doc/`.

## Build

Prerequisites:

- CMake >= 3.24
- C++20 compiler
- Boost 1.83+ (`system`, `filesystem`, `program_options`)

Configure and build:

```bash
cmake -S . -B build
cmake --build build
```

Run smoke test:

```bash
ctest --test-dir build --output-on-failure
```

Run server scaffold:

```bash
./build/src/cefwebsearchmcp_server --help
```

## Repository layout

- `include/cefwebsearchmcp/`: public headers and interfaces.
- `src/`: implementation.
- `tests/`: smoke/unit tests.
- `cmake/`: CMake helper modules (including future CEF wiring).
- `doc/`: architecture, roadmap, API draft, requirements, risk register.

## Documentation index

- `doc/overview.md`
- `doc/architecture.md`
- `doc/roadmap.md`
- `doc/requirements.md`
- `doc/mcp-api.md`
- `doc/risk-register.md`
- `doc/crawler-server-gap-analysis.md`

## Next implementation steps

1. Integrate local libcef distribution in `cmake/CEFConfig.cmake`.
2. Implement persisted profile/session manager for providers.
3. Add provider adapters and HTML/result normalization.
4. Add MCP transport and tool handlers.
