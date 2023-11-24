#include "threadgeneratestring.h"

#include <Windows.h>

#include <QRandomGenerator>

ThreadGenerateString::ThreadGenerateString(SharedState* shared_state,
                                           int interval, int number,
                                           QObject* parent)
    : ss(shared_state), delay(interval), nThread(number), QThread{parent} {}

void ThreadGenerateString::run() {
  forever {
    int newVal = QRandomGenerator::global()->generate() % 100;

    EnterCriticalSection(&ss->criticalSection);
    while (ss->buf.size() >= BUF_SIZE) {
      LeaveCriticalSection(&ss->criticalSection);
      QThread::msleep(delay);
      EnterCriticalSection(&ss->criticalSection);
    }
    ss->buf.push_back({nThread, newVal});
    LeaveCriticalSection(&ss->criticalSection);

    QThread::msleep(delay);
  }
}

void ThreadGenerateString::setDelay(int del) { delay = del; }
