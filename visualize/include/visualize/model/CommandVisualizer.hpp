#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <vector>
#include "visualize/model/IVisualizer.hpp"

namespace AlgoVi {
namespace Visual {

class CCommandVisualizer : public IVisualizer
{
public:
    using VizFunc = std::function<void(QPainter*)>;
public:
    CCommandVisualizer();

    void visualize(QPainter* painter) override;

    void updateData(const std::string& data);

private:
    VizFunc parseCmd(const std::string& s);

private:
    std::mutex m_access;
    std::vector<VizFunc> m_funcs;
};

} // namespace Visual
} // namespace AlgoVi
