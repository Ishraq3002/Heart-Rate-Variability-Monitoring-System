
#include "deviceconfig.h"
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>

DeviceConfig::DeviceConfig(QObject *parent) : QObject{parent} {
  deviceConfigFilePath = "device.json";

  QFile file(deviceConfigFilePath);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    hrSensorState = HeartRateSensor::Connected;
    batteryLevel = BatteryLevel::Half;
    writeToFile();
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (!doc.isObject()) {
    hrSensorState = HeartRateSensor::Connected;
    batteryLevel = BatteryLevel::Half;
    writeToFile();
    return;
  }

  QJsonObject obj = doc.object();

  if (obj.contains("hrSensorState") && obj.contains("batteryLevel")) {
    hrSensorState = static_cast<HeartRateSensor>(obj["hrSensorState"].toInt());
    batteryLevel = static_cast<BatteryLevel>(obj["batteryLevel"].toInt());
  } else {
    hrSensorState = HeartRateSensor::Connected;
    batteryLevel = BatteryLevel::Half;
    writeToFile();
    return;
  }
}

void DeviceConfig::writeToFile() {
  QJsonObject obj;
  obj["hrSensorState"] = hrSensorState;
  obj["batteryLevel"] = batteryLevel;
  QJsonDocument doc(obj);

  QFile file(deviceConfigFilePath);

  if (file.open(QIODevice::WriteOnly)) {
    file.write(doc.toJson());

    // Close the file
    file.close();
  }
}

void DeviceConfig::onBatteryLevelChanged(BatteryLevel _level) {
  batteryLevel = _level;
  writeToFile();
}

void DeviceConfig::onHeartRateSensorStateChanged(HeartRateSensor _state) {
  hrSensorState = _state;
  writeToFile();
}
