#ifndef THREADREADERSTRING_H
#define THREADREADERSTRING_H

#include <QThread>

#include "sharedstate.h"

class ThreadReaderString : public QThread {
  Q_OBJECT

 public:
  explicit ThreadReaderString(SharedState* shared_state, int delay,
                              QObject* parent = nullptr);
  void run();
  void setDelay(int del);

 signals:
  void updateGui(const int);

 private:
  SharedState* ss;
  int delay;
};

#endif  // THREADREADERSTRING_H
