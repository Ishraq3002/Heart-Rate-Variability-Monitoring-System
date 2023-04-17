
#include "heartrategraph.h"

HeartRateGraph::HeartRateGraph(QWidget *parent) : QWidget{parent} {

  QHBoxLayout *layout = new QHBoxLayout(this);

  layout->setMargin(0);

  mPlot = new QCustomPlot(this);

  mPlot->addGraph();
  mPlot->graph(0)->setData(x, y);

  mPlot->xAxis->setLabel("Time (Seconds)");
  mPlot->yAxis->setLabel("Heart Rate (BPM)");

  mPlot->xAxis->setRange(0, 300);
  mPlot->yAxis->setRange(40, 130);

  mPlot->replot();
  mPlot->setMaximumHeight(200);
  mPlot->setMaximumWidth(400);
  mPlot->setMinimumHeight(200);
  mPlot->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

  p = new Plotter(SessionSimulation::GoodCoherence, this);

  connect(p, &Plotter::updateData, this, &HeartRateGraph::onUpdateData);

  connect(p, &Plotter::finished, this, &HeartRateGraph::onPlottingFinished);

  layout->addWidget(mPlot);

  this->setLayout(layout);
  this->setContentsMargins(0, 20, 0, 20);
}

HeartRateGraph::~HeartRateGraph() {
  this->end();
  HeartRateGraph::x.clear();
  HeartRateGraph::y.clear();
}

void HeartRateGraph::begin() {
  // TODO: start session on button press
  p->start();
}

void HeartRateGraph::end() {
  // TODO: end session on button press
  if (p->isRunning()) {
    p->stop();
    p->wait();
  }
}

void HeartRateGraph::onStopSession() { end(); }

void HeartRateGraph::onStartSession() { begin(); }

void HeartRateGraph::onUpdateData(double _x, double _y) {
  x.push_back(_x);
  y.push_back(_y);
  mPlot->graph(0)->setData(x, y);
  mPlot->replot();
  if (x.length() == 300) {
    emit plottingFinished();
    this->end();
  }
}

void HeartRateGraph::onPlottingFinished() { qInfo() << "Plotting finished"; }
