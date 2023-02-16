#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "sharedstate.h"
#include "threadgeneratestring.h"
#include "threadreaderstring.h"

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

 public slots:
  void onAddItemToList(const int);

 private slots:
  void on_horizontalSlider_valueChanged(int value);

  void on_horizontalSlider_2_valueChanged(int value);

  void on_horizontalSlider_3_valueChanged(int value);

 private:
  Ui::MainWindow* ui;
  SharedState* shared_state;
  ThreadGenerateString* thread_generate;
  ThreadGenerateString* thread_generate2;
  ThreadReaderString* thread_reader;
};
#endif  // MAINWINDOW_H
