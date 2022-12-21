
#include "fcfs.h"

#include <QThread>
#include <QTime>

FCFS::FCFS(QObject* parent) : ProcessScheduler{parent} {}

void FCFS::addProcess(ProcessStates& states) {
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

int FCFS::currentProcessExecute() { return execute_pid; }

void FCFS::tick() {
  emit updateTicks(++count_ticks);

  if (processes.empty()) {
    return;
  }

  ProcessInfo& first = processes.first();

  if (first.idx_state >= first.states.size()) {
    processes.removeFirst();
    processes.first().current_state = State::Executing;
    emit updateProcess(processes.first());
    return;
  }

  first.idx_state += 1;
  if (first.idx_state >= first.states.size()) {
    first.current_state = State::Over;
    emit updateProcess(first);
    processes.removeFirst();
    processes.first().current_state = State::Executing;
    emit updateProcess(processes.first());
    return;
  }

  first.current_state = first.states[first.idx_state];
  emit updateProcess(first);
}

void FCFS::start() {
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
