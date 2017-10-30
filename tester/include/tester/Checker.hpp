#pragma once

#include "executor/Executor.hpp"
#include "tester/Test.hpp"

namespace SOME_NAME {
namespace Tester {

class Checker : public Executor::Executor
{
public:
    Checker(Executor::ExecutablePtr exec);
    ~Checker();

    void execute() override;

    Checker& setTest(const Test& test);
    Checker& setOutput(const std::string& output);

private:
    boost::optional<boost::filesystem::path> m_output_file;
    boost::optional<boost::filesystem::path> m_test_input_file;
    boost::optional<boost::filesystem::path> m_test_output_file;
};

} // namespace Tester
} // namespace SOME_NAME
