# CEF Runtime Notes

Operational notes for CEF bootstrap on Linux. These are expected third-party / platform behaviors, not project defects.

## Intended Runtime Shape

- The MCP server is meant to run as a long-lived background process (for example a systemd unit).
- A visible browser window is only needed for interactive search-provider authorization.
- Day-to-day search work should use persisted authenticated CEF profiles without showing UI.

## `--smoke-run`

Quick CEF bootstrap check:

```bash
./build-gcc/src/cefwebsearchmcp_server --smoke-run
```

Successful run prints config, then:

```text
CEF initialized successfully
Smoke run completed
```

Exit code `0` means bootstrap/shutdown succeeded.

### Benign `waitpid` error on shutdown

Smoke run may end with:

```text
ERROR:base/process/kill_posix.cc:… waitpid(…): No child processes (10)
```

Cause:
- `--smoke-run` calls `CefInitialize` and immediately `CefShutdown`.
- Chromium tears down child processes (zygote / GPU / network utility).
- One shutdown path already reaped a child; another still calls `waitpid` and gets `ECHILD`.

This is a Chromium process-teardown race. It does not fail the smoke run and is not actionable in project code. Do not raise CEF `log_severity` just to hide it — that would also hide real errors.

### Ozone / Wayland noise

On Wayland desktop sessions (`XDG_SESSION_TYPE=wayland`), CEF/Chromium may default to `ozone-platform=wayland` and log many:

```text
ERROR:ui/ozone/platform/wayland/… Not implemented reached in …
```

and occasionally a compositor warning such as:

```text
warning: queue … destroyed while proxies still attached:
  zwp_tablet_pad_v2#… still attached
```

These come from incomplete Wayland protocol stubs in Chromium, not from CEFWebSearchMCP logic.

Mitigation already applied in `SessionManager`:
- Default `SessionOptions::ozone_platform` is `"x11"`.
- `BrowserApp::OnBeforeCommandLineProcessing` appends `--ozone-platform=x11` when unset.
- On Wayland desktops this uses XWayland and removes the stub spam for windowless bootstrap.

`ozone-platform=headless` is also mostly quiet, but X11 better matches the future auth-window path.

## Related Files

- `src/cef/session_manager.cpp` — CEF init/shutdown and Ozone switch injection
- `src/core/application.cpp` — `--smoke-run` lifecycle
- `include/cefwebsearchmcp/cef/session_manager.hpp` — `SessionOptions::ozone_platform`
