#include "test_archive/TestFromFile.hpp"
#include "filesystem/FileIO.hpp"
#include <sstream>
#include <iomanip>

namespace AlgoVi {
namespace TestArchive {

TestFromFile::TestFromFile(
    const boost::filesystem::path& input_file,
    const boost::filesystem::path& output_file,
    ETestType type)
    : m_input_file(input_file)
    , m_output_file(output_file)
{
    if (type == ETestType::ReadFromFiles)
    {
        m_input = Filesystem::readFromFile(m_input_file);
        m_output = Filesystem::readFromFile(m_output_file);
    }
}

const std::string& TestFromFile::input() const 
{
    return m_input;
}

const std::string& TestFromFile::output() const
{
    return m_output;
}

boost::filesystem::path TestFromFile::inputFile()
{
    return m_input_file;
}

boost::filesystem::path TestFromFile::outputFile()
{
    return m_output_file;
}

void TestFromFile::move(
    const boost::filesystem::path& input_file,
    const boost::filesystem::path& output_file)
{
    boost::filesystem::rename(m_input_file, input_file);
    boost::filesystem::rename(m_output_file, output_file);
    m_input_file = input_file;
    m_output_file = output_file;
}

void TestFromFile::move(std::size_t number)
{
    std::stringstream stream;
    stream << std::setw(3) << std::setfill('0') << number;
    auto new_path_input = m_input_file.parent_path() / (std::string(stream.str()) + ".dat");
    auto new_path_output = m_output_file.parent_path() / (std::string(stream.str()) + ".ans");
    move(new_path_input, new_path_output);
}

void TestFromFile::remove()
{
    boost::filesystem::remove(m_input_file);
    boost::filesystem::remove(m_output_file);
}

std::size_t TestFromFile::getTestNumber()
{
    auto num = m_input_file.string().substr(0, 3);
    std::stringstream stream(num);
    std::size_t ret;
    stream >> ret;
    return ret;
}

} // namespace TestArchive
} // namespace AlgoVi
