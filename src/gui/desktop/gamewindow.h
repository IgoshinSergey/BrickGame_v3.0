#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <dlfcn.h>

#include <QDialog>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

#include "../../brick_game/common/common.h"

typedef GameInfo_t (*UpdateCurrentStateFunc)();
typedef void (*UserInputFunc)(UserAction_t, bool);
typedef void (*InitAllInfoFunc)();
typedef void (*DestroyGameFunc)();
typedef void (*SelectGameFunc)(int);
typedef void (*FreeGameInfoFunc)(int **, int **);

class GameWidget : public QDialog {
  Q_OBJECT
 public:
  explicit GameWidget(QWidget *parent = nullptr, int game_id = 1);
  ~GameWidget();

 protected:
  void paintEvent(QPaintEvent *e) override;
  void keyPressEvent(QKeyEvent *e) override;

 private:
  QTimer *timer;
  int game_id;
  void *handle;
  FreeGameInfoFunc FreeGameInfo;
  UserInputFunc userInput;
  UpdateCurrentStateFunc updateCurrentState;
};

#endif  // GAMEWINDOW_H
