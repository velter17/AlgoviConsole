#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "test_archive/TestArchive.hpp"
#include "filesystem/FSNavigation.hpp"
#include "termcolor/termcolor.hpp"

namespace po = boost::program_options;
using namespace AlgoVi;

int main(int argc, char** argv)
{
    if (argc > 2)
    {
        std::cout << "Wrong arguments for status command" << std::endl;
        return 1;
    }
    auto path =
        Filesystem::getAbsolutePath(argc == 2 ? argv[1] : boost::filesystem::current_path());
    if (boost::filesystem::exists(path / "tests"))
    {
        TestArchive::TestArchive archive(path / "tests");
        std::stringstream x;
        std::cout << "Test archive size is " << archive.size() << std::endl;
    }
    else
    {
        std::cout << "No test directory was found\n";
        return 1;
    }
    return 0;
}
