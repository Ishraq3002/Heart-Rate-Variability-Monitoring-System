#ifndef NAVIGATIONPAD_H
#define NAVIGATIONPAD_H

#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

class NavigationPad : public QWidget {
  Q_OBJECT

public:
  explicit NavigationPad(QWidget *parent = nullptr);

private:
  QGridLayout *gridLayout;
  QPushButton *upButton;
  QPushButton *downButton;
  QPushButton *leftButton;
  QPushButton *rightButton;
  QPushButton *centerButton;

signals:
  void upButtonClicked();
  void downButtonClicked();
  void leftButtonClicked();
  void rightButtonClicked();
  void centerButtonClicked();

private slots:
  void onUpButtonClicked();
  void onDownButtonClicked();
  void onLeftButtonClicked();
  void onRightButtonClicked();
  void onCenterButtonClicked();
};

#endif // NAVIGATIONPAD_H
