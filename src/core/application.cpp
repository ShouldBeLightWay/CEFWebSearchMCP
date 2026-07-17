#include "cefwebsearchmcp/core/application.hpp"

#include <iostream>

namespace cefwebsearchmcp::core {

Application::Application(config::RuntimeConfig config)
    : config_(std::move(config)),
      session_manager_(),
      search_orchestrator_(config_.provider_priority) {}

int Application::run() {
  // This is a scaffold entrypoint; network listeners and MCP transport are added later.
  std::cout << "CEFWebSearchMCP scaffold is running" << std::endl;
  std::cout << "Port: " << config_.port << std::endl;
  std::cout << "Max results: " << config_.max_results << std::endl;
  std::cout << "Timeout (ms): " << config_.request_timeout.count() << std::endl;
  std::cout << "Providers: " << search_orchestrator_.provider_chain_as_string() << std::endl;

  if (config_.enable_cef_profiles) {
    session_manager_.initialize();
  }

  return 0;
}

const config::RuntimeConfig& Application::config() const noexcept {
  return config_;
}

}  // namespace cefwebsearchmcp::core
