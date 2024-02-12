
#include "screencontextmanager.h"
#include <QDebug>

ScreenContextManager::ScreenContextManager(QObject *parent) : QObject{parent} {}

ScreenContextManager::~ScreenContextManager() {}

void ScreenContextManager::onScreenContextChange(ScreenContext context) {
  emit changeScreen(context);
}
