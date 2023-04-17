
#include "screen.h"
#include "initsessionview.h"
#include "logview.h"
#include "mainmenuview.h"
#include "newsessionview.h"
#include "settingsview.h"
#include "summaryview.h"

Screen::Screen(NavigationBar *_navBar, NavigationPad *_navPad,
               HeartRateIndicator *_hrIndicator, QWidget *parent)
    : QWidget{parent} {

  // Set up member variable references
  navBar = _navBar;
  navPad = _navPad;
  hrIndicator = _hrIndicator;
  contextManager = new ScreenContextManager();
  connect(contextManager, &ScreenContextManager::changeScreen, this,
          &Screen::onChangeScreen);

  // -------------  FRAME FOR SCREEN -------- start --------

  QFrame *borderFrame = new QFrame(this);
  borderFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
  borderFrame->setLineWidth(2);
  borderFrame->setMidLineWidth(0);
  borderFrame->setFrameShadow(QFrame::Plain);
  borderFrame->setStyleSheet("background-color: transparent; border: 2px solid "
                             "gray; border-radius: 10px;");
  QRect frameRect = QRect(
      this->geometry().topLeft().x() + 50, this->geometry().topLeft().y() + 50,
      this->geometry().width() - 149, this->geometry().height() - 50);
  borderFrame->setGeometry(frameRect);

  // ------------- FRAME FOR SCREEN ---------- end ----------

  screenLayout = new QVBoxLayout(this);

  this->setMaximumHeight(500);
  this->setMinimumHeight(500);
  this->setContentsMargins(0, 0, 0, 0);

  screenLayout->setAlignment(Qt::AlignCenter);

  // setup first screen as Menu
  onChangeScreen(ScreenContext::Menu);
}

Screen::~Screen() {}

void Screen::loadLogfile(QString _logfileName) {
  loadedLogfileName = _logfileName;
}

void Screen::setFromLogView(bool _value) { fromLogView = _value; }

void Screen::setSelectedLogIndex(int _index) { selectedLogIndex = _index; }

int Screen::getSelectedLogIndex() { return selectedLogIndex; }

void Screen::setSelectedMenuIndex(int _index) { selectedMenuIndex = _index; }

int Screen::getSelectedMenuIndex() { return selectedMenuIndex; }

void Screen::initializeLayout(QLayout *screenLayout) {
  screenLayout = new QVBoxLayout(this);

  this->setMaximumHeight(500);
  this->setMinimumHeight(500);
  this->setContentsMargins(0, 0, 0, 0);

  screenLayout->setAlignment(Qt::AlignCenter);
}

void Screen::onChangeScreen(ScreenContext context) {

  if (context == ScreenContext::Menu) {
    // render main menu on screen
    qInfo() << "Changing to Menu view";
    clearLayout(screenLayout);
    MainMenuView *menuWidget = new MainMenuView(navPad, this);
    screenLayout->addWidget(menuWidget);

    connect(menuWidget, &MainMenuView::changeScreenContext, contextManager,
            &ScreenContextManager::onScreenContextChange);
  }

  else if (context == ScreenContext::NewSession) {
    // render main menu on screen
    qInfo() << "Changing to New Session view";
    clearLayout(screenLayout);
    // Rendering new session view here ---

    NewSessionView *newSessionView = new NewSessionView(navBar, navPad, this);

    connect(newSessionView, &NewSessionView::changeScreenContext,
            contextManager, &ScreenContextManager::onScreenContextChange);

    screenLayout->addWidget(newSessionView);

    // -----------------------------------------
  }

  else if (context == ScreenContext::InitSession) {
    qInfo() << "Changing to InitSession view";
    clearLayout(screenLayout);
    InitSessionView *initSessionView = new InitSessionView(navBar, navPad);
    connect(initSessionView, &InitSessionView::changeScreenContext,
            contextManager, &ScreenContextManager::onScreenContextChange);
    screenLayout->addWidget(initSessionView);

  }

  else if (context == ScreenContext::Log) {
    qInfo() << "Changing to Log view";
    clearLayout(screenLayout);

    LogView *logView = new LogView(navBar, navPad, this);
    connect(logView, &LogView::changeScreenContext, contextManager,
            &ScreenContextManager::onScreenContextChange);
    screenLayout->addWidget(logView);
  }

  else if (context == ScreenContext::Summary) {
    qInfo() << "Changing to Summary view";
    clearLayout(screenLayout);
    SummaryView *summaryView;

    if (fromLogView) {
      summaryView = new SummaryView(loadedLogfileName, navBar);
      screenLayout->addWidget(summaryView);
    } else {
      summaryView = new SummaryView(navBar);
      screenLayout->addWidget(summaryView);
    }

    connect(summaryView, &SummaryView::changeScreenContext, contextManager,
            &ScreenContextManager::onScreenContextChange);

  }

  else if (context == ScreenContext::Settings) {
    qInfo() << "Changing to Settings view";
    clearLayout(screenLayout);

    SettingsView *settingsView = new SettingsView(navBar, navPad);

    connect(settingsView, &SettingsView::changeScreenContext, contextManager,
            &ScreenContextManager::onScreenContextChange);

    screenLayout->addWidget(settingsView);
  }
}

void Screen::clearLayout(QLayout *layout, bool deleteWidgets) {
  while (QLayoutItem *item = layout->takeAt(0)) {
    if (deleteWidgets) {
      if (QWidget *widget = item->widget())
        widget->deleteLater();
    }
    if (QLayout *childLayout = item->layout())
      clearLayout(childLayout, deleteWidgets);
    delete item;
  }
}
