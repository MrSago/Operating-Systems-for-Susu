#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include <QMutex>
#include <QWaitCondition>

#define BUF_SIZE (11)
#define DELAY (300)

struct SharedState {
  QMutex* mtx;
  QWaitCondition* cv;
  bool done;
  QChar buf[BUF_SIZE];

  SharedState() {
    mtx = new QMutex;
    cv = new QWaitCondition;
    done = false;
    std::fill(buf, buf + BUF_SIZE, '\0');
  }

  ~SharedState() {
    delete mtx;
    delete cv;
  }
};

#endif  // SHAREDSTATE_H
