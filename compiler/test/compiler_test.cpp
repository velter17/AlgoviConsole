#include "compiler/Compiler.hpp"

int main(int argc, char** argv)
{
    AlgoVi::Compiler::SourceCode code(R"(
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
)", AlgoVi::Compiler::ELanguage::CPP);
    AlgoVi::Compiler::Compiler comp(code);
    return 0;
}
