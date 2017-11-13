#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    po::options_description desc("Algovi tester");
    desc.add_options()
        ("help,h", "help message");

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

    auto current_folder = boost::filesystem::current_path();
    if (boost::filesystem::exists(current_folder / "tests"))
    {
        std::cout << "Tests structure was already created\n"
                     "Try 'algovi test status' to see more details"
                  << std::endl;
    }
    else
    {
        boost::filesystem::create_directory(current_folder / "tests");
        std::cout << "Empty 'tests/' directory was created" << std::endl;
    }

    return 0;
}
