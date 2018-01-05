[![GitHub (pre-)release](https://img.shields.io/github/release/ornladios/adios2/all.svg)]()
[![GitHub license](http://dmlc.github.io/img/apache2.svg)](./LICENSE)


[![Circle CI](https://circleci.com/gh/ornladios/ADIOS2.svg?style=shield)](https://circleci.com/gh/ornladios/ADIOS2)
[![Travis CI](https://travis-ci.org/ornladios/ADIOS2.svg)](https://travis-ci.org/ornladios/ADIOS2)
[![AppVeyor CI](https://ci.appveyor.com/api/projects/status/0s2a3qp57hgbvlhj?svg=true)](https://ci.appveyor.com/project/ornladios/adios2)

# Adaptable Input / Output System (ADIOS) v2.0
This is v2.0 of the ADIOS I/O system, developed as part of the
U.S. Department of Energy Exascale Computing Program.

## License
ADIOS >= 2.0 is licensed under the Apache License v2.0.  See the accompanying
Copyright.txt for more details.

## Directory layout

* cmake - Project specific CMake modules
* examples - ADIOS2 Examples
* scripts - Project maintenance and development scripts
* source - Main ADIOS2 source
* bindings - Additional language bindings (currently only Python is available)
* testing - Tests

## Documentation
Please find ADIOS2 software documentation online under the project GitHub wiki:
[https://github.com/ornladios/ADIOS2/wiki](https://github.com/ornladios/ADIOS2/wiki)

To generate Doxygen application programming interface (API) documentation see 
instructions under: [doc/ReadMe.md](doc/ReadMe.md)  

## Getting Started

ADIOS2 uses CMake for its build environment.  CMake expects projects
to use "out-of-source" builds, which means keeping a separate build and source
directory (different from autotools, which usually uses an in-source build).

To build ADIOS2:

1. Clone the repository:

```bash
$ mkdir adios2
$ cd adios2
$ git clone https://github.com/ornladios/adios2.git source
```

2. Create a separate build directory:

```bash
$ mkdir build
```

3. Configure the project with CMake:

```bash
$ cd build
$ make clean;
$ cmake -DCMAKE_INSTALL_PREFIX=/usr/local/adios2 
  -DADIOS1_DIR=/usr/local/adios1/ 
  -DADIOS1_USE_STATIC_LIBS=ON 
  -DADIOS2_USE_ADIOS1=ON 
  -DADIOS1_LIBRARY=/usr/local/adios1/lib/libadios.a 
  -DADIOS1_LIBRARY_PATH=/usr/local/adios1/lib/libadios.a 
  -DADIOS1_INCLUDE_DIR=/usr/local/adios1/include 
  -DADIOS2_USE_CEPH=ON 
  -DCEPH_INCLUDE_DIRS=/usr/include/rados ../source;
        
-- Could NOT find ZFP (missing: ZFP_LIBRARY ZFP_INCLUDE_DIR)
-- Found MPI_C: /usr/bin/mpicc (found version "3.1")
-- Could NOT find ZeroMQ (missing: ZeroMQ_LIBRARY ZeroMQ_INCLUDE_DIR)
-- Could NOT find PythonModule_numpy (missing: PythonModule_numpy_PATH)
-- Could NOT find PythonModule_mpi4py (missing: PythonModule_mpi4py_PATH)
-- Could NOT find PythonFull (missing: numpy mpi4py)
-- Could NOT find EVPath (missing: EVPath_LIBRARY EVPath_INCLUDE_DIR)
jpl: source.cmake.detectoptions.cmake detected ADIOS2_USE_CEPH ON, setting (ADIOS2_HAVE_CEPH TRUE)

ADIOS2 build configuration:
  ADIOS Version: 2.1.0
  C++ Compiler : GNU 7.2.0
    /usr/bin/c++

  Installation prefix: /usr/local/adios2
        bin: bin
        lib: lib
    include: include
      cmake: lib/cmake/adios2

  Features:
    Library Type: shared
    Build Type:   Debug
    Testing: ON
    Build Options:
      BZip2    : ON
      ZFP      : OFF
      MPI      : ON
      DataMan  : ON
      SST      : OFF
      ZeroMQ   : OFF
      HDF5     : OFF
      ADIOS1   : ON
      Python   : OFF
      Fortran  : OFF
      SysVShMem: ON
      CEPH     : ON

-- Configuring done
-- Generating done
-- Build files have been written to: /src/adios2/build

```

The following options can be specified with CMake's `-DVAR=VALUE` syntax to control which features get enabled or disabled:

| CMake Option         | Values              | Description                                                                      |
| :------------------- | :-------------------------: | :------------------------------------------------------------------------------- |
| `ADIOS2_USE_BZip2`   | **`AUTO`**/``ON``/``OFF`` | Enable [BZip2](http://www.bzip.org/) compression (not implemented).              |
| `ADIOS2_USE_ZFP`     | **`AUTO`**/``ON``/``OFF`` | Enable [ZFP](https://github.com/LLNL/zfp) compression (not implemented).         |
| `ADIOS2_USE_MPI`     | **`AUTO`**/``ON``/``OFF`` | Enable MPI.                                                                      |
| `ADIOS2_USE_DataMan` | **`AUTO`**/``ON``/``OFF`` | Enable the DataMan engine for WAN transports.                                    |
| `ADIOS2_USE_ZeroMQ`  | **`AUTO`**/``ON``/``OFF`` | Enable ZeroMQ for the DataMan engine.                                            |
| `ADIOS2_USE_HDF5`    | **`AUTO`**/``ON``/``OFF`` | Enable the [HDF5](https://www.hdfgroup.org) engine.                              |
| `ADIOS2_USE_ADIOS1`  | **`AUTO`**/``ON``/``OFF`` | Enable the [ADIOS 1.x](https://www.olcf.ornl.gov/center-projects/adios/) engine. |
| `ADIOS2_USE_Python`  | **`AUTO`**/``ON``/``OFF`` | Enable the Python >= 2.7 bindings. |
| `ADIOS2_USE_C`       | **`AUTO`**/``ON``/``OFF`` | Enable the C bindings library libadios2_c.so or libadios2_c.a |
| `ADIOS2_USE_CEPH`    | **`AUTO`**/``ON``/``OFF`` | Enable the [Ceph](http://www.ceph.com) engine. |

Note: The `ADIOS2_USE_HDF5` and `ADIOS2_USE_ADIOS1` options require the use of a matching serial or parallel version depending on whether `ADIOS2_USE_MPI` is enabled.  SImilary, enabling MPI and Python bindings requires the presence of `mpi4py`.

In addition to the `ADIOS2_USE_Feature` options, the following options are also available to control how the library get's built:

| CMake Options              | Values                                                    | Description                                                                           |
| :------------------------- | :-------------------------------------------------------: | :------------------------------------------------------------------------------------ |
| `ADIOS2_BUILD_SHARED_LIBS` | **`ON`**/`OFF`                                            | Build shared libraries.                                                               |
| `ADIOS2_ENABLE_PIC`        | **`ON`**/`OFF`                                            | Enable Position Independent Code for static libraries.                                |
| `ADIOS2_BUILD_EXAMPLES`    | **`ON`**/`OFF`                                            | Build examples.                                                                       |
| `ADIOS2_BUILD_TESTING`     | **`ON`**/`OFF`                                            | Build test code.                                                                      |
| `CMAKE_INSTALL_PREFIX`     | /path/to/install (`/usr/local`)                           | Install location.                                                                     |
| `CMAKE_BUILD_TYPE`         | **`Debug`** / `Release` / `RelWithDebInfo` / `MinSizeRel` | The level of compiler optimization to use.                                            |

4. Compile:

```bash
$ make -j8
```

5. Run tests:

```bash
$ sirius@a7abe61a96db:/src/adios2/build$ ctest --extra-verbose --output-log ctest.log --tests-regex .jeff
UpdateCTestConfiguration  from :/src/adios2/build/DartConfiguration.tcl
Parse Config file:/src/adios2/build/DartConfiguration.tcl
UpdateCTestConfiguration  from :/src/adios2/build/DartConfiguration.tcl
Parse Config file:/src/adios2/build/DartConfiguration.tcl
Test project /src/adios2/build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 43
    Start 43: BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8.jeff

43: Test command: /usr/bin/mpiexec "-n" "2" "/src/adios2/build/bin/TestBPWriteReadADIOS2Scratch" "--gtest_filter=BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8"
43: Test timeout computed to be: 1500
43: Note: Google Test filter = BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8
43: [==========] Running 1 test from 1 test case.
43: [----------] Global test environment set-up.
43: [----------] 1 test from BPWriteReadTestADIOS2Scratch
43: [ RUN      ] BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8
43: Note: Google Test filter = BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8
43: [==========] Running 1 test from 1 test case.
43: [----------] Global test environment set-up.
43: [----------] 1 test from BPWriteReadTestADIOS2Scratch
43: [ RUN      ] BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8
43: [       OK ] BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8 (39 ms)
43: [----------] 1 test from BPWriteReadTestADIOS2Scratch (40 ms total)
43:
43: [----------] Global test environment tear-down
43: [==========] 1 test from 1 test case ran. (44 ms total)
43: [  PASSED  ] 1 test.
43: [       OK ] BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8 (44 ms)
43: [----------] 1 test from BPWriteReadTestADIOS2Scratch (44 ms total)
43:
43: [----------] Global test environment tear-down
43: [==========] 1 test from 1 test case ran. (44 ms total)
43: [  PASSED  ] 1 test.
1/2 Test #43: BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8.jeff ...   Passed    0.39 sec
test 44
    Start 44: BPWriteReadTest.ADIOS2BPWriteADIOS1Read1D8.jeff

44: Test command: /usr/bin/mpiexec "-n" "2" "/src/adios2/build/bin/TestBPWriteReadADIOS2Scratch" "--gtest_filter=BPWriteReadTest.ADIOS2BPWriteADIOS1Read1D8"
44: Test timeout computed to be: 1500
44: Note: Google Test filter = BPWriteReadTest.ADIOS2BPWriteADIOS1Read1D8
44: [==========] Running 0 tests from 0 test cases.
44: Note: Google Test filter = BPWriteReadTest.ADIOS2BPWriteADIOS1Read1D8
44: [==========] Running 0 tests from 0 test cases.
44: [==========] 0 tests from 0 test cases ran. (0 ms total)
44: [  PASSED  ] 0 tests.
44: [==========] 0 tests from 0 test cases ran. (0 ms total)
44: [  PASSED  ] 0 tests.
2/2 Test #44: BPWriteReadTest.ADIOS2BPWriteADIOS1Read1D8.jeff ..........   Passed    0.25 sec

The following tests passed:
	BPWriteReadTestADIOS2Scratch.ADIOS2BPWriteRead1D8.jeff
	BPWriteReadTest.ADIOS2BPWriteADIOS1Read1D8.jeff

100% tests passed, 0 tests failed out of 2

Total Test time (real) =   1.01 sec
sirius@a7abe61a96db:/src/adios2/build$
$
```

6.  Install:
```
$ make install
[  7%] Built target adios2sys_objects
...
[ 61%] Built target adios2
[ 68%] Built target adios2py
...
Install the project...
-- Install configuration: "Debug"
-- Installing: /opt/adios2/2.0.0/gnu/openmpi/include/adios2/ADIOSConfig.h
...
-- Installing: /opt/adios2/2.0.0/gnu/openmpi/bin/adios2-config
...
-- Installing: /opt/adios2/2.0.0/gnu/openmpi/include/adios2.h
...
-- Installing: /opt/adios2/2.0.0/gnu/openmpi/lib/libadios2.so.2.0.0
-- Installing: /opt/adios2/2.0.0/gnu/openmpi/lib/libadios2.so.2
-- Installing: /opt/adios2/2.0.0/gnu/openmpi/lib/libadios2.so
...
$
```
