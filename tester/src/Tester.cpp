#include "tester/Tester.hpp"

namespace SOME_NAME {
namespace Tester {

Tester::Tester(Tester::ExecutorPtr program, Tester::CheckerPtr checker)
    : m_program(program)
    , m_checker(checker)
    , m_time_limit(10 * 100)
{
}

void Tester::setTimeLimit(std::uint64_t milliseconds)
{
    m_time_limit = milliseconds;
}

void Tester::setTest(const TestArchive::Test& test)
{
    m_test = test;
}

Tester::ResultPair Tester::test(bool save_output)
{
    assert("Program is nullptr" && m_program != nullptr);
    m_program->setInput(m_test.input());
    m_program->execute();
    auto program_exit_code = m_program->wait();
    if (program_exit_code == 0)
    {
        auto custom_check = [this, &save_output](const std::string& output) {
            if (output == m_test.output())
            {
                if (save_output)
                    return ResultPair{ETestResult::OK, "== is true", output};
                else
                    return ResultPair{ETestResult::OK, "== is true", boost::none};
            }
            else
            {
                if (save_output)
                    return ResultPair{ETestResult::WrongAnswer, "== is false", output};
                else
                    return ResultPair{ETestResult::WrongAnswer, "== is false", boost::none};
            }
        };

        if (m_checker)
        {
            m_checker->setTest(m_test);
            m_checker->setOutput(m_program->getOutput());
            m_checker->execute();
            auto checker_exit_code = m_checker->wait();
            if (checker_exit_code == 0)
            {
                if (save_output)
                    return ResultPair{
                        ETestResult::OK, m_checker->getOutput(), m_program->getOutput()};
                else
                    return ResultPair{ETestResult::OK, m_checker->getOutput()};
            }
            else
            {
                if (save_output)
                    return ResultPair{ETestResult::WrongAnswer, m_checker->getOutput(), m_program->getOutput()};
                else
                    return ResultPair{ETestResult::WrongAnswer, m_checker->getOutput()};
            }
        }
        else
        {
            return custom_check(m_program->getOutput());
        }
    }
    else
    {
        return ResultPair{ETestResult::RuntimeError, "Runtime error was occured", boost::none};
    }
}

} // namespace Tester
} // namespace SOME_NAME
