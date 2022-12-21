#include "mainwindow.h"

#include <QMessageBox>
#include <QRandomGenerator64>
#include <QStandardItemModel>

#include "./ui_mainwindow.h"
#include "fcfs.h"
#include "rr.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(new QStandardItemModel(0, 4)),
      scheduler(new FCFS()),
      scheduler_thread(new QThread()),
      sys_clock(new SystemClock()),
      sys_clock_thread(new QThread()) {
  ui->setupUi(this);

  connectSlots();
  connectScheduler();
  initTable();

  scheduler->moveToThread(scheduler_thread);
  sys_clock->moveToThread(sys_clock_thread);

  scheduler_thread->start();
  sys_clock_thread->start();

  // TEST
  ui->cbChooseAlgo->setCurrentIndex(0);
  for (int i = 0; i < 10; ++i) {
    onButtonRandomProcess();
  }
}

MainWindow::~MainWindow() {
  scheduler->stop();
  scheduler_thread->quit();
  scheduler_thread->wait();

  sys_clock->stop();
  sys_clock_thread->quit();
  sys_clock_thread->wait();

  delete model;
  delete scheduler;
  delete scheduler_thread;
  delete sys_clock;
  delete sys_clock_thread;
  delete ui;
}

void MainWindow::connectSlots() {
  connect(sys_clock, &SystemClock::update, this,
          &MainWindow::onUpdateSystemTime);
  connect(sys_clock_thread, &QThread::started, sys_clock, &SystemClock::start);

  connect(ui->buttonAddProcess, &QPushButton::clicked, this,
          &MainWindow::onButtonAddProcess);
  connect(ui->buttonStart, &QPushButton::clicked, this,
          &MainWindow::onButtonStart);
  connect(ui->buttonStop, &QPushButton::clicked, this,
          &MainWindow::onButtonStop);
  connect(ui->buttonTick, &QPushButton::clicked, this,
          &MainWindow::onButtonTick);
  connect(ui->buttonRandomProcess, &QPushButton::clicked, this,
          &MainWindow::onButtonRandomProcess);
  connect(ui->cbChooseAlgo, &QComboBox::currentIndexChanged, this,
          &MainWindow::onChooseAlgo);
  connect(ui->buttonClear, &QPushButton::clicked, this,
          &MainWindow::onChooseAlgo);
}

void MainWindow::connectScheduler() {
  connect(scheduler, &ProcessScheduler::updateProcess, this,
          &MainWindow::onUpdateTable);
  connect(scheduler_thread, &QThread::started, scheduler,
          &ProcessScheduler::start);

  connect(scheduler, &ProcessScheduler::addedProcess, this,
          &MainWindow::onAddTable);
  connect(scheduler, &ProcessScheduler::updateTicks, this,
          &MainWindow::onUpdateTicks);
}

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

void MainWindow::onAddTable(ProcessInfo& info) {
  model->insertRow(info.pid);
  onUpdateTable(info, true);
}

void MainWindow::onUpdateTable(ProcessInfo& info, bool isNewProcess) {
  int idx = info.pid;

  if (QChar(static_cast<char>(info.current_state)) !=
      model->data(model->index(idx, 3)).toChar()) {
    for (int i = 0; i < model->columnCount(); ++i) {
      QColor color("white");
      switch (info.current_state) {
        case State::Executing:
          color.setNamedColor("lightgreen");
          break;
        case State::Waiting:
          color.setNamedColor("lightblue");
          break;
        case State::Ready:
          color.setNamedColor("coral");
          break;
        case State::Over:
          color.setNamedColor("lightgrey");
          break;
      }
      model->setData(model->index(idx, i), color, Qt::BackgroundRole);
    }
  }

  if (isNewProcess) {
    model->setData(model->index(idx, 0), QString::number(info.pid));
    model->setData(
        model->index(idx, 1),
        QString("%1.%2").arg(
            info.add_time.toString(),
            QString::number(info.add_time.msec()).rightJustified(3, '0')));
  }

  QString states = "";
  for (int i = 0; i < info.states.size(); ++i) {
    if (i == info.idx_state) {
      states += QString("[%1]").arg(static_cast<char>(info.states[i]));
    } else {
      states += static_cast<char>(info.states[i]);
    }
  }
  model->setData(model->index(idx, 2), states);

  model->setData(model->index(idx, 3),
                 QChar(static_cast<char>(info.current_state)));

  if (info.current_state == State::Executing) {
    ui->labelViewPID->setText(QString::number(info.pid));
  } else if (info.current_state == State::Over) {
    ui->labelViewPID->setText("#");
  }
}

void MainWindow::onUpdateTicks(int ticks) {
  ui->labelViewCountTicks->setText(QString::number(ticks));
}

void MainWindow::onUpdateSystemTime(QString& time) {
  ui->labelViewTime->setText(time);
}

void MainWindow::onButtonAddProcess() {
  QString plan = ui->lineProcessPlan->text();
  if (plan.length() == 0) {
    return;
  }

  ProcessStates states;
  foreach (QChar state, plan) {
    switch (state.unicode()) {
      case 'W':
        states.push_back(State::Waiting);
        break;
      case 'E':
        states.push_back(State::Executing);
        break;
      default:
        QMessageBox msgBox(this);
        msgBox.setText("Invalid process plan!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
  }

  scheduler->addProcess(states);
}

void MainWindow::onButtonStart() {
  scheduler->setLatency(ui->spinBoxMsQuant->text().toInt());
  scheduler->resume();
  ui->lineProcessPlan->setEnabled(false);
  ui->buttonAddProcess->setEnabled(false);
  ui->buttonRandomProcess->setEnabled(false);
  ui->cbChooseAlgo->setEnabled(false);
  ui->spinBoxMsQuant->setEnabled(false);
  ui->spinBoxTicks->setEnabled(false);
  ui->buttonStart->setEnabled(false);
  ui->buttonStop->setEnabled(true);
  ui->buttonTick->setEnabled(false);
  ui->buttonClear->setEnabled(false);
}

void MainWindow::onButtonStop() {
  scheduler->pause();
  ui->lineProcessPlan->setEnabled(true);
  ui->buttonAddProcess->setEnabled(true);
  ui->buttonRandomProcess->setEnabled(true);
  ui->cbChooseAlgo->setEnabled(true);
  ui->spinBoxMsQuant->setEnabled(true);
  ui->spinBoxTicks->setEnabled(true);
  ui->buttonStart->setEnabled(true);
  ui->buttonStop->setEnabled(false);
  ui->buttonTick->setEnabled(true);
  ui->buttonClear->setEnabled(true);
}

void MainWindow::onButtonTick() { scheduler->tick(); }

void MainWindow::onButtonRandomProcess() {
  QList<State> states;
  int count = QRandomGenerator64::system()->bounded(4, 10);

  states.append(State::Executing);
  while (count--) {
    switch (QRandomGenerator64::system()->generate() % 2) {
      case 0:
        states.append(State::Waiting);
        break;
      case 1:
        states.append(State::Executing);
        break;
    }
  }
  states.append(State::Executing);

  scheduler->addProcess(states);
}

void MainWindow::onChooseAlgo(int index) {
  scheduler->stop();
  scheduler_thread->quit();
  scheduler_thread->wait();

  delete scheduler;
  delete scheduler_thread;
  delete model;

  model = new QStandardItemModel(0, 4);
  initTable();
  ui->processTable->setModel(model);

  switch (index) {
    case 0:
      scheduler = new FCFS();
      break;
    case 1:
      scheduler = new RR(ui->spinBoxTicks->text().toInt());
      break;
    default:
      return;
  }

  scheduler_thread = new QThread();
  scheduler->moveToThread(scheduler_thread);
  scheduler_thread->start();
  connectScheduler();
}
