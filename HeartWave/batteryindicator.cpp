#include "batteryindicator.h"
#include <QPainter>

BatteryIndicator::BatteryIndicator(QWidget *parent)
    : QWidget(parent), m_batteryLevel(Empty) {
  setFixedSize(30, 16);
}

void BatteryIndicator::setBatteryLevel(BatteryLevel level) {
  if (level != m_batteryLevel) {
    m_batteryLevel = level;
    update();
  }
}

void BatteryIndicator::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Draw the battery outline
  painter.setPen(Qt::black);
  painter.setBrush(Qt::NoBrush);
  QRectF batteryRect(0.5, 0.5, width() - 1, height() - 1);
  painter.drawRoundedRect(batteryRect, 3, 3);

  // Draw the battery level
  painter.setBrush(Qt::black);
  QRectF levelRect(2, 2, (width() - 5) * m_batteryLevel / Full, height() - 5);
  painter.drawRoundedRect(levelRect, 2, 2);
}
