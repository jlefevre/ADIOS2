
echo "a short script to cmake, make, ctest adios2 jeff dev files."
echo "..."
sleep 1
#/usr/lib/x86_64-linux-gnu/librados.so
if ! [ -z "$1" ]; then
	if [ $1 = "clean" ]; then 
		make clean;
		cmake -DCMAKE_INSTALL_PREFIX=/usr/local/adios2 -DADIOS1_DIR=/usr/local/adios1/ -DADIOS1_USE_STATIC_LIBS=ON -DADIOS2_USE_ADIOS1=ON -DADIOS1_LIBRARY=/usr/local/adios1/lib/libadios.a -DADIOS1_LIBRARY_PATH=/usr/local/adios1/lib/libadios.a -DADIOS1_INCLUDE_DIR=/usr/local/adios1/include -DADIOS2_USE_CEPH=ON -DCEPH_INCLUDE_DIRS=/usr/include/rados -DCEPH_LIBRARIES=/usr/lib/x86_64-linux-gnu/librados.so ../source;
	else 
		echo "skipping make clean & cmake"; 
	fi
fi
echo "running make..."
sleep 1
make -j6
echo "running ctest.."
sleep 1
#ctest --extra-verbose --output-log ctest.log --show-only --tests-regex .jeff
ctest --extra-verbose --output-log ctest.log --tests-regex .jeff
