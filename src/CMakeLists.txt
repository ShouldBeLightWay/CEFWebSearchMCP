add_library(cefwebsearchmcp_core
  core/application.cpp
  search/search_orchestrator.cpp
  cef/session_manager.cpp
)

target_include_directories(cefwebsearchmcp_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if(CEFWSMCP_ENABLE_CEF)
  target_include_directories(cefwebsearchmcp_core
    PRIVATE
      ${CEFWSMCP_CEF_ROOT}
  )
endif()

target_compile_features(cefwebsearchmcp_core PUBLIC cxx_std_20)

add_executable(cefwebsearchmcp_server
  main.cpp
)

if(CEFWSMCP_ENABLE_CEF)
  SET_EXECUTABLE_TARGET_PROPERTIES(cefwebsearchmcp_server)
  add_dependencies(cefwebsearchmcp_server libcef_dll_wrapper)

  if(OS_LINUX)
    set_target_properties(cefwebsearchmcp_server PROPERTIES
      INSTALL_RPATH "$ORIGIN"
      BUILD_WITH_INSTALL_RPATH TRUE
    )

    set(_CEFWSMCP_RUNTIME_DIR "$<TARGET_FILE_DIR:cefwebsearchmcp_server>")
    COPY_FILES("cefwebsearchmcp_server" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${_CEFWSMCP_RUNTIME_DIR}")
    COPY_FILES("cefwebsearchmcp_server" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${_CEFWSMCP_RUNTIME_DIR}")

    if(EXISTS "${CEF_BINARY_DIR}/libminigbm.so")
      COPY_FILES("cefwebsearchmcp_server" "libminigbm.so" "${CEF_BINARY_DIR}" "${_CEFWSMCP_RUNTIME_DIR}")
    endif()

    if(EXISTS "${CEF_BINARY_DIR}/chrome-sandbox")
      SET_LINUX_SUID_PERMISSIONS("cefwebsearchmcp_server" "${_CEFWSMCP_RUNTIME_DIR}/chrome-sandbox")
    endif()
  endif()
endif()

target_link_libraries(cefwebsearchmcp_server
  PRIVATE
    cefwebsearchmcp_core
)

if(CEFWSMCP_ENABLE_CEF)
  target_link_libraries(cefwebsearchmcp_server
    PRIVATE
      libcef_lib
      libcef_dll_wrapper
      ${CEF_STANDARD_LIBS}
  )
endif()

install(TARGETS cefwebsearchmcp_server
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
