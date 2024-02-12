
#ifndef SCREEN_H
#define SCREEN_H

#include "heartrateindicator.h"
#include "navigationbar.h"
#include "navigationpad.h"
#include "screencontextmanager.h"

#include "enums.h"
#include <QWidget>

class Screen : public QWidget {
  Q_OBJECT
public:
  explicit Screen(NavigationBar *_navBar, NavigationPad *_navPad,
                  HeartRateIndicator *_hrIndicator, QWidget *parent = nullptr);
  ~Screen();
  void loadLogfile(QString _logfileName);
  void setFromLogView(bool _value);
  void setSelectedLogIndex(int _index);
  int getSelectedLogIndex();

  void setSelectedMenuIndex(int _index);
  int getSelectedMenuIndex();
signals:

private:
  NavigationBar *navBar;
  NavigationPad *navPad;
  ScreenContextManager *contextManager;
  QVBoxLayout *screenLayout;
  HeartRateIndicator *hrIndicator;

  QString loadedLogfileName;
  bool fromLogView;
  int selectedLogIndex = 0;
  int selectedMenuIndex = 0;

  void initializeLayout(QLayout *screenLayout);
  void clearLayout(QLayout *layout, bool deleteWidgets = true);

private slots:
  void onChangeScreen(ScreenContext context);
};

#endif // SCREEN_H
