#include "compiler/Compiler.hpp"
#include <iostream>
#include "executor/Executor.hpp"
#include "filesystem/TempFile.hpp"
#include "utils/LangExtractor.hpp"
#include <thread>

namespace AlgoVi {
namespace Compiler {

using path = boost::filesystem::path;

template <ELanguage lang>
std::shared_ptr<Executable> getExecutable(const path& code_path, const path& exec)
{
    (void)code_path;
    (void)exec;
    assert("Compiler for executable was not specified" && false);
}

template <>
std::shared_ptr<Executable> getExecutable<ELanguage::CPP>(const path& code_path, const path& exec)
{
    return std::make_shared<Executable>(
        std::move(Executable()
                      .setFile("/usr/bin/g++")
                      .setArgs({"-std=c++11", "-o", exec.string(), code_path.string()})));
}

template <>
std::shared_ptr<Executable> getExecutable<ELanguage::JAVA>(const path& code_path, const path& exec)
{
    return std::make_shared<Executable>(
        std::move(Executable()
                      .setFile("javac")
                      .setArgs({code_path.string()})));
}

template <>
std::shared_ptr<Executable> getExecutable<ELanguage::BINARY>(
    const path& code_path,
    const path& exec)
{
    (void)exec;
    return std::make_shared<Executable>(code_path);
}

std::shared_ptr<Executable> getExecutableWithArgs(
    const path& exec,
    const std::vector<std::string>& args)
{
    return std::make_shared<Executable>(
            std::move(Executable().setFile(exec).setArgs(args).deleteAfter(false)));
}

Compiler::Compiler(const SourceCode& code)
    : m_code(code)
{
}

bool Compiler::isNeededCompilation()
{
    return m_code.isUpdated();
}

std::shared_ptr<Executable> Compiler::compile()
{
    auto lang = Utils::extractLang(m_code.getFilePath());
    if (lang == ELanguage::BINARY)
    {
        m_code.update();
        return getExecutable<ELanguage::BINARY>(m_code.getFilePath(), boost::filesystem::path());
    }
    else if (lang == ELanguage::CPP)
    {
        auto bin_path = Filesystem::getTempFile("");
        auto compiler = getExecutable<ELanguage::CPP>(m_code.getFilePath(), bin_path);
        Executor::Executor compiler_exec(compiler);
        compiler_exec.execute();
        auto res = compiler_exec.wait();
        if (res == 0)
        {
            auto ret = getExecutable<ELanguage::BINARY>(bin_path, "");
            ret->deleteAfter();
            m_code.update();
            return ret;
        }
        else
        {
            throw CompilationError(m_code.getFilePath().string(), compiler_exec.getError());
        }
    }
    else if (lang == ELanguage::JAVA)
    {
        auto bin_path = m_code.getFilePath().stem();
        auto compiler = getExecutable<ELanguage::JAVA>(m_code.getFilePath(), bin_path);
        Executor::Executor compiler_exec(compiler);
        compiler_exec.execute();
        auto res = compiler_exec.wait();
        if (res == 0)
        {
            auto ret = getExecutableWithArgs(path("/usr/bin/java"), {bin_path.string()});
            m_code.update();
            return ret;
        }
        else
        {
            throw CompilationError(m_code.getFilePath().string(), compiler_exec.getError());
        }
    }
    else if (lang == ELanguage::PYTHON3)
    {
        m_code.update();
        return getExecutableWithArgs("/usr/bin/python3", {m_code.getFilePath().string()});
    }
    else
    {}
}

} // namespace Compiler
} // namespace AlgoVi
