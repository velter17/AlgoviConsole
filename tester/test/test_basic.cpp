#include <iostream>
#include <sstream>
#include "filesystem/TempFile.hpp"
#include "tester/Test.hpp"

using namespace SOME_NAME;

int main()
{
    Tester::Test test;

    test = Tester::Test("some_input", "some_output");
    std::cout << (test.isEqual(Tester::Test{"some_input", "some_output"}) ? "Success" : "Failed")
              << std::endl;

    std::stringstream input("111");
    std::stringstream output("222");
    test = Tester::Test(std::move(input), std::move(output));
    std::cout << (test.isEqual(Tester::Test{"111", "222"}) ? "Success" : "Failed") << std::endl;

    test = Tester::Test(std::stringstream("dima"), std::stringstream("sadovyi"));
    std::cout << (test.isEqual(Tester::Test{"dima", "sadovyi"}) ? "Success" : "Failed")
              << std::endl;

    auto input_file = Filesystem::getTempFile(".in");
    auto output_file = Filesystem::getTempFile(".out");
    std::ofstream(input_file.string()) << "1 2 3";
    std::ofstream(output_file.string()) << "4 5 6";
    test = Tester::Test().readFromFiles(input_file, output_file);
    std::cout << (test.isEqual(Tester::Test{"1 2 3", "4 5 6"}) ? "Success" : "Failed") << std::endl;
    boost::filesystem::remove(input_file);
    boost::filesystem::remove(output_file);

    return 0;
}
