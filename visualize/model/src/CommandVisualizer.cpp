#include "visualize/model/CommandVisualizer.hpp"
#include <QPainter>
#include <QDebug>
#include <sstream>

namespace SOME_NAME {
namespace Visual {

CCommandVisualizer::CCommandVisualizer()
{
}

void CCommandVisualizer::visualize(QPainter* painter)
{
    std::vector<VizFunc> to_draw;
    {
        std::lock_guard<std::mutex> guard(m_access);
        to_draw = m_funcs;
    }

    for (const auto& f : to_draw)
    {
        f(painter);
    }
}

void CCommandVisualizer::updateData(const std::string& str)
{
    std::vector<VizFunc> to_draw;

    std::stringstream stream(str);
    std::string s;
    while (std::getline(stream, s))
    {
        try
        {
            to_draw.push_back(parseCmd(s));
        }
        catch(const std::runtime_error& e)
        {
            qDebug() << "error occured: " << e.what();
            to_draw.clear();
            break;
        }
    }

    std::lock_guard<std::mutex> guard(m_access);
    m_funcs.swap(to_draw);
}

CCommandVisualizer::VizFunc CCommandVisualizer::parseCmd(const std::string& s)
{
    const static auto invalid_func = VizFunc();

    std::stringstream stream(s);

    std::string cmd;
    stream >> cmd;
    if (cmd == "line")
    {
        double x[2], y[2];
        stream >> x[0] >> y[0] >> x[1] >> y[1];
        return [x, y](QPainter* painter) {
            painter->drawLine(QPointF(x[0], y[0]), QPointF(x[1], y[1]));
        };
    }
    else if (cmd == "circle")
    {
        double x, y, r;
        stream >> x >> y >> r;
        return [x, y, r](QPainter* painter) { painter->drawEllipse(QPointF(x, y), r, r); };
    }
    else if (cmd == "ellipse")
    {
        double x, y, rx, ry, angle;
        stream >> x >> y >> rx >> ry >> angle;
        angle = angle / acos(-1) * 180.0;
        return [x, y, rx, ry, angle](QPainter* painter) {
            painter->save();
            painter->translate(QPointF(x, y));
            painter->rotate(angle);
            painter->translate(QPointF(-x, -y));
            painter->drawEllipse(QPointF(x, y), rx, ry);
            painter->restore();
        };
    }
    else if (cmd == "rect")
    {
        double x[2], y[2];
        stream >> x[0] >> y[0];
        stream >> x[1] >> y[1];
        return [x, y](QPainter* painter) { painter->drawRect(x[0], y[0], x[1], y[1]); };
    }
    else if (cmd == "color")
    {
        int r, g, b;
        stream >> r >> g >> b;
        return [r, g, b](QPainter* painter) {
            QPen p = painter->pen();
            p.setColor(QColor(r, g, b));
            painter->setPen(p);
        };
    }
    else if (cmd == "width")
    {
        int val;
        stream >> val;
        return [val](QPainter* painter) {
            QPen p = painter->pen();
            p.setWidth(val);
            painter->setPen(p);
        };
    }
}

} // namespace Visual
} // namespace SOME_NAME
