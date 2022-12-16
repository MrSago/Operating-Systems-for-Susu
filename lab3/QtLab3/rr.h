#ifndef RR_H
#define RR_H

#include <QAbstractItemModel>
#include <QObject>
#include <QVector>

#include "processscheduler.h"

class RR : public ProcessScheduler {
  Q_OBJECT

 public:
  explicit RR(int ticks_per_process, QObject* parent = nullptr);

  virtual void addProcess(ProcessStates& states);
  virtual int currentProcessExecute();

 public slots:
  virtual void tick();
  virtual void start();

 private:
  int ticks_process;
  int current_ticks = 0;
};

#endif  // RR_H
