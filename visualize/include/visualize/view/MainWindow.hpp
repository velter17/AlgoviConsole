#pragma once

#include <QMainWindow>

class QGraphicsView;
class QGraphicsScene;
class QTextEdit;
class QPushButton;

namespace AlgoVi {
namespace Visual {

class CMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    CMainWindow();
    ~CMainWindow();

    void setScene(QGraphicsScene* scene);

signals:
    void textChanged(const QString& text);

private:
    QGraphicsView* m_view;
    QTextEdit* m_text_edit;
    QLayout* m_main_layout;
    QPushButton* m_edit_button;
};

} // namespace Visual
} // namespace AlgoVi
