/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * CephWriter.h
 *
 *  Created on: Dec 16, 2017
 *      Author: jpl
 */

#ifndef ADIOS2_ENGINE_CEPH_CEPHWRITER_H_
#define ADIOS2_ENGINE_CEPH_CEPHWRITER_H_

#include "adios2/ADIOSConfig.h"
#include "adios2/core/Engine.h"
#include "adios2/toolkit/format/bp3/BP3.h"
#include "adios2/toolkit/transportman/TransportMan.h" //transport::TransportsMan
#include "adios2/toolkit/interop/ceph/CephCommon.h"




namespace adios2
{

const unsigned int CEPH_CONF_OBJ_SZ = 10;  // MB
typedef struct _ObjStream *ObjStream;


class CephWriter : public Engine
{

public:
    /**
     * Constructor for Writer for Ceph object (librados) interface
     * @param name unique name given to the engine
     * @param openMode w (supported), r, a from OpenMode in ADIOSTypes.h
     * @param mpiComm MPI communicator
     */
    CephWriter(IO &io, const std::string &name, const Mode mode,
                 MPI_Comm mpiComm);


    virtual ~CephWriter() = default;

    StepStatus BeginStep(StepMode mode, const float timeoutSeconds = 0.f) final;
    void EndStep() final;

    void Close(const int transportIndex = -1) final;

private:
    void Init(); ///< calls InitCapsules and InitTransports based on Method,
                 /// called from constructor
    void InitParameters();  
    void InitObjector();  

#define declare_type(T)                                                        \
    void DoPutSync(Variable<T> &variable, const T *values) final;
    ADIOS2_FOREACH_TYPE_1ARG(declare_type)
#undef declare_type

    template <class T>
    void PutSyncCommon(Variable<T> &variable, const T *values);

    ObjStream m_Output;
};

} // end namespace adios2

#endif /* ADIOS2_ENGINE_CEPH_CEPHWRITER_H_ */
