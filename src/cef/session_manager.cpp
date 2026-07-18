#include "cefwebsearchmcp/cef/session_manager.hpp"

#include <filesystem>

#include "include/cef_app.h"

namespace cefwebsearchmcp::cef {

bool SessionManager::initialize(const SessionOptions& options, int argc, char** argv) {
  if (initialized_) {
    return true;
  }

  if (!options.cache_path.empty()) {
    std::filesystem::create_directories(options.cache_path);
  }
  if (!options.log_file.empty()) {
    std::filesystem::create_directories(std::filesystem::path(options.log_file).parent_path());
  }

  CefMainArgs main_args(argc, argv);

  const int subprocess_code = CefExecuteProcess(main_args, nullptr, nullptr);
  if (subprocess_code >= 0) {
    return false;
  }

  CefSettings settings;
  settings.no_sandbox = options.use_sandbox ? 0 : 1;
  settings.windowless_rendering_enabled = 1;

  if (!options.cache_path.empty()) {
    CefString(&settings.cache_path) = options.cache_path;
  }

  if (!options.log_file.empty()) {
    CefString(&settings.log_file) = options.log_file;
  }

  initialized_ = CefInitialize(main_args, settings, nullptr, nullptr);
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
