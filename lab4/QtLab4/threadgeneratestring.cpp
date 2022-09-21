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

    QChar gen[BUF_SIZE] = {QChar('\0')};
    for (int i = 0; i < BUF_SIZE; ++i) {
      emit updateGenerateTextEdit(QString(gen));
      gen[i] =
          QChar('A' + QRandomGenerator::global()->generate() % ('Z' - 'A'));
      QThread::msleep(DELAY);
    }
    std::copy(gen, gen + BUF_SIZE, ss->buf);
    emit updateBufferTextEdit(QString(ss->buf));

    ss->done = true;
    ss->cv->wakeAll();
    ss->mtx->unlock();
  }
}
