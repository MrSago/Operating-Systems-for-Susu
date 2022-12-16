
#ifndef PROCESSSCHEDULER_H
#define PROCESSSCHEDULER_H

#include <QObject>
#include <QQueue>
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
  ProcessInfo(int _pid, QTime _time, ProcessStates& _states, State _current)
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
      : QObject{parent},
        next_pid(0),
        execute_pid(-1),
        count_ticks(0),
        latency(1),
        isFinished(false),
        isPaused(true) {}

  void setLatency(int _latency) { latency = _latency; }

  virtual void addProcess(ProcessStates& states) = 0;
  virtual int currentProcessExecute() = 0;

 public slots:
  virtual void tick() = 0;
  virtual void start() = 0;
  void stop() { isFinished = true; }
  void resume() { isPaused = false; }
  void pause() { isPaused = true; }

 signals:
  void addedProcess(ProcessInfo& info);
  void updateProcess(ProcessInfo& info, bool isNewProcess = false);
  void updateTicks(int ticks);

 protected:
  QVector<ProcessInfo> processes;
  int next_pid;
  int execute_pid;
  int count_ticks;
  int latency;
  bool isFinished;
  bool isPaused;
};

#endif  // PROCESSSCHEDULER_H
