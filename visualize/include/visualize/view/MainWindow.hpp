#pragma once

#include <QMainWindow>

class QGraphicsView;
class QGraphicsScene;

namespace SOME_NAME {
namespace Visual {

class CMainWindow : public QMainWindow
{
public:
    CMainWindow();
    ~CMainWindow();

    void setScene(QGraphicsScene* scene);

private:
    QGraphicsView* m_view;
    QLayout* m_main_layout;
};

} // namespace Visual
} // namespace SOME_NAME
