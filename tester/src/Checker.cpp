#include "tester/Checker.hpp"
#include "filesystem/TempFile.hpp"
#include "filesystem/FileIO.hpp"

namespace SOME_NAME {
namespace Tester {

Checker::Checker(Executor::ExecutablePtr exec)
    : Executor::Executor(exec)
{
}

Checker::~Checker()
{
    assert(m_test_input_file);
    boost::filesystem::remove(*m_test_input_file);
    assert(m_test_output_file);
    boost::filesystem::remove(*m_test_output_file);
    assert(m_output_file);
    boost::filesystem::remove(*m_output_file);
}

void Checker::execute()
{
    assert("No executable" && m_executable);
    assert("No test_input file" && m_test_input_file);
    assert("No test_output file" && m_test_output_file);
    assert("No output file" && m_output_file);
    m_executable->setArgs(
        {m_test_input_file->string(), m_test_output_file->string(), m_output_file->string()});

    Executor::execute();
}

Checker& Checker::setTest(const Test& test)
{
    // TODO: handle test files
    m_test_input_file = Filesystem::getTempFile(".dat");
    Filesystem::writeToFile(*m_test_input_file, test.input());
    m_test_output_file = Filesystem::getTempFile(".res");
    Filesystem::writeToFile(*m_test_output_file, test.output());
    return *this;
}

Checker& Checker::setOutput(const std::string& output)
{
    m_output_file = Filesystem::getTempFile(".out");
    Filesystem::writeToFile(*m_output_file, output);
    return *this;
}

} // namespace Tester
} // namespace SOME_NAME
