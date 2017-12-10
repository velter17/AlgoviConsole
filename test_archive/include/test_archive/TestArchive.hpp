#pragma once

#include <boost/filesystem/path.hpp>
#include "test_archive/Test.hpp"

namespace AlgoVi {
namespace TestArchive {

class TestArchive
{
public:
    TestArchive(const std::vector<Test>& tests);
    TestArchive(const boost::filesystem::path& test_folder);

    const Test& operator[](std::size_t i) const;
    Test& operator[](std::size_t i);
    std::size_t size() const;

private:
    std::vector<Test> m_tests;
};

} // namespace TestArchive
} // namespace AlgoVi
