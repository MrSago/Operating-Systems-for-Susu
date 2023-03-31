#include "threadgeneratestring.h"

#include <Windows.h>

#include <QRandomGenerator>

ThreadGenerateString::ThreadGenerateString(SharedState* shared_state,
                                           int interval, QObject* parent)
    : ss(shared_state), delay(interval), QThread{parent} {}

void ThreadGenerateString::run() {
  forever {
    if (ss->buf.size() >= BUF_SIZE) {
      QThread::msleep(delay);
      continue;
    }

    int newVal = QRandomGenerator::global()->generate() % 100;

    EnterCriticalSection(&ss->criticalSection);
    ss->buf.push_back(newVal);
    LeaveCriticalSection(&ss->criticalSection);

    QThread::msleep(delay);
  }
}

void ThreadGenerateString::setDelay(int del) { delay = del; }
