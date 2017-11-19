#include <QApplication>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include "output_utils/PrintTable.hpp"
#include <thread>
#include "test_archive/TestArchive.hpp"
#include "compiler/Compiler.hpp"
#include "executor/Executor.hpp"
#include "visualize/view/MainWindow.hpp"
#include "visualize/view/GraphicsScene.hpp"
#include "visualize/model/CommandVisualizer.hpp"
#include "filesystem/TempFile.hpp"
#include "filesystem/FileIO.hpp"

using namespace SOME_NAME;

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

    auto visual_executable = Compiler::Compiler(Compiler::SourceCode(visualizer_code)).compile();
    //visual_executable->setArgs(
    //    {archive[test_num].inputFile()->string(), archive[test_num].outputFile()->string()});
    auto visual_exec = std::make_shared<Executor::Executor>(visual_executable);

    auto serializer_executable = Compiler::Compiler(Compiler::SourceCode(serializer_code)).compile();
    //serializer_executable->setArgs(
    //    {archive[test_num].inputFile()->string(), archive[test_num].outputFile()->string()});
    auto serializer_exec = std::make_shared<Executor::Executor>(serializer_executable);

    auto algo_executable =
        Compiler::Compiler(Compiler::SourceCode(algo_code)).compile();
    auto algo_exec = std::make_shared<Executor::Executor>(algo_executable);

    //visual_exec->execute();
    //auto res = visual_exec->wait();
    if (0 == 0)
    {
        QApplication app(argc, argv);
        Visual::CMainWindow win;
        Visual::CGraphicsScene scene;
        win.setScene(&scene);
        Visual::CCommandVisualizer visualizer;
        scene.addPainter(
            std::bind(&Visual::CCommandVisualizer::visualize, &visualizer, std::placeholders::_1));
        //visualizer.updateData(visual_exec->getOutput());
        win.show();

        std::cout << "HERE" << std::endl;
        std::thread t([&]() {
            auto temp_file_in = SOME_NAME::Filesystem::getTempFile("txt");
            auto temp_file_out = SOME_NAME::Filesystem::getTempFile("txt");

            std::cout << "HERE 2 " << std::endl;
            while (true)
            {
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
                std::cout << "run serializer" << std::endl;
                serializer_exec->setInput(pt_input.str());
                serializer_exec->execute();
                if (serializer_exec->wait() != 0)
                {
                    std::cout << "serializer error " << std::endl;
                    break;
                }
                
                algo_exec->setInput(serializer_exec->getOutput());
                algo_exec->execute();
                if (algo_exec->wait() != 0)
                {
                    std::cout << "algo error " << std::endl;
                    break;
                }

                SOME_NAME::Filesystem::writeToFile(temp_file_in, serializer_exec->getOutput());
                SOME_NAME::Filesystem::writeToFile(temp_file_out, algo_exec->getOutput());
                visual_executable->setArgs({temp_file_in.string(), temp_file_out.string()});

                visual_exec->execute();
                if (visual_exec->wait() != 0)
                {
                    std::cout << "visualizer error " << std::endl;
                    break;
                }

                visualizer.updateData(visual_exec->getOutput());
            }
        });

        app.exec();
    }
    else
    {
        //std::cout << "Cannon visualize, visualizer was finished with code " << res << std::endl;
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cout << " [ Error ] " << e.what() << std::endl;
}
