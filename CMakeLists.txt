cmake_minimum_required(VERSION 3.24)

project(CEFWebSearchMCP
    VERSION 0.1.0
    DESCRIPTION "Reliable localhost Web Search MCP server based on CEF"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CEFWSMCP_BUILD_TESTS "Build tests" ON)
option(CEFWSMCP_ENABLE_WARNINGS "Enable strict warnings" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(CTest)

if(CEFWSMCP_ENABLE_WARNINGS)
  add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
  )
endif()

find_package(Boost 1.83 REQUIRED COMPONENTS system filesystem program_options)

# NOTE: CEF integration is intentionally abstracted for initial scaffold.
# Add your local/libcef setup in cmake/CEFConfig.cmake and include it here.
# include(CEFConfig)

add_subdirectory(src)

if(BUILD_TESTING AND CEFWSMCP_BUILD_TESTS)
  add_subdirectory(tests)
endif()
