#include "coherencebar.h"
#include <QPainter>

CoherenceBar::CoherenceBar(QWidget *parent)
    : QWidget(parent), m_coherenceState(None) {
  setFixedSize(140, 5);
}

void CoherenceBar::setCoherenceState(CoherenceState state) {
  if (m_coherenceState != state) {
    m_coherenceState = state;
    update();
  }
}

void CoherenceBar::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  const int lineLength = 40;
  const int lineSpacing = 10;
  const int lineY = height() / 2;

  // Draw inactive lines
  QPen pen(Qt::black, 1, Qt::SolidLine);
  painter.setPen(pen);
  painter.drawLine(0, lineY, lineLength, lineY);
  painter.translate(lineLength + lineSpacing, 0);
  painter.drawLine(0, lineY, lineLength, lineY);
  painter.translate(lineLength + lineSpacing, 0);
  painter.drawLine(0, lineY, lineLength, lineY);

  // Draw active line
  switch (m_coherenceState) {
  case None:
    break;
  case LowCoherence:
    painter.translate(-(lineLength + lineSpacing) * 2, 0);
    pen.setColor(QColor(255, 0, 0)); // red
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(0, lineY, lineLength, lineY);
    break;
  case MediumCoherence:
    painter.translate(-(lineLength + lineSpacing), 0);
    pen.setColor(QColor(0, 0, 255)); // blue
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(0, lineY, lineLength, lineY);
    break;
  case HighCoherence:
    pen.setColor(QColor(0, 255, 0)); // green
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(0, lineY, lineLength, lineY);
    break;
  }
}

void CoherenceBar::onUpdateCoherence(double val, bool update) {
  if (!update)
    return;
  if (val <= 0) {
    setCoherenceState(CoherenceState::LowCoherence);
  }

  else if (val < 0.5) {
    setCoherenceState(CoherenceState::MediumCoherence);
  }

  else {
    setCoherenceState(CoherenceState::HighCoherence);
  }
}
