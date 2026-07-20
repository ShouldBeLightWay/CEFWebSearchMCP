set(CEF_USE_SANDBOX OFF CACHE BOOL "Disable CEF sandbox for initial Linux integration" FORCE)

find_package(CEF REQUIRED)

set(CEFWSMCP_CEF_ROOT "${_CEF_ROOT}" CACHE INTERNAL "Resolved CEF root path")

# Minimal CEF binary distributions ship Release/ only. Local full SDKs may also
# include Debug/. When Debug libcef is absent, reuse Release for Debug builds so
# linking and COPY_FILES still work with CMAKE_BUILD_TYPE=Debug.
if(DEFINED CEF_LIB_DEBUG AND NOT EXISTS "${CEF_LIB_DEBUG}")
  if(NOT EXISTS "${CEF_LIB_RELEASE}")
    message(FATAL_ERROR "CEF libcef.so not found in Debug/ or Release/ under ${_CEF_ROOT}")
  endif()
  message(STATUS "CEF Debug binaries missing; using Release libcef for Debug builds")
  set(CEF_LIB_DEBUG "${CEF_LIB_RELEASE}")
  set(CEF_BINARY_DIR_DEBUG "${CEF_BINARY_DIR_RELEASE}")
endif()

if(DEFINED CEF_BINARY_DIR AND NOT EXISTS "${CEF_BINARY_DIR}/libcef.so")
  if(EXISTS "${CEF_BINARY_DIR_RELEASE}/libcef.so")
    set(CEF_BINARY_DIR "${CEF_BINARY_DIR_RELEASE}")
  endif()
endif()

if(OS_MAC)
	message(FATAL_ERROR "CEFWebSearchMCP CEFConfig currently supports Linux/Windows style libcef linking only.")
endif()

if(NOT TARGET libcef_lib)
	ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

if(NOT TARGET libcef_dll_wrapper)
	add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} ${CMAKE_BINARY_DIR}/cef/libcef_dll_wrapper)
endif()

message(STATUS "CEF root: ${CEFWSMCP_CEF_ROOT}")
message(STATUS "CEF binary dir: ${CEF_BINARY_DIR}")
message(STATUS "CEF resource dir: ${CEF_RESOURCE_DIR}")
