#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace cefwebsearchmcp::config {

struct RuntimeConfig {
  std::uint16_t port{8080};
  std::size_t max_results{8};
  std::chrono::milliseconds request_timeout{15000};
  bool enable_cef_profiles{true};
  std::vector<std::string> provider_priority{"google", "bing", "yandex"};
};

}  // namespace cefwebsearchmcp::config
