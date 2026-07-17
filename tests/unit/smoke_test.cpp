#include <cassert>
#include <vector>

#include "cefwebsearchmcp/search/search_orchestrator.hpp"

int main() {
  cefwebsearchmcp::search::SearchOrchestrator orchestrator({"google", "bing", "yandex"});
  const auto& chain = orchestrator.provider_chain();

  assert(chain.size() == 3U);
  assert(chain.front() == "google");
  assert(chain.back() == "yandex");

  return 0;
}
