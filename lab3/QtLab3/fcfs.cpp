
#include "fcfs.h"

#include <QTime>

FCFS::FCFS(QObject* parent) : ProcessScheduler{parent} {}

void FCFS::tick() {}

void FCFS::addProcess(ProcessStates& states) {
  processes.push_back(
      ProcessInfo(processes.size(), QTime::currentTime(), states,
                  processes.size() > 0 ? states[0] : State::Executing));
  emit updateTable(processes.last());

  //  processes.push_back(states);
  //  int id = model->rowCount();
  //  model->insertRow(id);
  //  model->setData(model->index(id, 0), QString::number(id + 1));
  //  model->setData(model->index(id, 1), QTime::currentTime().toString());
  //  modfl->setData(model->index(id, 2), )
}
