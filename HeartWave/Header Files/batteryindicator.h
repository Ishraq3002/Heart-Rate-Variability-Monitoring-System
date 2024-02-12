#ifndef BATTERYINDICATOR_H
#define BATTERYINDICATOR_H

#include "enums.h"
#include <QWidget>

class BatteryIndicator : public QWidget {
  Q_OBJECT
public:
  explicit BatteryIndicator(QWidget *parent = nullptr);

  void setBatteryLevel(BatteryLevel level);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  BatteryLevel m_batteryLevel;
};

#endif // BATTERYINDICATOR_H
