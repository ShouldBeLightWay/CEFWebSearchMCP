#include "cefwebsearchmcp/core/application.hpp"

#include <iostream>

#include "cefwebsearchmcp/config/runtime_config.hpp"

namespace cefwebsearchmcp::core {

Application::Application(config::RuntimeConfig config)
    : config_(std::move(config)),
      session_manager_(),
      search_orchestrator_(config_.provider_priority) {}

int Application::run(int argc, char** argv) {
  std::cout << "CEFWebSearchMCP scaffold is running" << std::endl;
  std::cout << "Port: " << config_.port << std::endl;
  std::cout << "Max results: " << config_.max_results << std::endl;
  std::cout << "Timeout (ms): " << config_.request_timeout.count() << std::endl;
  std::cout << "Providers: " << search_orchestrator_.provider_chain_as_string() << std::endl;

  if (config_.enable_cef_profiles) {
    const cef::SessionOptions session_options{
        .use_sandbox = config_.use_sandbox,
        .cache_path = config_.cache_path,
        .log_file = config_.log_file,
    };

    if (!session_manager_.initialize(session_options, argc, argv)) {
      std::cerr << "CEF initialization failed" << std::endl;
      return 2;
    }

    std::cout << "CEF initialized successfully" << std::endl;

    if (config_.run_mode == config::RunMode::Smoke) {
      std::cout << "Smoke run completed" << std::endl;
      session_manager_.shutdown();
      return 0;
    }

    session_manager_.shutdown();
  }

  return 0;
}

const config::RuntimeConfig& Application::config() const noexcept {
  return config_;
}

}  // namespace cefwebsearchmcp::core
