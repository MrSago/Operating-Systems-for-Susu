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

    QString reader = "";
    for (int i = 0; i < STR_SIZE; ++i) {
      QChar sym = ss->buf.back();
      ss->buf.resize(ss->buf.size() - 1);
      reader.push_front(sym);
      emit updateBufferTextEdit(ss->buf);
      emit updateReaderTextEdit(reader);
      QThread::msleep(DELAY);
    }

    ss->buf.clear();
    ss->done = false;
    ss->cv->wakeAll();
    ss->mtx->unlock();
  }
}
