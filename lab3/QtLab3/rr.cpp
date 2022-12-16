#include "rr.h"

#include <QThread>
#include <QTime>

RR::RR(int ticks_per_process, QObject* parent)
    : ticks_process(ticks_per_process), ProcessScheduler{parent} {}

void RR::addProcess(ProcessStates& states) {
  if (!isPaused) {
    return;
  }

  processes.push_back(
      ProcessInfo(next_pid++, QTime::currentTime(), states,
                  states.first() == State::Executing
                      ? execute_pid == -1 ? State::Executing : State::Ready
                      : State::Waiting));
  ProcessInfo& added = processes.last();
  if (added.states.first() == State::Executing && execute_pid == -1) {
    execute_pid = added.pid;
  }
  emit addedProcess(added);
}

int RR::currentProcessExecute() { return execute_pid; }

void RR::tick() {
  emit updateTicks(++count_ticks);

  if (processes.empty()) {
    return;
  }

  bool next_process = false;
  bool back_process = false;

  ProcessInfo& first = processes.first();
  first.idx_state += 1;
  current_ticks += 1;
  if (first.idx_state == first.states.size()) {
    first.current_state = State::Over;
    emit updateProcess(first);
    processes.removeFirst();
    next_process = true;
  } else if (first.states[first.idx_state] == State::Waiting ||
             current_ticks >= ticks_process) {
    if (first.current_state == State::Executing &&
        first.states[first.idx_state] != State::Waiting) {
      first.current_state = State::Ready;
    } else {
      first.current_state = State::Waiting;
    }
    emit updateProcess(first);
    processes.push_back(first);
    processes.removeFirst();
    back_process = true;
    current_ticks = 0;
  }

  if (!next_process && !back_process) {
    emit updateProcess(first);
  }

  int cnt = processes.size() - back_process;
  for (int i = !back_process; i < cnt; ++i) {
    ProcessInfo& proc = processes[i];
    switch (proc.current_state) {
      case State::Waiting:
        proc.idx_state += 1;
        if (proc.idx_state == proc.states.size()) {
          processes.remove(proc.idx_state - 1);
        } else if (proc.states[proc.idx_state] == State::Executing) {
          proc.current_state = State::Ready;
        }
        emit updateProcess(proc);
        break;
      case State::Ready:
      case State::Executing:
      case State::Over:
        break;
    }
  }

  if (processes.empty()) {
    execute_pid = -1;
  } else if (!processes.empty() && (next_process || back_process) &&
             processes.first().current_state != State::Waiting) {
    processes.first().current_state = State::Executing;
    execute_pid = processes.first().pid;
    emit updateProcess(processes.first());
  }
}

void RR::start() {
  while (!isFinished) {
    while (isPaused) {
      if (isFinished) {
        return;
      }
      QThread::msleep(100);
    }
    tick();
    QThread::msleep(latency);
  }
}
