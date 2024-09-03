#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_MODEL_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_MODEL_H_

#include <time.h>

#include <algorithm>
#include <random>
#include <vector>

#include "../../common/common.h"

namespace s21 {

#define WIDTH 10
#define HEIGTH 20

typedef enum StateSnake_t { StartGame, Spawn, Moving, Shifting } StateSnake_t;

typedef enum Direction {
  UpDirection,
  DownDirection,
  RightDirection,
  LeftDirection
} Direction;

class Point {
 public:
  int x;
  int y;

  Point();
  Point(int x, int y);
  Point RandomPoint(std::vector<Point>& snake);
  Point(const Point& other);
  Point& operator=(const Point& other);
};

class SnakeGame {
 public:
  GameInfo_t info;
  StateSnake_t state;
  std::vector<Point> snake;
  Direction snake_direction;
  Direction next_direction;
  Point apple;
  bool start;
  struct timespec last_move_time;

  SnakeGame();
  ~SnakeGame();

  int GetSnakeScore();
  void SetSnakeScore(int high_score);
  void InitStartInfo();
  void InitSnakeVector();
  void RandomApple();
  void ClearField();
  void ConnectSnakeAndField();
  void ConnectAppleAndField();
  GameInfo_t ConnectFiguresAndField();
  bool CheckCollision();
  void UpdateInfo();
  void MoveSnake();
};

}  // namespace s21

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_MODEL_H_
