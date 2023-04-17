
#ifndef SCREENCONTEXTMANAGER_H
#define SCREENCONTEXTMANAGER_H

#include "enums.h"
#include <QObject>

class ScreenContextManager : public QObject {
  Q_OBJECT
public:
  explicit ScreenContextManager(QObject *parent = nullptr);
  ~ScreenContextManager();

signals:
  void changeScreen(ScreenContext context);

public slots:
  void onScreenContextChange(ScreenContext context);
};

#endif // SCREENCONTEXTMANAGER_H
