#pragma once

#include "test_archive/ITest.hpp"

namespace  AlgoVi {
namespace TestArchive {

class TestData : public ITest
{
public:
    TestData(const std::string& input, const std::string& output);

    const std::string& input() override;
    const std::string& output() override;

private:
    std::string m_input;
    std::string m_output;
};

}
}
