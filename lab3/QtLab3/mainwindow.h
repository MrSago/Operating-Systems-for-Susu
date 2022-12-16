
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractItemModel>
#include <QMainWindow>
#include <QThread>
#include <QVector>

#include "processscheduler.h"
#include "systemclock.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  void connectSlots();
  void connectScheduler();
  void initTable();

 private slots:
  void onAddTable(ProcessInfo& info);
  void onUpdateTable(ProcessInfo& info, bool isNewProcess = false);
  void onUpdateTicks(int ticks);
  void onUpdateSystemTime(QString& time);

  void onButtonAddProcess();
  void onButtonStart();
  void onButtonStop();
  void onButtonTick();
  void onButtonRandomProcess();
  void onChooseAlgo(int index);

 private:
  Ui::MainWindow* ui;
  QAbstractItemModel* model;

  ProcessScheduler* scheduler;
  QThread* scheduler_thread;

  SystemClock* sys_clock;
  QThread* sys_clock_thread;
};
#endif  // MAINWINDOW_H
