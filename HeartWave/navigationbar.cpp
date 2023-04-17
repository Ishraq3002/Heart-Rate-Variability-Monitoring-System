#include "navigationbar.h"
#include <QCoreApplication>
#include <QDebug>

NavigationBar::NavigationBar(QWidget *parent) : QWidget(parent) {
  // Set up the layout
  m_layout = new QHBoxLayout(this);
  m_layout->setAlignment(Qt::AlignLeft);

  // Set up the back button
  m_backButton = new QPushButton(this);
  m_backButton->setFixedSize(50, 50);
  QString backButtonIconPath = QString::fromStdString(
      QCoreApplication::applicationDirPath().toStdString() + "/back.svg");
  m_backButton->setIcon(QIcon(backButtonIconPath));
  m_backButton->setIconSize(QSize(40, 40));
  m_backButton->setFlat(true);

  m_backButton->setStyleSheet(
      "QPushButton { border-radius: 5px; "
      "border: 2px solid black;}"
      "QPushButton:hover:pressed { background-color: #dadbde}");

  connect(m_backButton, &QPushButton::clicked, this,
          &NavigationBar::onBackButtonClicked);

  // Add a spacer between the back button and the menu button
  m_layout->addWidget(m_backButton);
  m_layout->addSpacerItem(
      new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

  // Set up the menu button
  m_menuButton = new QPushButton(tr("Menu"), this);
  QFont font = m_menuButton->font();
  font.setPointSizeF(font.pointSizeF() *
                     2); // Increase font size by a factor of 2
  m_menuButton->setFont(font);
  m_menuButton->setFixedSize(90, 50);

  m_menuButton->setFlat(true);
  m_menuButton->setStyleSheet(
      "QPushButton { border-radius: 5px; "
      "border: 2px solid black;"
      "font-weight: bold;}"
      "QPushButton:hover:pressed { background-color: #dadbde}");

  connect(m_menuButton, &QPushButton::clicked, this,
          &NavigationBar::onMenuButtonClicked);

  // Add the menu button to the layout
  m_layout->addWidget(m_menuButton);
  m_layout->setAlignment(m_menuButton, Qt::AlignRight);

  this->setContentsMargins(40, 0, 30, 0);
}

NavigationBar::~NavigationBar() {
  delete m_menuButton;
  delete m_backButton;
  delete m_layout;
}

void NavigationBar::onBackButtonClicked() {
    qInfo() << "Back button pressed";
    emit backButtonClicked();
}

void NavigationBar::onMenuButtonClicked() {
    qInfo() << "Menu button pressed";
    emit menuButtonClicked();
}
