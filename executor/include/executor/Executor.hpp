#pragma once

#include <boost/signals2/signal.hpp>
#include "compiler/Executable.hpp"

namespace SOME_NAME {
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
    std::int32_t wait();
    
    void setInput(const std::string& input);
    void addInput(const std::string& input);
    const std::string& getOutput() const;
    std::int32_t getExitCode() const;
    ExecutablePtr getExecutable() const;

    void addStartSlot(const StartSignal::slot_type& slot);
    void addFinishedSlot(const FinishedSignal::slot_type& slot);
    void addOutputSlot(const OutputSignal::slot_type& slot);

protected:
    std::int32_t m_exit_code;
    std::string m_output;
    std::string m_input;
    StartSignal m_start_signal;
    FinishedSignal m_finished_signal;
    OutputSignal m_output_signal;
    ExecutablePtr m_executable;
    std::shared_ptr<ExecutorImpl> m_impl;
};

} // namespace Executor
} // namespace SOME_NAME
