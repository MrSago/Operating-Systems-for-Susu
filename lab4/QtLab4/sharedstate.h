#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include <QMutex>
#include <QVector>
#include <QWaitCondition>

#define BUF_SIZE (10)
#define GEN_DELAY (100)
#define READ_DELAY (300)

struct SharedState {
  QMutex* mtx;
  QWaitCondition* cv;
  bool done;
  QVector<QChar> buf;

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
