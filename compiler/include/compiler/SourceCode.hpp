#pragma once

#include <boost/filesystem/path.hpp>
#include "compiler/Language.hpp"

class QDateTime;
class QFileInfo;

namespace AlgoVi {
namespace Compiler {

class SourceCode
{
public:
    SourceCode(const std::string& code, ELanguage lang);
    SourceCode(const boost::filesystem::path& file);
    ~SourceCode();

    boost::filesystem::path getFilePath() const;

    bool isUpdated();
    void update();

private:
    bool m_file_delete_flag;
    boost::filesystem::path m_file;
    std::shared_ptr<QDateTime> m_update_time;
    std::shared_ptr<QFileInfo> m_file_info;
};

} // namespace Compiler
} // namespace AlgoVi
