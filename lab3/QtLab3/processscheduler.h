
#ifndef PROCESSSCHEDULER_H
#define PROCESSSCHEDULER_H

#include <QObject>
#include <QTime>
#include <QVector>

enum class State : char {
  Ready = 'R',
  Waiting = 'W',
  Executing = 'E',
  Over = 'O'
};

using ProcessStates = QVector<State>;

struct ProcessInfo {
  int pid;
  QTime add_time;
  ProcessStates states;
  State current_state;
  int idx_state;
  ProcessInfo(int _pid, QTime _time, ProcessStates _states, State _current)
      : pid(_pid),
        add_time(_time),
        states(_states),
        current_state(_current),
        idx_state(0) {}
};

class ProcessScheduler : public QObject {
  Q_OBJECT

 public:
  explicit ProcessScheduler(QObject* parent = nullptr)
      : QObject{parent}, next_pid(0), execute_pid(-1), count_ticks(0) {}

  virtual void tick() = 0;
  virtual void addProcess(ProcessStates& states) = 0;
  virtual int currentProcessExecute() = 0;

 signals:
  void updateProcess(ProcessInfo& info);
  void updateTicks(int ticks);

 protected:
  QVector<ProcessInfo> processes;
  int next_pid;
  int execute_pid;
  int count_ticks;
};

#endif  // PROCESSSCHEDULER_H
