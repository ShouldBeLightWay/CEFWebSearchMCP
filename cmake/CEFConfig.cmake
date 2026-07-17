set(CEF_USE_SANDBOX OFF CACHE BOOL "Disable CEF sandbox for initial Linux integration" FORCE)

find_package(CEF REQUIRED)

if(OS_MAC)
	message(FATAL_ERROR "CEFWebSearchMCP CEFConfig currently supports Linux/Windows style libcef linking only.")
endif()

if(NOT TARGET libcef_lib)
	ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

if(NOT TARGET libcef_dll_wrapper)
	add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} ${CMAKE_BINARY_DIR}/cef/libcef_dll_wrapper)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(libcef_dll_wrapper PRIVATE
		-Wno-gnu-zero-variadic-macro-arguments
		-Wno-error=gnu-zero-variadic-macro-arguments
	)
endif()

message(STATUS "CEF root: ${CEF_ROOT}")
message(STATUS "CEF binary dir: ${CEF_BINARY_DIR}")
message(STATUS "CEF resource dir: ${CEF_RESOURCE_DIR}")
