#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <string>
#include <fstream>

namespace AlgoVi {
namespace TestArchive {

class Test
{
public:
    Test();
    explicit Test(const std::string& input, const std::string& output);
    explicit Test(std::istream&& input, std::istream&& output);
    explicit Test(
        const boost::filesystem::path& input,
        const boost::filesystem::path& output,
        bool handle_modif,
        bool read_from_files = true);
    ~Test();

    Test& moveToFiles(const boost::filesystem::path& input, const boost::filesystem::path& output);
    //Test& setInput(const std::string& input);
    //Test& setOutput(const std::string& output);

    const std::string& input() const;
    const std::string& output() const;
    const std::string& input();
    const std::string& output();

    boost::optional<boost::filesystem::path> inputFile() const;
    boost::optional<boost::filesystem::path> outputFile() const;

    bool isEqual(const Test& other) const;

private:
    std::string m_input;
    std::string m_output;
    bool m_modified_flag;
    bool m_delete_flag;
    boost::optional<boost::filesystem::path> m_input_file;
    boost::optional<boost::filesystem::path> m_output_file;
    bool m_from_files_flag;
};

} // namespace TestArchive
} // namespace AlgoVi
