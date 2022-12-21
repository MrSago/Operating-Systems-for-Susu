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

    QChar newChar =
        QChar('A' + QRandomGenerator::global()->generate() % ('Z' - 'A'));
    ss->buf.push_back(newChar);

    QString str = "";
    for (auto& it : ss->buf) {
      str += it;
    }

    emit updateBufferTextEdit(str);
    QThread::msleep(delay);
  }
}

void ThreadGenerateString::setLatency(int del) { delay = del; }
