#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include <Windows.h>

#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

#define BUF_SIZE (10)
#define GEN_DELAY (500)
#define READ_DELAY (500)

struct SharedState {
  CRITICAL_SECTION criticalSection;
  QQueue<int> buf;

  SharedState() { InitializeCriticalSection(&criticalSection); }

  ~SharedState() { DeleteCriticalSection(&criticalSection); }
};

#endif  // SHAREDSTATE_H
