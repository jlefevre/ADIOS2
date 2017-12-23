/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * DataMan.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: Jason Wang wangr1@ornl.gov
 */

#include <fstream> //TODO go away

#include "DataMan.h"

#include "adios2/ADIOSMacros.h"
#include "adios2/helper/adiosFunctions.h"

#ifdef ADIOS2_HAVE_ZEROMQ
#include "adios2/toolkit/transport/wan/WANZmq.h"
#endif

namespace adios2
{
namespace transportman
{

DataMan::DataMan(MPI_Comm mpiComm, const bool debugMode)
: TransportMan(mpiComm, debugMode)
{
}

DataMan::~DataMan()
{
    for (auto &controlThread : m_ControlThreads)
    {
        m_Listening = false;
        controlThread.join();
    }
}

void DataMan::OpenWANTransports(const std::vector<std::string> &streamNames,
                                const Mode mode,
                                const std::vector<Params> &paramsVector,
                                const bool profile)
{
#ifdef ADIOS2_HAVE_ZEROMQ
    size_t counter = 0; // remove MACRO when more libraries are added
#endif

    if (streamNames.size() == 0)
    {
        throw("No streams to open from DataMan::OpenWANTransports");
    }

    for (size_t i = 0; i < streamNames.size(); ++i)
    {

        std::shared_ptr<Transport> wanTransport, controlTransport;

        const std::string library(GetParameter("Library", paramsVector[i], true,
                                               m_DebugMode,
                                               "Transport Library Parameter"));

        const std::string ipAddress(
            GetParameter("IPAddress", paramsVector[i], true, m_DebugMode,
                         "Transport IPAddress Parameter"));

        std::string portControl(GetParameter("Port", paramsVector[i], false,
                                             m_DebugMode,
                                             "Transport Port Parameter"));

        if (portControl.empty())
        {
            portControl = std::to_string(m_DefaultPort);
        }

        const std::string portData(std::to_string(stoi(portControl) + 1));

        std::string format(GetParameter("Format", paramsVector[i], false,
                                        m_DebugMode, "Format"));
        if (format.empty())
        {
            format = "bp";
        }

        if (library == "zmq" || library == "ZMQ")
        {
#ifdef ADIOS2_HAVE_ZEROMQ
            wanTransport = std::make_shared<transport::WANZmq>(
                ipAddress, portData, m_MPIComm, m_DebugMode);
            wanTransport->Open(streamNames[i], mode);
            m_Transports.emplace(counter, wanTransport);

            controlTransport = std::make_shared<transport::WANZmq>(
                ipAddress, portControl, m_MPIComm, m_DebugMode);
            controlTransport->Open(streamNames[i], mode);
            m_ControlTransports.emplace_back(controlTransport);

            if (mode == Mode::Read)
            {
                m_Listening = true;
                m_ControlThreads.emplace_back(
                    std::thread(&DataMan::ReadThread, this, wanTransport,
                                controlTransport, format));
            }
            ++counter;
#else
            throw std::invalid_argument(
                "ERROR: this version of ADIOS2 didn't compile with "
                "ZMQ library, in call to Open\n");
#endif
        }
        else
        {
            if (m_DebugMode)
            {
                throw std::invalid_argument("ERROR: wan transport " + library +
                                            " not supported or not "
                                            "provided in IO AddTransport, "
                                            "in call to Open\n");
            }
        }
    }
}

void DataMan::WriteWAN(const void *buffer, nlohmann::json jmsg)
{
    if (m_CurrentTransport >= m_ControlTransports.size())
    {
        throw std::runtime_error("ERROR: No valid control transports found, "
                                 "from DataMan::WriteWAN()");
    }
    if (m_CurrentTransport >= m_Transports.size())
    {
        throw std::runtime_error(
            "ERROR: No valid transports found, from DataMan::WriteWAN()");
    }
    m_ControlTransports[m_CurrentTransport]->Write(jmsg.dump().c_str(),
                                                   jmsg.dump().size());
    m_Transports[m_CurrentTransport]->Write(
        reinterpret_cast<const char *>(buffer), jmsg["bytes"].get<size_t>());
}

void DataMan::WriteWAN(const void *buffer, size_t size)
{
    if (m_CurrentTransport >= m_Transports.size())
    {
        throw std::runtime_error(
            "ERROR: No valid transports found, from DataMan::WriteWAN()");
    }

    m_Transports[m_CurrentTransport]->Write(
        reinterpret_cast<const char *>(buffer), size);

    /*
    //  dumping file for debugging
    std::ofstream bpfile("datamanW.bp", std::ios_base::binary);
    bpfile.write(reinterpret_cast<const char *>(buffer), size);
    bpfile.close();
    */
}

void DataMan::ReadWAN(void *buffer, nlohmann::json jmsg) {}

void DataMan::SetBP3Deserializer(format::BP3Deserializer &bp3Deserializer)
{
    m_BP3Deserializer = &bp3Deserializer;
}

void DataMan::SetIO(IO &io) { m_IO = &io; }

void DataMan::SetCallback(adios2::Operator &callback)
{
    m_Callback = &callback;
}

void DataMan::ReadThread(std::shared_ptr<Transport> trans,
                         std::shared_ptr<Transport> ctl_trans,
                         const std::string format)
{
    if (format == "json" || format == "JSON")
    {
        while (m_Listening)
        {
            char buffer[1024];
            size_t bytes = 0;
            nlohmann::json jmsg;
            adios2::Transport::Status status;
            ctl_trans->IRead(buffer, 1024, status, 0);
            if (status.Bytes > 0)
            {
                std::string smsg(buffer);
                jmsg = nlohmann::json::parse(smsg);
                bytes = jmsg.value("bytes", 0);
                if (bytes > 0)
                {
                    std::vector<char> data(bytes);
                    trans->Read(data.data(), bytes);
                    std::string doid =
                        jmsg.value("doid", "Unknown Data Object");
                    std::string var = jmsg.value("var", "Unknown Variable");
                    std::string dtype =
                        jmsg.value("dtype", "Unknown Data Type");
                    std::vector<size_t> putshape =
                        jmsg.value("putshape", std::vector<size_t>());
                    if (m_Callback != nullptr &&
                        m_Callback->m_Type == "Signature2")
                    {
                        m_Callback->RunCallback2(data.data(), doid, var, dtype,
                                                 putshape);
                    }
                }
            }
        }
    }
    else if (format == "bp" || format == "BP")
    {
        while (m_Listening)
        {
            std::vector<char> buffer;
            buffer.reserve(m_BufferSize);

            Transport::Status status;
            trans->IRead(buffer.data(), m_BufferSize, status);

            if (status.Bytes > 0)
            {
                m_BP3Deserializer->m_Data.Resize(
                    status.Bytes, "in DataMan Streaming Listener");

                std::memcpy(m_BP3Deserializer->m_Data.m_Buffer.data(),
                            buffer.data(), status.Bytes);

                /*    write bp file for debugging   */
                m_BP3Deserializer->ParseMetadata(m_BP3Deserializer->m_Data,
                                                 *m_IO);

                const auto variablesInfo = m_IO->GetAvailableVariables();
                for (const auto &variableInfoPair : variablesInfo)
                {

                    std::string var = variableInfoPair.first;
                    std::string type = "null";

                    for (const auto &parameter : variableInfoPair.second)
                    {
                        //  ** print out all parameters from BP metadata
                        /*
                            std::cout << "\tKey: " << parameter.first
                                      << "\t Value: " << parameter.second <<
                           "\n";
                        */
                        if (parameter.first == "Type")
                        {
                            type = parameter.second;
                        }
                    }

                    if (type == "compound")
                    {
                        // not supported
                    }
#define declare_type(T)                                                        \
    else if (type == GetType<T>())                                             \
    {                                                                          \
        adios2::Variable<T> *v = m_IO->InquireVariable<T>(var);                \
        m_BP3Deserializer->GetSyncVariableDataFromStream(                      \
            *v, m_BP3Deserializer->m_Data);                                    \
        if (v->GetData() == nullptr)                                           \
        {                                                                      \
            throw("Data pointer obtained from BP deserializer is a nullptr");  \
        }                                                                      \
        RunCallback(v->GetData(), "stream", var, type, v->m_Shape);            \
    }
                    ADIOS2_FOREACH_TYPE_1ARG(declare_type)
#undef declare_type
                }
            }
        }
    }
}

void DataMan::RunCallback(void *buffer, std::string doid, std::string var,
                          std::string dtype, std::vector<size_t> shape)
{
    if (m_Callback != nullptr && m_Callback->m_Type == "Signature2")
    {
        m_Callback->RunCallback2(buffer, doid, var, dtype, shape);
    }
}

} // end namespace transportman
} // end namespace adios2
