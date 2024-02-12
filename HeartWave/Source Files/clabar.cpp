#include "clabar.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

CLABar::CLABar(QWidget *parent)
    : QWidget(parent), m_layout(new QHBoxLayout),
      m_coherenceLabel(
          new QLabel(QString("Coherence\n%1").arg(0.0f, 0, 'f', 1))),
      m_lengthLabel(new QLabel(QString("Length\n%1.%2").arg(0).arg(0))),
      m_achievementLabel(
          new QLabel(QString("Achievement\n%1").arg(0.0f, 0, 'f', 1))),
      m_coherence(0.0f), m_length(0.0f), m_achievement(0.0f) {
  // Set up the coherence label
  m_coherenceLabel->setAlignment(Qt::AlignCenter);
  m_layout->addWidget(m_coherenceLabel);

  // Set up the length label
  m_lengthLabel->setAlignment(Qt::AlignCenter);
  m_layout->addWidget(m_lengthLabel);

  // Set up the achievement label
  m_achievementLabel->setAlignment(Qt::AlignCenter);
  m_layout->addWidget(m_achievementLabel);

  // Set up the layout
  setLayout(m_layout);

  this->setMaximumWidth(300);

  QFont font("Ubuntu Mono");
  font.setWeight(QFont::Bold);

  font.setPixelSize(18);
  this->setFont(font);

  this->setContentsMargins(0, 0, 10, 0);
}

void CLABar::setCoherence(float coherence) {
  if (coherence != m_coherence) {
    m_coherence = coherence;
    m_coherenceLabel->setText(
        QString("Coherence\n%1").arg(m_coherence, 0, 'f', 1));
    emit coherenceChanged(m_coherence);
  }
}

void CLABar::setLength(int length) {
  if (length != m_length) {
    m_length = length;

    QString minutes =
        QStringLiteral("%1").arg(m_length / 60, 2, 10, QLatin1Char('0'));

    QString seconds =
        QStringLiteral("%1").arg(m_length % 60, 2, 10, QLatin1Char('0'));

    m_lengthLabel->setText(QString("Length\n%1.%2").arg(minutes).arg(seconds));
    emit lengthChanged(m_length);
  }
}

void CLABar::setAchievement(float achievement) {
  if (achievement != m_achievement) {
    m_achievement = achievement;
    m_achievementLabel->setText(
        QString("Achievement\n%1").arg(m_achievement, 0, 'f', 1));
    emit achievementChanged(m_achievement);
  }
}

int CLABar::getLength() {
  qInfo() << "Returning length: " << m_length;
  return m_length;
}
float CLABar::getAchievement() { return m_achievement; }

void CLABar::onUpdateCoherence(double val, bool update) {
  if (update) {
    setCoherence(static_cast<float>(val));
    setLength(m_length + 1);
  } else {
    setLength(m_length + 1);
  }
}
