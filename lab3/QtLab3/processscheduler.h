
#ifndef PROCESSSCHEDULER_H
#define PROCESSSCHEDULER_H

#include <QObject>
#include <QTime>
#include <QVector>

enum class State { Ready, Waiting, Executing, Over };

using ProcessStates = QVector<State>;

struct ProcessInfo {
  int pid;
  QTime add_time;
  ProcessStates states;
  State current_state;
  ProcessInfo(int _pid, QTime _time, ProcessStates _states, State _current)
      : pid(_pid), add_time(_time), states(_states), current_state(_current) {}
};

class ProcessScheduler : public QObject {
  Q_OBJECT

 public:
  explicit ProcessScheduler(QObject* parent = nullptr) : QObject{parent} {}

  virtual void tick() = 0;
  virtual void addProcess(ProcessStates& states) = 0;

 signals:
  void updateTable(ProcessInfo& info);

 protected:
  QVector<ProcessInfo> processes;
};

#endif  // PROCESSSCHEDULER_H
