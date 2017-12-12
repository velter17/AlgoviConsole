#include <iostream>
#include <sstream>
#include "filesystem/TempFile.hpp"
#include "test_archive/TestData.hpp"

using namespace AlgoVi;
using Test = TestArchive::TestData;

int main()
{/* FIXME
    Test test;

    test = Test("some_input", "some_output");
    std::cout << (test.isEqual(Test{"some_input", "some_output"}) ? "Success"
                                                                               : "Failed")
              << std::endl;

    std::stringstream input("111");
    std::stringstream output("222");
    test = Test(std::move(input), std::move(output));
    std::cout << (test.isEqual(TestArchive::Test{"111", "222"}) ? "Success" : "Failed")
              << std::endl;

    test = TestArchive::Test(std::stringstream("dima"), std::stringstream("sadovyi"));
    std::cout << (test.isEqual(TestArchive::Test{"dima", "sadovyi"}) ? "Success" : "Failed")
              << std::endl;

    auto input_file = Filesystem::getTempFile(".in");
    auto output_file = Filesystem::getTempFile(".out");
    std::ofstream(input_file.string()) << "1 2 3";
    std::ofstream(output_file.string()) << "4 5 6";
    test = TestArchive::Test(input_file, output_file, true);
    std::cout << (test.isEqual(TestArchive::Test{"1 2 3", "4 5 6"}) ? "Success" : "Failed")
              << std::endl;
    boost::filesystem::remove(input_file);
    boost::filesystem::remove(output_file);
*/
    return 0;
}
