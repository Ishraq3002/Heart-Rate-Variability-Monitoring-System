
#ifndef NEWSESSIONVIEW_H
#define NEWSESSIONVIEW_H

#include "breathpacer.h"
#include "clabar.h"
#include "coherencecalculator.h"
#include "enums.h"
#include "heartrategraph.h"
#include "heartrateindicator.h"
#include "navigationbar.h"
#include "navigationpad.h"
#include "screen.h"
#include "screencontextmanager.h"
#include <QKeyEvent>
#include <QWidget>

class NewSessionView : public QWidget {
  Q_OBJECT
public:
  explicit NewSessionView(NavigationBar *_navBar, NavigationPad *_navPad,
                          Screen *scr, QWidget *parent = nullptr);
  ~NewSessionView();
  NavigationBar *navigationBar;
  NavigationPad *navigationPad;
  ScreenContextManager *contextManager;

private:
  HeartRateGraph *hrgraph;
  CoherenceCalculator *coherenceCalculator;
  CLABar *claBar;
  BreathPacer *bp;
  Screen *screen;
  HeartRateIndicator *hrIndicator;

  void endSession();
signals:
  void changeScreenContext(ScreenContext);

private slots:
  void onUpButtonClicked();
  void onDownButtonClicked();
  void onLeftButtonClicked();
  void onRightButtonClicked();
  void onCenterButtonClicked();
  void onBackButtonClicked();
  void onMenuButtonClicked();

  void onHeartRateSensorStateChanged(bool);

public slots:
  void onPlottingFinished();

protected:
  void keyPressEvent(QKeyEvent *event);
};

#endif // NEWSESSIONVIEW_H
