#include "threadreaderstring.h"

ThreadReaderString::ThreadReaderString(SharedState* shared_state,
                                       QObject* parent)
    : ss(shared_state), delay(READ_DELAY), QThread{parent} {}

void ThreadReaderString::run() {
  forever {
    if (ss->buf.size() <= 0) {
      QThread::msleep(delay);
      continue;
    }

    ss->buf.pop_front();

    QString str = "";
    for (auto& it : ss->buf) {
      str += it;
    }

    emit updateBufferTextEdit(str);
    QThread::msleep(delay);
  }
}

void ThreadReaderString::setLatency(int del) { delay = del; }
