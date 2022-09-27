#include "mainwindow.h"

#include <QMessageBox>
#include <QStandardItemModel>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      tick_timer(new QTimer(this)),
      sys_timer(new QTimer(this)),
      model(new QStandardItemModel(0, 4)),
      scheduler(new FCFS(this)) {
  ui->setupUi(this);
  connectSlots();
  connectScheduler();
  initTable();
  sys_timer->start(1);

  ui->lineProcessPlan->setText("EEEWWWEEEEE");
  onButtonAddProcessClicked();
  ui->lineProcessPlan->setText("EEEEEWWEE");
  onButtonAddProcessClicked();
  ui->lineProcessPlan->setText("EEWWWEE");
  onButtonAddProcessClicked();
}

MainWindow::~MainWindow() {
  delete tick_timer;
  delete sys_timer;
  delete scheduler;
  delete model;
  delete ui;
}

void MainWindow::connectScheduler() {
  connect(tick_timer, &QTimer::timeout, scheduler, &ProcessScheduler::tick);
  connect(scheduler, &ProcessScheduler::updateProcess, this,
          &MainWindow::onUpdateTable);
  connect(scheduler, &ProcessScheduler::updateTicks, this,
          &MainWindow::onUpdateTicks);
  connect(ui->buttonTick, &QPushButton::clicked, scheduler,
          &ProcessScheduler::tick);
}

void MainWindow::connectSlots() {
  connect(sys_timer, &QTimer::timeout, this, &MainWindow::onUpdateSystemTime);
  connect(ui->buttonAddProcess, &QPushButton::clicked, this,
          &MainWindow::onButtonAddProcessClicked);
  connect(ui->buttonStart, &QPushButton::clicked, this,
          &MainWindow::onButtonStart);
  connect(ui->buttonStop, &QPushButton::clicked, this,
          &MainWindow::onButtonStop);
  connect(ui->cbChooseAlgo, &QComboBox::currentIndexChanged, this,
          &MainWindow::onChooseAlgo);
  connect(ui->buttonClear, &QPushButton::clicked, this,
          &MainWindow::onChooseAlgo);
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

void MainWindow::onUpdateTable(ProcessInfo& info) {
  int idx = info.pid;
  if (!findProcess(idx)) {
    model->insertRow(idx);
  }

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

  model->setData(model->index(idx, 0), QString::number(info.pid));
  model->setData(
      model->index(idx, 1),
      QString("%1.%2").arg(
          info.add_time.toString(),
          QString::number(info.add_time.msec()).rightJustified(3, '0')));
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

void MainWindow::onUpdateSystemTime() {
  QTime time = QTime::currentTime();
  ui->labelViewTime->setText(QString("%1.%2").arg(
      time.toString(), QString::number(time.msec()).rightJustified(3, '0')));
}

void MainWindow::onButtonAddProcessClicked() {
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
  tick_timer->start(ui->spinBoxMsQuant->text().toInt());
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
  tick_timer->stop();
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

void MainWindow::onChooseAlgo(int index) {
  delete scheduler;
  delete model;
  model = new QStandardItemModel(0, 4);
  initTable();
  ui->processTable->setModel(model);
  switch (index) {
    case 0:
    case 1:
      scheduler = new FCFS(this);
      break;
    default:
      return;
  }
  connectScheduler();
}

bool MainWindow::findProcess(int pid) {
  for (int i = 0; i < model->rowCount(); ++i) {
    if (pid == model->data(model->index(i, 0)).toInt()) {
      return true;
    }
  }
  return false;
}
