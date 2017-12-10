#include "visualize/view/Line.hpp"
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <cmath>

namespace AlgoVi {
namespace Visual {

CLine::CLine(QGraphicsItem* from, QGraphicsItem* to, bool directed)
    : m_from(from)
    , m_to(to)
    , m_directed(directed)
{
    setLine(line());
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setAcceptHoverEvents(true);
    m_temp_position = from->scenePos() + QPointF(10, 10);
}

CLine::CLine(QGraphicsItem* from, bool directed)
    : CLine(from, nullptr, directed)
{
}

QLineF CLine::line() const
{
    return QLineF(m_from->scenePos(), m_to == nullptr ? m_temp_position : m_to->scenePos());
}

void CLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QStyleOptionGraphicsItem myoption(*option);
    myoption.state &= !QStyle::State_Selected;

    setLine(line());
    QGraphicsLineItem::paint(painter, &myoption, widget);

    QPen p = this->pen();
    p.setWidth(4);
    if (isSelected())
    {
        p.setColor(Qt::blue);
    }
    painter->setPen(p);

    painter->drawLine(line());
    if (m_directed && line().length() > 1e-9)
    {
        qreal arrow_size = 15;
        double angle = std::acos(line().dx() /  line().length());
        static const double s_pi = std::acos(-1);
        if (line().dy() >= 0.0)
        {
            angle = s_pi * 2 - angle;
        }
        QPointF a1 = line().p1() + QPointF{std::sin(angle + s_pi / 3) * arrow_size,
                                           std::cos(angle + s_pi / 3) * arrow_size};
        QPointF a2 = line().p1() + QPointF{std::sin(angle + s_pi - s_pi / 3) * arrow_size,
                                           std::cos(angle + s_pi - s_pi / 3) * arrow_size};
        painter->drawLine(line().p1(), a1);
        painter->drawLine(line().p1(), a2);
    }
}

QRectF CLine::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(
               line().p1(),
               QSizeF{line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y()})
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void CLine::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug () << "move";
    if (nullptr == m_to)
    {
        m_temp_position = event->scenePos();
        update();
    }
    //QGraphicsItem::mouseMoveEvent(event);
}

} // namespace Visual
} // namespace AlgoVi
