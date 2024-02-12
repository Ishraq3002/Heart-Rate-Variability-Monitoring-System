
#include "settingsconfig.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

SettingsConfig::SettingsConfig(QObject *parent) : QObject{parent} {
  QFile file("settings.json");

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    breathDuration = 10;
    challengeLevel = ChallengeLevel::Beginner;
    writeToFile();
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    breathDuration = 10;
    challengeLevel = ChallengeLevel::Beginner;
    writeToFile();
    return;
  }

  QJsonObject obj = doc.object();

  if (obj.contains("breathDuration") && obj.contains("challengeLevel")) {
    breathDuration = obj["breathDuration"].toInt();
    challengeLevel = static_cast<ChallengeLevel>(obj["challengeLevel"].toInt());
  } else {
    breathDuration = 10;
    challengeLevel = ChallengeLevel::Beginner;
    writeToFile();
    return;
  }
}

void SettingsConfig::writeToFile() {
  QJsonObject obj;
  obj["breathDuration"] = breathDuration;
  obj["challengeLevel"] = challengeLevel;
  QJsonDocument doc(obj);

  QFile file("settings.json");
  if (file.open(QIODevice::WriteOnly)) {
    // Write the JSON document to the file
    file.write(doc.toJson());

    // Close the file
    file.close();
  }
}

void SettingsConfig::onBreathDurationChanged(int _breathDuration) {
  breathDuration = _breathDuration;
  // TODO: write file with new values
  writeToFile();
}

void SettingsConfig::onChallengeLevelChanged(ChallengeLevel _challengeLevel) {
  challengeLevel = _challengeLevel;
  // TODO: write file with new values
  writeToFile();
}
