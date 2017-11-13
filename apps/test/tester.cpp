#include "tester/Tester.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include "compiler/Compiler.hpp"
#include "test_archive/TestArchive.hpp"

namespace po = boost::program_options;
using namespace SOME_NAME;

int main(int argc, char** argv)
{
    po::options_description desc("Algovi tester");
    desc.add_options()
        ("help,h", "help message")
        ("src,s", po::value<std::string>()->default_value("code.cpp"), "path to source code to test")
        ("checker,c", po::value<std::string>()->default_value("testlib_wcmp"), "path to checker")
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
    std::string test_folder = vm["test"].as<std::string>();

    auto code_executable = Compiler::Compiler(Compiler::SourceCode(code_path)).compile();
    auto checker_executable = Compiler::Compiler(Compiler::SourceCode(checker_path)).compile();
    
    auto code_exec = std::make_shared<Executor::Executor>(code_executable);
    auto checker_exec = std::make_shared<Tester::Checker>(checker_executable);

    TestArchive::TestArchive tests(test_folder);
    std::cout << tests.size() << " tests was read" << std::endl;

    Tester::Tester tester(code_exec, checker_exec);

    for (std::size_t i = 0; i < tests.size(); ++i)
    {
        std::cout << "Test #" << i + 1 << std::endl;
        tester.setTest(tests[i]);
        auto res = tester.test();
        printf(
            "%s: %s\n",
            res.result == Tester::Tester::ETestResult::OK ? "OK" : "WA",
            res.detailed_message.data());
    }

    return 0;
}
