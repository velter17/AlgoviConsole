#pragma once

#include <string>
#include <fstream>

namespace AlgoVi {
namespace TestArchive {

class ITest
{
public:
    virtual const std::string& input() const = 0;
    virtual const std::string& output() const = 0;
    virtual ~ITest() = default;
};

} // namespace TestArchive
} // namespace AlgoVi
