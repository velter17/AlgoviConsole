#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include "output_utils/PrintTable.hpp"
#include "test_archive/TestArchive.hpp"
#include "settings_reader/Settings.hpp"

using namespace AlgoVi;
namespace po = boost::program_options;

std::vector<std::string> split(const std::string& s)
{
    const std::size_t width_max = 50;
    const std::size_t height_max = 50;
    std::stringstream str(s);
    std::vector<std::string> ret;
    std::string temp;
    int additional_lines = 0;
    while (std::getline(str, temp))
    {
        if (temp.length() > width_max)
        {
            temp = temp.substr(0, width_max) + " ... [ +" + std::to_string(temp.length() - width_max) + " ] ";
        }
        if (ret.size() < height_max)
        {
            ret.push_back(temp);
        }
        else
        {
            ++additional_lines;
        }
    }
    if (additional_lines > 0)
    {
        ret.push_back("...");
        ret.push_back("[ + " + std::to_string(additional_lines) + " lines ]");
    }
    return ret;
}

std::vector<std::size_t> parseRange(const std::string& s)
{
    std::vector<std::size_t> ret;

    std::size_t i = 0;
    std::size_t x = 0;
    bool is_dash = false;
    while (i < s.length())
    {
        char c = s[i++];
        if (c == ',' || c == '-' || i == s.length())
        {
            if (i == s.length())
            {
                if (!std::isdigit(c))
                {
                    throw std::runtime_error(
                        std::string("Unrecognized character ") + c + " is test range");
                }
                x = x * 10 + c - '0';
            }
            ret.push_back(x);
            x = 0;
        }
        else if (std::isdigit(c))
        {
            x = x * 10 + c - '0';
        }
        else
        {
            throw std::runtime_error(std::string("Unrecognized character ") + c + " is test range");
        }
        is_dash |= c == '-';
    }
    if (ret.size() == 2 && is_dash)
    {
        auto t(std::move(ret));
        if (t.front() > t.back())
        {
            throw std::runtime_error("Invalid test range");
        }
        ret.resize(std::min<std::size_t>(100, t.back() - t.front() + 1));
        std::iota(ret.begin(), ret.end(), t.front());
    }
    return ret;
}

int main(int argc, char** argv) try
{
    po::options_description desc("Algovi tester");
    desc.add_options()
        ("help,h", "help message")
        ("all,a", "remove all")
        ("test,t", po::value<std::string>(), "tests to remove");

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

    SettingsReader::CSettings settings(boost::filesystem::current_path() / ".settings.ini");
    if (settings.get<std::string>("test_folder") != "tests")
    {
        std::cout << " [ ! ] Your test directory is '" << settings.get<std::string>("test_folder")
                  << "'" << std::endl;
    }

    TestArchive::TestArchive archive(
        boost::filesystem::current_path() / settings.get<std::string>("test_folder"));

    std::vector<std::size_t> to_remove;
    if (vm.count("all"))
    {
        for(std::size_t i = 0; i < archive.size(); ++i)
        {
            to_remove.push_back(i + 1);
        }
    }
    else if(vm.count("test"))
    {
        to_remove = parseRange(vm["test"].as<std::string>());
    }
    else
    {
        std::cout << " [ Error ] You didn't specify any tests" << std::endl;
        return 1;
    }

    std::sort(to_remove.begin(), to_remove.end());
    to_remove.resize(std::unique(to_remove.begin(), to_remove.end()) - to_remove.begin());

    auto itr = std::find_if_not(to_remove.begin(), to_remove.end(), [&](const std::size_t i) {
        return i <= archive.size() && i > 0;
    });
    if (itr != to_remove.end())
    {
        throw std::runtime_error("Wrong test to remove: " + std::to_string(*itr));
    }

    for (auto t : to_remove)
    {
        archive[t - 1].remove();
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cout << " [ Error ] " << e.what() << std::endl;
}
