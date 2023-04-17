#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "navigationbar.h"

SettingsConfig *MainWindow::settingsConfig = new SettingsConfig();
DeviceConfig *MainWindow::deviceConfig = new DeviceConfig();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setFixedSize(QSize(600, 1000));

  QWidget *mainWidget = new QWidget();
  mainWidget->setObjectName("Main Widget");
  QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

  QVBoxLayout *centralLayout = new QVBoxLayout();
  QHBoxLayout *navPadLayout = new QHBoxLayout();

  //  QVBoxLayout *layout = new QVBoxLayout(this);
  NavigationBar *navBar = new NavigationBar();
  NavigationPad *navPad = new NavigationPad();

  hrIndicator = new HeartRateIndicator();

  // Since we aren't using a physical sensor, set to true to simulate
  hrIndicator->setState(MainWindow::deviceConfig->hrSensorState);

  screen = new Screen(navBar, navPad, hrIndicator);

  centralLayout->addWidget(screen);
  centralLayout->addWidget(navBar);

  navPadLayout->addWidget(navPad);

  navPadLayout->setContentsMargins(0, 0, 0, 50);

  mainLayout->addLayout(centralLayout);
  mainLayout->addLayout(navPadLayout);

  //  mainLayout->addWidget(navPad);

  this->setCentralWidget(mainWidget);
  this->setStyleSheet(
      "* {font-family: Ubuntu Mono; background-color: white; color: black; }");

  int width = 580;
  int height = 500;

  screen->setMaximumHeight(height);
  screen->setMinimumHeight(height);

  screen->setMaximumWidth(width);
  screen->setMinimumWidth(width);

  setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() { delete ui; }
