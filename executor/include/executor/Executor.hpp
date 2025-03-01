#pragma once

#include <cstdint>
#include <boost/signals2/signal.hpp>
#include "compiler/Executable.hpp"

namespace AlgoVi {
namespace Executor {

using StartSignal = boost::signals2::signal<void()>;
using OutputSignal = boost::signals2::signal<void(const std::string&)>;
using FinishedSignal = boost::signals2::signal<void(std::int32_t)>;

class ExecutorImpl;

class Executor
{
public:
    using ExecutablePtr = std::shared_ptr<Compiler::Executable>;
public:
    Executor(ExecutablePtr exec);
    virtual ~Executor() = default;

    virtual void execute();
    virtual std::int32_t wait();

    void setExecutable(ExecutablePtr executable);
    
    void setInput(const std::string& input);
    void addInput(const std::string& input);
    const std::string& getOutput() const;
    const std::string& getError() const;
    int32_t getExitCode() const;
    ExecutablePtr getExecutable() const;
    int32_t getExecutionTime() const;
    void setTimeLimit(int32_t time_limit);

    void addStartSlot(const StartSignal::slot_type& slot);
    void addFinishedSlot(const FinishedSignal::slot_type& slot);
    void addOutputSlot(const OutputSignal::slot_type& slot);

protected:
    std::int32_t m_exit_code;
    std::int32_t m_execution_time;
    std::string m_output;
    std::string m_error;
    std::string m_input;
    StartSignal m_start_signal;
    FinishedSignal m_finished_signal;
    OutputSignal m_output_signal;
    ExecutablePtr m_executable;
    std::shared_ptr<ExecutorImpl> m_impl;
};

} // namespace Executor
} // namespace AlgoVi
