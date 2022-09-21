
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

 private slots:
  void updateSystemTime();
  void onButtonAddProcessReleased();
  void onUpdateTable(ProcessInfo& info);

 private:
  Ui::MainWindow* ui;
  QAbstractItemModel* model;
  QTimer* sys_timer;
  ProcessScheduler* fcfs;
  // ProcessScheduler* rr;

 private:
  void initTable();
  void connectSlots();
};
#endif  // MAINWINDOW_H
