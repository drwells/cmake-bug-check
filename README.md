# checkbug
A repository showing a minimal working example of the issue
in [deal.II issue 6589](https://github.com/dealii/dealii/issues/6589): we run a
small number of tests to verify that everything has been compiled and linked
correctly, but we get errors in `make` when we run
```
make -j2 test
```

The tests all fail with the following sort of error message:
```
[drwells@build]$ make -j2 test
[100%] Running quicktests...
Test project /home/drwells/Documents/Code/CPP/checkbug/build/tests/quick_tests
    Start 1: step-1.quicktest
    Start 2: step-2.quicktest
    Start 3: step-3.quicktest
1/3 Test #3: step-3.quicktest .................***Failed    0.07 sec
make[6]: *** read jobs pipe: Bad file descriptor.  Stop.
make[6]: *** Waiting for unfinished jobs....
[ 20%] Built target obj_slow
make[5]: *** [CMakeFiles/Makefile2:357: tests/quick_tests/CMakeFiles/step-3.quicktest.run.dir/rule] Error 2
make[4]: *** [Makefile:145: tests/quick_tests/CMakeFiles/step-3.quicktest.run.dir/rule] Error 2

2/3 Test #1: step-1.quicktest .................***Failed    0.08 sec
make[6]: *** read jobs pipe: Bad file descriptor.  Stop.
make[6]: *** Waiting for unfinished jobs....
[ 20%] Built target obj_slow
make[5]: *** [CMakeFiles/Makefile2:293: tests/quick_tests/CMakeFiles/step-1.quicktest.run.dir/rule] Error 2
make[4]: *** [Makefile:115: tests/quick_tests/CMakeFiles/step-1.quicktest.run.dir/rule] Error 2

3/3 Test #2: step-2.quicktest .................***Failed    0.08 sec
make[6]: *** read jobs pipe: Bad file descriptor.  Stop.
make[6]: *** Waiting for unfinished jobs....
[ 20%] Built target obj_slow
make[5]: *** [CMakeFiles/Makefile2:325: tests/quick_tests/CMakeFiles/step-2.quicktest.run.dir/rule] Error 2
make[4]: *** [Makefile:130: tests/quick_tests/CMakeFiles/step-2.quicktest.run.dir/rule] Error 2


0% tests passed, 3 tests failed out of 3

Total Test time (real) =   0.08 sec

The following tests FAILED:
          1 - step-1.quicktest (Failed)
          2 - step-2.quicktest (Failed)
          3 - step-3.quicktest (Failed)
Errors while running CTest

***************************************************************************
**                 Error: Some of the quick tests failed.                **
***************************************************************************

CMake Error at /home/drwells/Documents/Code/CPP/checkbug/tests/quick_tests/run.cmake:22 (MESSAGE):


make[3]: *** [tests/quick_tests/CMakeFiles/test.dir/build.make:58: tests/quick_tests/CMakeFiles/test] Error 1
make[2]: *** [CMakeFiles/Makefile2:414: tests/quick_tests/CMakeFiles/test.dir/all] Error 2
make[1]: *** [CMakeFiles/Makefile2:421: tests/quick_tests/CMakeFiles/test.dir/rule] Error 2
make: *** [Makefile:209: test] Error 2
```
which indicates that the tests are not set up as make targets that use (in
automake parlance) `$(MAKEFLAGS)` to propagate flags recursively. `make -j1`
succeeds with the following output:

```
[drwells@build]$ make -j1 test
[100%] Running quicktests...
Test project /home/drwells/Documents/Code/CPP/checkbug/build/tests/quick_tests
    Start 1: step-1.quicktest
    Start 2: step-2.quicktest
    Start 3: step-3.quicktest
1/3 Test #1: step-1.quicktest .................   Passed    2.17 sec
2/3 Test #2: step-2.quicktest .................   Passed    2.17 sec
3/3 Test #3: step-3.quicktest .................   Passed    2.17 sec

100% tests passed, 0 tests failed out of 3

Total Test time (real) =   2.18 sec
Built target test
```

The `test` target is setup as a custom target which runs a CMake script which
runs `ctest` on a small number of tests.

This is probably a bug in CMake since it is only evident when we link against a
library that is compiled from multiple component object files. Here are two
checks that show how this happens:

1. If we comment out the call to `TARGET_LINK_LIBRARIES` in
`tests/quick_tests/CMakeLists.txt`, i.e.,
```cmake
  ADD_EXECUTABLE(${_target} EXCLUDE_FROM_ALL ${test_basename}.cc)
  SET_TARGET_PROPERTIES(${_target} PROPERTIES
    LINKER_LANGUAGE "CXX"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  # TARGET_LINK_LIBRARIES(${_target} checkbug)
```

then `make -j2 test` works.

2. If comment out one of the component subdirectories, i.e., convert
`source/CMakeLists.txt` to
```cmake
ADD_SUBDIRECTORY(quick)
# ADD_SUBDIRECTORY(slow)

GET_PROPERTY(_objects GLOBAL PROPERTY CHECKBUG_OBJECTS)

ADD_LIBRARY(checkbug ${_objects})
```
then `make -j2 test` works.
