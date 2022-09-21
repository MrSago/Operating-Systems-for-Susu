#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  shared_state = new SharedState;
  thread_generate = new ThreadGenerateString(shared_state, this);
  thread_reader = new ThreadReaderString(shared_state, this);

  connect(thread_generate, SIGNAL(updateBufferTextEdit(const QString&)), this,
          SLOT(onUpdateBufferTextEdit(const QString&)));

  connect(thread_generate, SIGNAL(updateGenerateTextEdit(const QString&)), this,
          SLOT(onUpdateGenerateTextEdit(const QString&)));

  connect(thread_reader, SIGNAL(updateBufferTextEdit(const QString&)), this,
          SLOT(onUpdateBufferTextEdit(const QString&)));

  connect(thread_reader, SIGNAL(updateReaderTextEdit(const QString&)), this,
          SLOT(onUpdateReaderTextEdit(const QString&)));

  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  thread_generate->terminate();
  thread_generate->wait();
  delete thread_generate;

  thread_reader->terminate();
  thread_reader->wait();
  delete thread_reader;

  delete shared_state;
  delete ui;
}

void MainWindow::startThreads() {
  thread_generate->start();
  thread_reader->start();
}

void MainWindow::onUpdateBufferTextEdit(const QString& str) {
  ui->textEditStrBuffer->setText(str);
}
void MainWindow::onUpdateGenerateTextEdit(const QString& str) {
  ui->textEditStrGenerator->setText(str);
}
void MainWindow::onUpdateReaderTextEdit(const QString& str) {
  ui->textEditStrReader->setText(str);
}
