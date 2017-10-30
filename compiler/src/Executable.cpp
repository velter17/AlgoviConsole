#include "compiler/Executable.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace SOME_NAME {
namespace Compiler {

Executable::Executable(boost::optional<boost::filesystem::path> file)
    : m_file(file)
    , m_file_delete_flag(false)
{
}

Executable::Executable(Executable&& other)
    : m_file(std::move(other.m_file))
    , m_args(std::move(other.m_args))
{
    if (other.isDeleteAfter())
    {
        this->deleteAfter();
        other.deleteAfter(false);
    }
    else
    {
        this->deleteAfter(false);
    }
}

Executable::~Executable()
{
    if (m_file_delete_flag)
    {
        std::cout << "remove " << m_file->string() << std::endl;
        boost::filesystem::remove(*m_file);
    }
}

Executable& Executable::setFile(boost::filesystem::path file)
{
    std::cout << "Executable set file" << std::endl;
    m_file = file;
    return *this;
}

Executable& Executable::setArgs(const std::vector<std::string>& args)
{
    m_args = args;
    return *this;
}

Executable& Executable::appendArgs(const std::vector<std::string>& args)
{
    m_args.insert(m_args.end(), args.begin(), args.end());
    return *this;
}

Executable& Executable::deleteAfter(bool value)
{
    std::cout << "created " << m_file->string() << std::endl;
    m_file_delete_flag = value;
    return *this;
}

bool Executable::isDeleteAfter() const
{
    return m_file_delete_flag;
}

bool Executable::isValid() const
{
    return m_file.is_initialized();
}

boost::filesystem::path Executable::getAppPath() const
{
    return m_file.get();
}

const std::vector<std::string>& Executable::getArgs() const
{
    return m_args;
}

} // namespace Compiler
} // namespace SOME_NAME
