#pragma once

#include <boost/filesystem/path.hpp>
#include "compiler/Language.hpp"

namespace SOME_NAME {
namespace Compiler {

class SourceCode
{
public:
    SourceCode(const std::string& code, ELanguage lang);
    SourceCode(const boost::filesystem::path& file);
    ~SourceCode();

    boost::filesystem::path getFilePath() const;

private:
    bool m_file_delete_flag;
    boost::filesystem::path m_file;
};

} // namespace Compiler
} // namespace SOME_NAME
