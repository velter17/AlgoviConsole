#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <functional>
#include <memory>
#include "visualize/view/Point.hpp"
#include "visualize/view/Line.hpp"

namespace AlgoVi {
namespace Visual {

class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraphicsScene();
    ~CGraphicsScene();

    void addPainter(std::function<void(QPainter* painter)> func);

    std::vector<QPointF> getPoints();

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    std::function<void(QPainter* painter)> m_draw_func;
    std::vector<std::shared_ptr<CPoint>> m_points;
    std::vector<std::shared_ptr<CLine>> m_lines;
    bool m_A_pressed;
};

} // namespace Visual
} // namespace AlgoVi
