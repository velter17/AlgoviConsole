#include "visualize/view/Point.hpp"
#include <QBrush>
#include <QPainter>
#include <cmath>

namespace AlgoVi {
namespace Visual {

CPoint::CPoint(const QPointF& position)
{
    setPos(position);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setZValue(1);
}

void CPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();
    QBrush br;
    br.setColor(Qt::black);
    QPen p = painter->pen();
    p.setWidth(this->isSelected() ? 3 : 1);
    p.setColor(Qt::black);
    painter->setPen(p);
    painter->setBrush(br);
    painter->drawEllipse(QPoint(0, 0), 6, 6);
    if (!isSelected())
    {
        painter->drawEllipse(QPoint(0, 0), 3, 3);
    }

    painter->restore();
}

QRectF CPoint::boundingRect() const
{
    static const int margin = 5;
    return QRectF(QPoint(-6 - margin, -6 - margin), QPoint(6 + margin, 6 + margin));
}

} // namespace Visual
} // namespace AlgoVi
