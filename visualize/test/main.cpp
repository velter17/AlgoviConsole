#include <QApplication>
#include <iostream>
#include "visualize/view/MainWindow.hpp"
#include "visualize/view/GraphicsScene.hpp"
#include "visualize/model/CommandVisualizer.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    SOME_NAME::Visual::CMainWindow win;
    SOME_NAME::Visual::CGraphicsScene scene;
    win.setScene(&scene);
    SOME_NAME::Visual::CCommandVisualizer visualizer;
    scene.addPainter(std::bind(
        &SOME_NAME::Visual::CCommandVisualizer::visualize, &visualizer, std::placeholders::_1));
    visualizer.updateData("line 100 100 150 150\n"
                          "rect 80 80 160 160");
    win.show();
    return app.exec();
}
