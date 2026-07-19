cmake_minimum_required(VERSION 3.24)

project(CEFWebSearchMCP
    VERSION 0.1.0
    DESCRIPTION "Reliable localhost Web Search MCP server based on CEF"
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CEFWSMCP_BUILD_TESTS "Build tests" ON)
option(CEFWSMCP_ENABLE_WARNINGS "Enable strict warnings" ON)
option(CEFWSMCP_ENABLE_CEF "Enable libcef integration" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(CTest)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(EXISTS "/usr/lib/gcc/x86_64-linux-gnu/15")
    add_compile_options("--gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/15")
    add_link_options("--gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/15")
    link_directories("/usr/lib/gcc/x86_64-linux-gnu/15")
  endif()

  if(EXISTS "/usr/include/c++/15")
    add_compile_options("-isystem/usr/include/c++/15")
  endif()

  if(EXISTS "/usr/include/x86_64-linux-gnu/c++/15")
    add_compile_options("-isystem/usr/include/x86_64-linux-gnu/c++/15")
  endif()
endif()

if(CEFWSMCP_ENABLE_WARNINGS)
  add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wextra>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wpedantic>
  )
endif()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

if(CEFWSMCP_ENABLE_CEF)
  include(CEFConfig)
endif()

add_subdirectory(src)

if(BUILD_TESTING AND CEFWSMCP_BUILD_TESTS)
  add_subdirectory(tests)
endif()
