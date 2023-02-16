#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

#define BUF_SIZE (10)
#define GEN_DELAY (500)
#define READ_DELAY (500)

struct SharedState {
  QMutex* mtx;
  QWaitCondition* cv;
  bool done;
  QQueue<int> buf;

  SharedState() {
    mtx = new QMutex;
    cv = new QWaitCondition;
    done = false;
  }

  ~SharedState() {
    delete mtx;
    delete cv;
  }
};

#endif  // SHAREDSTATE_H
