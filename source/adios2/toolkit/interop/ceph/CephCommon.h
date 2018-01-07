/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * CephFSCommon.h
 *
 *  Created on: 
 *      Author: 
 */

#ifndef ADIOS2_TOOLKIT_INTEROP_CEPH_CEPHCOMMON_H_
#define ADIOS2_TOOLKIT_INTEROP_CEPH_CEPHCOMMON_H_

//#include <hdf5.h>

#include <string>

#include "adios2/ADIOSMPICommOnly.h"
#include "adios2/ADIOSMacros.h"
#include "adios2/ADIOSTypes.h"
#include "adios2/core/IO.h" // for CreateVar
#include "adios2/core/Variable.h"

#include <stdexcept> // for Intel Compiler

namespace adios2
{
namespace interop
{

typedef enum {
    E_CEPH_FILE = 0,
    E_CEPH_OBJ = 1,
} ADIOS_ENUM_CEPH_STORAGE;

typdef ADIOS_ENUM_CEPH_STORAGE cephstorage_t;

typdef unsigned int cephid_t;
typdef unsigned int cephsz_t;
typdef unsigned int cephtype_t;

// for testing only.
const cephtype_t CEPH_STRING = 0,
const cephtype_t CEPH_INT = 1,
const cephtype_t CEPH_FLOAT = 2,
const cephtype_t CEPH_DOUBLE = 3,

class CephFSCommon
{

public:
    /**
     * Unique constructor for Ceph objs
     * @param debugMode true: extra exception checks
     */
    CephFSCommon(const bool debugMode);

    void Init(const std::string &name, MPI_Comm comm, bool toWrite);

    template <class T>
    void Write(Variable<T> &variable, const T *values);

    void Close();
    void Advance();

    void SetTimeStep(int ts);

    unsigned int GetNumTimeSteps();
    void WriteTimeSteps();
    void CreateVar(IO &io, cephtype_t cephType, std::string const &name);

    template <class T>
    void AddVar(IO &io, std::string const &name, cephid_t datasetId);

    static void StaticGetTimeStepString(std::string &timeStepName, int ts);

    cephid_t m_cephid = -1;
    cephid_t m_FileID = -1;
    cephid_t m_PoolID = -1;

    unsigned int m_CurrentTimeStep = 0;

    template <class T>
    cephtype_t GetCephType(); // should this be public?

    template <class T>
    T GetADIOSType(cephtype_t);

private:
    const bool m_DebugMode;
    bool m_WriteMode = false;
    unsigned int m_NumTimeSteps = 0;
};

// Explicit declaration of the public template methods
#define declare_template_instantiation(T)                                      \
    extern template void CephFSCommon::Write(Variable<T> &variable,              \
                                           const T *value);

ADIOS2_FOREACH_TYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

} // end namespace interop
} // end namespace adios

#endif /* ADIOS2_TOOLKIT_INTEROP_CEPH_CEPHCOMMON_H_ */
