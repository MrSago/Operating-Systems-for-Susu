#ifndef THREADGENERATESTRING_H
#define THREADGENERATESTRING_H

#include <QThread>

#include "sharedstate.h"

class ThreadGenerateString : public QThread {
  Q_OBJECT

 public:
  explicit ThreadGenerateString(SharedState* shared_state, int interval,
                                int number, QObject* parent = nullptr);
  void run();
  void setDelay(int del);

 private:
  SharedState* ss;
  int delay;
  int nThread;
};

#endif  // THREADGENERATESTRING_H
