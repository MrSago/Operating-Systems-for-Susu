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
  void startThreads();

 public slots:
  void onUpdateBufferTextEdit(const QString&);
  void onUpdateGenerateTextEdit(const QString&);
  void onUpdateReaderTextEdit(const QString&);

 private:
  Ui::MainWindow* ui;
  SharedState* shared_state;
  ThreadGenerateString* thread_generate;
  ThreadReaderString* thread_reader;
};
#endif  // MAINWINDOW_H
