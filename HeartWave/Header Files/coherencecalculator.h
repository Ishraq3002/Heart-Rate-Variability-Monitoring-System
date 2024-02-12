
#ifndef COHERENCECALCULATOR_H
#define COHERENCECALCULATOR_H

#include "clabar.h"
#include "coherencebar.h"
#include <QObject>
#include <QThread>

struct HRV_Coherence_Result {
  double meanHR;
  double sdNN;
  double rmssd;
  double hrvCoherence;
};

class CoherenceCalculator : public QThread {
  Q_OBJECT
public:
  explicit CoherenceCalculator(CLABar *_claBar, CoherenceBar *_coherenceBar,
                               QObject *parent = nullptr);
  QVector<double> heartRateValues;
  HRV_Coherence_Result *currentCoherenceResult;
  QVector<double> coherenceValues;
  void run();

private:
  CLABar *claBar;
  CoherenceBar *coherenceBar;
  void calculateHRV_Coherence(QVector<double> heartRateValues);
  int _inc;
  double _coherenceTempSum;
signals:
  void updateCoherenceValue(double, bool);

public slots:
  void onUpdateData(double, double);
};

#endif // COHERENCECALCULATOR_H
