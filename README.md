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

Current recommendation on Linux (Ubuntu): use GCC via presets.

`clang++` builds are currently unstable in this project+environment combination due to toolchain/stdlib resolution issues and occasional runtime crashes during CEF bootstrap.

Prerequisites:

- CMake >= 3.24
- C++20 compiler
- A built CEF binary distribution with `CEF_ROOT` pointing to its root directory, either via environment variable or `-DCEF_ROOT=...`

Notes:

- The project does not use Boost for CLI parsing anymore.
- `CEF_ROOT` must point at the unpacked CEF distribution root that contains its `cmake/` directory.
- On Linux, the CEF runtime files and helper binaries are copied from that distribution during the build.

Configure and build:

```bash
cmake -S . -B build
cmake --build build
```

Recommended preset flow:

```bash
cmake --preset gcc-debug
cmake --build --preset gcc-debug
ctest --preset gcc-debug
```

Alternative (recommended on Linux when `clang++-22` has libstdc++ autodetection issues):

```bash
cmake -S . -B build-gcc -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build build-gcc
```

Run smoke test:

```bash
ctest --test-dir build --output-on-failure
```

Run server scaffold:

```bash
./build/src/cefwebsearchmcp_server --help
```

Run CEF bootstrap smoke test:

```bash
./build-gcc/src/cefwebsearchmcp_server --smoke-run
```

## CI

GitHub Actions (`.github/workflows/ci.yml`) runs on `push` to `main`, pull requests, and manual `workflow_dispatch`:

1. Download the pinned CEF **minimal** Linux binary distribution (`ci/cef-version.env` + `scripts/fetch-cef.sh`).
2. Configure/build with the `gcc-debug` CMake preset.
3. Run `ctest`, `--help`, and `--smoke-run` under `xvfb-run` (no desktop required).

CEF downloads are cached between workflow runs. Bump the pin in `ci/cef-version.env` when upgrading CEF locally and in CI together.

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
