#include "visualize/view/MainWindow.hpp"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLayout>
#include <QTextEdit>
#include <QPushButton>

namespace AlgoVi {
namespace Visual {

CMainWindow::CMainWindow()
{
    m_main_layout = new QHBoxLayout;
    m_view = new QGraphicsView();
    m_main_layout->addWidget(m_view);

    auto edit_layout = new QVBoxLayout;
    m_text_edit = new QTextEdit;
    edit_layout->addWidget(m_text_edit);
    m_text_edit->setEnabled(false);

    m_edit_button = new QPushButton("Edit");
    edit_layout->QLayout::addWidget(m_edit_button);
    m_edit_button->setCheckable(true);

    connect(m_edit_button, &QPushButton::clicked, [this](bool checked) {
        m_text_edit->setEnabled(checked);
        m_edit_button->setText(checked ? "Save" : "Edit");
        if (checked)
        {
            m_text_edit->setFocus();
        }
        else
        {
            emit textChanged(m_text_edit->toPlainText());
        }
    });

    m_main_layout->addItem(edit_layout);
    setCentralWidget(new QWidget());
    centralWidget()->setLayout(m_main_layout);

    setGeometry(geometry().x(), geometry().y(), 600, 400);
    setWindowTitle("AlgoVi");
}

CMainWindow::~CMainWindow()
{
    m_main_layout->deleteLater();
    m_text_edit->deleteLater();
}

void CMainWindow::setScene(QGraphicsScene* scene)
{
    m_view->setScene(scene);
}

} // namespace Visual
} // namespace AlgoVi
