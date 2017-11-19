#include "visualize/view/GraphicsScene.hpp"
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

namespace SOME_NAME {
namespace Visual {

CGraphicsScene::CGraphicsScene()
{
    setSceneRect(0, 0, 1000, 1000);
}

CGraphicsScene::~CGraphicsScene()
{
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_points.push_back(std::make_shared<CPoint>(event->scenePos()));
    addItem(m_points.back().get());
    QGraphicsScene::mousePressEvent(event);
}

void CGraphicsScene::addPainter(std::function<void(QPainter* painter)> func)
{
    m_draw_func = func;
}

std::vector<QPointF> CGraphicsScene::getPoints()
{
    std::vector<QPointF> points;
    for(QGraphicsItem* item : this->items(Qt::SortOrder::AscendingOrder))
    {
        points.push_back(item->scenePos());
    }
    return points;
}

void CGraphicsScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    QGraphicsScene::drawForeground(painter, rect);
    painter->save();
    painter->translate(QPointF(500, 500));
    if (m_draw_func)
    {
        m_draw_func(painter);
    }
    painter->restore();
}

} // namespace Visual
} // namespace SOME_NAME
