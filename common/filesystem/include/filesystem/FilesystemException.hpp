#pragma once

#include <stdexcept>
#include <string>

namespace SOME_NAME {
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
} // namespace SOME_NAME
