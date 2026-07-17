#include "cefwebsearchmcp/cef/session_manager.hpp"

namespace cefwebsearchmcp::cef {

void SessionManager::initialize() {
  // CEF bootstrap and persisted profile wiring will be implemented in the next milestone.
  initialized_ = true;
}

bool SessionManager::initialized() const noexcept {
  return initialized_;
}

}  // namespace cefwebsearchmcp::cef
