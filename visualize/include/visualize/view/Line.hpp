#pragma once

#include <QGraphicsLineItem>

namespace AlgoVi {
namespace Visual {

class CLine : public QGraphicsLineItem
{
public:
    CLine(QGraphicsItem* from, QGraphicsItem* to, bool directed = false);
    CLine(QGraphicsItem* from, bool directed = false);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    QRectF boundingRect() const override;

private:
    QLineF line() const;

private:
    QGraphicsItem* m_from;
    QGraphicsItem* m_to;
    bool m_directed;
    QPointF m_temp_position;
};

} // namespace Visual
} // namespace AlgoVi
