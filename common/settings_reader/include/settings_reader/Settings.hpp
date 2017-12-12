#pragma once

#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace AlgoVi {
namespace SettingsReader {

class CSettings
{
public:
    CSettings(const boost::filesystem::path& p);
    ~CSettings();

    template <typename T>
    T get(const std::string& key);

    template <typename T>
    void set(const std::string& key, const T& value);

private:
    const boost::filesystem::path m_path;
    boost::property_tree::ptree m_tree;
};

inline CSettings::CSettings(const boost::filesystem::path& p)
    : m_path(p)
{
    boost::property_tree::read_ini(p.string(), m_tree);
}

inline CSettings::~CSettings()
{
    boost::property_tree::write_ini(m_path.string(), m_tree);
}

template <typename T>
inline T CSettings::get(const std::string& key)
{
    return m_tree.get<T>(key);
}

template <typename T>
inline void CSettings::set(const std::string& key, const T& value)
{
    m_tree.put(key, value);
}

} // namespace SettingsReader
} // namespace AlgoVi
