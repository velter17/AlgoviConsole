#pragma once

#include <memory>

#include "compiler/SourceCode.hpp"
#include "compiler/Executable.hpp"

namespace AlgoVi {
namespace Compiler {

class CompilationError : public std::exception
{
public:
    CompilationError(const std::string& file, const std::string& error)

        : m_file(file)
        , m_error(error)
    {
    }

    const std::string& getFile() const
    {
        return m_file;
    }

    const char* what() const throw() override
    {
        return m_error.data();
    }

private:
    std::string m_file;
    std::string m_error;
};

class Compiler
{
public:
    Compiler(const SourceCode& source_code);

    bool isNeededCompilation();

    std::shared_ptr<Executable> compile();
private:
    SourceCode m_code;
    std::string m_error_message;
};

} // namespace Compiler
} // namespace AlgoVi
