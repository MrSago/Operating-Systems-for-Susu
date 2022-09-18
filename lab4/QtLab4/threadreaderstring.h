#ifndef THREADREADERSTRING_H
#define THREADREADERSTRING_H

#include <QThread>

#include "sharedstate.h"

class ThreadReaderString : public QThread {
  Q_OBJECT

 public:
  explicit ThreadReaderString(SharedState* shared_state,
                              QObject* parent = nullptr);
  void run();

 signals:
  void updateBufferTextEdit(const QString&);
  void updateReaderTextEdit(const QString&);

 public slots:

 private:
  SharedState* ss;
};

#endif  // THREADREADERSTRING_H
