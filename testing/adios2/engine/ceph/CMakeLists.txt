# jpl
# added for cephfs testing

add_executable(TestCephWriteReadADIOS2 TestCephWriteReadADIOS2.cpp) # jeff 
target_link_libraries(TestCephWriteReadADIOS2 adios2 gtest gtest_main) # jeff

#target_include_directories(TestCephWriteRead PRIVATE ${CEPH_INCLUDE_DIRS}) # from ceph tests cmake.
#target_link_libraries(TestCephWriteRead adios2 gtest ${CEPH_LIBRARIES})

if(ADIOS2_HAVE_MPI)
    target_link_libraries(TestCephWriteReadADIOS2 MPI::MPI_C) # jeff
    set(extra_test_args EXEC_WRAPPER ${MPIEXEC_COMMAND})
endif()

gtest_add_tests(TARGET TestCephWriteReadADIOS2 TEST_SUFFIX ".ceph" ${extra_test_args}) 


