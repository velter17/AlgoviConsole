#include <QApplication>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include <thread>
#include "compiler/Compiler.hpp"
#include "executor/Executor.hpp"
#include "filesystem/FileIO.hpp"
#include "filesystem/TempFile.hpp"
#include "output_utils/PrintTable.hpp"
#include "test_archive/TestArchive.hpp"
#include "visualize/model/CommandVisualizer.hpp"
#include "visualize/view/GraphicsScene.hpp"
#include "visualize/view/MainWindow.hpp"

using namespace AlgoVi;

namespace {

struct Application
{
    Compiler::Compiler compiler;
    std::unique_ptr<Executor::Executor> executor;

    Application(const boost::filesystem::path& source_code)
        : compiler(source_code)
    {
    }

    void recompile()
    {
        if (compiler.isNeededCompilation())
        {
            std::cout << "need compilation ! " << std::endl;
            executor.reset(new Executor::Executor(compiler.compile()));
        }
    }

    Executor::Executor* exe()
    {
        recompile();
        return executor.get();
    }
};

} // namespace

int main(int argc, char** argv) try
{
    auto visualizer_code = boost::filesystem::current_path() / "visual.cpp";
    if (!boost::filesystem::is_regular_file(visualizer_code))
    {
        std::cout << "visual.cpp was not found " << std::endl;
        return 1;
    }

    auto serializer_code = boost::filesystem::current_path() / "serialize.cpp";
    if (!boost::filesystem::is_regular_file(serializer_code))
    {
        std::cout << "serialize.cpp was not found " << std::endl;
        return 1;
    }

    auto algo_code = boost::filesystem::current_path() / "code.cpp";
    if (!boost::filesystem::is_regular_file(algo_code))
    {
        std::cout << "code.cpp was not found " << std::endl;
        return 1;
    }

    Application visual_app(visualizer_code);
    Application serializer_app(serializer_code);
    Application algo_app(algo_code);

    QApplication app(argc, argv);
    Visual::CMainWindow win;
    Visual::CGraphicsScene scene;
    win.setScene(&scene);
    Visual::CCommandVisualizer cmd_visualizer;
    scene.addPainter(
        std::bind(&Visual::CCommandVisualizer::visualize, &cmd_visualizer, std::placeholders::_1));
    win.show();

    std::string win_text;
    QObject::connect(&win, &Visual::CMainWindow::textChanged, [&win_text](const QString& text) {
        win_text = text.toStdString();
    });

    std::thread t([&]() {
        auto temp_file_in = AlgoVi::Filesystem::getTempFile("txt");
        auto temp_file_out = AlgoVi::Filesystem::getTempFile("txt");
        auto temp_file_text = AlgoVi::Filesystem::getTempFile("txt");

        while (true)
        {
            bool success = true;

            /* get points from scene */
            auto points = scene.getPoints();
            if (points.size() < 3)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(400));
                continue;
            }
            std::stringstream pt_input;
            for (auto& p : points)
            {
                pt_input << p.x() << " " << p.y() << "\n";
            }


            /* prepare apps */
            Executor::Executor* visualizer;
            Executor::Executor* app;
            Executor::Executor* serializer;
            try
            {
                visualizer = visual_app.exe();
                app = algo_app.exe();
                serializer = serializer_app.exe();
            }
            catch(const Compiler::CompilationError& e)
            {
                std::cout << "Failed to recompile " << e.getFile() << std::endl;
                success = false;
            }

            /* execute */
            if (success)
            {
                serializer->setInput(pt_input.str());
                AlgoVi::Filesystem::writeToFile(temp_file_text, win_text);
                serializer->getExecutable()->setArgs({temp_file_text.string()});
                serializer->execute();
                if (serializer->wait() != 0)
                {
                    std::cout << "Serializer application failed" << std::endl;
                    success = false;
                }
            }

            if (success)
            {
                app->setInput(serializer->getOutput());
                app->execute();
                if (app->wait() != 0)
                {
                    std::cout << "Algorithm application failed" << std::endl;
                    success = false;
                }
            }

            if (success)
            {
                AlgoVi::Filesystem::writeToFile(temp_file_in, serializer->getOutput());
                AlgoVi::Filesystem::writeToFile(temp_file_out, app->getOutput());
                visualizer->getExecutable()->setArgs(
                    {temp_file_in.string(), temp_file_out.string(), temp_file_text.string()});

                visualizer->setInput(pt_input.str());
                visualizer->execute();
                if (visualizer->wait() != 0)
                {
                    std::cout << "Visualizer application failed" << std::endl;
                    break;
                }
            }

            if (success)
            {
                cmd_visualizer.updateData(visualizer->getOutput());
                scene.update();
            }
            else
            {
                cmd_visualizer.updateData("");
                scene.update();
            }
        }
    });

    app.exec();

    return 0;
}
catch (const std::exception& e)
{
    std::cout << " [ Error ] " << e.what() << std::endl;
}
