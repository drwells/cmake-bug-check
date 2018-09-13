ENABLE_TESTING()
MESSAGE(STATUS "Setting up quick_tests in \"DEBUG\" mode")
SET(ALL_TESTS)

MACRO(make_quicktest test_basename)
  SET(_target ${test_basename}.quicktest)
  LIST(APPEND ALL_TESTS "${_target}")
  ADD_EXECUTABLE(${_target} EXCLUDE_FROM_ALL ${test_basename}.cc)
  SET_TARGET_PROPERTIES(${_target} PROPERTIES
    LINKER_LANGUAGE "CXX"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  TARGET_LINK_LIBRARIES(${_target} checkbug)

  ADD_CUSTOM_TARGET(${_target}.run
    DEPENDS ${_target}
    COMMAND
      ./${_target} > ${_target}-OK 2>&1
      ||(echo "${_target}: RUN failed."
         && exit 1)
    COMMAND echo "${_target}: PASSED."
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  ADD_TEST(NAME ${_target}
    COMMAND ${CMAKE_COMMAND}
    --build . --target ${_target}.run
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
ENDMACRO()

make_quicktest("step-1")
make_quicktest("step-2")
make_quicktest("step-3")

# A custom test target:
ADD_CUSTOM_TARGET(test
  COMMAND ${CMAKE_COMMAND} -D ALL_TESTS="${ALL_TESTS}" -P ${CMAKE_CURRENT_SOURCE_DIR}/run.cmake
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Running quicktests..."
  )

MESSAGE(STATUS "Setting up quick_tests in \"DEBUG\" mode - Done")
