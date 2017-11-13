#include "executor/Executor.hpp"
#include <QProcess>
#include <QObject>
#include <iostream>
#include <boost/asio/io_service.hpp>

namespace SOME_NAME {
namespace Executor {

class ExecutorImpl : public QObject
{
    Q_OBJECT
public:
    ExecutorImpl(Executor::ExecutablePtr exec);
    void run();
    void wait();
    void appendInput(const QString& input);

signals:
    void started();
    void finished(std::int32_t);
    void error(const QString& str);
    void output(const QString& out);

private:
    QProcess* m_process;
    Executor::ExecutablePtr m_exec;
};

ExecutorImpl::ExecutorImpl(Executor::ExecutablePtr exec)
    : m_exec(exec)
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
            emit finished(exit_code);
            m_process->deleteLater();
        });

    connect(
        m_process,
        static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
        [this](QProcess::ProcessError err) {
            //std::cout << "Something going wrong..." << std::endl;
            emit finished(1);
        });

    //connect(
    //    m_process, &QProcess::started, [this]() { std::cout << "QProcess::started" << std::endl; });

    connect(m_process, &QProcess::readyReadStandardOutput, [this]() {
        emit output(m_process->readAllStandardOutput());
    });

    //std::cout << "start " << m_exec->getAppPath().string() << "... " << std::endl;
    QStringList args;
    for (const auto& arg : m_exec->getArgs())
    {
        args.append(QString::fromStdString(arg));
    }
    m_process->start(QString::fromStdString(m_exec->getAppPath().string()), args);
}

void ExecutorImpl::wait()
{
    m_process->waitForFinished();
}

Executor::Executor(Executor::ExecutablePtr exec)
    : m_executable(exec)
    , m_impl(new ExecutorImpl(exec))
{
    QObject::connect(m_impl.get(), &ExecutorImpl::started, [this]() { m_start_signal(); });
    QObject::connect(m_impl.get(), &ExecutorImpl::finished, [this](std::int32_t exit_code) {
        m_exit_code = exit_code;
        m_finished_signal(exit_code);
    });
    QObject::connect(m_impl.get(), &ExecutorImpl::output, [this](const QString& str) {
        m_output_signal(str.toStdString());
        m_output += str.toStdString();
    });
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
}

void Executor::execute()
{
    m_output.clear();
    m_impl->run();
    m_impl->appendInput(QString::fromStdString(m_input));
}

std::int32_t Executor::wait()
{
    m_impl->wait();
    return m_exit_code;
}

std::int32_t Executor::getExitCode() const
{
    return m_exit_code;
}

Executor::ExecutablePtr Executor::getExecutable() const
{
    return m_executable;
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
} // namespace SOME_NAME
