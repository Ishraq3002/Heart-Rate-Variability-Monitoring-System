#include "summaryview.h"
#include "navigationbar.h"
#include <QDateTime>
#include <QVBoxLayout>

SummaryView::SummaryView(QString _logfileName, NavigationBar *navigationBar,
                         QWidget *parent)
    : QWidget(parent) {

  initializeSummaryView();
  logfileName = _logfileName;

  if (logfileName != "") {
    fromLogView = true;
    logfileName = _logfileName;
    populateFromLoadedFile();
  }

  if (navigationBar != nullptr) {
    connect(navigationBar, &NavigationBar::backButtonClicked, this,
            &SummaryView::onBackButtonClicked);
    connect(navigationBar, &NavigationBar::menuButtonClicked, this,
            &SummaryView::onMenuButtonClicked);
  }
}

SummaryView::SummaryView(SummaryData *s, NavigationBar *navigationBar,
                         QWidget *parent) {
  fromLogView = false;
  logfileName = "";

  initializeSummaryView();

  summaryData.challengeLevel = s->challengeLevel;
  summaryData.lowCoherence = s->lowCoherence;
  summaryData.mediumCoherence = s->mediumCoherence;
  summaryData.highCoherence = s->highCoherence;
  summaryData.averageCoherence = s->averageCoherence;
  summaryData.sessionLength = s->sessionLength;
  summaryData.achievementScore = s->achievementScore;

  // TODO: deep copy the QVector
  summaryData.heartRateValues = s->heartRateValues;

  if (navigationBar != nullptr) {
    connect(navigationBar, &NavigationBar::backButtonClicked, this,
            &SummaryView::onBackButtonClicked);
    connect(navigationBar, &NavigationBar::menuButtonClicked, this,
            &SummaryView::onMenuButtonClicked);
  }
}

SummaryView::SummaryView(NavigationBar *navigationBar, QWidget *parent) {
  initializeSummaryView();
  populateFromLatestFile();

  if (navigationBar != nullptr) {
    connect(navigationBar, &NavigationBar::backButtonClicked, this,
            &SummaryView::onBackButtonClicked);
    connect(navigationBar, &NavigationBar::menuButtonClicked, this,
            &SummaryView::onMenuButtonClicked);
  }
}

void SummaryView::writeToFile() {

  // Create the logs folder if it doesn't exist
  QDir().mkdir("logs");

  // Create a JSON object to hold the session data
  QJsonObject sessionObject;
  sessionObject["challengeLevel"] = summaryData.challengeLevel;
  sessionObject["lowCoherence"] = summaryData.lowCoherence;
  sessionObject["mediumCoherence"] = summaryData.mediumCoherence;
  sessionObject["highCoherence"] = summaryData.highCoherence;
  sessionObject["averageCoherence"] = summaryData.averageCoherence;
  sessionObject["sessionLength"] = summaryData.sessionLength;
  sessionObject["achievementScore"] = summaryData.achievementScore;

  QJsonArray heartRateArray;

  for (int i = 0; i < summaryData.sessionLength; i++) {
    heartRateArray.append(summaryData.heartRateValues[i]);
  }

  sessionObject["heartRateValues"] = heartRateArray;

  // ------- TODO: ADD QVector<double> RESULTS FOR THE GRAPH PLOT -------

  // Create a JSON document from the session object
  QJsonDocument sessionDoc(sessionObject);

  // Create a file with the session label as the filename
  QFile file("logs/" + sessionLabel->text() + ".json");
  if (file.open(QIODevice::WriteOnly)) {
    // Write the JSON document to the file
    file.write(sessionDoc.toJson());

    // Close the file
    file.close();
  }
}

int SummaryView::getNextLogNumber() {
  // Check if the logs folder exists
  QDir logsDir("logs");
  if (!logsDir.exists()) {
    return 1;
  }

  // Get a list of files in the logs folder
  QFileInfoList fileList = logsDir.entryInfoList(QDir::Files);

  // If the logs folder is empty, return 1
  if (fileList.empty()) {
    return 1;
  }

  // Otherwise, return the number of files in the logs folder + 1
  return fileList.count() + 1;
}

void SummaryView::initializeSummaryView() {
  // Set up the layout
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(10, 10, 10, 10);
  layout->setSpacing(10);

  // Set up the session label
  QFont sessionFont("Arial", 14, QFont::Bold);
  sessionLabel = new QLabel(this);
  sessionLabel->setFont(sessionFont);
  sessionLabel->setAlignment(Qt::AlignHCenter);
  layout->addWidget(sessionLabel);

  // Set up the challenge label
  QFont labelFont("Arial", 12);
  challengeLabel = new QLabel(this);
  challengeLabel->setFont(labelFont);
  layout->addWidget(challengeLabel);

  // Set up the low coherence label
  lowCoherenceLabel = new QLabel(this);
  lowCoherenceLabel->setFont(labelFont);
  layout->addWidget(lowCoherenceLabel);

  // Set up the medium coherence label
  mediumCoherenceLabel = new QLabel(this);
  mediumCoherenceLabel->setFont(labelFont);
  layout->addWidget(mediumCoherenceLabel);

  // Set up the high coherence label
  highCoherenceLabel = new QLabel(this);
  highCoherenceLabel->setFont(labelFont);
  layout->addWidget(highCoherenceLabel);

  // Set up the average coherence label
  averageCoherenceLabel = new QLabel(this);
  averageCoherenceLabel->setFont(labelFont);
  layout->addWidget(averageCoherenceLabel);

  // Set up the session length label
  sessionLengthLabel = new QLabel(this);
  sessionLengthLabel->setFont(labelFont);
  layout->addWidget(sessionLengthLabel);

  // Set up the achievement score label
  achievementScoreLabel = new QLabel(this);
  achievementScoreLabel->setFont(labelFont);
  layout->addWidget(achievementScoreLabel);

  // Set the default values for the labels
  summaryData.challengeLevel = ChallengeLevel::Beginner;
  summaryData.lowCoherence = 0.0;
  summaryData.mediumCoherence = 0.0;
  summaryData.highCoherence = 0.0;
  summaryData.averageCoherence = 0.0;
  summaryData.sessionLength = 0;
  summaryData.achievementScore = 0.0;

  QHBoxLayout *graphLayout = new QHBoxLayout();
  graphLayout->setAlignment(Qt::AlignHCenter);

  mPlot = new QCustomPlot();

  mPlot->addGraph();
  QVector<double> x, y;
  mPlot->graph(0)->setData(x, y);

  mPlot->xAxis->setLabel("Time (Seconds)");
  mPlot->yAxis->setLabel("Heart Rate (BPM)");

  mPlot->xAxis->setRange(0, 300);
  mPlot->yAxis->setRange(40, 130);

  mPlot->replot();
  mPlot->setMaximumHeight(150);
  mPlot->setMinimumHeight(150);

  mPlot->setMaximumWidth(450);
  mPlot->setMinimumWidth(450);

  mPlot->setContentsMargins(0, 20, 0, 0);

  mPlot->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

  graphLayout->addWidget(mPlot);
  layout->addLayout(graphLayout);

  // Update the labels
  updateLabels();
  this->setMinimumWidth(480);
  this->setMinimumHeight(400);
}

void SummaryView::setupUi() {
  sessionLabel->setAlignment(Qt::AlignHCenter);
  challengeLabel->setText("Challenge Level: ");
  lowCoherenceLabel->setText("Low coherence: ");
  mediumCoherenceLabel->setText("Med. coherence: ");
  highCoherenceLabel->setText("High coherence: ");
  averageCoherenceLabel->setText("Average coherence: ");
  sessionLengthLabel->setText("Session Length: ");
  achievementScoreLabel->setText("Achievement Score: ");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(sessionLabel);
  layout->addWidget(challengeLabel);
  layout->addWidget(lowCoherenceLabel);
  layout->addWidget(mediumCoherenceLabel);
  layout->addWidget(highCoherenceLabel);
  layout->addWidget(averageCoherenceLabel);
  layout->addWidget(sessionLengthLabel);
  layout->addWidget(achievementScoreLabel);
}

void SummaryView::updateLabels() {
  QDateTime now = QDateTime::currentDateTime();
  sessionLabel->setText(QString("Session %1 - %2")
                            .arg(QStringLiteral("%1").arg(getNextLogNumber(), 3,
                                                          10, QLatin1Char('0')))
                            .arg(now.toString("dd MMMM, yyyy")));

  challengeLabel->setText(
      QString("Challenge Level: %1").arg(summaryData.challengeLevel));
  lowCoherenceLabel->setText(
      QString("Low coherence: %1%")
          .arg(QString::number(summaryData.lowCoherence, 'f', 2)));
  mediumCoherenceLabel->setText(
      QString("Med. coherence: %1%")
          .arg(QString::number(summaryData.mediumCoherence, 'f', 2)));
  highCoherenceLabel->setText(
      QString("High coherence: %1%")
          .arg(QString::number(summaryData.highCoherence, 'f', 2)));
  averageCoherenceLabel->setText(
      QString("Average coherence: %1")
          .arg(QString::number(summaryData.averageCoherence, 'f', 2)));

  int minutes = summaryData.sessionLength / 60;
  int seconds = summaryData.sessionLength % 60;

  if (minutes > 0)
    sessionLengthLabel->setText(QString("Session Length: %1 minutes %2 seconds")
                                    .arg(minutes)
                                    .arg(seconds));
  else
    sessionLengthLabel->setText(
        QString("Session Length: %1 seconds").arg(seconds));

  achievementScoreLabel->setText(
      QString("Achievement Score: %1%")
          .arg(QString::number(summaryData.achievementScore, 'f', 1)));

  QVector<double> x;

  for (int i = 0; i < summaryData.heartRateValues.length(); i++) {
    x.append(i);
  }
  mPlot->graph(0)->setData(x, summaryData.heartRateValues);
  mPlot->replot();
}

void SummaryView::populateFromLatestFile() {
  QFileInfoList fileList =
      QDir("logs").entryInfoList(QDir::Files, QDir::SortFlag::Name);
  QString latestFileName = fileList.last().fileName();
  QFile file(QString("logs/" + latestFileName));

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    return;
  }

  QJsonObject obj = doc.object();

  if (obj.contains("challengeLevel") && obj.contains("lowCoherence") &&
      obj.contains("mediumCoherence") && obj.contains("highCoherence") &&
      obj.contains("averageCoherence") && obj.contains("sessionLength") &&
      obj.contains("achievementScore")) {

    summaryData.challengeLevel =
        static_cast<ChallengeLevel>(obj["challengeLevel"].toInt());
    summaryData.lowCoherence = (double)obj["lowCoherence"].toDouble();
    summaryData.mediumCoherence = (double)obj["mediumCoherence"].toDouble();
    summaryData.highCoherence = (double)obj["highCoherence"].toDouble();
    summaryData.averageCoherence = (double)obj["averageCoherence"].toDouble();
    summaryData.sessionLength = obj["sessionLength"].toInt();
    summaryData.achievementScore = (double)obj["achievementScore"].toDouble();
    QJsonArray temp = obj["heartRateValues"].toArray();

    for (int i = 0; i < temp.size(); i++) {
      summaryData.heartRateValues.append(temp.at(i).toDouble());
    }

    updateLabels();

    if (file.fileName().contains("/")) {
      sessionLabel->setText(file.fileName().split("/")[1].split(".")[0]);
    } else {
      sessionLabel->setText(file.fileName().split(".")[0]);
    }
  }
}

void SummaryView::populateFromLoadedFile() {
  if (fromLogView) {
    QFile file(QString("logs/" + logfileName));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
      return;
    }

    QJsonObject obj = doc.object();

    if (obj.contains("challengeLevel") && obj.contains("lowCoherence") &&
        obj.contains("mediumCoherence") && obj.contains("highCoherence") &&
        obj.contains("averageCoherence") && obj.contains("sessionLength") &&
        obj.contains("achievementScore")) {

      summaryData.challengeLevel =
          static_cast<ChallengeLevel>(obj["challengeLevel"].toInt());
      summaryData.lowCoherence = obj["lowCoherence"].toDouble();
      summaryData.mediumCoherence = obj["mediumCoherence"].toDouble();
      summaryData.highCoherence = obj["highCoherence"].toDouble();
      summaryData.averageCoherence = obj["averageCoherence"].toDouble();
      summaryData.sessionLength = obj["sessionLength"].toInt();
      summaryData.achievementScore = obj["achievementScore"].toDouble();
      QJsonArray temp = obj["heartRateValues"].toArray();

      for (int i = 0; i < temp.size(); i++) {
        summaryData.heartRateValues.append(temp.at(i).toDouble());
      }
      updateLabels();
      if (file.fileName().contains("/")) {
        sessionLabel->setText(file.fileName().split("/")[1].split(".")[0]);
      } else {
        sessionLabel->setText(file.fileName().split(".")[0]);
      }
    }
  }
}

void SummaryView::onBackButtonClicked() {
  if (fromLogView)
    emit changeScreenContext(ScreenContext::Log);
  else
    emit changeScreenContext(ScreenContext::Menu);
}

void SummaryView::onMenuButtonClicked() {
  emit changeScreenContext(ScreenContext::Menu);
}
