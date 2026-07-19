# AGENTS.md

## Project Snapshot

CEFWebSearchMCP is a localhost Web Search MCP server scaffold focused on reliability and authenticated search sessions through CEF.

Primary goals:
- Use persisted authenticated browser profiles for Google/Bing/Yandex.
- Reduce anonymous-request bans and captcha frequency.
- Provide configurable timeout and provider fallback behavior.
- Expose MCP-compatible search tooling.

## Current Implementation Status

Implemented:
- C++20/CMake scaffold with modular layout.
- CEF integration through official binary SDK (`find_package(CEF)` + `libcef_dll_wrapper`).
- Runtime deployment of CEF binaries/resources near executable.
- Basic `CefInitialize`/`CefShutdown` bootstrap path.
- Smoke run CLI mode (`--smoke-run`) that performs bootstrap and shutdown.
- Basic smoke test target in `tests/unit/smoke_test.cpp`.

Not implemented yet (planned):
- Real provider adapters (Google/Bing/Yandex parsing/execution).
- MCP transport/tool handlers.
- Full session/auth orchestration UX.
- Structured observability and production diagnostics.

## Build and Test (Recommended)

Use GCC presets on Linux Ubuntu.

```bash
cmake --preset gcc-debug
cmake --build --preset gcc-debug
ctest --preset gcc-debug
```

Run server help:

```bash
./build-gcc/src/cefwebsearchmcp_server --help
```

Run CEF bootstrap smoke path:

```bash
./build-gcc/src/cefwebsearchmcp_server --smoke-run
```

## CI (GitHub-hosted)

Workflow: `.github/workflows/ci.yml` (`ubuntu-24.04`, GCC preset).

Pinned CEF version: `ci/cef-version.env` (fetched by `scripts/fetch-cef.sh`).

After meaningful CI-related edits, prefer validating locally with the same preset flow, then push and confirm the Actions run is green.

## Toolchain Guidance

Current recommendation:
- Use GCC (`gcc`, `g++`) on Linux Ubuntu.

Reason:
- Clang toolchain in this environment has unresolved stdlib/toolchain issues and unstable runtime behavior for this project setup.

## Runtime Notes

- CEF may warn if cache path is not absolute.
- Expected `--smoke-run` / Ozone quirks: see `doc/cef-runtime-notes.md`.
- `chrome-sandbox` SUID permissions may be required for full Linux runtime behavior:

```bash
EXE="${path/to/repository}/CEFWebSearchMCP/build-gcc/src/chrome-sandbox"
sudo chown root:root "$EXE"
sudo chmod 4755 "$EXE"
```

## Important Files

- Root build config: `CMakeLists.txt`
- CEF integration glue: `cmake/CEFConfig.cmake`, `cmake/FindCEF.cmake`
- Presets: `CMakePresets.json`
- Entry point: `src/main.cpp`
- CEF bootstrap: `src/cef/session_manager.cpp`
- App lifecycle: `src/core/application.cpp`

## Documentation Index

Use these docs as source of truth for architecture and planning:
- `doc/overview.md`
- `doc/architecture.md`
- `doc/roadmap.md`
- `doc/requirements.md`
- `doc/mcp-api.md`
- `doc/risk-register.md`
- `doc/crawler-server-gap-analysis.md`
- `doc/clangd-setup.md`
- `doc/cef-runtime-notes.md`

## Suggested Agent Behavior in New Chats

When starting a new task:
1. Assume GCC preset workflow unless user requests otherwise.
2. Prefer minimal, incremental changes over broad refactors.
3. Keep CEF bootstrap stable before adding new provider/MCP features.
4. Validate with build + test after meaningful edits.
5. Reference `/doc` files instead of rewriting architecture decisions.

## Problem Solving Strategy

Before modifying code determine whether the issue belongs to

- project code
- build configuration
- operating system
- compiler
- dependency version
- third-party library

If the issue is external to the project,
explain it instead of implementing workarounds.
