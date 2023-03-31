#ifndef THREADGENERATESTRING_H
#define THREADGENERATESTRING_H

#include <QThread>

#include "sharedstate.h"

class ThreadGenerateString : public QThread {
  Q_OBJECT

 public:
  explicit ThreadGenerateString(SharedState* shared_state, int interval,
                                QObject* parent = nullptr);
  void run();
  void setDelay(int del);

 signals:
  void updateGui(const int);

 private:
  SharedState* ss;
  int delay;
};

#endif  // THREADGENERATESTRING_H
