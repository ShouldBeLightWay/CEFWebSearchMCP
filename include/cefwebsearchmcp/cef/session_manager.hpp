#pragma once

namespace cefwebsearchmcp::cef {

class SessionManager {
 public:
  void initialize();

  [[nodiscard]] bool initialized() const noexcept;

 private:
  bool initialized_{false};
};

}  // namespace cefwebsearchmcp::cef
