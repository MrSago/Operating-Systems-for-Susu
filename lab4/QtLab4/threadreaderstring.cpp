#include "threadreaderstring.h"

ThreadReaderString::ThreadReaderString(SharedState* shared_state,
                                       QObject* parent)
    : ss(shared_state), delay(READ_DELAY), QThread{parent} {}

void ThreadReaderString::run() {
  forever {
    if (ss->buf.size() == 0) {
      QThread::msleep(delay);
      continue;
    }

    int inputValue;

    ss->mtx->lock();
    inputValue = ss->buf.front();
    ss->buf.pop_front();
    ss->mtx->unlock();

    emit updateGui(inputValue);

    QThread::msleep(delay);
  }
}

void ThreadReaderString::setDelay(int del) { delay = del; }
