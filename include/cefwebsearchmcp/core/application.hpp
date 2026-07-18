#pragma once

#include "cefwebsearchmcp/cef/session_manager.hpp"
#include "cefwebsearchmcp/config/runtime_config.hpp"
#include "cefwebsearchmcp/search/search_orchestrator.hpp"

namespace cefwebsearchmcp::core {

class Application {
 public:
  explicit Application(config::RuntimeConfig config);

  int run(int argc, char** argv);
  const config::RuntimeConfig& config() const noexcept;

 private:
  config::RuntimeConfig config_;
  cef::SessionManager session_manager_;
  search::SearchOrchestrator search_orchestrator_;
};

}  // namespace cefwebsearchmcp::core
