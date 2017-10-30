#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <string>
#include <fstream>

namespace SOME_NAME {
namespace Tester {

class Test
{
public:
    Test();
    explicit Test(const std::string& input, const std::string& output);
    explicit Test(std::istream&& input, std::istream&& output);

    Test& readFromFiles(
        const boost::filesystem::path& input,
        const boost::filesystem::path& output);
    Test& moveToFiles(const boost::filesystem::path& input, const boost::filesystem::path& output);
    Test& setInput(const std::string& input);
    Test& setOutput(const std::string& output);

    const std::string& input() const;
    const std::string& output() const;

    boost::optional<boost::filesystem::path> inputFile() const;
    boost::optional<boost::filesystem::path> outputFile() const;

    bool isEqual(const Test& other) const;

private:
    std::string m_input;
    std::string m_output;
    bool m_modified_flag;
    boost::optional<boost::filesystem::path> m_input_file;
    boost::optional<boost::filesystem::path> m_output_file;
};

} // namespace Tester
} // namespace SOME_NAME
