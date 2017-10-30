#include "tester/Test.hpp"

namespace SOME_NAME {
namespace Tester {

std::string convert(std::istream& stream)
{
    std::string ret;
    std::string line;
    while (std::getline(stream, line))
    {
        ret += line + "\n";
    }
    ret.pop_back();
    return ret;
}

std::string convert(const boost::filesystem::path& path)
{
    std::ifstream file(path.string());
    if (file.is_open())
    {
        return convert(file);
    }
    else
    {
        throw std::runtime_error(path.string() + " - no such file");
    }
}

Test::Test()
    : m_modified_flag(false)
{
}

Test::Test(const std::string& input, const std::string& output)
    : m_input(input)
    , m_output(output)
{
}

Test::Test(std::istream&& input, std::istream&& output)
    : Test(convert(input), convert(output))
{
}

Test& Test::readFromFiles(const boost::filesystem::path& input, const boost::filesystem::path& output)
{
    *this = Test(convert(input), convert(output));
    m_input_file = input;
    m_output_file = output;
    return *this;
}

Test& Test::setInput(const std::string& input)
{
    m_input = input;
    m_modified_flag = true;
    return *this;
}

Test& Test::setOutput(const std::string& output)
{
    m_output = output;
    m_modified_flag = true;
    return *this;
}

Test& Test::moveToFiles(const boost::filesystem::path& input, const boost::filesystem::path& output)
{
    m_input_file = input;
    m_output_file = output;
    return *this;
}

const std::string& Test::input() const
{
    return m_input;
}

const std::string& Test::output() const
{
    return m_output;
}

boost::optional<boost::filesystem::path> Test::inputFile() const
{
    return m_input_file;
}

boost::optional<boost::filesystem::path> Test::outputFile() const
{
    return m_output_file;
}

bool Test::isEqual(const Test& other) const
{
    return m_input == other.input() && m_output == other.output();
}

} // namespace Tester
} // namespace SOME_NAME
