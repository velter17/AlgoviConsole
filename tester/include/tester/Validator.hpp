#pragma once

#include "executor/Executor.hpp"
#include "test_archive/ITest.hpp"

namespace AlgoVi {
namespace Tester {

class Validator : public Executor::Executor
{
public:
    Validator(Executor::ExecutablePtr exec);
    ~Validator();

    void execute() override;
    std::int32_t wait() override;

    Validator& setTest(TestArchive::ITest* test);

private:
    boost::optional<boost::filesystem::path> m_test_input_file;
    boost::optional<boost::filesystem::path> m_test_output_file;
    boost::optional<boost::filesystem::path> m_result_file;
    bool m_input_file_delete;
    bool m_output_file_delete;
};

} // namespace Tester
} // namespace AlgoVi
