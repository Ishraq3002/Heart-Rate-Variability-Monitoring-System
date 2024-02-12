#include "breathmeter.h"
#include "mainwindow.h"
#include <QDebug>
#include <QElapsedTimer>

BreathMeter::BreathMeter(QObject *parent) : QThread{parent} {}

void BreathMeter::run() {
  double x = 0.0;

  m_stop = false;

  double limit = 375;
  double delayBetweenBreaths = 20.0;

  // TODO: Scale breath times to global timer
  int breathDuration = MainWindow::settingsConfig->breathDuration;

  double increment = 12.573 / (double)breathDuration;

  while (!m_stop) {
    for (double i = 0; (int)i < limit + delayBetweenBreaths; i += increment) {
      if (m_stop)
        break;
      x = qMin((double)i, limit - 1);
      emit updateBreathPacer(x);
      QThread::currentThread()->msleep(10);
    }
    for (double i = limit; (int)i >= 0 - delayBetweenBreaths; i -= increment) {
      if (m_stop)
        break;
      x = qMax((double)i, 0.0);
      emit updateBreathPacer(x);
      QThread::currentThread()->msleep(10);
    }
  }
}

void BreathMeter::stop() { m_stop = true; }
