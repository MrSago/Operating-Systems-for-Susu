
#include "mainwindow.h"

#include <QStandardItemModel>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(new QStandardItemModel(0, 4)),
      sys_timer(new QTimer(this)),
      fcfs(new FCFS(this)) {
  ui->setupUi(this);
  initTable();
  connectSlots();
  sys_timer->start(1);
}

MainWindow::~MainWindow() {
  delete fcfs;
  delete sys_timer;
  delete model;
  delete ui;
}

void MainWindow::updateSystemTime() {
  QTime time = QTime::currentTime();
  ui->labelViewTime->setText(QString("%1.%2").arg(
      time.toString(), QString::number(time.msec()).rightJustified(3, '0')));
}

void MainWindow::onButtonAddProcessReleased() {
  QString plan = ui->lineProcessPlan->text();
  ProcessStates states(plan.length());
  foreach (QChar state, plan) {
    switch (state.unicode()) {
      case 'R':
        states.append(State::Ready);
        break;
      case 'W':
        states.append(State::Waiting);
        break;
      case 'E':
        states.append(State::Executing);
        break;
      case 'O':
        states.append(State::Over);
        break;
      default:
        qDebug("gg wp\n");
        break;
    }
  }

  fcfs->addProcess(states);
}

void MainWindow::onUpdateTable(ProcessInfo& info) { qDebug("it's work!\n"); }

void MainWindow::initTable() {
  model->setHeaderData(0, Qt::Horizontal, "PID");
  model->setHeaderData(1, Qt::Horizontal, "Time");
  model->setHeaderData(2, Qt::Horizontal, "Plan");
  model->setHeaderData(3, Qt::Horizontal, "Status");
  ui->processTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  ui->processTable->verticalHeader()->hide();
  ui->processTable->setModel(model);
}

void MainWindow::connectSlots() {
  connect(sys_timer, &QTimer::timeout, this, &MainWindow::updateSystemTime);
  connect(ui->buttonAddProcess, &QPushButton::released, this,
          &MainWindow::onButtonAddProcessReleased);
  connect(fcfs, &FCFS::updateTable, this, &MainWindow::onUpdateTable);
}
