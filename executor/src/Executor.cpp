#include "executor/Executor.hpp"
#include <QProcess>
#include <QObject>
#include <iostream>
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/timer/timer.hpp>

namespace AlgoVi {
namespace Executor {

class ExecutorImpl : public QObject
{
    Q_OBJECT
public:
    ExecutorImpl(Executor::ExecutablePtr exec);
    void run();
    void wait();
    void appendInput(const QString& input);
    void setTimeLimit(int32_t time_limit);
    void terminate();

signals:
    void started();
    void finished(std::int32_t, int32_t);
    void error(const QString& str);
    void output(const QString& out);

private:
    void checkTime();

private:
    QProcess* m_process;
    int32_t m_time_limit;
    boost::timer::cpu_timer m_timer;
    Executor::ExecutablePtr m_exec;
};

ExecutorImpl::ExecutorImpl(Executor::ExecutablePtr exec)
    : m_time_limit(0)
    , m_exec(exec)
{
}

#include "Executor.moc"

void ExecutorImpl::run()
{
    emit started();
    assert("Wrong executable inside Executor" && m_exec->isValid());
    m_process = new QProcess();
    connect(
        m_process,
        static_cast<void (QProcess::*)(std::int32_t)>(&QProcess::finished),
        [this](std::int32_t exit_code) {
            boost::timer::cpu_times const elapsed_times(m_timer.elapsed());
            boost::timer::nanosecond_type const elapsed(elapsed_times.system
                        + elapsed_times.user);
            auto exec_time = elapsed / 1000000;
            m_timer.stop();
            emit finished(exit_code, exec_time);
            m_process->deleteLater();
        });

    connect(
        m_process,
        static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
        [this](QProcess::ProcessError err) {
            std::cout << "error occured" << std::endl;
            m_timer.stop();
            emit finished(1, -1);
        });

    connect(m_process, &QProcess::readyReadStandardOutput, [this]() {
        emit output(m_process->readAllStandardOutput());
    });
    connect(m_process, &QProcess::readyReadStandardError, [this]() {
        emit error(m_process->readAllStandardError());
    });

    QStringList args;
    for (const auto& arg : m_exec->getArgs())
    {
        args.append(QString::fromStdString(arg));
    }

    connect(m_process, &QProcess::started, [this]() {
        if (m_time_limit > 0)
        {
            m_timer.start();
            std::thread(std::bind(&ExecutorImpl::checkTime, this)).detach();
        }
    });

    m_process->start(QString::fromStdString(m_exec->getAppPath().string()), args);
}

void ExecutorImpl::wait()
{
    m_process->waitForFinished();
}

void ExecutorImpl::terminate()
{
    m_process->terminate();
}

void ExecutorImpl::checkTime()
{
    while (!m_timer.is_stopped())
    {
        boost::timer::cpu_times const elapsed_times(m_timer.elapsed());
        boost::timer::nanosecond_type const elapsed(elapsed_times.system + elapsed_times.user);
        auto exec_time = elapsed / 1000000;
        if (exec_time >= m_time_limit)
        {
            std::cout << "exec_time = " << exec_time << " -> break " << std::endl;
            terminate();
            break;
        }
        else
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(std::min(200L, m_time_limit - exec_time)));
        }
    }
}

void ExecutorImpl::appendInput(const QString& input)
{
    assert("appendInput to nullptr process" && m_process != nullptr);
    if (!input.isEmpty())
    {
        //std::cout << "append input " << input.toStdString() << std::endl;
        m_process->write(input.toLocal8Bit());
        m_process->write("\n");
    }
    m_process->closeWriteChannel();
}

void ExecutorImpl::setTimeLimit(int32_t time_limit)
{
    m_time_limit = time_limit;
}

Executor::Executor(Executor::ExecutablePtr exec)
    : m_executable(exec)
    , m_impl(new ExecutorImpl(exec))
{
    QObject::connect(m_impl.get(), &ExecutorImpl::started, [this]() { m_start_signal(); });
    QObject::connect(
        m_impl.get(),
        &ExecutorImpl::finished,
        [this](std::int32_t exit_code, int32_t execution_time) {
            m_exit_code = exit_code;
            m_execution_time = execution_time;
            m_finished_signal(exit_code);
        });
    QObject::connect(m_impl.get(), &ExecutorImpl::output, [this](const QString& str) {
        m_output_signal(str.toStdString());
        m_output += str.toStdString();
    });
    QObject::connect(m_impl.get(), &ExecutorImpl::error, [this](const QString& str) {
        // m_error_signal(str.toString());
        m_error += str.toStdString();
    });
}

void Executor::execute()
{
    m_output.clear();
    m_error.clear();
    m_impl->run();
    m_impl->appendInput(QString::fromStdString(m_input));
}

std::int32_t Executor::wait()
{
    m_impl->wait();
    return m_exit_code;
}

int32_t Executor::getExitCode() const
{
    return m_exit_code;
}

void Executor::setTimeLimit(int32_t time_limit)
{
    m_impl->setTimeLimit(time_limit);
}

Executor::ExecutablePtr Executor::getExecutable() const
{
    return m_executable;
}

void Executor::setExecutable(ExecutablePtr executable)
{
    m_impl.reset(new ExecutorImpl(executable));
}

int32_t Executor::getExecutionTime() const
{
    return m_execution_time;
}

void Executor::setInput(const std::string& input)
{
    m_input = input;
}

void Executor::addInput(const std::string& input)
{
    m_input += "\n" + input;
    m_impl->appendInput(QString::fromStdString(input));
}

const std::string& Executor::getOutput() const
{
    return m_output;
}

const std::string& Executor::getError() const
{
    return m_error;
}

void Executor::addStartSlot(const StartSignal::slot_type& slot)
{
    m_start_signal.connect(slot);
}

void Executor::addOutputSlot(const OutputSignal::slot_type& slot)
{
    m_output_signal.connect(slot);
}

void Executor::addFinishedSlot(const FinishedSignal::slot_type& slot)
{
    m_finished_signal.connect(slot);
}

} // namespace Executor
} // namespace AlgoVi
