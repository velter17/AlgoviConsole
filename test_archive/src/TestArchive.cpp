#include "test_archive/TestArchive.hpp"
#include <map>
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
            m_tests.push_back(Test(itr.second.first.get(), itr.second.second.get(), true, false));
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

} // namespace TestArchive
} // namespace AlgoVi
