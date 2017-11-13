#include "tester/Checker.hpp"
#include "filesystem/TempFile.hpp"
#include "filesystem/FileIO.hpp"
#include <iostream>

namespace SOME_NAME {
namespace Tester {

Checker::Checker(Executor::ExecutablePtr exec)
    : Executor::Executor(exec)
{
    m_result_file = Filesystem::getTempFile(".txt");
    std::cout << "temp file is " << m_result_file->string() << std::endl;
}

Checker::~Checker()
{
    if (m_input_file_delete)
    {
        assert(m_test_input_file);
        boost::filesystem::remove(*m_test_input_file);
    }
    if (m_output_file_delete)
    {
        assert(m_test_output_file);
        boost::filesystem::remove(*m_test_output_file);
    }
    assert(m_output_file);
    boost::filesystem::remove(*m_output_file);
    boost::filesystem::remove(*m_result_file);
}

void Checker::execute()
{
    assert("No executable" && m_executable);
    assert("No test_input file" && m_test_input_file);
    assert("No test_output file" && m_test_output_file);
    assert("No output file" && m_output_file);
    m_executable->setArgs({m_test_input_file->string(),
                           m_test_output_file->string(),
                           m_output_file->string(),
                           m_result_file->string()});

    Executor::execute();
}

std::int32_t Checker::wait()
{
    auto res = Executor::wait();
    if (m_output.empty())
    {
        m_output = Filesystem::readFromFile(*m_result_file);
    }
    //std::cout << "output is " << m_output << std::endl;
    return res;
}

Checker& Checker::setTest(const TestArchive::Test& test)
{
    if (m_input_file_delete)
    {
        boost::filesystem::remove(m_test_input_file.get());
        m_input_file_delete = false;
    }

    if (test.inputFile())
    {
        m_test_input_file = test.inputFile().get();
    }
    else
    {
        if (!m_test_input_file)
        {
            m_test_input_file = Filesystem::getTempFile(".dat");
        }
        Filesystem::writeToFile(*m_test_input_file, test.input());
        m_input_file_delete = true;
    }

    if (test.outputFile())
    {
        m_test_output_file = test.outputFile();
    }
    else
    {
        if (!m_test_output_file)
        {
            m_test_output_file = Filesystem::getTempFile(".res");
        }
        Filesystem::writeToFile(*m_test_output_file, test.output());
        m_output_file_delete = true;
    }
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
