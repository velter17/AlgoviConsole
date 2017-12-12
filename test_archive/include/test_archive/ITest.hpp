#pragma once

#include <string>
#include <fstream>

namespace AlgoVi {
namespace TestArchive {

class ITest
{
public:
    virtual const std::string& input() = 0;
    virtual const std::string& output() = 0;
    virtual ~ITest() = default;
};

} // namespace TestArchive
} // namespace AlgoVi
