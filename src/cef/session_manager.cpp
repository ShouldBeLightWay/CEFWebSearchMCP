#include "cefwebsearchmcp/cef/session_manager.hpp"

#include <filesystem>
#include <system_error>

#include "include/cef_app.h"
#include "include/cef_command_line.h"

namespace cefwebsearchmcp::cef {

namespace {

std::filesystem::path to_absolute_path(const std::string& value) {
  if (value.empty()) {
    return {};
  }

  std::error_code ec;
  auto path = std::filesystem::path(value);
  if (path.is_relative()) {
    path = std::filesystem::absolute(path, ec);
    if (ec) {
      return std::filesystem::path(value);
    }
  }

  return path.lexically_normal();
}

void ensure_directory_exists(const std::filesystem::path& path) {
  if (path.empty()) {
    return;
  }

  std::error_code ec;
  std::filesystem::create_directories(path, ec);
}

// Browser-process CefApp used to inject Chromium switches before CEF starts.
// On Linux Wayland sessions CEF defaults to ozone/wayland, which currently logs
// many NOTIMPLEMENTED stubs. Prefer X11 (XWayland) for windowless bootstrap.
class BrowserApp : public CefApp {
 public:
  explicit BrowserApp(std::string ozone_platform)
      : ozone_platform_(std::move(ozone_platform)) {}

  void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) override {
    if (!process_type.empty() || ozone_platform_.empty()) {
      return;
    }

    if (!command_line->HasSwitch("ozone-platform")) {
      command_line->AppendSwitchWithValue("ozone-platform", ozone_platform_);
    }
  }

 private:
  std::string ozone_platform_;

  IMPLEMENT_REFCOUNTING(BrowserApp);
};

}  // namespace

bool SessionManager::initialize(const SessionOptions& options, int argc, char** argv) {
  if (initialized_) {
    return true;
  }

  const auto cache_path = to_absolute_path(options.cache_path);
  const auto log_file = to_absolute_path(options.log_file);

  ensure_directory_exists(cache_path);
  ensure_directory_exists(log_file.parent_path());

  CefMainArgs main_args(argc, argv);
  CefRefPtr<BrowserApp> app(new BrowserApp(options.ozone_platform));

  const int subprocess_code = CefExecuteProcess(main_args, app, nullptr);
  if (subprocess_code >= 0) {
    return false;
  }

  CefSettings settings;
  settings.no_sandbox = options.use_sandbox ? 0 : 1;
  settings.windowless_rendering_enabled = 1;

  if (!cache_path.empty()) {
    const auto cache_path_str = cache_path.string();
    CefString(&settings.cache_path) = cache_path_str;
    CefString(&settings.root_cache_path) = cache_path_str;
  }

  if (!log_file.empty()) {
    CefString(&settings.log_file) = log_file.string();
  }

  initialized_ = CefInitialize(main_args, settings, app, nullptr);
  return initialized_;
}

void SessionManager::shutdown() {
  if (!initialized_) {
    return;
  }

  CefShutdown();
  initialized_ = false;
}

bool SessionManager::initialized() const noexcept {
  return initialized_;
}

}  // namespace cefwebsearchmcp::cef
