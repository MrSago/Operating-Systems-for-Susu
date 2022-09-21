#include "threadreaderstring.h"

ThreadReaderString::ThreadReaderString(SharedState* shared_state,
                                       QObject* parent)
    : ss(shared_state), QThread{parent} {}

void ThreadReaderString::run() {
  forever {
    ss->mtx->lock();
    while (!ss->done) {
      ss->cv->wait(ss->mtx);
    }

    QChar reader[BUF_SIZE] = {QChar('\0')};
    for (int i = 1; i < BUF_SIZE; ++i) {
      QChar sym = ss->buf[BUF_SIZE - 1 - i];
      ss->buf[BUF_SIZE - 1 - i] = QChar('\0');
      reader[BUF_SIZE - 1 - i] = sym;
      emit updateBufferTextEdit(QString(ss->buf));
      emit updateReaderTextEdit(QString(reader + BUF_SIZE - 1 - i));
      QThread::msleep(DELAY);
    }

    ss->done = false;
    ss->cv->wakeAll();
    ss->mtx->unlock();
  }
}
