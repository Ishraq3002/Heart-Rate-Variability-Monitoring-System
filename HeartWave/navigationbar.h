#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

class NavigationBar : public QWidget {
  Q_OBJECT

public:
  explicit NavigationBar(QWidget *parent = nullptr);
  ~NavigationBar();
signals:

    void backButtonClicked();
    void menuButtonClicked();
public slots:
  void onBackButtonClicked();
  void onMenuButtonClicked();

private:
  QHBoxLayout *m_layout;
  QPushButton *m_backButton;
  QPushButton *m_menuButton;
};

#endif // NAVIGATIONBAR_H
