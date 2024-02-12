
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "deviceconfig.h"
#include "screen.h"
#include "settingsconfig.h"
#include <QKeyEvent>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
} // namespace Ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  static SettingsConfig *settingsConfig;
  static DeviceConfig *deviceConfig;

private:
  Ui::MainWindow *ui;
  Screen *screen;
  HeartRateIndicator *hrIndicator;
};

#endif // MAINWINDOW_H
