#include "threadreaderstring.h"

ThreadReaderString::ThreadReaderString(SharedState* shared_state, int interval,
                                       QObject* parent)
    : ss(shared_state), delay(interval), QThread{parent} {}

void ThreadReaderString::run() {
  forever {
    if (ss->buf.empty()) {
      QThread::msleep(delay);
      continue;
    }

    auto inputValue = ss->buf.front();
    ss->buf.pop_front();
    emit updateGui(inputValue.first, inputValue.second);

    QThread::msleep(delay);
  }
}

void ThreadReaderString::setDelay(int del) { delay = del; }
