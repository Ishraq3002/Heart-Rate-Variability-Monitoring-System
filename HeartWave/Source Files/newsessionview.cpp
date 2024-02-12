
#include "newsessionview.h"
#include "batteryindicator.h"
#include "breathpacer.h"
#include "coherencebar.h"
#include "heartrategraph.h"
#include "mainwindow.h"
#include "summaryview.h"

NewSessionView::NewSessionView(NavigationBar *_navBar, NavigationPad *_navPad,
                               Screen *scr, QWidget *parent)
    : QWidget{parent} {

  navigationBar = _navBar;
  navigationPad = _navPad;
  screen = scr;
  QVBoxLayout *newSessionLayout = new QVBoxLayout(this);
  QHBoxLayout *topBarLayout = new QHBoxLayout();
  QVBoxLayout *graphLayout = new QVBoxLayout();

  BatteryIndicator *batt = new BatteryIndicator();
  batt->setBatteryLevel(MainWindow::deviceConfig->batteryLevel);

  claBar = new CLABar();

  hrIndicator = new HeartRateIndicator();
  hrIndicator->setState(MainWindow::deviceConfig->hrSensorState);

  connect(hrIndicator, &HeartRateIndicator::heartRateSensorStateChanged, this,
          &NewSessionView::onHeartRateSensorStateChanged);

  topBarLayout->addWidget(batt);
  topBarLayout->insertSpacing(2, 30);
  topBarLayout->addWidget(claBar);
  topBarLayout->insertSpacing(3, 30);
  topBarLayout->addWidget(hrIndicator);
  topBarLayout->setContentsMargins(40, 0, 40, 20);

  CoherenceBar *coherenceBar = new CoherenceBar();
  coherenceBar->setMaximumHeight(5);
  coherenceBar->setContentsMargins(0, 0, 0, 0);
  coherenceBar->setCoherenceState(CoherenceState::LowCoherence);

  QHBoxLayout *coherenceBarLayout = new QHBoxLayout();
  coherenceBarLayout->addWidget(coherenceBar);
  coherenceBarLayout->setAlignment(Qt::AlignCenter);

  hrgraph = new HeartRateGraph();
  coherenceCalculator = new CoherenceCalculator(claBar, coherenceBar);
  bp = new BreathPacer();

  connect(hrgraph->p, &Plotter::updateData, coherenceCalculator,
          &CoherenceCalculator::onUpdateData);
  connect(hrgraph, &HeartRateGraph::plottingFinished, this,
          &NewSessionView::onPlottingFinished);

  graphLayout->setMargin(0);
  graphLayout->addWidget(hrgraph);
  graphLayout->addWidget(bp);

  newSessionLayout->addLayout(topBarLayout);
  newSessionLayout->addLayout(coherenceBarLayout);
  newSessionLayout->addLayout(graphLayout);

  connect(navigationBar, &NavigationBar::backButtonClicked, this,
          &NewSessionView::onBackButtonClicked);
  connect(navigationBar, &NavigationBar::menuButtonClicked, this,
          &NewSessionView::onMenuButtonClicked);
  connect(navigationPad, &NavigationPad::centerButtonClicked, this,
          &NewSessionView::onCenterButtonClicked);

  bp->begin();
  hrgraph->begin();
}

NewSessionView::~NewSessionView() {
  hrgraph->end();
  bp->end();
}

void NewSessionView::endSession() {
  // first make a copy of the heart rate values
  QVector<double> heartRateValues;
  for (int i = 0; i < hrgraph->y.length(); i++) {
    heartRateValues.append(hrgraph->y[i]);
  }

  // stop everything
  hrgraph->end();
  bp->end();

  // calculate data
  double lowCoherence = 0.0;
  double mediumCoherence = 0.0;
  double highCoherence = 0.0;
  double averageCoherence = 0.0;
  int sessionLength = claBar->getLength();
  double achievementScore = claBar->getAchievement();

  QVector<double> lowCoherenceValues, mediumCoherenceValues,
      highCoherenceValues;

  for (int i = 0; i < coherenceCalculator->coherenceValues.length(); i++) {
    if (coherenceCalculator->coherenceValues[i] <= 0)
      lowCoherenceValues.append(coherenceCalculator->coherenceValues[i]);

    else if (coherenceCalculator->coherenceValues[i] < 0.5)
      mediumCoherenceValues.append(coherenceCalculator->coherenceValues[i]);
    else
      highCoherenceValues.append(coherenceCalculator->coherenceValues[i]);

    averageCoherence += coherenceCalculator->coherenceValues[i];
  }

  if (coherenceCalculator->coherenceValues.length() != 0) {
    lowCoherence = ((double)lowCoherenceValues.length() /
                    (double)coherenceCalculator->coherenceValues.length()) *
                   100.0;
    mediumCoherence = ((double)mediumCoherenceValues.length() /
                       (double)coherenceCalculator->coherenceValues.length()) *
                      100.0;
    highCoherence = ((double)highCoherenceValues.length() /
                     (double)coherenceCalculator->coherenceValues.length()) *
                    100.0;
    averageCoherence = averageCoherence /
                       (double)coherenceCalculator->coherenceValues.length();
  }

  else
    lowCoherence = 100.0;

  // save data
  SummaryData s{
      MainWindow::settingsConfig->challengeLevel,
      lowCoherence,
      mediumCoherence,
      highCoherence,
      averageCoherence,
      sessionLength,
      achievementScore,
      heartRateValues,
  };

  // temporarily create SummaryView instance to write to file
  SummaryView temp(&s);

  temp.writeToFile();

  // TODO: set screen's fromLogView to false
  screen->setFromLogView(false);

  // change screen to summary view --- load latest file
  emit changeScreenContext(ScreenContext::Summary);
}

void NewSessionView::keyPressEvent(QKeyEvent *event) {
  // Disable/disconnect the heart rate sensor
  if (event->key() == Qt::Key_H) {
    if (MainWindow::deviceConfig->hrSensorState == HeartRateSensor::Connected) {
      MainWindow::deviceConfig->hrSensorState = HeartRateSensor::Disconnected;
      MainWindow::deviceConfig->writeToFile();
    } else {
      MainWindow::deviceConfig->hrSensorState = HeartRateSensor::Connected;
      MainWindow::deviceConfig->writeToFile();
    }
    hrIndicator->setState(MainWindow::deviceConfig->hrSensorState);
    QWidget::keyPressEvent(event);
    qInfo() << "Heart rate sensor ---"
            << ((MainWindow::deviceConfig->hrSensorState ==
                 HeartRateSensor::Connected)
                    ? "Connected"
                    : "Disconnected");
  }

  // Change coherence simulation mode
  else if (event->key() == Qt::Key_C) {
    hrgraph->p->cycleCoherenceMode();
  }
}

void NewSessionView::onUpButtonClicked() {}
void NewSessionView::onLeftButtonClicked() {}
void NewSessionView::onRightButtonClicked() {}
void NewSessionView::onDownButtonClicked() {}

void NewSessionView::onCenterButtonClicked() { endSession(); }

void NewSessionView::onMenuButtonClicked() {
  hrgraph->end();
  bp->end();
  emit changeScreenContext(ScreenContext::Menu);
}

void NewSessionView::onHeartRateSensorStateChanged(bool isConnected) {
  if (!isConnected) {
    endSession();
  }
}

void NewSessionView::onPlottingFinished() { endSession(); }

void NewSessionView::onBackButtonClicked() {
  hrgraph->end();
  bp->end();
  emit changeScreenContext(ScreenContext::InitSession);
}
