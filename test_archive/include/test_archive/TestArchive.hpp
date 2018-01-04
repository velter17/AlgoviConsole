#pragma once

#include <boost/filesystem/path.hpp>
#include "test_archive/TestFromFile.hpp"

namespace AlgoVi {
namespace TestArchive {

using Test = TestFromFile;

class TestArchive
{
public:
    TestArchive(const std::vector<Test>& tests);
    TestArchive(const boost::filesystem::path& test_folder);
    ~TestArchive();

    const Test& operator[](std::size_t i) const;
    Test& operator[](std::size_t i);
    std::size_t size() const;
    void clear();
    void append(const ITest& test);
    void remove(std::size_t idx); // 0..n-1

private:
    void shrink();

private:
    std::vector<Test> m_tests;
    std::vector<std::size_t> m_removed_list;
    boost::filesystem::path m_test_folder;
};

} // namespace TestArchive
} // namespace AlgoVi
