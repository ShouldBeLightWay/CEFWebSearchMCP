add_executable(cefwebsearchmcp_smoke_test
  unit/smoke_test.cpp
)

target_link_libraries(cefwebsearchmcp_smoke_test
  PRIVATE
    cefwebsearchmcp_core
)

add_test(
  NAME smoke
  COMMAND cefwebsearchmcp_smoke_test
)
