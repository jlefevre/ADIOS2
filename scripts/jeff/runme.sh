HDF5_ROOT=/usr/local/hdf5/HDF_Group/HDF5/1.8.20; export HDF5_ROOT;

ADIOS1_ROOT=/usr/local/adios1; export ADIOS1_ROOT;

echo "a short script to cmake, make, ctest adios2 jeff dev files."
echo "add DADIOS2_BUILD_EXAMPLES=ON to get examples programs:"
echo "https://github.com/ornladios/ADIOS2/wiki/Hello-ADIOS2-Write-Example"
echo "..."
#/usr/lib/x86_64-linux-gnu/librados.so
if ! [ -z "$1" ]; then
	if [ $1 = "clean" ]; then
		make clean;
		cmake -DCMAKE_INSTALL_PREFIX=/usr/local/adios2 -DADIOS1_DIR=/usr/local/adios1/ -DADIOS1_USE_STATIC_LIBS=ON -DADIOS2_USE_ADIOS1=ON -DADIOS1_LIBRARY=/usr/local/adios1/lib/libadios.a -DADIOS1_LIBRARY_PATH=/usr/local/adios1/lib/libadios.a -DADIOS1_INCLUDE_DIR=/usr/local/adios1/include -DADIOS2_USE_CEPH=ON -DCEPH_INCLUDE_DIRS=/usr/include/rados/ -DCEPH_LIBRARIES=/usr/lib/x86_64-linux-gnu/librados.so ../source;
	else
		echo "skipping make clean & cmake";
	fi
fi
echo "running make.git add 