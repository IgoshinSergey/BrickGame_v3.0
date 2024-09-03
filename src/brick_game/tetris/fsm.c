#include "inc/tetris.h"

GameInfo_t updateCurrentState() {
  AllInfo_t* info = GetAllInfo();
  StateTetris_t* state = GetState();
  bool result;

  if (!info->player.pause && info->player.start) {
    struct timespec cur_time = {0};
    double difference = 0;

    switch (*state) {
      case StartGame:
        *state = Spawn;
        break;
      case Spawn:
        SpawnFigure(info);
        *state = Moving;
        break;
      case Moving:
        clock_gettime(CLOCK_REALTIME, &cur_time);
        difference =
            (cur_time.tv_sec - info->last_drop_time.tv_sec) * 1000 +
            (cur_time.tv_nsec - info->last_drop_time.tv_nsec) / 1000000;
        if (difference >= info->player.speed) {
          clock_gettime(CLOCK_REALTIME, &info->last_drop_time);
          *state = Shifting;
        }
        break;
      case Shifting:
        result = MoveDown(info);
        if (!result) {
          *state = Attaching;
        } else {
          *state = Moving;
        }
        break;
      case Attaching:
        if (IsEndGame(info)) {
          SetTetrisScore(info->player.high_score);
          InitAllInfo();
          *state = StartGame;
        } else {
          ConnectFigure(info);
          UpdateAllInfo(info);
          *state = Spawn;
        }
        break;
      default:
        break;
    }
  }

  GameInfo_t display = MakeGameInfo(info);
  return display;
}

void userInput(UserAction_t action, bool hold) {
  AllInfo_t* info = GetAllInfo();
  StateTetris_t* state = GetState();
  bool pause = info->player.pause;

  if (!hold) {
    switch (action) {
      case Start:
        if (*state == StartGame) {
          info->player.start = 1;
        }
        break;
      case Pause:
        if (*state != StartGame) {
          info->player.pause = !info->player.pause;
        }
        break;
      case Terminate:
        SetTetrisScore(info->player.high_score);
        InitAllInfo();
        *state = StartGame;
        break;
      case Left:
        if (*state == Moving && !pause) {
          MoveLeft(info);
        }
        break;
      case Right:
        if (*state == Moving && !pause) {
          MoveRight(info);
        }
        break;
      case Up:
        break;
      case Down:
        if (*state == Moving && !pause) {
          while (MoveDown(info)) {
            MoveDown(info);
          }
        }
        break;
      case Action:
        if (*state == Moving && !pause) {
          Rotate(info);
        }
        break;
      default:
        break;
    }
  }
}
