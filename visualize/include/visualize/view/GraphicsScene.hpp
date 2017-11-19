#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <functional>
#include <memory>
#include "visualize/view/Point.hpp"

namespace SOME_NAME {
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
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    std::function<void(QPainter* painter)> m_draw_func;
    std::vector<std::shared_ptr<CPoint>> m_points;
};

} // namespace Visual
} // namespace SOME_NAME
