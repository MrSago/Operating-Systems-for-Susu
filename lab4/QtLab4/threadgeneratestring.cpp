#include "threadgeneratestring.h"

#include <QRandomGenerator>

ThreadGenerateString::ThreadGenerateString(SharedState* shared_state,
                                           QObject* parent)
    : ss(shared_state), QThread{parent} {}

void ThreadGenerateString::run() {
  forever {
    ss->mtx->lock();
    while (ss->done) {
      ss->cv->wait(ss->mtx);
    }

    QString gen = "";
    for (int i = 0; i < STR_SIZE; ++i) {
      emit updateGenerateTextEdit(gen);
      gen.push_back(
          QChar('A' + QRandomGenerator::global()->generate() % ('Z' - 'A')));
      QThread::msleep(DELAY);
    }
    ss->buf = qMove(gen);
    emit updateBufferTextEdit(ss->buf);

    ss->done = true;
    ss->cv->wakeAll();
    ss->mtx->unlock();
  }
}
