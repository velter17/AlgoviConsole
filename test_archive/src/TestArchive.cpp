#include "test_archive/TestArchive.hpp"
#include <map>
#include <algorithm>
#include "filesystem/FSNavigation.hpp"

namespace AlgoVi {
namespace TestArchive {

TestArchive::TestArchive(const std::vector<Test>& tests)
    : m_tests(tests)
{
}

TestArchive::TestArchive(const boost::filesystem::path& test_folder)
{
    auto files = Filesystem::fileList(test_folder);
    std::map<
        std::size_t,
        std::pair<
            boost::optional<boost::filesystem::path>,
            boost::optional<boost::filesystem::path>>>
        mp;

    for (auto file : files)
    {
        if (Filesystem::getFileExtension(file) == ".dat")
        {
            auto test_idx = std::stoi(Filesystem::getFileName(file));
            mp[test_idx].first = file;
        }
        else if (Filesystem::getFileExtension(file) == ".ans")
        {
            auto test_idx = std::stoi(Filesystem::getFileName(file));
            mp[test_idx].second = file;
        }
    }

    for (const auto& itr : mp)
    {
        if (itr.second.first && itr.second.second)
        {
            m_tests.emplace_back(
                itr.second.first.get(),
                itr.second.second.get());
                //ETestType::KeepInFiles);
        }
    }
}

const Test& TestArchive::operator[](std::size_t i) const
{
    return m_tests[i];
}

Test& TestArchive::operator[](std::size_t i)
{
    return m_tests[i];
}

std::size_t TestArchive::size() const
{
    return m_tests.size();
}

void TestArchive::clear()
{
    m_removed_list.clear();
    for (std::size_t i = 0; i < size(); ++i)
    {
        m_removed_list.push_back(i);
    }
    shrink();
}

void TestArchive::shrink()
{
    if (m_removed_list.empty())
    {
        return;
    }

    std::sort(m_removed_list.begin(), m_removed_list.end());

    std::vector<Test> new_tests;
    std::size_t j = 0;
    for (std::size_t i = 0; i < size(); ++i)
    {
        bool rm = j < m_removed_list.size() && m_removed_list[j] == i;
        if (rm)
        {
            m_tests[i].remove();
            ++j;
        }
        else
        {
            new_tests.push_back(m_tests[i]);
            new_tests.back().move(new_tests.size() + 1);
        }
    }

    m_tests.swap(new_tests);
}

} // namespace TestArchive
} // namespace AlgoVi
