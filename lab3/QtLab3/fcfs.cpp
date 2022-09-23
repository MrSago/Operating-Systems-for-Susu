
#include "fcfs.h"

#include <QTime>

FCFS::FCFS(QObject* parent) : ProcessScheduler{parent} {}

void FCFS::tick() {
  emit updateTicks(++count_ticks);

  if (processes.empty()) {
    return;
  }

  bool next_process = false;
  bool back_process = false;

  processes.first().idx_state += 1;
  if (processes.first().idx_state == processes.first().states.size()) {
    processes.first().current_state = State::Over;
    emit updateProcess(processes.first());
    processes.removeFirst();
    next_process = true;
  } else if (processes.first().states[processes.first().idx_state] ==
             State::Waiting) {
    processes.first().current_state = State::Waiting;
    emit updateProcess(processes.first());
    processes.push_back(processes.first());
    processes.removeFirst();
    back_process = true;
  }

  int cnt = processes.size() - back_process;
  for (int i = !back_process; i < cnt; ++i) {
    ProcessInfo& proc = processes[i];
    switch (proc.current_state) {
      case State::Waiting:
        proc.idx_state += 1;
        if (proc.states[proc.idx_state] == State::Executing) {
          proc.current_state = State::Ready;
        }
        break;
      case State::Ready:
      case State::Executing:
      case State::Over:
        break;
    }
    emit updateProcess(proc);
  }

  if (processes.empty()) {
    execute_pid = -1;
  } else if (!processes.empty() && (next_process || back_process)) {
    processes.first().current_state = State::Executing;
    execute_pid = processes.first().pid;
    emit updateProcess(processes.first());
  }
}

void FCFS::addProcess(ProcessStates& states) {
  processes.push_back(
      ProcessInfo(next_pid++, QTime::currentTime(), states,
                  states.first() == State::Executing
                      ? execute_pid == -1 ? State::Executing : State::Ready
                      : State::Waiting));
  ProcessInfo& added = processes.last();
  if (added.states.first() == State::Executing && execute_pid == -1) {
    execute_pid = added.pid;
  }
  emit updateProcess(added);
}

int FCFS::currentProcessExecute() { return execute_pid; }
