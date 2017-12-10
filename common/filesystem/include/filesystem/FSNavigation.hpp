#pragma once

#include <boost/filesystem.hpp>
#include "filesystem/FilesystemException.hpp"

namespace AlgoVi {
namespace Filesystem {

inline std::vector<boost::filesystem::path> fileList(const boost::filesystem::path& folder)
{
    std::vector<boost::filesystem::path> ret;
    if(!boost::filesystem::exists(folder))
    {
        throw FilesystemException("No such folder");
    }
    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(folder); itr != end_itr; ++itr)
    {
        const boost::filesystem::directory_entry& entry = *itr;
        if (entry.path().filename().c_str()[0] == '.')
        {
            continue;
        }
        if (boost::filesystem::is_regular_file(entry))
        {
            ret.push_back(entry);
        }
    }
    return ret;
}

inline std::string getFileName(const boost::filesystem::path& file)
{
    return file.filename().string();
}

inline std::string getFileExtension(const boost::filesystem::path& file)
{
    return file.extension().string();
}

inline boost::filesystem::path getAbsolutePath(const boost::filesystem::path& p)
{
    return boost::filesystem::absolute(p, boost::filesystem::current_path());
}

} // namespace Filesystem
} // namespace AlgoVi
