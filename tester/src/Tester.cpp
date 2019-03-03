#include "tester/Tester.hpp"

namespace AlgoVi {
namespace Tester {

Tester::Tester(Tester::ExecutorPtr program, Tester::CheckerPtr checker)
    : m_program(program)
    , m_checker(checker)
    , m_time_limit(std::numeric_limits<uint64_t>::max())
{
}

void Tester::setTimeLimit(std::uint64_t milliseconds)
{
    m_time_limit = milliseconds;
}

void Tester::setTest(TestArchive::ITest* test)
{
    m_test = test;
}

Tester::ResultPair Tester::test(bool save_output)
{
    assert("Program is nullptr" && m_program != nullptr);
    assert("No test" && m_test);
    m_program->setInput(m_test->input());
    if (m_time_limit != 0)
    {
        m_program->setTimeLimit(m_time_limit);
    }
    m_program->execute();
    auto program_exit_code = m_program->wait();
    if (program_exit_code == 0)
    {
        auto custom_check = [this, &save_output](const std::string& output) {
            if (output == m_test->output())
            {
                if (save_output)
                    return ResultPair{ETestResult::OK, "== is true", 0, output};
                else
                    return ResultPair{ETestResult::OK, "== is true", 0, boost::none};
            }
            else
            {
                if (save_output)
                    return ResultPair{ETestResult::WrongAnswer, "== is false", 0, output};
                else
                    return ResultPair{ETestResult::WrongAnswer, "== is false", 0, boost::none};
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
                    return ResultPair{ETestResult::OK,
                                      m_checker->getOutput(),
                                      m_program->getExecutionTime(),
                                      m_program->getOutput()};
                else
                    return ResultPair{
                        ETestResult::OK, m_checker->getOutput(), m_program->getExecutionTime()};
            }
            else
            {
                if (save_output)
                    return ResultPair{ETestResult::WrongAnswer,
                                      m_checker->getOutput(),
                                      m_program->getExecutionTime(),
                                      m_program->getOutput()};
                else
                    return ResultPair{ETestResult::WrongAnswer,
                                      m_checker->getOutput(),
                                      m_program->getExecutionTime()};
            }
        }
        else
        {
            return custom_check(m_program->getOutput());
        }
    }
    else
    {
        return ResultPair{ETestResult::RuntimeError, "Runtime error was occured", 0, boost::none};
    }
}

} // namespace Tester
} // namespace AlgoVi
