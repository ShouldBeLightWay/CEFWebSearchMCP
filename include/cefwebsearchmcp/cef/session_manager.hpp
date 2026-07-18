#pragma once

#include <string>

namespace cefwebsearchmcp::cef {

struct SessionOptions {
  bool use_sandbox{false};
  std::string cache_path;
  std::string log_file;
};

class SessionManager {
 public:
  bool initialize(const SessionOptions& options, int argc, char** argv);
  void shutdown();

  [[nodiscard]] bool initialized() const noexcept;

 private:
  bool initialized_{false};
};

}  // namespace cefwebsearchmcp::cef
