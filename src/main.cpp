#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>

#include "cefwebsearchmcp/config/runtime_config.hpp"
#include "cefwebsearchmcp/core/application.hpp"

namespace {

void print_help() {
  std::cout
      << "CEFWebSearchMCP options\n"
      << "  --help, -h                Show this help message\n"
      << "  --port, -p <value>        HTTP port for MCP server\n"
      << "  --max-results <value>     Maximum number of search results\n"
      << "  --timeout-ms <value>      Search request timeout in milliseconds\n"
      << "  --provider <value>        Add provider to chain (repeatable)\n";
}

bool has_next(int index, int argc) {
  return (index + 1) < argc;
}

}  // namespace

int main(int argc, char** argv) {
  cefwebsearchmcp::config::RuntimeConfig config;
  std::vector<std::string> providers;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      print_help();
      return 0;
    }

    if ((arg == "--port" || arg == "-p") && has_next(i, argc)) {
      config.port = static_cast<std::uint16_t>(std::strtoul(argv[++i], nullptr, 10));
      continue;
    }

    if (arg == "--max-results" && has_next(i, argc)) {
      config.max_results = static_cast<std::size_t>(std::strtoull(argv[++i], nullptr, 10));
      continue;
    }

    if (arg == "--timeout-ms" && has_next(i, argc)) {
      config.request_timeout = std::chrono::milliseconds(std::strtoul(argv[++i], nullptr, 10));
      continue;
    }

    if (arg == "--provider" && has_next(i, argc)) {
      providers.emplace_back(argv[++i]);
      continue;
    }

    std::cerr << "Unknown or incomplete argument: " << arg << std::endl;
    print_help();
    return 1;
  }

  if (!providers.empty()) {
    config.provider_priority = std::move(providers);
  }

  cefwebsearchmcp::core::Application app(std::move(config));
  return app.run();
}
