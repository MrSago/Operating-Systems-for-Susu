
#include "systemclock.h"

#include <QThread>

SystemClock::SystemClock(QObject* parent)
    : QObject{parent}, isFinished(false) {}

void SystemClock::start() {
  QTime time;
  QString str_time;
  while (!isFinished) {
    time = QTime::currentTime();
    str_time = QString("%1.%2").arg(
        time.toString(), QString::number(time.msec()).rightJustified(3, '0'));
    emit update(str_time);
    QThread::msleep(1);
  }
}

void SystemClock::stop() {
  isFinished = true;
  emit finished();
}
