#pragma once

#include <string>
#include <vector>

namespace cefwebsearchmcp::search {

class SearchOrchestrator {
 public:
  explicit SearchOrchestrator(std::vector<std::string> provider_chain);

  const std::vector<std::string>& provider_chain() const noexcept;
  std::string provider_chain_as_string() const;

 private:
  std::vector<std::string> provider_chain_;
};

}  // namespace cefwebsearchmcp::search
