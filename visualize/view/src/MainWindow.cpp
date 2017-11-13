#include "visualize/view/MainWindow.hpp"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLayout>

namespace SOME_NAME {
namespace Visual {

CMainWindow::CMainWindow()
{
    m_main_layout = new QVBoxLayout;
    m_view = new QGraphicsView();
    m_main_layout->addWidget(m_view);

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(m_main_layout);

    setGeometry(geometry().x(), geometry().y(), 600, 400);
    setWindowTitle("AlgoVi");
}

CMainWindow::~CMainWindow()
{
    m_main_layout->deleteLater();
}

void CMainWindow::setScene(QGraphicsScene* scene)
{
    m_view->setScene(scene);
}

} // namespace Visual
} // namespace SOME_NAME
