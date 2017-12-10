#include <QApplication>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include "output_utils/PrintTable.hpp"
#include "test_archive/TestArchive.hpp"
#include "compiler/Compiler.hpp"
#include "executor/Executor.hpp"
#include "visualize/view/MainWindow.hpp"
#include "visualize/view/GraphicsScene.hpp"
#include "visualize/model/CommandVisualizer.hpp"

using namespace AlgoVi;

int main(int argc, char** argv) try
{
    TestArchive::TestArchive archive(boost::filesystem::current_path() / "tests");
    if (archive.size() == 0)
    {
        std::cout << "Test archive is empty\n";
        return 1;
    }
    if (argc < 2)
    {
        std::cout << "No test was specified\n";
        return 1;
    }

    auto test_num = atoi(argv[1]);

    if (test_num < 1 || test_num > archive.size())
    {
        std::cout << "No such test: " << test_num << std::endl;
        return 1;
    }
    --test_num;

    auto visualizer_code = boost::filesystem::current_path() / "visual.cpp";
    if (!boost::filesystem::is_regular_file(visualizer_code))
    {
        std::cout << "visual.cpp was not found " << std::endl;
        return 1;
    }

    auto visual_executable = Compiler::Compiler(Compiler::SourceCode(visualizer_code)).compile();
    visual_executable->setArgs(
        {archive[test_num].inputFile()->string(), archive[test_num].outputFile()->string()});
    auto visual_exec = std::make_shared<Executor::Executor>(visual_executable);

    visual_exec->execute();
    auto res = visual_exec->wait();
    if (res == 0)
    {
        QApplication app(argc, argv);
        Visual::CMainWindow win;
        Visual::CGraphicsScene scene;
        win.setScene(&scene);
        Visual::CCommandVisualizer visualizer;
        scene.addPainter(std::bind(
            &Visual::CCommandVisualizer::visualize, &visualizer, std::placeholders::_1));
        visualizer.updateData(visual_exec->getOutput());
        win.show();
        app.exec();
    }
    else
    {
        std::cout << "Cannon visualize, visualizer was finished with code " << res << std::endl;
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cout << " [ Error ] " << e.what() << std::endl;
}
