#include "visualize/view/GraphicsScene.hpp"
#include <QPainter>
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
    QGraphicsScene::mousePressEvent(event);
}

void CGraphicsScene::addPainter(std::function<void(QPainter* painter)> func)
{
    m_draw_func = func;
}

void CGraphicsScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    qDebug () << "CGraphicsScene::drawForeground";
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
