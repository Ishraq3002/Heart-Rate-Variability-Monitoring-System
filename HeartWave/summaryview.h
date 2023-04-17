
#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include "enums.h"
#include "navigationbar.h"
#include "qcustomplot.h"
#include <QLabel>
#include <QWidget>

struct SummaryData {
  ChallengeLevel challengeLevel;
  double lowCoherence;
  double mediumCoherence;
  double highCoherence;
  double averageCoherence;
  int sessionLength;
  double achievementScore;
  QVector<double> heartRateValues;
};

class SummaryView : public QWidget {
  Q_OBJECT

public:
  explicit SummaryView(QString _logfileName = "",
                       NavigationBar *navigationBar = nullptr,
                       QWidget *parent = nullptr);
  explicit SummaryView(SummaryData *s, NavigationBar *navigationBar = nullptr,
                       QWidget *parent = nullptr);

  explicit SummaryView(NavigationBar *navigationBar = nullptr,
                       QWidget *parent = nullptr);

  void writeToFile();
  int getNextLogNumber();

private:
  void initializeSummaryView();

  QLabel *sessionLabel;
  QLabel *challengeLabel;
  QLabel *lowCoherenceLabel;
  QLabel *mediumCoherenceLabel;
  QLabel *highCoherenceLabel;
  QLabel *averageCoherenceLabel;
  QLabel *sessionLengthLabel;
  QLabel *achievementScoreLabel;

  QCustomPlot *mPlot;
  QString logfileName;
  bool fromLogView;
  SummaryData summaryData;

  void setupUi();
  void updateLabels();
  void populateFromLoadedFile();
  void populateFromLatestFile();

signals:
  void changeScreenContext(ScreenContext);

private slots:
  //  void onUpButtonClicked();
  //  void onDownButtonClicked();
  //  void onLeftButtonClicked();
  //  void onRightButtonClicked();
  //  void onCenterButtonClicked();

  void onBackButtonClicked();
  void onMenuButtonClicked();
};

#endif // SUMMARYVIEW_H
