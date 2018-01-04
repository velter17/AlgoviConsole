#pragma once

#include "test_archive/ITest.hpp"
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

namespace AlgoVi {
namespace TestArchive {

enum class ETestType
{
    KeepInFiles,
    ReadFromFiles,
};

class TestFromFile : public ITest
{
public:
    TestFromFile(
        const boost::filesystem::path& input_file,
        const boost::filesystem::path& output_file,
        ETestType type = ETestType::ReadFromFiles);

    const std::string& input() const override;
    const std::string& output() const override;

    boost::filesystem::path inputFile();
    boost::filesystem::path outputFile();

    std::size_t getTestNumber();

    void move(
        const boost::filesystem::path& input_file,
        const boost::filesystem::path& output_file);
    void move(std::size_t number);

    void remove();

private:
    boost::filesystem::path m_input_file;
    boost::filesystem::path m_output_file;
    std::string m_input;
    std::string m_output;
};

}
} // namespace AlgoVi
