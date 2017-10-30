#include "executor/Executor.hpp"
#include <iostream>
#include <thread>
#include "compiler/Compiler.hpp"
#include "compiler/SourceCode.hpp"

int main(int argc, char** argv)
{    
    SOME_NAME::Compiler::SourceCode code(R"(
#include <iostream>

int sum(int a, int b) 
{
    return a + b;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        return 1;
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    std::cout << sum(a, b) << std::endl;
    return 0;
}
)", SOME_NAME::Compiler::ELanguage::CPP);

    auto executable = SOME_NAME::Compiler::Compiler(code).compile();
    executable->setArgs({"20", "-3"});
    SOME_NAME::Executor::Executor exec(executable);
    exec.addStartSlot([]() { std::cout << "main::started" << std::endl; });
    exec.addFinishedSlot([](std::int32_t exit_code) {
        std::cout << "main::finished with code " << exit_code << std::endl;
    });
    exec.addOutputSlot([](const std::string& str) { std::cout << "main::output " << str << std::endl; });
    exec.execute();
    exec.wait();
    std::cout << (exec.getOutput() == "17\n" ? "Success" : "Failed") << std::endl;
    return 0;
}
