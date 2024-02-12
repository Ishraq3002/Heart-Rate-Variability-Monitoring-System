
#include "coherencecalculator.h"
#include "coherencebar.h"
#include <math.h>

CoherenceCalculator::CoherenceCalculator(CLABar *_claBar,
                                         CoherenceBar *_coherenceBar,
                                         QObject *parent)
    : QThread{parent} {
  currentCoherenceResult = new HRV_Coherence_Result();
  claBar = _claBar;
  coherenceBar = _coherenceBar;
  _inc = 0;
  _coherenceTempSum = 0;
  connect(this, &CoherenceCalculator::updateCoherenceValue, claBar,
          &CLABar::onUpdateCoherence);
  connect(this, &CoherenceCalculator::updateCoherenceValue, coherenceBar,
          &CoherenceBar::onUpdateCoherence);
}

void CoherenceCalculator::run() {}

void CoherenceCalculator::calculateHRV_Coherence(
    QVector<double> heartRateValues) {
  // Calculate mean HR
  double meanHR = 0.0;
  for (int i = 0; i < heartRateValues.size(); ++i) {
    meanHR += heartRateValues[i];
  }
  meanHR /= static_cast<double>(heartRateValues.size());

  // Calculate SDNN
  double sumOfSquaredDiffs = 0.0;
  for (int i = 0; i < heartRateValues.size(); ++i) {
    sumOfSquaredDiffs += pow(heartRateValues[i] - meanHR, 2);
  }
  double sdNN = sqrt(sumOfSquaredDiffs / (heartRateValues.size() - 1));

  // Calculate RMSSD
  double sumOfSquaredDiffsBetweenAdjacent = 0.0;
  for (int i = 0; i < heartRateValues.size() - 1; ++i) {
    sumOfSquaredDiffsBetweenAdjacent +=
        pow(heartRateValues[i + 1] - heartRateValues[i], 2);
  }
  double rmssd =
      sqrt(sumOfSquaredDiffsBetweenAdjacent / (heartRateValues.size() - 1));

  // Calculate Coherence
  double numerator = 0.0;
  double denominator1 = 0.0;
  double denominator2 = 0.0;
  for (int i = 0; i < heartRateValues.size() - 1; ++i) {
    numerator +=
        (heartRateValues[i] - meanHR) * (heartRateValues[i + 1] - meanHR);
    denominator1 += pow(heartRateValues[i] - meanHR, 2);
    denominator2 += pow(heartRateValues[i + 1] - meanHR, 2);
  }
  double hrvCoherence = numerator / sqrt(denominator1 * denominator2);

  // Create and return the result struct
  currentCoherenceResult->meanHR = meanHR;
  currentCoherenceResult->sdNN = sdNN;
  currentCoherenceResult->rmssd = rmssd;
  currentCoherenceResult->hrvCoherence = hrvCoherence;
}

void CoherenceCalculator::onUpdateData(double x, double y) {
  heartRateValues.append(y);

  if (heartRateValues.length() >= 64) {
    if (heartRateValues.length() > 64)
      heartRateValues.removeFirst();

    calculateHRV_Coherence(heartRateValues);

    if (_inc == 0) {
      _coherenceTempSum += currentCoherenceResult->hrvCoherence;
      _coherenceTempSum = _coherenceTempSum / 5.0;
      coherenceValues.append(_coherenceTempSum);
      emit updateCoherenceValue(_coherenceTempSum, true);
      _coherenceTempSum = 0;
    }

    else {
      _coherenceTempSum += currentCoherenceResult->hrvCoherence;
      emit updateCoherenceValue(0, false);
    }

    _inc = (_inc + 1) % 5;
  }

  else
    emit updateCoherenceValue(0, false);
}
