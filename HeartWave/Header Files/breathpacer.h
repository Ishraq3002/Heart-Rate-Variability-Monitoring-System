
#ifndef BREATHPACER_H
#define BREATHPACER_H

#include "breathmeter.h"
#include <QFrame>
#include <QWidget>

class BreathPacer : public QFrame {

public:
  BreathPacer(QWidget *pQParent = nullptr);
  ~BreathPacer();
  void begin();
  void end();

  BreathPacer(const BreathPacer &) = delete;
  BreathPacer &operator=(const BreathPacer &) = delete;

private:
  double x;
  BreathMeter *bmThread;

private slots:
  void onUpdateBreathPacer(double _x);

protected:
  virtual void paintEvent(QPaintEvent *pQEvent) override;
};
#endif // BREATHPACER_H
