#ifndef CLABAR_H
#define CLABAR_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class CLABar : public QWidget {
  Q_OBJECT
public:
  explicit CLABar(QWidget *parent = nullptr);

  void setCoherence(float coherence);
  void setLength(int length);
  void setAchievement(float achievement);

  int getLength();
  float getAchievement();

signals:
  void coherenceChanged(float coherence);
  void lengthChanged(int length);
  void achievementChanged(float achievement);

private:
  QHBoxLayout *m_layout;
  QLabel *m_coherenceLabel;
  QLabel *m_lengthLabel;
  QLabel *m_achievementLabel;

  float m_coherence;
  int m_length;
  float m_achievement;

public slots:
  void onUpdateCoherence(double, bool);
};

#endif // CLABAR_H
