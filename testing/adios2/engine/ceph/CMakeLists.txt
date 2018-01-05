# jpl
# added for cephfs testing

add_executable(TestCephFSWriteReadADIOS2 TestCephFSWriteReadADIOS2.cpp) # jeff 
target_link_libraries(TestCephFSWriteReadADIOS2 adios2 gtest gtest_main) # jeff

#target_include_directories(TestCephFSWriteRead PRIVATE ${CEPH_INCLUDE_DIRS}) # from hdf5 tests cmake.
#target_link_libraries(TestCephFSWriteRead adios2 gtest ${CEPH_LIBRARIES})

if(ADIOS2_HAVE_MPI)
    target_link_libraries(TestCephFSWriteReadADIOS2 MPI::MPI_C) # jeff
    set(extra_test_args EXEC_WRAPPER ${MPIEXEC_COMMAND})
endif()

gtest_add_tests(TARGET TestCephFSWriteReadADIOS2 TEST_SUFFIX ".cephfs" ${extra_test_args}) 


