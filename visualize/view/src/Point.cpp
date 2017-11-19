#include "visualize/view/Point.hpp"
#include <QBrush>
#include <QPainter>

namespace SOME_NAME {
namespace Visual {

CPoint::CPoint(const QPointF& position)
{
    setPos(position);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void CPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QBrush br;
    br.setColor(Qt::black);
    painter->setBrush(br);
    painter->drawEllipse(QPoint(0, 0), 6, 6);
}

QRectF CPoint::boundingRect() const
{
    static const int margin = 5;
    return QRectF(QPoint(-6 - margin, -6 - margin), QPoint(6 + margin, 6 + margin));
}

} // namespace Visual
} // namespace SOME_NAME
