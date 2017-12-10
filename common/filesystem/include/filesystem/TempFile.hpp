#pragma once

#include <boost/filesystem.hpp>

namespace AlgoVi {
namespace Filesystem {

inline boost::filesystem::path getTempFile(const std::string& extension)
{
    return boost::filesystem::temp_directory_path() /
           (boost::filesystem::unique_path().string() + extension);
}

} // namespace Filesystem
} // namespace AlgoVi

