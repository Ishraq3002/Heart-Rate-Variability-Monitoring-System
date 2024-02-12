// HeartRateIndicator

#include "heartrateindicator.h"
#include <QColor>
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QSvgRenderer>

HeartRateIndicator::HeartRateIndicator(QWidget *parent) : QWidget(parent) {
  setFixedSize(20, 20); // Set the fixed size of the widget to match the battery
                        // indicator widget
}

QSize HeartRateIndicator::sizeHint() const {
  return QSize(
      20,
      20); // Set the default size hint to match the fixed size of the widget
}

bool HeartRateIndicator::isActive() {
  return m_state == HeartRateSensor::Connected;
}

void HeartRateIndicator::setState(HeartRateSensor state) {

  m_state = state;
  update(); // Redraw the widget with the new state
  emit heartRateSensorStateChanged(isActive());
}

void HeartRateIndicator::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Set the color of the widget to match the battery indicator widget
  painter.setPen(Qt::black);
  painter.setBrush(Qt::black);
  //  painter.setBrush(Qt::NoBrush);

  // If state is active, draw the heart icon with heartbeat arcs
  if (isActive()) {
    // Load the heart icon from the SVG file
    QString iconPath = QString::fromStdString(
        QCoreApplication::applicationDirPath().toStdString() +
        "/heart-icon-active.svg");
    QSvgRenderer renderer(iconPath);
    QRectF viewBox = renderer.viewBoxF();

    // Scale the icon to fit the widget and center it
    double scaleFactor =
        qMin(width() / viewBox.width(), height() / viewBox.height());
    QRectF targetRect = QRectF(QPointF(), viewBox.size() * scaleFactor * .92);
    targetRect.moveCenter(rect().center());

    // Draw the heart icon
    renderer.render(&painter, targetRect);

  } else { // If state is inactive, draw an X over the heart icon
    // Load the heart icon from the SVG file
    QString iconPath = QString::fromStdString(
        QCoreApplication::applicationDirPath().toStdString() +
        "/heart-icon-inactive.svg");
    QSvgRenderer renderer(iconPath);
    QRectF viewBox = renderer.viewBoxF();

    // Scale the icon to fit the widget and center it
    double scaleFactor =
        qMin(width() / viewBox.width(), height() / viewBox.height());
    QRectF targetRect = QRectF(QPointF(), viewBox.size() * scaleFactor * .92);
    targetRect.moveCenter(rect().center());

    // Draw the heart icon
    renderer.render(&painter, targetRect);
  }
}
