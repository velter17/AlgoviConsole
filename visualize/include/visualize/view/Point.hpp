#pragma once

#include <QGraphicsItem>
#include <QObject>

namespace SOME_NAME {
namespace Visual {

class CPoint
    : public QObject
    , public QGraphicsItem
{
    Q_OBJECT
public:
    CPoint(const QPointF& position);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;
};

} // namespace Visual
} // namespace SOME_NAME
