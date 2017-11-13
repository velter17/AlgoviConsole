#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "test_archive/TestArchive.hpp"
#include "filesystem/FSNavigation.hpp"

namespace po = boost::program_options;
using namespace SOME_NAME;

int main(int argc, char** argv)
{
    po::options_description desc("Algovi tester");
    desc.add_options()
        ("help,h", "help message")
        ("folder,f", po::value<std::string>()->default_value(
                         (boost::filesystem::current_path() / "tests").string()), 
            "folder with tests")
        ("count,c", "test count");

    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }
        po::notify(vm);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    boost::filesystem::path folder = vm["folder"].as<std::string>();
    if (!boost::filesystem::exists(folder))
    {
        return 1;
    }

    if (boost::filesystem::exists(folder))
    {
        TestArchive::TestArchive archive(folder);
        std::cout << archive.size() << std::endl;
    }
    else
    {
        return 1;
    }
    return 0;
}
