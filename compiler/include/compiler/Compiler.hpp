#pragma once

#include "compiler/SourceCode.hpp"
#include "compiler/Executable.hpp"

namespace SOME_NAME {
namespace Compiler {

class Compiler
{
public:
    Compiler(const SourceCode& source_code);

    std::shared_ptr<Executable> compile();

private:
    const SourceCode& m_code;
};

} // namespace Compiler
} // namespace SOME_NAME
