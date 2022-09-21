#ifndef THREADGENERATESTRING_H
#define THREADGENERATESTRING_H

#include <QThread>

#include "sharedstate.h"

class ThreadGenerateString : public QThread {
  Q_OBJECT

 public:
  explicit ThreadGenerateString(SharedState* shared_state,
                                QObject* parent = nullptr);
  void run();

 signals:
  void updateBufferTextEdit(const QString&);
  void updateGenerateTextEdit(const QString&);

 private:
  SharedState* ss;
};

#endif  // THREADGENERATESTRING_H
