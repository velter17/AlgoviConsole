#pragma once

#include "executor/Executor.hpp"
#include "tester/Checker.hpp"
#include "test_archive/Test.hpp"

namespace AlgoVi {
namespace Tester {

class Tester
{
public:
    enum class ETestResult
    {
        OK,
        WrongAnswer,
        RuntimeError,
        InternalError,
        TimeLimitExceeded,
        MemoryLimitExceeded,
        CheckFailed,
        Total,
    };
    using ExecutorPtr = std::shared_ptr<Executor::Executor>;
    using CheckerPtr = std::shared_ptr<Checker>;
    struct ResultPair
    {
        ETestResult result;
        std::string detailed_message;
        int32_t execution_time;
        boost::optional<std::string> output;
    };

public:
    Tester(ExecutorPtr program, CheckerPtr checker = CheckerPtr());

    void setTimeLimit(std::uint64_t milliseconds);
    void setTest(const TestArchive::Test& test);

    ResultPair test(bool save_output = false);
    ResultPair getResult() const;

private:
    ExecutorPtr m_program;
    CheckerPtr m_checker;
    TestArchive::Test m_test;
    std::uint64_t m_time_limit;
};

} // namespace Tester
} // namespace AlgoVi
