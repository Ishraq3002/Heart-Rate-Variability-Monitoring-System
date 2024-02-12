
#ifndef BREATHMETER_H
#define BREATHMETER_H

#include <QObject>
#include <QThread>

class BreathMeter : public QThread {

  Q_OBJECT
public:
  explicit BreathMeter(QObject *parent = nullptr);
  void run();
  void stop();

signals:
  void updateBreathPacer(double);

private:
  bool m_stop;
};

#endif // BREATHMETER_H
