#include "cefwebsearchmcp/cef/session_manager.hpp"

#include <cerrno>
#include <filesystem>
#include <system_error>

#include "include/cef_app.h"

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

  const int subprocess_code = CefExecuteProcess(main_args, nullptr, nullptr);
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
