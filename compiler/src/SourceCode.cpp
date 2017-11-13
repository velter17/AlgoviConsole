#include "compiler/SourceCode.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include "filesystem/TempFile.hpp"

namespace SOME_NAME {
namespace Compiler {

SourceCode::SourceCode(const std::string& code, ELanguage lang)
    : m_file_delete_flag(true)
{
    auto path = Filesystem::getTempFile(".cpp");
    std::ofstream file(path.string());
    file << code;
    m_file = path;
    //std::cout << "created " << m_file.string() << std::endl;
}

SourceCode::SourceCode(const boost::filesystem::path& file)
    : m_file_delete_flag(false)
    , m_file(file)
{
}

SourceCode::~SourceCode()
{
    if (m_file_delete_flag)
    {
        //std::cout << "deteled " << m_file.string() << std::endl;
        boost::filesystem::remove(m_file);
    }
}

boost::filesystem::path SourceCode::getFilePath() const
{
    return m_file;
}

} // namespace Compiler
} // namespace SOME_NAME
