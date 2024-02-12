
#ifndef HEARTRATEGRAPH_H
#define HEARTRATEGRAPH_H

#include "plotter.h"
#include "qcustomplot.h"
#include <QWidget>

class HeartRateGraph : public QWidget {
  Q_OBJECT
public:
  explicit HeartRateGraph(QWidget *parent = nullptr);
  ~HeartRateGraph();

  // TODO: make these private
  void begin();
  void end();
  Plotter *p;
  QVector<double> x, y;

signals:
  void plottingFinished();
private slots:
  void onStopSession();
  void onStartSession();
  void onUpdateData(double _x, double _y);
  void onPlottingFinished();

private:
  QCustomPlot *mPlot;
};

#endif // HEARTRATEGRAPH_H
