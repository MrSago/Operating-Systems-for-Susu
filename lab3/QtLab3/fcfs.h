
#ifndef FCFS_H
#define FCFS_H

#include <QAbstractItemModel>
#include <QObject>
#include <QVector>

#include "processscheduler.h"

class FCFS : public ProcessScheduler {
  Q_OBJECT

 public:
  explicit FCFS(QObject* parent = nullptr);

  virtual void tick();
  virtual void addProcess(ProcessStates& states);
  virtual int currentProcessExecute();

 private:
  int count_ticks;
};

#endif  // FCFS_H
