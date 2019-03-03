#pragma once

#include <boost/filesystem/path.hpp>
#include <iostream>
#include <map>
#include "compiler/Language.hpp"

namespace AlgoVi {
namespace Compiler {
namespace Utils {

inline ELanguage extractLang(const boost::filesystem::path& file_path)
{
    static std::map<std::string, ELanguage> s_map = {
        {".cpp", ELanguage::CPP},
        {".py", ELanguage::PYTHON3},
        {".java", ELanguage::JAVA},
        {".sys", ELanguage::SYSTEM},
    };

    std::string extension = file_path.extension().string();
    //std::cout << "extension == " << extension << std::endl;

    return s_map.find(extension) == s_map.end() ? ELanguage::BINARY : s_map[extension];
}

inline ELanguage extractLang(const std::string& file_name)
{
    return extractLang(boost::filesystem::path(file_name));
}

} // namespace Utils
} // namespace Compiler
} // namespace AlgoVi
