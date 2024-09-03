#include "gamewindow.h"

GameWidget::GameWidget(QWidget *parent, int game_id)
    : QDialog{parent}, timer{new QTimer(this)}, game_id{game_id} {
  setFixedSize(470, 605);
  setFocusPolicy(Qt::StrongFocus);
  setWindowTitle("BrickGame v3.0");

  if (game_id == 1) {
    handle = dlopen("./libtetris.so", RTLD_LAZY);
    InitAllInfoFunc InitAllInfo = (InitAllInfoFunc)dlsym(handle, "InitAllInfo");
    InitAllInfo();
  } else if (game_id == 2) {
    handle = dlopen("./libsnake.so", RTLD_LAZY);
  } else {
    handle = dlopen("./libclient.so", RTLD_LAZY);
    SelectGameFunc SelectGame = (SelectGameFunc)dlsym(handle, "SelectGame");
    SelectGame(3);
    FreeGameInfo = (FreeGameInfoFunc)dlsym(handle, "FreeGameInfo");
  }
  userInput = (UserInputFunc)dlsym(handle, "userInput");
  updateCurrentState =
      (UpdateCurrentStateFunc)dlsym(handle, "updateCurrentState");
  connect(timer, &QTimer::timeout, this,
          static_cast<void (QWidget::*)()>(&QWidget::update));
  timer->start(20);
}

GameWidget::~GameWidget() {
  if (game_id == 1) {
    DestroyGameFunc DestroyGame = (DestroyGameFunc)dlsym(handle, "DestroyGame");
    DestroyGame();
  }
  userInput(Terminate, false);
  dlclose(handle);
  delete timer;
}

void GameWidget::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e)
  QPainter p(this);
  p.drawRect(0, 0, 300, 600);
  GameInfo_t game = updateCurrentState();
  for (int i{}; i < 20; ++i) {
    for (int j{}; j < 10; ++j) {
      if (game.field[i][j]) {
        p.setBrush(Qt::black);
      } else {
        p.setBrush(Qt::white);
      }
      p.drawRect(30 * j, 30 * i, 30, 30);
    }
  }

  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
      if (game.next[i][j]) {
        p.setBrush(Qt::black);
      } else {
        p.setBrush(Qt::white);
      }
      p.drawRect(30 * j + 310, 30 * i + 35, 30, 30);
    }
  }

  QFont font;
  font.setPointSize(14);
  p.setFont(font);
  p.drawText(305, 25, "Next:");
  std::string s = "Score: " + std::to_string(game.score);
  p.drawText(305, 185, s.data());
  std::string hs = "High score: " + std::to_string(game.high_score);
  p.drawText(305, 205, hs.data());
  std::string l = "Level: " + std::to_string(game.level);
  p.drawText(305, 225, l.data());
  std::string sp = "Speed: " + std::to_string(game.speed);
  p.drawText(305, 245, sp.data());
  if (game.pause) {
    p.drawText(305, 265, "Pause");
  }
  if (game_id == 3) {
    FreeGameInfo(game.field, game.next);
  }
}

void GameWidget::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Down) {
    userInput(Down, false);
  } else if (e->key() == Qt::Key_Up) {
    userInput(Up, false);
  } else if (e->key() == Qt::Key_Left) {
    userInput(Left, false);
  } else if (e->key() == Qt::Key_Right) {
    userInput(Right, false);
  } else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
    userInput(Start, false);
  } else if (e->key() == Qt::Key_Escape) {
    QWidget::close();
  } else if (e->key() == 'p' || e->key() == 'P') {
    userInput(Pause, false);
  } else if (e->key() == 'z' || e->key() == 'Z') {
    userInput(Action, false);
  }
}
