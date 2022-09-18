#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include <QMutex>
#include <QWaitCondition>

#define STR_SIZE (10)
#define DELAY (300)

struct SharedState {
  QMutex* mtx;
  QWaitCondition* cv;
  bool done;
  QString buf;

  SharedState() {
    mtx = new QMutex;
    cv = new QWaitCondition;
    done = false;
    buf = "";
  }
  ~SharedState() {
    delete mtx;
    delete cv;
  }
};

#endif  // SHAREDSTATE_H
