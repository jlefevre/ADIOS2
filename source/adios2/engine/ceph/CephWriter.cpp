/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * CephWriter.cpp
 *
 *  Created on: Dec 19, 2017
 *      Author: jpl
 */

#include "CephWriter.h"
#include "CephWriter.tcc"

#include <iostream>

#include "adios2/ADIOSMPI.h"
#include "adios2/ADIOSMacros.h"
#include "adios2/core/IO.h"
#include "adios2/helper/adiosFunctions.h" //CheckIndexRange
#include "adios2/toolkit/transport/file/FileFStream.h"

// jpl

#include "adios2/engine/bp/BPFileWriter.h"
#include "adios2/engine/bp/BPFileWriter.tcc"


namespace adios2
{

CephWriter::CephWriter(IO &io, const std::string &name, const Mode mode,
                           MPI_Comm mpiComm)
: Engine("CephWriter", io, name, mode, mpiComm)
{
    m_EndMessage = " in call to IO Open CephWriter " + m_Name + "\n";
    Init();
}

StepStatus CephWriter::BeginStep(StepMode mode, const float timeoutSeconds)
{
    return StepStatus::OK;
}

void CephWriter::EndStep() 
{
  // should flush the buffer, check the obj size, and keep going or write obj and get new oid.
}


void CephWriter::Close(const int transportIndex) 
{ 
    // write any remaining then close and write metadata to empress if needed. 
    //CephWriterClose(m_Output); 
    //WriteCollectiveMetadataFile();
}


// PRIVATE FUNCTIONS
// PRIVATE
void CephWriter::Init()
{
    InitParameters();
    InitObjector();
}


void CephWriter::InitParameters()
{
    // m_IO.m_Parameters;
    // register empress metadata here
}

void CephWriter::InitObjector()
{
    // get function pointer from empress using params.
}


#define declare_type(T)                                                        \
    void CephWriter::DoPutSync(Variable<T> &variable, const T *values)          \
    {                                                                          \
        PutSyncCommon(variable, values);                                       \
    }
ADIOS2_FOREACH_TYPE_1ARG(declare_type)
#undef declare_type



} // end namespace adios2
