#include "tester/Tester.hpp"
#include <QApplication>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <atomic>
#include <iomanip>
#include <csignal>
#include <condition_variable>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include <thread>
#include "compiler/Compiler.hpp"
#include "output_utils/PrintTable.hpp"
#include "termcolor/termcolor.hpp"
#include "test_archive/TestArchive.hpp"

namespace po = boost::program_options;
using namespace AlgoVi;

std::atomic_bool s_done;
bool s_was_terminated;

void handlesigterm(int)
{
    s_was_terminated = true;
    s_done.store(true);
    // TODO: handle qt+unix signals
}

std::vector<std::string> split(const std::string& s)
{
    const std::size_t width_max = 50;
    const std::size_t height_max = 50;
    std::stringstream str(s);
    std::vector<std::string> ret;
    std::string temp;
    int additional_lines = 0;
    while (std::getline(str, temp))
    {
        if (temp.length() > width_max)
        {
            temp = temp.substr(0, width_max) + " ... [ +" +
                   std::to_string(temp.length() - width_max) + " ] ";
        }
        if (ret.size() < height_max)
        {
            ret.push_back(temp);
        }
        else
        {
            ++additional_lines;
        }
    }
    if (additional_lines > 0)
    {
        ret.push_back("...");
        ret.push_back("[ + " + std::to_string(additional_lines) + " lines ]");
    }
    return ret;
}

std::vector<std::size_t> parseRange(const std::string& s)
{
    std::vector<std::size_t> ret;

    std::size_t i = 0;
    std::size_t x = 0;
    bool is_dash = false;
    while (i < s.length())
    {
        char c = s[i++];
        if (c == ',' || c == '-' || i == s.length())
        {
            if (i == s.length())
            {
                if (!std::isdigit(c))
                {
                    throw std::runtime_error(
                        std::string("Unrecognized character ") + c + " is test range");
                }
                x = x * 10 + c - '0';
            }
            ret.push_back(x);
            x = 0;
        }
        else if (std::isdigit(c))
        {
            x = x * 10 + c - '0';
        }
        else
        {
            throw std::runtime_error(std::string("Unrecognized character ") + c + " is test range");
        }
        is_dash |= c == '-';
    }
    if (ret.size() == 2 && is_dash)
    {
        auto t(std::move(ret));
        if (t.front() > t.back())
        {
            throw std::runtime_error("Invalid test range");
        }
        ret.resize(std::min<std::size_t>(100, t.back() - t.front() + 1));
        std::iota(ret.begin(), ret.end(), t.front());
    }
    return ret;
}

void show_stat(
    QPainter* p,
    const std::vector<std::pair<std::size_t, Tester::Tester::ResultPair>>& results)
{
    auto pen = p->pen();
    pen.setColor(QColor("#000000"));
    pen.setWidthF(2.0);
    p->setBackground(QBrush("#ffffff"));
    p->setPen(pen);

    p->drawLine(10, 380, 760, 380);
    p->drawLine(30, 40, 30, 390); 
    p->drawText(QPoint(765, 380), "test");
    p->drawText(20, 20, "time, ms");

    std::int32_t mx_time = 0.0;
    for (const auto& r : results)
    {
        mx_time = std::max(mx_time, r.second.execution_time);
    }

    if (mx_time >= 1000)
    {
        if (mx_time % 1000)
        {
            mx_time += 1000;
        }
        mx_time -= mx_time % 1000;
    }

    for (std::size_t i = 0; i <= 10; ++i)
    {
        auto t = i * mx_time / 10;
        p->drawText(QPoint(5, 380 - 34 * i), QString::number(t));
        p->drawLine(28, 380 - 34 * i, 32, 380 - 34 * i);
    }

    auto test_width = 730 / results.size();
    std::size_t idx = 0;
    QColor clr[2] = {QColor("#ff05ab"), QColor("#0f5cd3")};
    for (const auto& r : results)
    {
        auto br = p->brush();
        br.setColor(clr[idx % 2]);
        br.setStyle(Qt::SolidPattern);
        p->setBrush(br);
        
        int height = 340 * r.second.execution_time / mx_time;
        p->drawRect(30 + test_width * idx, 380 - height, test_width, height);
        ++idx;
    }
}

int main(int argc, char** argv) try
{
    std::signal(SIGINT, handlesigterm);
    std::signal(SIGTERM, handlesigterm);

    po::options_description desc("Algovi tester");
    desc.add_options()("help,h", "help message")
        ("src,s", po::value<std::string>()->default_value("code.cpp"),
            "path to source code to test")
        ("checker,c",po::value<std::string>()
            ->default_value(
                boost::filesystem::exists("checker.cpp") ? "checker.cpp" : "testlib_wcmp"),
            "path to checker")
        ("time-limit", po::value<int32_t>()->default_value(2000), "time limit in milliseconds")
        ("test-folder,f", po::value<std::string>()->default_value("tests"), "path to test folder")
        ("test,t", po::value<std::string>()->default_value("all"), "test on specific tests")
        ("statistics", "show graphical statistics after testing process")
        ("oneline", "verbose messages in one line (use with --verbose)")
        ("verbose", "detailed test report");

    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }
        po::notify(vm);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    bool show_statistics = vm.count("statistics");
    std::string code_path = vm["src"].as<std::string>();
    std::string checker_path = vm["checker"].as<std::string>();
    if (boost::starts_with(checker_path, "testlib_"))
    {
        checker_path = (boost::filesystem::path("/opt/algovi/checkers/") / checker_path).string();
    }
    std::string test_folder = vm["test-folder"].as<std::string>();
    int32_t time_limit = vm["time-limit"].as<int32_t>();

    auto code_executable = Compiler::Compiler(Compiler::SourceCode(code_path)).compile();
    auto checker_executable = Compiler::Compiler(Compiler::SourceCode(checker_path)).compile();

    auto code_exec = std::make_shared<Executor::Executor>(code_executable);
    auto checker_exec = std::make_shared<Tester::Checker>(checker_executable);

    TestArchive::TestArchive tests(test_folder);

    Tester::Tester tester(code_exec, checker_exec);
    tester.setTimeLimit(time_limit);

    std::vector<std::size_t> tests_to_test;
    if (vm["test"].as<std::string>() == "all")
    {
        tests_to_test.resize(tests.size());
        std::iota(tests_to_test.begin(), tests_to_test.end(), 1);
    }
    else
    {
        tests_to_test = parseRange(vm["test"].as<std::string>());
    }

    bool verbose = vm.count("verbose");
    bool oneline = vm.count("oneline");

    std::cout << " Running..." << std::endl;

    Tester::Tester::ResultPair result;
    std::int32_t min_time = std::numeric_limits<int32_t>::max();
    std::int32_t max_time = std::numeric_limits<int32_t>::min();
    std::int32_t av_time = 0;
    std::vector<std::pair<std::size_t, Tester::Tester::ResultPair >> result_vec;

    std::thread test_thread([&]() {
        for (std::size_t i = 0; i < tests_to_test.size() && s_done.load() == false; ++i)
        {
            if (!verbose)
            {
                std::cout << "\r Running on test " << tests_to_test[i] << " .. " << std::flush;
            }
            tester.setTest(&tests[tests_to_test[i] - 1]);
            result = tester.test(verbose);
            if (s_done.load())
            {
                break;
            }
            result_vec.emplace_back(tests_to_test[i], result);

            min_time = std::min(min_time, result.execution_time);
            max_time = std::max(max_time, result.execution_time);
            av_time += result.execution_time;

            if (verbose)
            {
                if (oneline)
                {
                    std::cout << std::setw(3) << std::right << tests_to_test[i];
                    std::cout << ": ";
                    if (result.result == Tester::Tester::ETestResult::OK)
                    {
                        std::cout << termcolor::green << "OK" << termcolor::reset;
                    }
                    else
                    {
                        std::cout << termcolor::bred << "WA" << termcolor::reset;
                    }
                    std::cout << ", ( " << result.detailed_message << " )" << std::endl;
                }
                else
                {
                    std::vector<Utils::Cell> head = {
                        Utils::Cell().setData({std::to_string(tests_to_test[i])}),
                        Utils::Cell()
                            .setAlign(Utils::EAlign::Center)
                            .setColor(termcolor::bcyan)
                            .setData({"input"}),
                        Utils::Cell()
                            .setAlign(Utils::EAlign::Center)
                            .setColor(termcolor::bgreen)
                            .setData({"output"}),
                        Utils::Cell()
                            .setAlign(Utils::EAlign::Center)
                            .setColor(termcolor::bmagenta)
                            .setData({"actual"}),
                    };
                    auto table = Utils::getTable(
                        Utils::Table{
                            head,
                            {Utils::Cell(),
                             Utils::Cell().setData(split(tests[tests_to_test[i] - 1].input())),
                             Utils::Cell().setData(split(tests[tests_to_test[i] - 1].output())),
                             Utils::Cell().setData(split(*result.output))},
                            {Utils::Cell().setData({"Time:"}).setCollspan(2).setColor(
                                 termcolor::bwhite),
                             Utils::Cell()
                                 .setData({std::to_string(result.execution_time) + "ms"})
                                 .setCollspan(2)},
                            {Utils::Cell()
                                 .setData({result.result == Tester::Tester::ETestResult::OK
                                               ? "Accepted"
                                               : "WrongAnswer"})
                                 .setColor(
                                     result.result == Tester::Tester::ETestResult::OK
                                         ? termcolor::bgreen
                                         : termcolor::bred)
                                 .setCollspan(2),
                             Utils::Cell()
                                 .setData({"Checker: " + result.detailed_message})
                                 .setCollspan(2)},
                        },
                        termcolor::white);
                    std::cout << table << std::endl;
                }
            }
            else if (result.result != Tester::Tester::ETestResult::OK)
            {
                std::cout << std::endl;
                std::cout << termcolor::bred << "Wrong answer" << termcolor::reset << " on test "
                          << i + 1 << std::flush;
                break;
            }
        }
        s_done.store(true);
    });

    while (s_done.load() == false)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    test_thread.join();
    if (s_was_terminated)
    {
        result_vec.pop_back();
        result = result_vec.back().second;
    }

    std::cout << std::string(20, ' ') << " Testing process was finished" << std::endl;

    std::string result_string =
        result.result == Tester::Tester::ETestResult::OK
            ? (result_vec.size() == tests.size()
                   ? "Accepted"
                   : "Partly accepted (" + std::to_string(result_vec.size()) + "/" +
                         std::to_string(tests.size()) + ")")
            : "Wrong Answer";
    auto table = Utils::Table(
        {{Utils::Cell().setData({"Verdict"}),
          Utils::Cell()
              .setData(
                  {result_string})
              .setColor(
                  result.result == Tester::Tester::ETestResult::OK ? termcolor::bgreen
                                                                   : termcolor::bred)},
         {Utils::Cell().setData({"Max time"}),
          Utils::Cell().setData({std::to_string(max_time) + "ms"}).setColor(termcolor::bblue)},
         {Utils::Cell().setData({"Average"}),
          Utils::Cell().setData({std::to_string(av_time / result_vec.size()) + "ms"})}});
    if (result.result != Tester::Tester::ETestResult::OK)
    {
        table.push_back({Utils::Cell().setData({"Checker message"}),
                         Utils::Cell().setData({result.detailed_message})});
    }
    std::cout << Utils::getTable(table) << std::endl;

    if (show_statistics)
    {
        QApplication app(argc, argv);
        QSize win_size(800, 400);
        QPixmap pixmap(win_size);
        QPainter* p = new QPainter(&pixmap);
        pixmap.fill(QColor("#ffffff"));

        show_stat(p, result_vec);
        QLabel lbl;
        lbl.setFixedSize(win_size);
        lbl.setPixmap(pixmap);
        lbl.show();

        return app.exec();
    }

    return 0;
}
catch (const Compiler::CompilationError& e)
{
    std::cout << termcolor::bred << "Cannot compile " << e.getFile() << termcolor::reset
              << std::endl;
    std::cout << "Do you want to see compiler output? [ y / n ]" << std::endl;
    std::string ans;
    std::cin >> ans;
    if (ans == "y")
    {
        std::cout << termcolor::red << e.what() << termcolor::reset << std::endl;
    }
    return 1;
}
