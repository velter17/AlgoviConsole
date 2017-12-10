#pragma once

#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace AlgoVi {
namespace Compiler {

class Executable
{
public:
    Executable(boost::optional<boost::filesystem::path> file = boost::none);
    Executable(Executable&& other);
    ~Executable();

    Executable& setFile(boost::filesystem::path file);
    Executable& setArgs(const std::vector<std::string>& args);
    Executable& appendArgs(const std::vector<std::string>& args);
    Executable& deleteAfter(bool value = true);
    bool isDeleteAfter() const;

    bool isValid() const;

    boost::filesystem::path getAppPath() const;
    const std::vector<std::string>& getArgs() const;

private:
    boost::optional<boost::filesystem::path> m_file;
    std::vector<std::string> m_args;
    bool m_file_delete_flag;
};

} // namespace Compiler
} // namespace AlgoVi
