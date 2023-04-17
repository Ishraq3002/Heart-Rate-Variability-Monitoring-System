
#ifndef LOGVIEW_H
#define LOGVIEW_H
#include "enums.h"
#include "navigationbar.h"
#include "navigationpad.h"
#include "screen.h"
#include <QFileInfoList>
#include <QListView>
#include <QStringListModel>
#include <QWidget>

class LogView : public QWidget {
  Q_OBJECT
public:
  explicit LogView(NavigationBar *_navigationBar, NavigationPad *_navigationPad,
                   Screen *scr, QWidget *parent = nullptr);

private:
  QListView *listView;
  QFileInfoList fileList;
  QStringList *fileNames;
  QStringListModel *model;
  NavigationBar *navigationBar;
  NavigationPad *navigationPad;
  Screen *screen;
  bool isEmpty;
  void deleteLogs();

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

#endif // LOGVIEW_H
