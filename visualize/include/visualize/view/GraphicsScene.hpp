#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <functional>

namespace SOME_NAME {
namespace Visual {

class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraphicsScene();
    ~CGraphicsScene();

    void addPainter(std::function<void(QPainter* painter)> func);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    std::function<void(QPainter* painter)> m_draw_func;
};

} // namespace Visual
} // namespace SOME_NAME
