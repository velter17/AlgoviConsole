#include "test_archive/Test.hpp"
#include <boost/filesystem.hpp>
#include "filesystem/FileIO.hpp"

namespace SOME_NAME {
namespace TestArchive {

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
    , m_from_files_flag(false)
    , m_delete_flag(true)
{
}

Test::Test(const std::string& input, const std::string& output)
    : m_input(input)
    , m_output(output)
    , m_modified_flag(false)
    , m_from_files_flag(false)
    , m_delete_flag(true)
{
}

Test::Test(std::istream&& input, std::istream&& output)
    : Test(convert(input), convert(output))
{
}

Test::Test(
    const boost::filesystem::path& input,
    const boost::filesystem::path& output,
    bool handle_modif,
    bool read_from_files)
    : m_input_file(input)
    , m_output_file(output)
    , m_from_files_flag(handle_modif)
    , m_delete_flag(false)
{
    if (read_from_files)
    {
        m_input = Filesystem::readFromFile(input);
        m_output = Filesystem::readFromFile(output);
    }
}

Test::~Test()
{
    if (m_from_files_flag)
    {
        if (m_delete_flag)
        {
            Filesystem::writeToFile(*m_input_file, m_input);
            Filesystem::writeToFile(*m_output_file, m_output);
        }
    }
    else
    {
        if (m_input_file)
        {
            boost::filesystem::remove(*m_input_file);
            boost::filesystem::remove(*m_output_file);
        }
    }
}

/*Test& Test::setInput(const std::string& input)
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
}*/

Test& Test::moveToFiles(const boost::filesystem::path& input, const boost::filesystem::path& output)
{
    m_input_file = input;
    m_output_file = output;
    m_from_files_flag = true;
    m_modified_flag = true;
    return *this;
}

const std::string& Test::input()
{
    if (m_input.empty() && !m_delete_flag && m_from_files_flag)
    {
        m_input = Filesystem::readFromFile(m_input_file.get());
    }
    return m_input;
}

const std::string& Test::input() const
{
    return m_input;
} 

const std::string& Test::output()
{
    if (m_output.empty() && !m_delete_flag && m_from_files_flag)
    {
        m_output = Filesystem::readFromFile(m_output_file.get());
    }
    return m_output;
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

} // namespace TestArchive
} // namespace SOME_NAME
