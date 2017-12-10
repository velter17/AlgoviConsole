#include "visualize/view/GraphicsScene.hpp"
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

namespace AlgoVi {
namespace Visual {

CGraphicsScene::CGraphicsScene()
    : m_A_pressed(false)
{
    setSceneRect(0, 0, 1000, 1000);
}

CGraphicsScene::~CGraphicsScene()
{
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug () << "Scene pressed";
    if (event->button() == Qt::LeftButton)
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            m_points.push_back(std::make_shared<CPoint>(event->scenePos()));
            addItem(m_points.back().get());
            /*if (m_points.size() > 1)
            {
                auto sz = m_points.size();
                m_lines.push_back(std::make_shared<CLine>(
                    m_points[sz-1].get(), m_points[sz-2].get(), true));
                addItem(m_lines.back().get());
            }*/
        }
        else if (m_A_pressed)
        {
            qDebug () << "here";
            auto item = itemAt(event->scenePos(), QTransform());
            if (item)
            {
                auto check_point = [](QGraphicsItem* t) {
                    return dynamic_cast<CPoint*>(t) != nullptr;
                };
                if (check_point(item))
                {
                    qDebug () << "ADD!!!!";
                    m_lines.emplace_back(new CLine(item, false));
                    addItem(m_lines.back().get());
                    //QGraphicsScene::mousePressEvent(event);
                    m_lines.back()->grabMouse();
                }
            }
        }
        else
        {
            QGraphicsScene::mousePressEvent(event);
        }
    }
    else
    {
        QGraphicsScene::mousePressEvent(event);
    }
}

void CGraphicsScene::keyPressEvent(QKeyEvent* key)
{
    m_A_pressed = key->key() == Qt::Key_A;
}

void CGraphicsScene::keyReleaseEvent(QKeyEvent* key)
{
    m_A_pressed = false;
}

void CGraphicsScene::addPainter(std::function<void(QPainter* painter)> func)
{
    m_draw_func = func;
}

std::vector<QPointF> CGraphicsScene::getPoints()
{
    std::vector<QPointF> points;
    for (const auto& point : m_points)
    {
        points.push_back(point->scenePos() - QPointF(this->width() / 2, this->height() / 2));
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
} // namespace AlgoVi
