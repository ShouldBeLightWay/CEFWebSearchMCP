#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#define CXXOPTS_NO_EXCEPTIONS
#define CXXOPTS_NO_RTTI
#include "cefwebsearchmcp/core/cxxopts.hpp"

#include "cefwebsearchmcp/config/runtime_config.hpp"
#include "cefwebsearchmcp/core/application.hpp"

namespace {

bool is_cef_subprocess_arg(const char* arg) {
  return arg != nullptr && std::string(arg).rfind("--type=", 0) == 0;
}

}  // namespace

int main(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    if (is_cef_subprocess_arg(argv[i])) {
      cefwebsearchmcp::config::RuntimeConfig child_config;
      child_config.enable_cef_profiles = true;
      cefwebsearchmcp::core::Application child_app(std::move(child_config));
      return child_app.run(argc, argv);
    }
  }

  cefwebsearchmcp::config::RuntimeConfig config;
  cxxopts::Options options("cefwebsearchmcp_server", "CEFWebSearchMCP options");
  options.add_options()
      ("h,help", "Show this help message")
      ("p,port", "HTTP port for MCP server", cxxopts::value<unsigned int>())
      ("max-results", "Maximum number of search results", cxxopts::value<std::size_t>())
      ("timeout-ms", "Search request timeout in milliseconds", cxxopts::value<std::uint64_t>())
      ("provider", "Add provider to chain (repeatable)", cxxopts::value<std::vector<std::string>>())
      ("smoke-run", "Initialize and shutdown CEF, then exit")
      ("cef-cache-path", "CEF cache directory path", cxxopts::value<std::string>())
      ("cef-log-file", "CEF log file path", cxxopts::value<std::string>())
      ("cef-sandbox", "Enable CEF sandbox");

  const auto result = options.parse(argc, argv);

  if (result.count("help") > 0) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result.count("port") > 0) {
    const auto port_value = result["port"].as<unsigned int>();
    if (port_value > std::numeric_limits<std::uint16_t>::max()) {
      std::cerr << "Invalid --port value: out of range for uint16_t" << std::endl;
      return 1;
    }
    config.port = static_cast<std::uint16_t>(port_value);
  }

  if (result.count("max-results") > 0) {
    config.max_results = result["max-results"].as<std::size_t>();
  }

  if (result.count("timeout-ms") > 0) {
    const auto timeout_ms = result["timeout-ms"].as<std::uint64_t>();
    using MillisRep = std::chrono::milliseconds::rep;
    if (timeout_ms > static_cast<std::uint64_t>(std::numeric_limits<MillisRep>::max())) {
      std::cerr << "Invalid --timeout-ms value: out of range" << std::endl;
      return 1;
    }
    config.request_timeout = std::chrono::milliseconds(static_cast<MillisRep>(timeout_ms));
  }

  if (result.count("provider") > 0) {
    config.provider_priority = result["provider"].as<std::vector<std::string>>();
  }

  if (result.count("smoke-run") > 0) {
    config.run_mode = cefwebsearchmcp::config::RunMode::Smoke;
  }

  if (result.count("cef-cache-path") > 0) {
    config.cache_path = result["cef-cache-path"].as<std::string>();
  }

  if (result.count("cef-log-file") > 0) {
    config.log_file = result["cef-log-file"].as<std::string>();
  }

  if (result.count("cef-sandbox") > 0) {
    config.use_sandbox = true;
  }

  cefwebsearchmcp::core::Application app(std::move(config));
  return app.run(argc, argv);
}
