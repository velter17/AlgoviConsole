#pragma once

#include "executor/Executor.hpp"
#include "test_archive/Test.hpp"

namespace AlgoVi {
namespace Tester {

class Checker : public Executor::Executor
{
public:
    Checker(Executor::ExecutablePtr exec);
    ~Checker();

    void execute() override;
    std::int32_t wait() override;

    Checker& setTest(const TestArchive::Test& test);
    Checker& setOutput(const std::string& output);

private:
    boost::optional<boost::filesystem::path> m_output_file;
    boost::optional<boost::filesystem::path> m_test_input_file;
    boost::optional<boost::filesystem::path> m_test_output_file;
    boost::optional<boost::filesystem::path> m_result_file;
    bool m_input_file_delete;
    bool m_output_file_delete;
};

} // namespace Tester
} // namespace AlgoVi
