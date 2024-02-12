
#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

#include "enums.h"
#include <QObject>

class DeviceConfig : public QObject {
  Q_OBJECT
public:
  explicit DeviceConfig(QObject *parent = nullptr);
  BatteryLevel batteryLevel;
  HeartRateSensor hrSensorState;

  void writeToFile();

private:
  QString deviceConfigFilePath;

private slots:
  void onBatteryLevelChanged(BatteryLevel);
  void onHeartRateSensorStateChanged(HeartRateSensor);

signals:
};

#endif // DEVICECONFIG_H
