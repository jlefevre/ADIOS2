/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * CephCommon.cpp
 *
 *  Created on: April 20, 2017
 *      Author: Junmin
 */

#include "CephCommon.h"
#include "CephCommon.tcc"

#include <complex>
#include <ios>
#include <iostream>
#include <stdexcept>

#include "adios2/ADIOSMPI.h"

namespace adios2
{
namespace interop
{

CephCommon::CephCommon(const bool debugMode) : m_DebugMode(debugMode)
{
}

void CephCommon::Init(const std::string &name, MPI_Comm comm, bool toWrite)
{
    m_WriteMode = toWrite;
    std::string ts0;
    StaticGetTimeStepString(ts0, 0);

    m_cephid = -1;
    
    if (toWrite)
    {
        /*
         * Create a new file collectively and release property list identifier.
         */
        m_cephid = 0;
        if (m_DebugMode) { std::cout << "CephCommon Init(): in toWrite mode, fid=" << m_cephid << std::endl;}
        // create/open file for writing here.
    }
    else
    {
        // read a file collectively
        m_cephid = 1;
        if (m_DebugMode) { std::cout << "CephCommon Init(): in READ mode, fid=" << m_cephid << std::endl;}
        if (m_cephid >= 0)
        {
            // open file here for read.
        }
    }
}

void CephFSCommon::WriteTimeSteps()
{
    uint totalTimeSteps = m_CurrentTimeStep + 1;
    if (m_DebugMode) { std::cout << "CephFSCommon WriteTimeSteps: totalTimeSteps=" << totalTimeSteps << std::endl;}
}

unsigned int CephCommon::GetNumTimeSteps()
{
    if (m_DebugMode) { std::cout << "CephFSCommon GetNumTimeSteps: m_NumTimeSteps=" << m_NumTimeSteps << std::endl;}
    if (m_NumTimeSteps <= 0)
    {
        // initial file open to write a ts to file.
    }
    return m_NumTimeSteps;
}

// read from all time steps
void CephFSCommon::ReadAllVariables(IO &io)
{
    int i = 0;
    std::string timestepStr;
    cephsz_t nobjs;
    for (i = 0; i < m_NumTimeSteps; i++)
    {
        nobjs =  10;  // compute all of the oids needed to read from Ceph.
        if (m_DebugMode) { std::cout << "CephFSCommon ReadAllVariables: timestep=" << i << "nobjs="<< nobjs << std::endl;}
        // ReadVariables(i, io);
    }
}

// read variables from the input timestep
void CephFSCommon::ReadVariables(unsigned int ts, IO &io)
{
    std::string timestepStr;
    StaticGetTimeStepString(timestepStr, ts);
    if (m_DebugMode) { std::cout << "CephFSCommon ReadVariables: timestepStr=" << timestepStr << std::endl;}
}

template <class T>
void CephCommon::AddVar(IO &io, std::string const &name, m_cephid datasetId)
{
    if (m_DebugMode) {std::cout << "CephCommon AddVar(): name=" << name << std::endl;}
}

void CephCommon::CreateVar(IO &io, m_cephid datasetId, std::string const &name)
{
    if (m_DebugMode) {std::cout << "CephCommon CreateVar(): name=" << name << std::endl;}
}

void CephCommon::Close()
{
    if (m_DebugMode) {std::cout << "CephCommon Close(): m_cephid=" << m_cephid << std::endl;}

    if (m_cephid < 0)
    {
        return;
    }

    WriteTimeSteps();
    m_cephid = -1;
}

void CephCommon::SetTimeStep(int timeStep)
{
    if (m_DebugMode) {std::cout << "CephCommon SetTimeStep(): timeStep=" << timeStep << std::endl;}

    if (m_WriteMode)
        throw std::ios_base::failure(
            "ERROR: unable to change timestep at Write MODE.");

    if (timeStep < 0)
        throw std::ios_base::failure(
            "ERROR: unable to change to negative timestep.");

    GetNumTimeSteps();

    if (timeStep >= m_NumTimeSteps)
        throw std::ios_base::failure(
            "ERROR: given time step is more than actual known steps.");

    if (m_CurrentTimeStep == timeStep)
    {
        return;
    }

    std::string timeStepName;
    StaticGetTimeStepString(timeStepName, timeStep);
    m_CurrentTimeStep = timeStep;
    if (m_DebugMode) {std::cout << "CephCommon SetTimeStep(): set m_CurrentTimeStep=" << m_CurrentTimeStep << std::endl;}

}

void CephCommon::Advance()
{
    if (m_DebugMode){ std::cout << "CephCommon Advance(): BEGIN. m_CurrentTimeStep=" << m_CurrentTimeStep << std::endl;}

    if (m_WriteMode)
    {
    
    }
    else
    {
        if (m_NumTimeSteps == 0)
        {
            GetNumTimeSteps();
        }
        if (m_CurrentTimeStep + 1 >= m_NumTimeSteps)
        {
            if (m_DebugMode) {std::cout << "CephCommon Advance(): RETURNING since m_CurrentTimeStep+1=" << m_CurrentTimeStep << "AND m_NumTimeSteps=" << m_NumTimeSteps << std::endl;}
            return;
        }

        std::string timeStepName;
        StaticGetTimeStepString(timeStepName, m_CurrentTimeStep + 1);

    }
    ++m_CurrentTimeStep;
    if (m_DebugMode) {std::cout << "CephCommon Advance(): END. m_CurrentTimeStep=" << m_CurrentTimeStep <<std::endl;}

}

void CephCommon::StaticGetTimeStepString(std::string &timeStepName, int ts)
{
    timeStepName = "/TimeStep" + std::to_string(ts);
    if (m_DebugMode) {std::cout << "CephCommon StaticGetTimeStepString()=" << timeStepName <<std::endl;}
}

#define declare_template_instantiation(T)                                      \
    template void CephCommon::Write(Variable<T> &variable, const T *value);

ADIOS2_FOREACH_TYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

} // end namespace interop
} // end namespace adios
