#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "test_archive/TestArchive.hpp"
#include "filesystem/FSNavigation.hpp"
#include "termcolor/termcolor.hpp"
#include "settings_reader/Settings.hpp"

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
    std::string test_folder =
        SettingsReader::CSettings(boost::filesystem::current_path() / ".settings.ini")
            .get<std::string>("test_folder");
    std::cout << "Your current test folder is '" << test_folder << "'" << std::endl;
    if (boost::filesystem::exists(path / test_folder))
    {
        TestArchive::TestArchive archive(path / test_folder);
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
