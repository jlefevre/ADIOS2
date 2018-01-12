/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * CephWriter.tcc implementation of template functions with known type
 *
 *  Created on: Dec 17, 2017
 *      Author: jpl
 */
#ifndef ADIOS2_ENGINE_CEPH_CEPHWRITER_TCC_
#define ADIOS2_ENGINE_CEPH_CEPHWRITER_TCC_

#include "CephWriter.h"
#include <iostream>

namespace adios2
{


template <class T>
void CephWriter::PutSyncCommon(Variable<T> &variable, const T *values)
{
    // set variable
    variable.SetData(values);

    // if first timestep Write create new oid via objector.


    const size_t dataSize = variable.PayloadSize();

    if (dataSize >= CEPH_CONF_OBJ_SZ)
    {
        // write out ceph object and  generate new objector value here
        //~ m_BP3Serializer.SerializeData(m_IO);
        //~ m_FileDataManager.WriteFiles(m_BP3Serializer.m_Data.m_Buffer.data(),
                                     //~ m_BP3Serializer.m_Data.m_Position);
        //~ m_BP3Serializer.ResetBuffer(m_BP3Serializer.m_Data);
        //~ // new group index for incoming variable
    }

    // ELSE : keep filling mem buffer until desired object size
    //~ // WRITE INDEX to data buffer and metadata structure (in memory)
    //~ m_BP3Serializer.PutVariableMetadata(variable);
    //~ m_BP3Serializer.PutVariablePayload(variable);

    if (variable.m_Count.empty())
    {
        variable.m_Count = variable.m_Shape;
    }
    if (variable.m_Start.empty())
    {
        variable.m_Start.assign(variable.m_Count.size(), 0);
    }

    std::cout << "I am hooked to the Ceph library\n";
    std::cout << "Variable " << variable.m_Name << "\n";
    std::cout << "putshape " << variable.m_Count.size() << "\n";
    std::cout << "varshape " << variable.m_Shape.size() << "\n";
    std::cout << "offset " << variable.m_Start.size() << "\n";
}


} // end namespace adios2

#endif /* ADIOS2_ENGINE_CEPH_CEPHWRITER_TCC_ */
