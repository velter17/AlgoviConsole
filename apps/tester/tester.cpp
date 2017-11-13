#include "tester/Tester.hpp"
#include <boost/program_options.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "compiler/Compiler.hpp"
#include "test_archive/TestArchive.hpp"
#include "termcolor/termcolor.hpp"
#include "output_utils/PrintTable.hpp"

namespace po = boost::program_options;
using namespace SOME_NAME;

int main(int argc, char** argv)
{
    po::options_description desc("Algovi tester");
    desc.add_options()
        ("help,h", "help message")
        ("src,s", po::value<std::string>()->default_value("code.cpp"), "path to source code to test")
        ("checker,c", po::value<std::string>()->default_value(
            boost::filesystem::exists("checker.cpp") ? "checker.cpp" : "testlib_wcmp"), "path to checker")
        ("test,t", po::value<std::string>()->default_value("tests"), "path to test folder");

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

    std::string code_path = vm["src"].as<std::string>();
    std::string checker_path = vm["checker"].as<std::string>();
    if (boost::starts_with(checker_path, "testlib_"))
    {
        checker_path = (boost::filesystem::path("/opt/algovi/checkers/") / checker_path).string();
    }
    std::string test_folder = vm["test"].as<std::string>();

    auto code_executable = Compiler::Compiler(Compiler::SourceCode(code_path)).compile();
    auto checker_executable = Compiler::Compiler(Compiler::SourceCode(checker_path)).compile();
    
    auto code_exec = std::make_shared<Executor::Executor>(code_executable);
    auto checker_exec = std::make_shared<Tester::Checker>(checker_executable);

    TestArchive::TestArchive tests(test_folder);

    Tester::Tester tester(code_exec, checker_exec);

    std::cout << " Running...";

    Tester::Tester::ResultPair result;
    for (std::size_t i = 0; i < tests.size(); ++i)
    {
        std::cout << "\r Running on test " << i + 1 << " .. " << std::flush;
        tester.setTest(tests[i]);
        result = tester.test();
        if (result.result != Tester::Tester::ETestResult::OK)
        {
            std::cout << std::endl;
            std::cout << termcolor::bred << "Wrong answer" << termcolor::reset << " on test " << i + 1 << std::flush;
            //std::cout << "[ Checker message ]: " << res.detailed_message << std::endl;
            break;
        }
    }
    std::cout << std::string(20, ' ') << " Testing process was finished" << std::endl;

    auto table = Utils::Table(
        {{Utils::Cell().setData({"Verdict"}),
          Utils::Cell()
              .setData(
                  {result.result == Tester::Tester::ETestResult::OK ? "Accepted" : "WrongAnswer"})
              .setColor(
                  result.result == Tester::Tester::ETestResult::OK ? termcolor::bgreen
                                                                   : termcolor::bred)},
         {Utils::Cell().setData({"Time"}),
          Utils::Cell().setData({"128ms"}).setColor(termcolor::bblue)}});
    if (result.result != Tester::Tester::ETestResult::OK)
    {
        table.push_back({Utils::Cell().setData({"Checker message"}),
                         Utils::Cell().setData({result.detailed_message})});
    }
    std::cout << Utils::getTable(table) << std::endl;

    return 0;
}
