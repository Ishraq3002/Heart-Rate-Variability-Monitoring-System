#include "navigationpad.h"
#include <QCoreApplication>
#include <QDebug>

NavigationPad::NavigationPad(QWidget *parent) : QWidget(parent) {
  // Create the layout
  gridLayout = new QGridLayout(this);

  // Create the buttons
  upButton = new QPushButton(this);
  downButton = new QPushButton(this);
  leftButton = new QPushButton(this);
  rightButton = new QPushButton(this);
  centerButton = new QPushButton(this);

  QString upButtonIconPath = QString::fromStdString(
      QCoreApplication::applicationDirPath().toStdString() + "/nav_up.svg");
  QString downButtonIconPath = QString::fromStdString(
      QCoreApplication::applicationDirPath().toStdString() + "/nav_down.svg");
  QString leftButtonIconPath = QString::fromStdString(
      QCoreApplication::applicationDirPath().toStdString() + "/nav_left.svg");
  QString rightButtonIconPath = QString::fromStdString(
      QCoreApplication::applicationDirPath().toStdString() + "/nav_right.svg");

  QString circleIconPath = QString::fromStdString(
      QCoreApplication::applicationDirPath().toStdString() + "/circle.svg");

  // Set the icons for the buttons

  int iconSize = 70;

  upButton->setFixedSize(iconSize, iconSize);
  upButton->setIcon(QIcon(upButtonIconPath));
  upButton->setIconSize(QSize(iconSize, iconSize));

  downButton->setFixedSize(iconSize, iconSize);
  downButton->setIcon(QIcon(downButtonIconPath));
  downButton->setIconSize(QSize(iconSize, iconSize));

  leftButton->setFixedSize(iconSize, iconSize);
  leftButton->setIcon(QIcon(leftButtonIconPath));
  leftButton->setIconSize(QSize(iconSize, iconSize));

  rightButton->setFixedSize(iconSize, iconSize);
  rightButton->setIcon(QIcon(rightButtonIconPath));
  rightButton->setIconSize(QSize(iconSize, iconSize));

  centerButton->setFixedSize(iconSize, iconSize);
  centerButton->setIcon(QIcon(circleIconPath));
  centerButton->setIconSize(QSize(iconSize - 10, iconSize - 10));

  // Remove borders from the buttons
  upButton->setFlat(true);
  downButton->setFlat(true);
  leftButton->setFlat(true);
  rightButton->setFlat(true);
  centerButton->setFlat(true);

  // Button emits signals
  connect(upButton, &QPushButton::clicked, this,
          &NavigationPad::onUpButtonClicked);
  connect(downButton, &QPushButton::clicked, this,
          &NavigationPad::onDownButtonClicked);
  connect(leftButton, &QPushButton::clicked, this,
          &NavigationPad::onLeftButtonClicked);
  connect(rightButton, &QPushButton::clicked, this,
          &NavigationPad::onRightButtonClicked);
  connect(centerButton, &QPushButton::clicked, this,
          &NavigationPad::onCenterButtonClicked);

  // Add the buttons to the layout
  gridLayout->addWidget(upButton, 0, 1);
  gridLayout->addWidget(downButton, 2, 1);
  gridLayout->addWidget(leftButton, 1, 0);
  gridLayout->addWidget(rightButton, 1, 2);
  gridLayout->addWidget(centerButton, 1, 1);

  // Set the layout for the widget
  setLayout(gridLayout);

  this->setMaximumHeight(250);
  this->setMaximumWidth(250);
}

void NavigationPad::onUpButtonClicked() { emit upButtonClicked(); }

void NavigationPad::onDownButtonClicked() { emit downButtonClicked(); }

void NavigationPad::onLeftButtonClicked() { emit leftButtonClicked(); }

void NavigationPad::onRightButtonClicked() { emit rightButtonClicked(); }

void NavigationPad::onCenterButtonClicked() { emit centerButtonClicked(); }
