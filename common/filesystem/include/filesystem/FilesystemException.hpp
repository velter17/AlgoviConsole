#pragma once

#include <stdexcept>
#include <string>

namespace AlgoVi {
namespace Filesystem {

class FilesystemException : public std::runtime_error
{
public:
    FilesystemException(const std::string& msg);
    char const* what() const throw() override;
};

inline FilesystemException::FilesystemException(const std::string& msg)
    : std::runtime_error(msg)
{
}

inline char const* FilesystemException::what() const throw()
{
    return std::exception::what();
}

} // namespace Filesystem
} // namespace AlgoVi
