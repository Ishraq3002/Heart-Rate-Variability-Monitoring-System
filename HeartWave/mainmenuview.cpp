#include "mainmenuview.h"
#include "batteryindicator.h"
#include "mainwindow.h"
#include <QDebug>

MainMenuView::MainMenuView(NavigationPad *_navigationPad, Screen *scr,
                           QWidget *parent)
    : QWidget(parent) {
  // Set up the main menu list view
  navigationPad = _navigationPad;
  screen = scr;
  listView = new QListView(this);
  model = new QStringListModel(this);
  model->setStringList({"New Session", "Settings", "Log/History"});
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

  // Connect signals and slots
  connect(navigationPad, &NavigationPad::upButtonClicked, this,
          &MainMenuView::onUpButtonClicked);
  connect(navigationPad, &NavigationPad::downButtonClicked, this,
          &MainMenuView::onDownButtonClicked);
  connect(navigationPad, &NavigationPad::leftButtonClicked, this,
          &MainMenuView::onLeftButtonClicked);
  connect(navigationPad, &NavigationPad::rightButtonClicked, this,
          &MainMenuView::onRightButtonClicked);
  connect(navigationPad, &NavigationPad::centerButtonClicked, this,
          &MainMenuView::onCenterButtonClicked);

  listView->setCurrentIndex(model->index(screen->getSelectedMenuIndex(), 0));
  listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  //  this->setMaximumHeight(480);
  //  this->setMinimumHeight(480);

  this->setMaximumWidth(518);
  this->setMinimumWidth(518);

  this->setContentsMargins(10, 0, 0, 20);
}

void MainMenuView::onUpButtonClicked() {
  QModelIndex currentIndex = listView->currentIndex();
  if (currentIndex.row() > 0) {
    screen->setSelectedMenuIndex(currentIndex.row() - 1);
    listView->setCurrentIndex(model->index(screen->getSelectedMenuIndex(), 0));
  }
}

void MainMenuView::onDownButtonClicked() {
  QModelIndex currentIndex = listView->currentIndex();
  if (currentIndex.row() < model->rowCount() - 1) {
    screen->setSelectedMenuIndex(currentIndex.row() + 1);
    listView->setCurrentIndex(model->index(screen->getSelectedMenuIndex(), 0));
  }
}

void MainMenuView::onLeftButtonClicked() {
  // do nothing
}

void MainMenuView::onRightButtonClicked() {
  // do nothing
}

void MainMenuView::onCenterButtonClicked() {
  int choiceIndex = listView->currentIndex().row();
  switch (choiceIndex) {
  case 0: // New session
    emit changeScreenContext(ScreenContext::InitSession);
    break;
  case 1: // Settings
    emit changeScreenContext(ScreenContext::Settings);
    break;
  case 2: // Log
    emit changeScreenContext(ScreenContext::Log);
    break;
  default:
    break;
  }
}

void MainMenuView::onBackButtonClicked() {}
void MainMenuView::onMenuButtonClicked() {}
