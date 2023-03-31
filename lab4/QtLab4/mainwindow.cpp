#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  shared_state = new SharedState;
  thread_generate = new ThreadGenerateString(shared_state, 1000, this);
  thread_generate2 = new ThreadGenerateString(shared_state, 1000, this);
  thread_reader = new ThreadReaderString(shared_state, 1000, this);

  connect(thread_generate, SIGNAL(updateGui(int)), this,
          SLOT(onAddItemToList(const int)));

  connect(thread_generate2, SIGNAL(updateGui(int)), this,
          SLOT(onAddItemToList(const int)));

  connect(thread_reader, SIGNAL(updateGui(int)), this,
          SLOT(onAddItemToList(const int)));

  thread_generate->start();
  thread_generate2->start();
  thread_reader->start();

  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  thread_generate->terminate();
  thread_generate->wait();
  delete thread_generate;

  thread_generate2->terminate();
  thread_generate2->wait();
  delete thread_generate2;

  thread_reader->terminate();
  thread_reader->wait();
  delete thread_reader;

  delete shared_state;
  delete ui;
}

void MainWindow::onAddItemToList(const int val) {
  ui->listWidget->addItem(QString::number(val));
  ui->listWidget->scrollToBottom();
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
  thread_generate->setDelay(value);
  ui->label_3->setText(QString::number(value));
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value) {
  thread_reader->setDelay(value);
  ui->label_4->setText(QString::number(value));
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value) {
  thread_generate2->setDelay(value);
  ui->label_6->setText(QString::number(value));
}
