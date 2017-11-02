/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * ADIOS.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "ADIOS.h"

/// \cond EXCLUDE_FROM_DOXYGEN
#include <fstream>
#include <ios> //std::ios_base::failure
#include <iostream>
#include <sstream>
#include <utility>
/// \endcond

#include "adios2/ADIOSMPI.h"
#include "adios2/helper/adiosFunctions.h" //InquireKey

// compress
#ifdef ADIOS2_HAVE_BZIP2
#include "adios2/operator/compress/CompressBZip2.h"
#endif

#ifdef ADIOS2_HAVE_ZFP
#include "adios2/operator/compress/CompressZfp.h"
#endif

// callback
#include "adios2/operator/callback/Signature1.h"
#include "adios2/operator/callback/Signature2.h"

namespace adios2
{

ADIOS::ADIOS(const std::string configFile, MPI_Comm mpiComm,
             const bool debugMode)
: m_MPIComm(mpiComm), m_ConfigFile(configFile), m_DebugMode(debugMode)
{
    if (m_DebugMode)
    {
        CheckMPI();
    }
    if (!configFile.empty())
    {
        if (configFile.substr(configFile.size() - 3) == "xml")
        {
            InitXML(configFile, m_MPIComm, m_HostLanguage, m_DebugMode,
                    m_Operators, m_IOs);
        }
        // TODO expand for other formats
    }
}

ADIOS::ADIOS(const std::string configFile, const bool debugMode)
: ADIOS(configFile, MPI_COMM_SELF, debugMode)
{
}

ADIOS::ADIOS(MPI_Comm mpiComm, const bool debugMode)
: ADIOS("", mpiComm, debugMode)
{
}

ADIOS::ADIOS(const bool debugMode) : ADIOS("", MPI_COMM_SELF, debugMode) {}

IO &ADIOS::DeclareIO(const std::string name)
{
    if (m_DebugMode && m_IOs.count(name) == 1)
    {
        throw std::invalid_argument("ERROR: IO with name " + name +
                                    " previously declared in config file or "
                                    "with DeclareIO, name must be unique "
                                    " , in call to DeclareIO\n");
    }

    auto ioPair = m_IOs.emplace(
        name, IO(name, m_MPIComm, false, m_HostLanguage, m_DebugMode));
    return ioPair.first->second;
}

IO *ADIOS::InquireIO(const std::string name) noexcept
{
    return InquireKey(name, m_IOs);
}

Operator &ADIOS::DefineOperator(const std::string name, const std::string type,
                                const Params &parameters)
{
    std::shared_ptr<Operator> operatorPtr;

    if (m_DebugMode && m_Operators.count(name) == 1)
    {
        throw std::invalid_argument("ERROR: Operator with name " + name +
                                    ", is already defined in config file "
                                    "or with call to DefineOperator, name must "
                                    "be unique, in call to DefineOperator\n");
    }

    if (type == "bzip2" || type == "BZip2")
    {
#ifdef ADIOS2_HAVE_BZIP2
        auto itPair = m_Operators.emplace(
            name, std::make_shared<adios2::compress::CompressBZip2>(
                      parameters, m_DebugMode));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(
            "ERROR: this version of ADIOS2 didn't compile with the "
            "bzip2 library, in call to DefineOperator\n");
#endif
    }
    else if (type == "zfp" || type == "Zfp")
    {
#ifdef ADIOS2_HAVE_ZFP
        auto itPair = m_Operators.emplace(
            name, std::make_shared<adios2::compress::CompressZfp>(parameters,
                                                                  m_DebugMode));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(
            "ERROR: this version of ADIOS2 didn't compile with the "
            "zfp library (minimum v1.5), in call to DefineOperator\n");
#endif
    }
    else
    {
        if (m_DebugMode)
        {
            throw std::invalid_argument(
                "ERROR: Operator " + name + " of type " + type +
                " is not supported by ADIOS2, in call to DefineOperator\n");
        }
    }

    if (m_DebugMode && !operatorPtr)
    {
        throw std::invalid_argument(
            "ERROR: Operator " + name + " of type " + type +
            " couldn't be defined, in call to DefineOperator\n");
    }

    return *operatorPtr.get();
}

Operator *ADIOS::InquireOperator(const std::string name) noexcept
{
    return InquireKey(name, m_Operators)->get();
}

// PRIVATE FUNCTIONS
void ADIOS::CheckMPI() const
{
    if (m_MPIComm == MPI_COMM_NULL)
    {
        throw std::ios_base::failure("ERROR: MPI communicator is MPI_COMM_NULL,"
                                     " in call to ADIOS constructor\n");
    }
}

#define declare_type(T)                                                        \
    Operator &ADIOS::DefineCallBack(                                           \
        const std::string name,                                                \
        const std::function<void(const T *, const std::string,                 \
                                 const std::string, const std::string,         \
                                 const Dims &)> &function,                     \
        const Params &parameters)                                              \
    {                                                                          \
        std::shared_ptr<Operator> callbackOperator =                           \
            std::make_shared<callback::Signature1<T>>(function, parameters,    \
                                                      m_DebugMode);            \
                                                                               \
        auto itPair = m_Operators.emplace(name, std::move(callbackOperator));  \
        return *itPair.first->second;                                          \
    }

ADIOS2_FOREACH_TYPE_1ARG(declare_type)
#undef declare_type

Operator &ADIOS::DefineCallBack(
    const std::string name,
    const std::function<void(void *, const std::string, const std::string,
                             const std::string, const Dims &)> &function,
    const Params &parameters)
{
    std::shared_ptr<Operator> callbackOperator =
        std::make_shared<callback::Signature2>(function, parameters,
                                               m_DebugMode);

    auto itPair = m_Operators.emplace(name, std::move(callbackOperator));
    return *itPair.first->second;
}

} // end namespace adios2
