#include "threadgeneratestring.h"

#include <QRandomGenerator>

ThreadGenerateString::ThreadGenerateString(SharedState* shared_state,
                                           QObject* parent)
    : ss(shared_state), delay(GEN_DELAY), QThread{parent} {}

void ThreadGenerateString::run() {
  forever {
    if (ss->buf.size() > BUF_SIZE) {
      QThread::msleep(delay);
      continue;
    }

    int newVal = QRandomGenerator::global()->generate() % 100;

    ss->mtx->lock();
    ss->buf.push_back(newVal);
    ss->mtx->unlock();

    // emit updateGui(newVal);

    QThread::msleep(delay);
  }
}

void ThreadGenerateString::setDelay(int del) { delay = del; }
