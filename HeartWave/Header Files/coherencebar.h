#ifndef COHERENCEBAR_H
#define COHERENCEBAR_H

#include "enums.h"
#include <QWidget>

class CoherenceBar : public QWidget {
  Q_OBJECT
public:
  explicit CoherenceBar(QWidget *parent = nullptr);
  //  QSize sizeHint() const override;

public slots:
  void setCoherenceState(CoherenceState state);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  CoherenceState m_coherenceState;
public slots:
  void onUpdateCoherence(double val, bool update);
};

#endif // COHERENCEBAR_H
