#include "logview.h"
#include "batteryindicator.h"
#include "mainwindow.h"
#include <QDebug>
#include <QDir>

LogView::LogView(NavigationBar *_navigationBar, NavigationPad *_navigationPad,
                 Screen *scr, QWidget *parent) {
  navigationBar = _navigationBar;
  navigationPad = _navigationPad;
  screen = scr;
  listView = new QListView(this);
  model = new QStringListModel(this);
  fileList = QDir("logs").entryInfoList(QDir::Files);

  isEmpty = fileList.length() == 0;
  fileNames = new QStringList();
  fileNames->append("Clear all logs");

  if (!isEmpty) {
    for (int i = 0; i < fileList.length(); i++)
      fileNames->append(fileList[i].fileName().split(".")[0]);
    model->setStringList(*fileNames);
  } else {
    model->setStringList(QStringList({"No logs yet..."}));
  }

  listView->setModel(model);

  listView->setStyleSheet(
      "QListView { background-color: transparent; border: none; }"
      "QListView::item { height: 40px; }"
      "QListView::item:selected { background-color: black; color: white; }"
      "* { font-size: 20px; }");

  // Set up the main layout
  QVBoxLayout *mainLayout = new QVBoxLayout();

  QHBoxLayout *topBarLayout = new QHBoxLayout();

  BatteryIndicator *batt = new BatteryIndicator();
  batt->setBatteryLevel(MainWindow::deviceConfig->batteryLevel);
  topBarLayout->addWidget(batt);
  topBarLayout->setContentsMargins(42, 73, 20, 20);
  topBarLayout->setAlignment(Qt::AlignLeft);

  mainLayout->addLayout(topBarLayout);
  mainLayout->addWidget(listView);
  setLayout(mainLayout);

  // connect navbar and navpad to slots
  connect(navigationPad, &NavigationPad::upButtonClicked, this,
          &LogView::onUpButtonClicked);
  connect(navigationPad, &NavigationPad::downButtonClicked, this,
          &LogView::onDownButtonClicked);
  connect(navigationPad, &NavigationPad::leftButtonClicked, this,
          &LogView::onLeftButtonClicked);
  connect(navigationPad, &NavigationPad::rightButtonClicked, this,
          &LogView::onRightButtonClicked);
  connect(navigationPad, &NavigationPad::centerButtonClicked, this,
          &LogView::onCenterButtonClicked);
  connect(navigationBar, &NavigationBar::menuButtonClicked, this,
          &LogView::onMenuButtonClicked);
  connect(navigationBar, &NavigationBar::backButtonClicked, this,
          &LogView::onBackButtonClicked);

  listView->setCurrentIndex(model->index(screen->getSelectedLogIndex(), 0));
  listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  //  this->setMaximumHeight(480);
  //  this->setMinimumHeight(480);

  this->setMaximumWidth(518);
  this->setMinimumWidth(518);
  this->setContentsMargins(10, 0, 0, 20);
}

void LogView::onUpButtonClicked() {
  QModelIndex currentIndex = listView->currentIndex();
  if (currentIndex.row() > 0) {
    screen->setSelectedLogIndex(currentIndex.row() - 1);
    listView->setCurrentIndex(model->index(screen->getSelectedLogIndex(), 0));
  }
}

void LogView::onDownButtonClicked() {
  QModelIndex currentIndex = listView->currentIndex();
  if (currentIndex.row() < model->rowCount() - 1) {
    screen->setSelectedLogIndex(currentIndex.row() + 1);
    listView->setCurrentIndex(model->index(screen->getSelectedLogIndex(), 0));
  }
}

void LogView::deleteLogs() {
  // delete each file in the list
  foreach (const QFileInfo &fileInfo, fileList) {
    QFile file(fileInfo.absoluteFilePath());
    if (file.remove()) {
      qDebug() << "Deleted file:" << fileInfo.absoluteFilePath();
    } else {
      qDebug() << "Failed to delete file:" << fileInfo.absoluteFilePath();
    }
  }

  fileList = QDir("logs").entryInfoList(QDir::Files);
  fileNames->clear();
  isEmpty = true;
  model->setStringList(QStringList({"No logs yet..."}));
}

void LogView::onLeftButtonClicked() {}
void LogView::onRightButtonClicked() {}
void LogView::onCenterButtonClicked() {
  if (isEmpty)
    return;
  else {
    int index = listView->currentIndex().row();

    if (index == 0) {
      deleteLogs();
    }

    else {
      screen->setFromLogView(true);
      screen->loadLogfile(fileList.at(index - 1).fileName());

      qInfo() << "Loaded logfile: " << fileNames->at(index - 1);

      emit changeScreenContext(ScreenContext::Summary);
    }
  }
}

void LogView::onMenuButtonClicked() {
  emit changeScreenContext(ScreenContext::Menu);
}
void LogView::onBackButtonClicked() {
  emit changeScreenContext(ScreenContext::Menu);
}
