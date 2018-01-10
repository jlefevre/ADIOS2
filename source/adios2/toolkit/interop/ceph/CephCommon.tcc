/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * HDF5Common.tcc
 *
 *  Created on: Jun 1, 2017
 *      Author: Junmin
 */

#ifndef ADIOS2_TOOLKIT_INTEROP_CEPH_CEPHCOMMON_TCC_
#define ADIOS2_TOOLKIT_INTEROP_CEPH_CEPHCOMMON_TCC_

#include "CephCommon.h"
#include <iostream>
#include <vector>

namespace adios2
{
namespace interop
{

template <class T>
void CephCommon::Write(Variable<T> &variable, const T *values)
{
    int dimSize = std::max(variable.m_Shape.size(), variable.m_Count.size());
    cephtype_t cephType = GetCephType<T>();

    if (dimSize == 0)
    {
        // write scalar
        return;
    }

    //~ std::vector<cephsz_t> dimsf, count, offset;
    //~ for (int i = 0; i < dimSize; ++i)
    //~ {
        //~ if (variable.m_Shape.size() == dimSize)
        //~ {
            //~ dimsf.push_back(variable.m_Shape[i]);
        //~ }
        //~ else
        //~ {
            //~ dimsf.push_back(variable.m_Count[i]);
        //~ }
    //~ }

    cephstorage_t EMPRESSFUNC = E_CEPH_OBJ;
    cephid_t objset = EMPRESSFUNC;
    cephid_t dsetID = objset; // H5Dcreate(m_GroupId, variable.m_Name.c_str(), h5Type, fileSpace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //~ //  Create property list for collective dataset write.
    //~ hid_t plistID = H5Pcreate(H5P_DATASET_XFER);
//~ #ifdef ADIOS2_HAVE_MPI
    //~ H5Pset_dxpl_mpio(plistID, H5FD_MPIO_COLLECTIVE);
//~ #endif
    //~ herr_t status;
    //~ status = H5Dwrite(dsetID, h5Type, memSpace, fileSpace, plistID, values);


}



template <>
cephtype_t CephCommon::GetCephType<std::string>()
{
    return CEPH_STRING;
}
template <>
cephtype_t CephCommon::GetCephType<int>()
{
    return CEPH_INT;
}
template <>
cephtype_t CephCommon::GetCephType<float>()
{
    return CEPH_STRING;
}
template <>
cephtype_t CephCommon::GetCephType<double>()
{
    return CEPH_DOUBLE;
}

} // end namespace interop
} // end namespace adios

#endif /* ADIOS2_TOOLKIT_INTEROP_CEPH_CEPHCOMMON_TCC_ */
