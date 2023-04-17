#ifndef MAINMENUVEIEW_H
#define MAINMENUVEIEW_H

#include "enums.h"
#include "navigationbar.h"
#include "navigationpad.h"
#include "screen.h"
#include <QListView>
#include <QStringListModel>
#include <QWidget>

class MainMenuView : public QWidget {
  Q_OBJECT
public:
  explicit MainMenuView(NavigationPad *_navigationPad, Screen *scr,
                        QWidget *parent = nullptr);

private:
  QListView *listView;
  QStringListModel *model;
  NavigationBar *navigationBar;
  NavigationPad *navigationPad;
  Screen *screen;

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
};

#endif // MAINMENUVIEW_H
