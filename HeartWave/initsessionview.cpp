#include "initsessionview.h"
#include "batteryindicator.h"
#include "mainwindow.h"
#include <QDebug>

InitSessionView::InitSessionView(NavigationBar *_navBar, NavigationPad *_navPad,
                                 QWidget *parent) {
  navigationBar = _navBar;
  navigationPad = _navPad;
  // Set up the label
  m_label = new QLabel(
      "To start a new session, press the center button of the navigation pad. "
      "To go back, press Back or Menu. Ensure you have the heart rate sensor "
      "attached, before starting a session.");
  m_label->setAlignment(Qt::AlignJustify);
  m_label->setWordWrap(true);
  m_label->setContentsMargins(50, 50, 50, 50);

  // Set up the layout
  centralLayout = new QHBoxLayout();
  centralLayout->addWidget(m_label);

  QVBoxLayout *mainLayout = new QVBoxLayout();

  QHBoxLayout *topBarLayout = new QHBoxLayout();

  BatteryIndicator *batt = new BatteryIndicator();
  batt->setBatteryLevel(MainWindow::deviceConfig->batteryLevel);
  topBarLayout->addWidget(batt);
  topBarLayout->setContentsMargins(38, 70, 20, 20);
  topBarLayout->setAlignment(Qt::AlignLeft);

  heartRateIndicator = new HeartRateIndicator();
  heartRateIndicator->setState(MainWindow::deviceConfig->hrSensorState);

  topBarLayout->addSpacing(340);
  topBarLayout->addWidget(heartRateIndicator);

  mainLayout->addLayout(topBarLayout);
  mainLayout->addLayout(centralLayout);
  setLayout(mainLayout);

  connect(navigationPad, &NavigationPad::upButtonClicked, this,
          &InitSessionView::onUpButtonClicked);
  connect(navigationPad, &NavigationPad::downButtonClicked, this,
          &InitSessionView::onDownButtonClicked);
  connect(navigationPad, &NavigationPad::leftButtonClicked, this,
          &InitSessionView::onLeftButtonClicked);
  connect(navigationPad, &NavigationPad::rightButtonClicked, this,
          &InitSessionView::onRightButtonClicked);
  connect(navigationPad, &NavigationPad::centerButtonClicked, this,
          &InitSessionView::onCenterButtonClicked);

  connect(navigationBar, &NavigationBar::backButtonClicked, this,
          &InitSessionView::onBackButtonClicked);
  connect(navigationBar, &NavigationBar::menuButtonClicked, this,
          &InitSessionView::onMenuButtonClicked);

  this->setMaximumSize(490, 475);
  this->setMinimumSize(490, 475);
  this->setStyleSheet("QLabel { font-size: 20px; }");

  setFocusPolicy(Qt::StrongFocus);
}

void InitSessionView::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_H) {
    if (MainWindow::deviceConfig->hrSensorState == HeartRateSensor::Connected) {
      MainWindow::deviceConfig->hrSensorState = HeartRateSensor::Disconnected;
      MainWindow::deviceConfig->writeToFile();
    } else {
      MainWindow::deviceConfig->hrSensorState = HeartRateSensor::Connected;
      MainWindow::deviceConfig->writeToFile();
    }
    heartRateIndicator->setState(MainWindow::deviceConfig->hrSensorState);
    QWidget::keyPressEvent(event);
    qInfo() << "Heart rate sensor ---"
            << ((MainWindow::deviceConfig->hrSensorState ==
                 HeartRateSensor::Connected)
                    ? "Connected"
                    : "Disconnected");
  }
}

// these do nothing
void InitSessionView::onUpButtonClicked() {}
void InitSessionView::onDownButtonClicked() {}
void InitSessionView::onLeftButtonClicked() {}
void InitSessionView::onRightButtonClicked() {}

void InitSessionView::onCenterButtonClicked() {
  // check if heartrate sensor is active
  // if active, start new session
  // if inactive, do nothing

  if (MainWindow::deviceConfig->hrSensorState == HeartRateSensor::Connected) {
    emit changeScreenContext(ScreenContext::NewSession);
  }
}
void InitSessionView::onBackButtonClicked() {
  emit changeScreenContext(ScreenContext::Menu);
}
void InitSessionView::onMenuButtonClicked() {
  emit changeScreenContext(ScreenContext::Menu);
}
