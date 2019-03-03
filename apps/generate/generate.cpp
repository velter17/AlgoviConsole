#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include <ctime>
#include "output_utils/PrintTable.hpp"
#include "test_archive/TestArchive.hpp"
#include "test_archive/TestData.hpp"
#include "settings_reader/Settings.hpp"
#include "compiler/Compiler.hpp"
#include "executor/Executor.hpp"

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
    srand(time(0));

    SettingsReader::CSettings settings(boost::filesystem::current_path() / ".settings.ini");
    std::string code_path = "code.cpp";
    std::string generator_path = "generator.cpp";
    std::vector<std::string> args;
    std::uint32_t tests_to_generate;
    std::string destination_test_folder = settings.get<std::string>("test_folder");

    po::options_description desc("Algovi test generator");
    desc.add_options()
        ("help,h", "help message")
        ("src,s", po::value<std::string>(&code_path)->default_value(code_path), "correct solution code")
        ("generator,g", po::value<std::string>(&generator_path)->default_value(generator_path), "generator code path")
        ("arg,a", po::value<std::vector<std::string>>(&args), "arguments for generator")
        ("tests,t", po::value<std::uint32_t>(&tests_to_generate)->default_value(1), "tests to generate")
        ("dest,d", po::value<std::string>(&destination_test_folder)->default_value(destination_test_folder),
             "switch into test folder and save test");

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

    TestArchive::TestArchive archive(boost::filesystem::current_path() / destination_test_folder);
    if (archive.size() != 0)
    {
        std::cout << "Test folder '" << destination_test_folder << "' is not empty\n";
        std::cout << " 1 - append generated tests" << std::endl;
        std::cout << " 2 - clear folder" << std::endl;
        std::cout << " 3 - cancel" << std::endl;
        std::string ans;
        std::cin >> ans;
        if (ans.length() == 1 && ans[0] >= '1' && ans[0] <= '2')
        {
            if (ans == "2")
            {
                if (destination_test_folder == "tests")
                {
                    std::cout << "'tests' folder shouldn't been cleared that way. Please, use 'test --del' instead" << std::endl;
                    return 1;
                }
                archive.clear();
            }
            else
            {
                //
            }
        }
        else
        {
            std::cout << " [ Canceled ] " << std::endl;
            return 0;
        }
    }

    auto code_executable = Compiler::Compiler(Compiler::SourceCode(code_path)).compile();
    auto generator_executable = Compiler::Compiler(Compiler::SourceCode(generator_path)).compile();

    auto code_exec = std::make_shared<Executor::Executor>(code_executable);
    auto gen_exec = std::make_shared<Executor::Executor>(generator_executable);

    for (std::size_t i = 0; i < tests_to_generate; ++i)
    {
        std::int32_t rnd_num = (rand() << 16) & rand();
        generator_executable->setArgs({std::to_string(rnd_num)});
        generator_executable->appendArgs(args);

        gen_exec->execute();
        auto ret_code = gen_exec->wait();
        if (ret_code == 0)
        {
            const auto& test_in = gen_exec->getOutput();
            code_exec->setInput(test_in);
            code_exec->execute();
            ret_code = code_exec->wait();
            if (ret_code == 0)
            {
                archive.append(TestArchive::TestData(test_in, code_exec->getOutput()));
                std::cout << "Test was appended" << std::endl;
            }
            else
            {
                std::cout << "Application was finished with code " << ret_code << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << "Generator was finished with code " << ret_code << std::endl;
            return 1;
        }
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cout << " [ Error ] " << e.what() << std::endl;
}
