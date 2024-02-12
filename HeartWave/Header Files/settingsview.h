#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include "enums.h"
#include "navigationbar.h"
#include "navigationpad.h"
#include "settingsconfig.h"
#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>

class SettingsView : public QWidget {
  Q_OBJECT
public:
  explicit SettingsView(NavigationBar *_navigationBar,
                        NavigationPad *_navigationPad,
                        QWidget *parent = nullptr);

signals:
  void goBack();
  void changeScreenContext(ScreenContext);

public slots:
  void onUpButtonClicked();
  void onDownButtonClicked();
  void onLeftButtonClicked();
  void onRightButtonClicked();
  void onCenterButtonClicked();

  void onMenuButtonClicked();
  void onBackButtonClicked();

private:
  QLabel *resetToDefaultLabel;
  QLabel *breathDurationLabel;
  QLabel *challengeLevelLabel;
  QVBoxLayout *mainLayout;
  QListView *listView;
  SettingsConfig settingsConfig;

  int breathDuration = 10;
  QStringList challengeLevels =
      QStringList{"Beginner", "Intermediate", "Advanced", "Master"};

  QStringListModel *model;

  int selectedRow = 0;
  int selectedChallengeLevelIndex = 0;

  void updateLabels();

  NavigationBar *navigationBar;
  NavigationPad *navigationPad;
};

#endif // SETTINGSVIEW_H
