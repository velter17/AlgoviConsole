#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include "output_utils/PrintTable.hpp"
#include "test_archive/TestArchive.hpp"

using namespace AlgoVi;

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
    TestArchive::TestArchive archive(boost::filesystem::current_path() / "tests");
    if (archive.size() == 0)
    {
        std::cout << "Test archive is empty\n";
        return 1;
    }

    std::vector<std::size_t> to_print;
    if (argc > 1)
    {
        to_print = parseRange(argv[1]);
        auto itr = std::find_if_not(to_print.begin(), to_print.end(), [&](const std::size_t i) {
            return i <= archive.size() && i > 0;
        });
        if (itr != to_print.end())
        {
            throw std::runtime_error("Wrong test to print: " + std::to_string(*itr));
        }
    }
    else
    {
        to_print.resize(archive.size());
        std::iota(to_print.begin(), to_print.end(), 1);
    }

    for (auto t : to_print)
    {
        std::vector<Utils::Cell> head = {
            Utils::Cell().setData({std::to_string(t)}),
            Utils::Cell()
                .setAlign(Utils::EAlign::Center)
                .setColor(termcolor::cyan)
                .setData({"input"}),
            Utils::Cell()
                .setAlign(Utils::EAlign::Center)
                .setColor(termcolor::green)
                .setData({"output"}),
        };
        std::cout << Utils::getTable(
            Utils::Table{head,
                         {Utils::Cell(),
                          Utils::Cell().setData(split(archive[t - 1].input())),
                          Utils::Cell().setData(split(archive[t - 1].output()))}},
            termcolor::white);
    }
    return 0;
}
catch (const std::exception& e)
{
    std::cout << " [ Error ] " << e.what() << std::endl;
}
