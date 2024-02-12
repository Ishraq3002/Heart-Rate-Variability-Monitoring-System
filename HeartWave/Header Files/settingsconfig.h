
#ifndef SETTINGSCONFIG_H
#define SETTINGSCONFIG_H

#include "enums.h"
#include <QObject>

class SettingsConfig : public QObject {
  Q_OBJECT
public:
  explicit SettingsConfig(QObject *parent = nullptr);
  int breathDuration;
  ChallengeLevel challengeLevel;
  void writeToFile();

private:
  QString settingsFilePath;

private slots:
  void onBreathDurationChanged(int);
  void onChallengeLevelChanged(ChallengeLevel);

signals:
};

#endif // SETTINGSCONFIG_H
