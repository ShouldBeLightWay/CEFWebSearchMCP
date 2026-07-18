# clangd Setup Notes

## Context

This project uses Clangd with `compile_commands.json` from `build/`.
On Ubuntu-like systems, Clang may select a GCC toolchain version that does not
have matching `libstdc++` headers installed. In that state, Clangd can report:

- `'cstdint' file not found`
- `use of undeclared identifier 'std'`

even when the project builds with `g++`.

## Symptoms

Common log fragments from Clangd:

- `IncludeCleaner: ... include <cstdint> : No such file or directory`
- `use of undeclared identifier 'std'`

Quick check command:

```bash
/home/natsuki/.config/Code/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/22.1.6/clangd_22.1.6/bin/clangd \
	--check=src/main.cpp \
	--compile-commands-dir=build \
	--enable-config \
	--query-driver=/home/natsuki/work/CEFWebSearchMCP/scripts/clangd-cxx-driver.sh,/usr/bin/clang++-*,/usr/bin/g++-* \
	--log=verbose
```

## Project Fix (version-resilient)

The repository includes a local wrapper:

- `scripts/clangd-cxx-driver.sh`

Behavior:

- Detects an installed GCC directory that has corresponding `libstdc++` headers.
- Picks the newest valid candidate.
- Runs `clang++` with `--gcc-install-dir=<detected-path>`.

Clangd is configured to use it through:

- `.clangd` (`CompileFlags.Compiler: ../scripts/clangd-cxx-driver.sh`)
- `.vscode/settings.json` (`clangd.arguments` with `--query-driver=...`)

This avoids hardcoding one GCC version (for example `15`) and survives package
upgrades better.

## Verify Setup

1. Confirm wrapper can see standard C++ includes:

```bash
./scripts/clangd-cxx-driver.sh -E -x c++ - -v < /dev/null
```

Expected: search list includes paths like:

- `/usr/include/c++/<version>`
- `/usr/include/x86_64-linux-gnu/c++/<version>`

2. Run Clangd check command (above) and ensure there is no `pp_file_not_found`
	 for `<cstdint>` and no `undeclared identifier 'std'`.

3. In VS Code run:

- `Clangd: Restart language server`
- If needed: `Developer: Reload Window`

## Troubleshooting

- If wrapper says `clang++ not found`, ensure at least one of these exists:
	- `/usr/bin/clang++`
	- `/usr/bin/clang++-<version>`

- If Clangd does not execute the wrapper for system include extraction, verify
	`--query-driver` contains an absolute path to the wrapper in
	`.vscode/settings.json`.

- If headers are still missing, inspect available GCC dirs:

```bash
g++ -dumpmachine
ls -1 /usr/lib/gcc/$(g++ -dumpmachine)
ls -1 /usr/include/c++
```

and confirm at least one version exists in both GCC and `/usr/include/c++`.
