// HeartRateIndicator

#ifndef HEARTRATEINDICATOR_H
#define HEARTRATEINDICATOR_H

#include "enums.h"
#include <QWidget>

class HeartRateIndicator : public QWidget {
  Q_OBJECT

public:
  explicit HeartRateIndicator(QWidget *parent = nullptr);

  QSize sizeHint() const override;

  void setState(HeartRateSensor);
  bool isActive();
signals:
  void heartRateSensorStateChanged(bool);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  HeartRateSensor m_state;
};

#endif // HEARTRATEINDICATOR_H
