#include "cefwebsearchmcp/search/search_orchestrator.hpp"

#include <sstream>

namespace cefwebsearchmcp::search {

SearchOrchestrator::SearchOrchestrator(std::vector<std::string> provider_chain)
    : provider_chain_(std::move(provider_chain)) {}

const std::vector<std::string>& SearchOrchestrator::provider_chain() const noexcept {
  return provider_chain_;
}

std::string SearchOrchestrator::provider_chain_as_string() const {
  std::ostringstream out;
  for (std::size_t i = 0; i < provider_chain_.size(); ++i) {
    out << provider_chain_[i];
    if (i + 1 < provider_chain_.size()) {
      out << " -> ";
    }
  }
  return out.str();
}

}  // namespace cefwebsearchmcp::search
