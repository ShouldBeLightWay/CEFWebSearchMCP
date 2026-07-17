add_library(cefwebsearchmcp_core
  core/application.cpp
  search/search_orchestrator.cpp
  cef/session_manager.cpp
)

target_include_directories(cefwebsearchmcp_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_compile_features(cefwebsearchmcp_core PUBLIC cxx_std_20)

add_executable(cefwebsearchmcp_server
  main.cpp
)

target_link_libraries(cefwebsearchmcp_server
  PRIVATE
    cefwebsearchmcp_core
)

install(TARGETS cefwebsearchmcp_server
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
