#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include "settings_reader/Settings.hpp"
#include "filesystem/FSNavigation.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    AlgoVi::SettingsReader::CSettings settings(boost::filesystem::current_path() / ".settings.ini");
    if (argc == 1)
    {
        settings.set("test_folder", "tests");
    }
    else
    {
        std::string test_folder = argv[1];
        if (!boost::filesystem::exists(boost::filesystem::current_path() / test_folder))
        {
            std::cout << " [ Info ] test folder '" << test_folder << "' was created" << std::endl;
            boost::filesystem::create_directory(test_folder);
        }
        settings.set("test_folder", test_folder);
    }
    std::cout << "On test folder '" << settings.get<std::string>("test_folder") << "'" << std::endl;
    return 0;
}
