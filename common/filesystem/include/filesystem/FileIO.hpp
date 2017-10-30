#pragma once

#include <boost/filesystem/path.hpp>
#include <fstream>

namespace SOME_NAME {
namespace Filesystem {

inline std::string readFromStream(std::istream&& stream)
{
    std::string ret;
    std::string tmp;
    while (std::getline(stream, tmp))
    {
        ret += tmp + "\n";
    }
    if (!ret.empty())
    {
        ret.pop_back();
    }
    return ret;
}

inline void writeToStream(std::ostream&& stream, const std::string& str)
{
    stream << str;
}

inline std::string readFromFile(const boost::filesystem::path& path)
{
    return readFromStream(std::ifstream(path.string()));
}

inline void writeToFile(const boost::filesystem::path& path, const std::string& str)
{
    writeToStream(std::ofstream(path.string()), str);
}

} // namespace Filesystem
} // namespace SOME_NAME
