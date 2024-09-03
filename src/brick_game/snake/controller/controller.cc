#include "controller.h"

namespace s21 {

Controller::Controller(SnakeGame* model) : model{model} {}
Controller::~Controller() {}

void Controller::userInput(UserAction_t action, bool hold) {
  auto state = model->state;
  auto pause = model->info.pause;
  if (!hold) {
    switch (action) {
      case Start:
        model->start = true;
        break;
      case Pause:
        if (state != StartGame) model->info.pause = !pause;
        break;
      case Terminate:
        model->SetSnakeScore(model->info.high_score);
        model->ClearField();
        model->snake.clear();
        model->state = StartGame;
        break;
      case Left:
        if (model->snake_direction != RightDirection) {
          model->next_direction = LeftDirection;
        }
        break;
      case Right:
        if (model->snake_direction != LeftDirection) {
          model->next_direction = RightDirection;
        }
        break;
      case Up:
        if (model->snake_direction != DownDirection) {
          model->next_direction = UpDirection;
        }
        break;
      case Down:
        if (model->snake_direction != UpDirection) {
          model->next_direction = DownDirection;
        }
        break;
      case Action:
        model->MoveSnake();
        break;
      default:
        break;
    }
  }
}

GameInfo_t Controller::updateCurrentState() {
  if (!model->info.pause && model->start) {
    struct timespec current;
    double difference;
    switch (model->state) {
      case StartGame:
        model->InitStartInfo();
        model->state = Moving;
        break;
      case Spawn:
        model->RandomApple();
        model->state = Moving;
        break;
      case Shifting:
        model->MoveSnake();
        break;
      case Moving:
        clock_gettime(CLOCK_REALTIME, &current);
        difference =
            (current.tv_sec - model->last_move_time.tv_sec) * 1000 +
            (current.tv_nsec - model->last_move_time.tv_nsec) / 1000000;
        if (difference >= model->info.speed) {
          model->state = Shifting;
        }
        break;
      default:
        break;
    }
  }
  model->ConnectFiguresAndField();
  return model->info;
}

}  // namespace s21

s21::Controller& GetController() {
  static s21::SnakeGame model{};
  static s21::Controller controller{&model};
  return controller;
}

void userInput(UserAction_t action, bool hold) {
  s21::Controller& c = GetController();
  c.userInput(action, hold);
}

GameInfo_t updateCurrentState() {
  s21::Controller& c = GetController();
  return c.updateCurrentState();
}
