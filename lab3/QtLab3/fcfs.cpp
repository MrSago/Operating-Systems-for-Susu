
#include "fcfs.h"

#include <QTime>

FCFS::FCFS(QObject* parent) : ProcessScheduler{parent}, count_ticks(0) {}

void FCFS::tick() { qDebug("tick #%d\n", ++count_ticks); }

void FCFS::addProcess(ProcessStates& states) {
  processes.push_back(
      ProcessInfo(processes.size(), QTime::currentTime(), states,
                  states.first() == State::Executing
                      ? execute_pid == -1 ? State::Executing : State::Ready
                      : State::Waiting));

  ProcessInfo& added = processes.last();
  if (added.states.first() == State::Executing && execute_pid == -1) {
    execute_pid = added.pid;
  }

  emit updateTable(added);
}

int FCFS::currentProcessExecute() { return execute_pid; }
