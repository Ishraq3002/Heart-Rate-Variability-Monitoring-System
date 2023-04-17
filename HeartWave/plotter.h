
#ifndef PLOTTER_H
#define PLOTTER_H

#include "enums.h"
#include <QObject>
#include <QThread>
class Plotter : public QThread {
  Q_OBJECT
public:
  explicit Plotter(SessionSimulation _mode = SessionSimulation::GoodCoherence,
                   QObject *parent = nullptr);
  void run();
  void stop();
  void cycleCoherenceMode();

private:
  SessionSimulation mode;
  bool m_stop;
signals:
  void updateData(double, double);
};

#endif // PLOTTER_H
