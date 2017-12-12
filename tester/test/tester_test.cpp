#include "executor/Executor.hpp"
#include <iostream>
#include <thread>
#include "compiler/Compiler.hpp"
#include "compiler/SourceCode.hpp"
#include "tester/Tester.hpp"
#include "test_archive/TestData.hpp"

using namespace AlgoVi;

int main(int argc, char** argv)
{    
    Compiler::SourceCode code_to_test(R"(
#include <iostream>

int sum(int a, int b) 
{
    return a + b;
}

int main(int argc, char** argv)
{
    int a, b;
    std::cin >> a >> b;
    std::cout << sum(a, b) << std::endl;
    return 0;
}
)", Compiler::ELanguage::CPP);

    Compiler::SourceCode checker_code(R"(
#include <iostream>
#include <fstream>
#include <string>

std::string readFromStream(std::istream&& stream)
{
    std::string ret, tmp;
    while (std::getline(stream, tmp))
    {
        ret += tmp + "\n";
    }
    if (!ret.empty())
    {
        ret.pop_back();
    }
    return ret;
}

int main(int argc, char** argv)
{
    bool res = readFromStream(std::ifstream(argv[2])) == readFromStream(std::ifstream(argv[3]));
    std::cout << (res ? "Ok, Ok" : "Not equal :(");
    return res ? 0 : 1;
}
)", Compiler::ELanguage::CPP);

    auto code_executable = Compiler::Compiler(code_to_test).compile();
    auto checker_executable = Compiler::Compiler(checker_code).compile();

    auto code_exec = std::make_shared<Executor::Executor>(code_executable);
    auto checker_exec = std::make_shared<Tester::Checker>(checker_executable);

    Tester::Tester tester(code_exec, checker_exec);
    auto test = TestArchive::TestData{"1, 2", "3"};
    tester.setTest(&test);

    auto res = tester.test();
    std::cout << (res.result == Tester::Tester::ETestResult::OK ? "OK" : "Not OK") << ":"
              << res.detailed_message << std::endl;
    return 0;
}
