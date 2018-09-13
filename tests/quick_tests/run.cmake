include(ProcessorCount)
PROCESSORCOUNT(_n_processors)
IF(_n_processors EQUAL 0)
  SET(_n_processors "1")
ENDIF()

SEPARATE_ARGUMENTS(ALL_TESTS)

EXECUTE_PROCESS(COMMAND ${CMAKE_CTEST_COMMAND} -j${_n_processors}
  --force-new-ctest-process
  --output-on-failure
  -O quicktests.log
  RESULT_VARIABLE res_var)

IF(NOT "${res_var}" STREQUAL "0")
  MESSAGE("
***************************************************************************
**                 Error: Some of the quick tests failed.                **
***************************************************************************
")
  # set the error code to a nonzero value
  MESSAGE(SEND_ERROR "")
ENDIF()
