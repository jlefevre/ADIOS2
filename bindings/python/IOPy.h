/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * IOPy.h
 *
 *  Created on: Mar 14, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_BINDINGS_PYTHON_SOURCE_IOPY_H_
#define ADIOS2_BINDINGS_PYTHON_SOURCE_IOPY_H_

/// \cond EXCLUDE_FROM_DOXYGEN
#include <string>
/// \endcond

#include <pybind11/numpy.h>

#include "EnginePy.h"

namespace adios2
{

class IOPy
{

public:
    IO &m_IO;

    IOPy(IO &io, const bool debugMode);

    ~IOPy() = default;

    void SetEngine(const std::string type) noexcept;
    void SetParameters(const Params &parameters) noexcept;

    VariableBase &DefineVariable(const std::string &name, const Dims &shape,
                                 const Dims &start, const Dims &count,
                                 const bool isConstantDims,
                                 pybind11::array &array);

    VariableBase *InquireVariable(const std::string &name) noexcept;

    EnginePy Open(const std::string &name, const int openMode);

private:
    const bool m_DebugMode;
    /**
     *  Placeholder map needed as Variables are not created in ADIOS at
     *  DefineVariable, but until Put when type is known from numpy
     */
    std::map<std::string, VariableBase> m_VariablesPlaceholder;
};

} // end namespace adios2

#endif /* BINDINGS_PYTHON_SOURCE_IOPY_H_ */
