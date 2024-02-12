#include "settingsview.h"
#include "batteryindicator.h"
#include "mainwindow.h"
#include <QFile>

SettingsView::SettingsView(NavigationBar *_navigationBar,
                           NavigationPad *_navigationPad, QWidget *parent)
    : QWidget(parent) {

  breathDuration = settingsConfig.breathDuration;
  selectedChallengeLevelIndex = settingsConfig.challengeLevel - 1;

  navigationPad = _navigationPad;
  navigationBar = _navigationBar;
  resetToDefaultLabel = new QLabel();
  breathDurationLabel = new QLabel();
  challengeLevelLabel = new QLabel();

  resetToDefaultLabel->setText("Reset to default");

  mainLayout = new QVBoxLayout();

  listView = new QListView();
  QHBoxLayout *topBarLayout = new QHBoxLayout();

  BatteryIndicator *batt = new BatteryIndicator();
  batt->setBatteryLevel(MainWindow::deviceConfig->batteryLevel);
  topBarLayout->addWidget(batt);
  topBarLayout->setContentsMargins(42, 73, 20, 20);
  topBarLayout->setAlignment(Qt::AlignLeft);

  mainLayout->addLayout(topBarLayout);
  mainLayout->addWidget(listView);

  setLayout(mainLayout);

  listView->setStyleSheet(
      "QListView { background-color: transparent; border: none; }"
      "QListView::item { height: 40px; }"
      "QListView::item:selected { background-color: black; color: white; }"
      "* { font-size: 20px; }");

  this->setMaximumSize(520, 600);
  this->setMinimumSize(520, 600);

  this->setContentsMargins(11, 0, 0, 0);

  connect(navigationPad, &NavigationPad::upButtonClicked, this,
          &SettingsView::onUpButtonClicked);
  connect(navigationPad, &NavigationPad::downButtonClicked, this,
          &SettingsView::onDownButtonClicked);
  connect(navigationPad, &NavigationPad::leftButtonClicked, this,
          &SettingsView::onLeftButtonClicked);
  connect(navigationPad, &NavigationPad::rightButtonClicked, this,
          &SettingsView::onRightButtonClicked);
  connect(navigationPad, &NavigationPad::centerButtonClicked, this,
          &SettingsView::onCenterButtonClicked);

  connect(navigationBar, &NavigationBar::menuButtonClicked, this,
          &SettingsView::onMenuButtonClicked);
  connect(navigationBar, &NavigationBar::backButtonClicked, this,
          &SettingsView::onBackButtonClicked);

  model = new QStringListModel();
  updateLabels();
}

void SettingsView::updateLabels() {
  breathDurationLabel->setText(
      "Breath Duration:  " + QString::number(breathDuration) + " seconds");
  challengeLevelLabel->setText("Challenge Level:  " +
                               challengeLevels[selectedChallengeLevelIndex]);
  listView->reset();

  delete model;

  model = new QStringListModel(QStringList{resetToDefaultLabel->text(),
                                           breathDurationLabel->text(),
                                           challengeLevelLabel->text()});

  listView->setModel(model);

  listView->setCurrentIndex(model->index(selectedRow, 0));
}

void SettingsView::onUpButtonClicked() {
  if (selectedRow > 0) {
    selectedRow--;
    updateLabels();
  }
}

void SettingsView::onDownButtonClicked() {
  if (selectedRow < 2) {
    selectedRow++;
    updateLabels();
  }
}

void SettingsView::onLeftButtonClicked() {
  if (selectedRow == 0) {
    return;
  } else if (selectedRow == 1) {
    if (breathDuration == 1)
      breathDuration = 30;
    else
      breathDuration--;

    updateLabels();
  } else {
    selectedChallengeLevelIndex = (selectedChallengeLevelIndex - 1) % 4;
    updateLabels();
  }
}

void SettingsView::onRightButtonClicked() {
  if (selectedRow == 0) {
    return;
  } else if (selectedRow == 1) {
    breathDuration += 1;
    breathDuration = breathDuration % 31;
    if (breathDuration == 0)
      breathDuration = 1;
    updateLabels();
  } else {
    selectedChallengeLevelIndex = (selectedChallengeLevelIndex + 1) % 4;
    updateLabels();
  }
}

void SettingsView::onCenterButtonClicked() {
  if (selectedRow == 0) {
    breathDuration = 10;
    selectedChallengeLevelIndex = 0;
    updateLabels();
  }
}

void SettingsView::onMenuButtonClicked() {
  // save to file
  MainWindow::settingsConfig->challengeLevel =
      static_cast<ChallengeLevel>(selectedChallengeLevelIndex + 1);
  MainWindow::settingsConfig->breathDuration = breathDuration;
  MainWindow::settingsConfig->writeToFile();

  // change screen
  emit changeScreenContext(ScreenContext::Menu);
}

void SettingsView::onBackButtonClicked() {
  // save to file
  MainWindow::settingsConfig->challengeLevel =
      static_cast<ChallengeLevel>(selectedChallengeLevelIndex + 1);
  MainWindow::settingsConfig->breathDuration = breathDuration;
  MainWindow::settingsConfig->writeToFile();

  // change screen
  emit changeScreenContext(ScreenContext::Menu);
}
