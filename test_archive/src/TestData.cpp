#include "test_archive/TestData.hpp"

namespace AlgoVi {
namespace TestArchive {

TestData::TestData(const std::string& input, const std::string& output)
    : m_input(input)
    , m_output(output)
{
}

const std::string& TestData::input()
{
    return m_input;
}

const std::string& TestData::output()
{
    return m_output;
}

} // namespace TestArchive
} // namespace AlgoVi
