#include <atomic>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <condition_variable>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <thread>
#include "compiler/Compiler.hpp"
#include "output_utils/PrintTable.hpp"
#include "termcolor/termcolor.hpp"
#include "test_archive/TestArchive.hpp"
#include "tester/Validator.hpp"

namespace po = boost::program_options;
using namespace AlgoVi;

std::atomic_bool s_done;
bool s_was_terminated;

void handlesigterm(int)
{
    s_was_terminated = true;
    s_done.store(true);
}

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
            temp = temp.substr(0, width_max) + " ... [ +" +
                   std::to_string(temp.length() - width_max) + " ] ";
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
    std::signal(SIGINT, handlesigterm);
    std::signal(SIGTERM, handlesigterm);

    po::options_description desc("Algovi tester");
    desc.add_options()("help,h", "help message")
        ("src,s", po::value<std::string>()->default_value("validator.cpp"),
            "path to source code of validator")
        ("test-folder,f", po::value<std::string>()->default_value("tests"), "path to test folder")
        ("test,t", po::value<std::string>()->default_value("all"), "test on specific tests")
        ("verbose", po::value<int>()->default_value(0), "detailed test report\n 0 - no verbose\n 1 - oneline\n 2 - tabled\n 3 - with test data");

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
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::string code_path = vm["src"].as<std::string>();
    std::string test_folder = vm["test-folder"].as<std::string>();

    auto code_executable = Compiler::Compiler(Compiler::SourceCode(code_path)).compile();
    //auto code_exec = std::make_shared<Executor::Executor>(code_executable);

    TestArchive::TestArchive tests(test_folder);

    Tester::Validator validator(code_executable);

    std::vector<std::size_t> tests_to_test;
    if (vm["test"].as<std::string>() == "all")
    {
        tests_to_test.resize(tests.size());
        std::iota(tests_to_test.begin(), tests_to_test.end(), 1);
    }
    else
    {
        tests_to_test = parseRange(vm["test"].as<std::string>());
    }

    int verbose = vm["verbose"].as<int>();

    std::cout << " Running..." << std::endl;

    //std::vector<std::pair<std::size_t, Tester::Tester::ResultPair >> result_vec;

    std::thread test_thread([&]() {
        for (std::size_t i = 0; i < tests_to_test.size() && s_done.load() == false; ++i)
        {
            if (!verbose)
            {
                std::cout << "\r Running on test " << tests_to_test[i] << " .. " << std::flush;
            }
            validator.setTest(&tests[tests_to_test[i] - 1]);
            validator.execute();
            auto code = validator.wait();
            auto message = validator.getOutput();
            //if (code != 0)
            //{
            //    auto message = validator.getOutput();
            //    std::cout << message << std::endl;
            //}

            if (s_done.load())
            {
                break;
            }

            if (verbose)
            {
                if (verbose == 1)
                {
                    std::cout << std::setw(3) << std::right << tests_to_test[i];
                    std::cout << ": ";
                    if (code == 0)
                    {
                        std::cout << termcolor::green << "Passed" << termcolor::reset;
                    }
                    else
                    {
                        std::cout << termcolor::bred << "Not passed" << termcolor::reset;
                    }
                    std::cout << ", ( " << message << " )" << std::endl;
                }
                else if (verbose == 2 || verbose == 3)
                {
                    std::vector<Utils::Cell> head = {
                        Utils::Cell().setData({std::to_string(tests_to_test[i])}),
                        Utils::Cell()
                            .setAlign(Utils::EAlign::Center)
                            .setColor(termcolor::bcyan)
                            .setData({"input"}),
                        Utils::Cell()
                            .setAlign(Utils::EAlign::Center)
                            .setColor(termcolor::bgreen)
                            .setData({"output"}),
                    };
                    auto table = Utils::getTable(
                        Utils::Table{
                            head,
                            {Utils::Cell(),
                             Utils::Cell().setData(split(tests[tests_to_test[i] - 1].input())),
                             Utils::Cell().setData(split(tests[tests_to_test[i] - 1].output()))},
                            {Utils::Cell()
                                 .setData({code == 0
                                               ? "Passed"
                                               : "Not passed"})
                                 .setColor(
                                     code == 0 ? termcolor::bgreen : termcolor::bred)
                                 .setCollspan(1),
                             Utils::Cell()
                                 .setData({message})
                                 .setCollspan(2)},
                        },
                        termcolor::white);
                    std::cout << table << std::endl;
                }
            }
            else if (code != 0) 
            {
                std::cout << std::endl;
                std::cout << termcolor::bred << "Not passed" << termcolor::reset << " on test "
                          << i + 1 << std::flush;
                break;
            }
        }
        s_done.store(true);
    });

    while (s_done.load() == false)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    test_thread.join();
    return 0;
}
catch (const Compiler::CompilationError& e)
{
    std::cout << termcolor::bred << "Cannot compile " << e.getFile() << termcolor::reset
              << std::endl;
    std::cout << "Do you want to see compiler output? [ y / n ]" << std::endl;
    std::string ans;
    std::cin >> ans;
    if (ans == "y")
    {
        std::cout << termcolor::red << e.what() << termcolor::reset << std::endl;
    }
    return 1;
}
