
#include "breathpacer.h"
#include <QHBoxLayout>
#include <QPainter>

BreathPacer::BreathPacer(QWidget *parent) : QFrame{parent} {
  setFrameStyle(NoFrame);
  x = 0.0;
  bmThread = new BreathMeter(this);
  connect(bmThread, &BreathMeter::updateBreathPacer, this,
          &BreathPacer::onUpdateBreathPacer);
  this->setMinimumHeight(30);
  this->setContentsMargins(0, 10, 0, 10);
}

void BreathPacer::onUpdateBreathPacer(double _x) {
  x = _x;
  this->update();
}

void BreathPacer::paintEvent(QPaintEvent *e) {
  {
    int barWidth = 360;
    int barHeight = 25;
    int barOriginX = 40;
    int barOriginY = 0;
    int circleRadius = barHeight;

    QPainter qPainter(this);

    qPainter.setPen(QPen(QColor(0, 0, 0, 150), 1));
    qPainter.setBrush(QBrush(QColor(0, 0, 0, 50)));
    qPainter.drawRoundedRect(barOriginX, barOriginY, barOriginX + barWidth,
                             barHeight, circleRadius / 2, circleRadius / 2);

    qPainter.setPen(QPen(QColor(50, 100, 50), 2));
    qPainter.setBrush(QBrush(QColor(11, 186, 0)));
    qPainter.drawEllipse(barOriginX + x, barOriginY, circleRadius,
                         circleRadius);
  }

  QFrame::paintEvent(e);
}

void BreathPacer::begin() {
  if (!bmThread->isRunning())
    bmThread->start();
}

void BreathPacer::end() {
  if (bmThread->isRunning()) {
    bmThread->stop();
    bmThread->wait();
  }
}

BreathPacer::~BreathPacer() { this->end(); }
