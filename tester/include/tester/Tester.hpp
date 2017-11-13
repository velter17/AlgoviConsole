#pragma once

#include "executor/Executor.hpp"
#include "tester/Checker.hpp"
#include "test_archive/Test.hpp"

namespace SOME_NAME {
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
    };

public:
    Tester(ExecutorPtr program, CheckerPtr checker = CheckerPtr());

    void setTimeLimit(std::uint64_t milliseconds);
    void setTest(const TestArchive::Test& test);

    ResultPair test();
    ResultPair getResult() const;

private:
    ExecutorPtr m_program;
    CheckerPtr m_checker;
    TestArchive::Test m_test;
    std::uint64_t m_time_limit;
};

} // namespace Tester
} // namespace SOME_NAME
