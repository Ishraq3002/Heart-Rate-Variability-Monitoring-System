
#include "plotter.h"
#include <QDebug>
#include <QTime>
#include <math.h>
#include <random>

Plotter::Plotter(SessionSimulation _mode, QObject *parent) : QThread{parent} {
  mode = _mode;
}

void Plotter::run() {
  double i = 0;
  double bpm = 0;
  m_stop = false;

  while (!m_stop) {
    i += 1;
    if (mode == SessionSimulation::GoodCoherence)
      bpm = 60 + 10 * sin(2 * 3.1416 * i / 20);
    else
      bpm = 60 + 10 * (((double)qrand() / (double)RAND_MAX) - 0.5);

    emit updateData(i, bpm);
    if (!m_stop)
      QThread::currentThread()->msleep(1000);
  }
}

void Plotter::stop() { m_stop = true; }

void Plotter::cycleCoherenceMode() {
  if (mode == SessionSimulation::GoodCoherence) {
    mode = SessionSimulation::PoorCoherence;
  } else {
    mode = SessionSimulation::GoodCoherence;
  }
  qInfo() << "Coherence state changed ---"
          << ((mode == SessionSimulation::GoodCoherence) ? "Good Coherence"
                                                         : "Poor Coherence");
}
