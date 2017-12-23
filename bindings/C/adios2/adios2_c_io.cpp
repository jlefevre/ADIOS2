/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * adios2_c_io.cpp
 *
 *  Created on: Nov 8, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "adios2_c_io.h"

#include <vector>

#include "adios2/ADIOSMacros.h"
#include "adios2/core/IO.h"
#include "adios2/helper/adiosFunctions.h" //GetType<T>

adios2_Variable *
adios2_define_variable(adios2_IO *io, const char *name, const adios2_type type,
                       const size_t ndims, const size_t *shape,
                       const size_t *start, const size_t *count,
                       const adios2_constant_dims constant_dims, void *data)
{
    const bool constantSizeBool =
        (constant_dims == adios2_constant_dims_true) ? true : false;

    std::vector<size_t> shapeV, startV, countV;

    if (shape != nullptr)
    {
        shapeV.assign(shape, shape + ndims);
    }

    if (start != nullptr)
    {
        startV.assign(start, start + ndims);
    }

    if (count != nullptr)
    {
        countV.assign(count, count + ndims);
    }

    adios2::IO &ioCpp = *reinterpret_cast<adios2::IO *>(io);
    adios2::VariableBase *variable = nullptr;

    switch (type)
    {

    case (adios2_type_string):
    {
        variable = &ioCpp.DefineVariable<std::string>(name, shapeV, startV,
                                                      countV, constantSizeBool);
        break;
    }
    case (adios2_type_char):
    {
        variable = &ioCpp.DefineVariable<char>(name, shapeV, startV, countV,
                                               constantSizeBool,
                                               reinterpret_cast<char *>(data));
        break;
    }
    case (adios2_type_signed_char):
    {
        variable = &ioCpp.DefineVariable<signed char>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<signed char *>(data));
        break;
    }
    case (adios2_type_short):
    {
        variable = &ioCpp.DefineVariable<short>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<short *>(data));
        break;
    }
    case (adios2_type_int):
    {
        variable = &ioCpp.DefineVariable<int>(name, shapeV, startV, countV,
                                              constantSizeBool,
                                              reinterpret_cast<int *>(data));
        break;
    }
    case (adios2_type_long_int):
    {
        variable = &ioCpp.DefineVariable<long int>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<long int *>(data));
        break;
    }
    case (adios2_type_long_long_int):
    {
        variable = &ioCpp.DefineVariable<long long int>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<long long int *>(data));
        break;
    }
    case (adios2_type_unsigned_char):
    {
        variable = &ioCpp.DefineVariable<unsigned char>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<unsigned char *>(data));
        break;
    }
    case (adios2_type_unsigned_short):
    {
        variable = &ioCpp.DefineVariable<unsigned short>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<unsigned short *>(data));
        break;
    }
    case (adios2_type_unsigned_int):
    {
        variable = &ioCpp.DefineVariable<unsigned int>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<unsigned int *>(data));
        break;
    }
    case (adios2_type_unsigned_long_int):
    {
        variable = &ioCpp.DefineVariable<unsigned long int>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<unsigned long int *>(data));
        break;
    }
    case (adios2_type_unsigned_long_long_int):
    {
        variable = &ioCpp.DefineVariable<unsigned long long int>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<unsigned long long int *>(data));
        break;
    }
    case (adios2_type_float):
    {
        variable = &ioCpp.DefineVariable<float>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<float *>(data));
        break;
    }
    case (adios2_type_double):
    {
        variable = &ioCpp.DefineVariable<double>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<double *>(data));
        break;
    }
    case (adios2_type_float_complex):
    {
        variable = &ioCpp.DefineVariable<std::complex<float>>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<std::complex<float> *>(data));
        break;
    }
    case (adios2_type_double_complex):
    {
        variable = &ioCpp.DefineVariable<std::complex<double>>(
            name, shapeV, startV, countV, constantSizeBool);
        break;
    }
    case (adios2_type_int8_t):
    {

        variable = &ioCpp.DefineVariable<int8_t>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<int8_t *>(data));
        break;
    }
    case (adios2_type_int16_t):
    {
        variable = &ioCpp.DefineVariable<int16_t>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<int16_t *>(data));
        break;
    }
    case (adios2_type_int32_t):
    {
        variable = &ioCpp.DefineVariable<int32_t>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<int32_t *>(data));
        break;
    }
    case (adios2_type_int64_t):
    {
        variable = &ioCpp.DefineVariable<int64_t>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<int64_t *>(data));
        break;
    }
    case (adios2_type_uint8_t):
    {

        variable = &ioCpp.DefineVariable<uint8_t>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<uint8_t *>(data));
        break;
    }
    case (adios2_type_uint16_t):
    {
        variable = dynamic_cast<adios2::Variable<uint16_t> *>(
            &ioCpp.DefineVariable<uint16_t>(
                name, shapeV, startV, countV, constantSizeBool,
                reinterpret_cast<uint16_t *>(data)));
        break;
    }
    case (adios2_type_uint32_t):
    {
        variable = &ioCpp.DefineVariable<uint32_t>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<uint32_t *>(data));
        break;
    }
    case (adios2_type_uint64_t):
    {
        variable = &ioCpp.DefineVariable<uint64_t>(
            name, shapeV, startV, countV, constantSizeBool,
            reinterpret_cast<uint64_t *>(data));
        break;
    }
    }

    return reinterpret_cast<adios2_Variable *>(variable);
}

adios2_Variable *adios2_inquire_variable(adios2_IO *io, const char *name)
{
    adios2::IO &ioCpp = *reinterpret_cast<adios2::IO *>(io);
    const auto &dataMap = ioCpp.GetVariablesDataMap();

    auto itVariable = dataMap.find(name);
    if (itVariable == dataMap.end()) // not found
    {
        return nullptr;
    }

    const std::string type(itVariable->second.first);
    adios2::VariableBase *variable = nullptr;

    if (type == "compound")
    {
        // not supported
    }
#define declare_template_instantiation(T)                                      \
    else if (type == adios2::GetType<T>())                                     \
    {                                                                          \
        variable = ioCpp.InquireVariable<T>(name);                             \
    }
    ADIOS2_FOREACH_TYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

    return reinterpret_cast<adios2_Variable *>(variable);
}

void adios2_set_engine(adios2_IO *io, const char *engine_type)
{
    reinterpret_cast<adios2::IO *>(io)->SetEngine(engine_type);
}

void adios2_set_parameter(adios2_IO *io, const char *key, const char *value)
{
    reinterpret_cast<adios2::IO *>(io)->SetParameter(key, value);
}

unsigned int adios2_add_transport(adios2_IO *io, const char *transport_type)
{
    return reinterpret_cast<adios2::IO *>(io)->AddTransport(transport_type);
}

void adios2_set_transport_parameter(adios2_IO *io,
                                    const unsigned int transport_index,
                                    const char *key, const char *value)
{
    reinterpret_cast<adios2::IO *>(io)->SetTransportParameter(transport_index,
                                                              key, value);
}

adios2_Engine *adios2_open(adios2_IO *io, const char *name,
                           const adios2_mode mode)
{
    auto &ioCpp = *reinterpret_cast<adios2::IO *>(io);
    return adios2_open_new_comm(io, name, mode, ioCpp.m_MPIComm);
}

adios2_Engine *adios2_open_new_comm(adios2_IO *io, const char *name,
                                    const adios2_mode mode, MPI_Comm mpi_comm)
{
    auto &ioCpp = *reinterpret_cast<adios2::IO *>(io);
    adios2::Engine *engine = nullptr;

    switch (mode)
    {

    case adios2_mode_write:
        engine = &ioCpp.Open(name, adios2::Mode::Write, mpi_comm);
        break;

    case adios2_mode_read:
        engine = &ioCpp.Open(name, adios2::Mode::Read, mpi_comm);
        break;

    case adios2_mode_append:
        engine = &ioCpp.Open(name, adios2::Mode::Append, mpi_comm);
        break;

    case adios2_mode_undefined:

        break;
    }

    return reinterpret_cast<adios2_Engine *>(engine);
}
