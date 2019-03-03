#include "tester/Validator.hpp"
#include "filesystem/TempFile.hpp"
#include "filesystem/FileIO.hpp"
#include <iostream>
#include "test_archive/TestFromFile.hpp"

namespace AlgoVi {
namespace Tester {

Validator::Validator(Executor::ExecutablePtr exec)
    : Executor::Executor(exec)
    , m_input_file_delete(false)
    , m_output_file_delete(false)
{
    m_result_file = Filesystem::getTempFile(".txt");
}

Validator::~Validator()
{
    /*if (m_input_file_delete)
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
    boost::filesystem::remove(*m_result_file);*/
}

void Validator::execute()
{
    assert("No executable" && m_executable);
    assert("No test_input file" && m_test_input_file);
    assert("No test_output file" && m_test_output_file);
    m_executable->setArgs({m_test_input_file->string(),
                           m_test_output_file->string(),
                           m_result_file->string()});

    Executor::execute();
}

std::int32_t Validator::wait()
{
    auto res = Executor::wait();
    if (m_output.empty())
    {
        m_output = Filesystem::readFromFile(*m_result_file);
    }
    //std::cout << "output is " << m_output << std::endl;
    return res;
}

Validator& Validator::setTest(TestArchive::ITest* test)
{
    if (m_input_file_delete)
    {
        boost::filesystem::remove(m_test_input_file.get());
        m_input_file_delete = false;
    }

    /* TODO : FIX ME*/
    auto from_file_test = dynamic_cast<TestArchive::TestFromFile*>(test);

    //if (test.inputFile())
    if (from_file_test != nullptr)
    {
        m_test_input_file = from_file_test->inputFile();
    }
    else
    {
        if (!m_test_input_file)
        {
            m_test_input_file = Filesystem::getTempFile(".dat");
        }
        Filesystem::writeToFile(*m_test_input_file, test->input());
        m_input_file_delete = true;
    }

    //if (test.outputFile())
    if (from_file_test != nullptr)
    {
        m_test_output_file = from_file_test->outputFile();
    }
    else
    {
        if (!m_test_output_file)
        {
            m_test_output_file = Filesystem::getTempFile(".res");
        }
        Filesystem::writeToFile(*m_test_output_file, test->output());
        m_output_file_delete = true;
    }
    return *this;
}

} // namespace Tester
} // namespace AlgoVi
