#ifndef INITSESSIONVIEW_H
#define INITSESSIONVIEW_H

#include "enums.h"
#include "heartrateindicator.h"
#include "navigationbar.h"
#include "navigationpad.h"
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QWidget>

class InitSessionView : public QWidget {
  Q_OBJECT
public:
  explicit InitSessionView(NavigationBar *_navBar, NavigationPad *_navPad,
                           QWidget *parent = nullptr);
signals:
  void changeScreenContext(ScreenContext);

private:
  QHBoxLayout *centralLayout;
  QLabel *m_label;
  NavigationBar *navigationBar;
  HeartRateIndicator *heartRateIndicator;
  NavigationPad *navigationPad;

  protected:
  void keyPressEvent(QKeyEvent *event);
  private slots:
  void onUpButtonClicked();
  void onDownButtonClicked();
  void onLeftButtonClicked();
  void onRightButtonClicked();
  void onCenterButtonClicked();
  void onBackButtonClicked();
  void onMenuButtonClicked();
};

#endif // INITSESSIONVIEW_H
