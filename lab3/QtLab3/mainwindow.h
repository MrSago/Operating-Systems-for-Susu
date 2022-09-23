
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractItemModel>
#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QVector>

#include "fcfs.h"
#include "processscheduler.h"

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
  void connectScheduler();
  void connectSlots();
  void initTable();

 private slots:
  void onUpdateTicks(int ticks);
  void onUpdateSystemTime();
  void onButtonAddProcessClicked();
  void onButtonStart();
  void onButtonStop();
  void onUpdateTable(ProcessInfo& info);
  void onChooseAlgo(int index);

 private:
  Ui::MainWindow* ui;
  QTimer* tick_timer;
  QTimer* sys_timer;
  QAbstractItemModel* model;
  ProcessScheduler* scheduler;

  bool findProcess(int pid);
};
#endif  // MAINWINDOW_H
