
#ifndef SYSTEMCLOCK_H
#define SYSTEMCLOCK_H

#include <QObject>
#include <QTime>

class SystemClock : public QObject {
  Q_OBJECT
 public:
  explicit SystemClock(QObject* parent = nullptr);

 private:
  bool isFinished;

 signals:
  void update(QString& time);
  void finished();

 public slots:
  void start();
  void stop();
};

#endif  // SYSTEMCLOCK_H
