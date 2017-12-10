#include "compiler/SourceCode.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include "filesystem/TempFile.hpp"
#include <QDateTime>
#include <QFileInfo>

namespace AlgoVi {
namespace Compiler {

SourceCode::SourceCode(const std::string& code, ELanguage lang)
    : m_file_delete_flag(true)
{
    assert( "Need support of other languages" && lang == ELanguage::CPP);
    auto path = Filesystem::getTempFile(".cpp");
    std::ofstream file(path.string());
    file << code;
    m_file = path;
}

SourceCode::SourceCode(const boost::filesystem::path& file)
    : m_file_delete_flag(false)
    , m_file(file)
    , m_update_time(new QDateTime())
    , m_file_info(new QFileInfo(QString::fromStdString(file.string())))
{
}

SourceCode::~SourceCode()
{
    if (m_file_delete_flag)
    {
        boost::filesystem::remove(m_file);
    }
}

boost::filesystem::path SourceCode::getFilePath() const
{
    return m_file;
}

bool SourceCode::isUpdated() 
{
    m_file_info->refresh();
    return m_update_time->toTime_t() != m_file_info->lastModified().toTime_t();
}

void SourceCode::update()
{
    *m_update_time = m_file_info->lastModified();
}

} // namespace Compiler
} // namespace AlgoVi
