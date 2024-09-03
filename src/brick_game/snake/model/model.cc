#include "model.h"

namespace s21 {

// class Point
Point::Point() : x(0), y(0) {}
Point::Point(int x, int y) : x(x), y(y) {}
Point::Point(const Point& other) : x(other.x), y(other.y) {}

Point Point::RandomPoint(std::vector<Point>& snake) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> x_dis(0, WIDTH - 1);
  std::uniform_int_distribution<> y_dis(0, HEIGTH - 1);
  Point res;
  do {
    res.x = x_dis(gen);
    res.y = y_dis(gen);
  } while (std::any_of(snake.begin(), snake.end(), [&](const Point& p) {
    return p.x == res.x && p.y == res.y;
  }));
  return res;
}

Point& Point::operator=(const Point& other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
  }
  return *this;
}

SnakeGame::SnakeGame() {
  info.field = new int*[HEIGTH];
  for (int i = 0; i < HEIGTH; ++i) {
    info.field[i] = new int[WIDTH]{};
  }
  info.next = new int*[4];
  for (int i = 0; i < 4; ++i) {
    info.next[i] = new int[4]{};
  }
  state = StateSnake_t::StartGame;
  snake_direction = Direction::DownDirection;
  next_direction = Direction::DownDirection;
  info.score = info.level = 0;
  info.high_score = GetSnakeScore();
  info.pause = start = false;
  info.speed = 600;
  InitSnakeVector();
  clock_gettime(CLOCK_REALTIME, &last_move_time);
}

SnakeGame::~SnakeGame() {
  for (int i = 0; i < HEIGTH; ++i) {
    delete[] info.field[i];
  }
  delete[] info.field;
  for (int i = 0; i < 4; ++i) {
    delete[] info.next[i];
  }
  delete[] info.next;
}

int SnakeGame::GetSnakeScore() {
  int high_score = 0;
  FILE* f = fopen("./snake_score.txt", "r");
  if (f != nullptr) {
    int res = fscanf(f, "%d", &high_score);
    if (res) fclose(f);
  }
  return high_score;
}

void SnakeGame::SetSnakeScore(int high_score) {
  FILE* f = fopen("./snake_score.txt", "w");
  if (f != nullptr) {
    fprintf(f, "%d", high_score);
    fclose(f);
  }
}

void SnakeGame::RandomApple() {
  Point tmp = apple.RandomPoint(snake);
  apple.x = tmp.x;
  apple.y = tmp.y;
}

void SnakeGame::InitStartInfo() {
  ClearField();
  snake.clear();
  InitSnakeVector();
  RandomApple();
  snake_direction = Direction::DownDirection;
  next_direction = Direction::DownDirection;
  ConnectFiguresAndField();
}

void SnakeGame::InitSnakeVector() {
  snake.clear();
  snake.push_back(Point(WIDTH / 2, HEIGTH / 2 + 1));
  snake.push_back(Point(WIDTH / 2, HEIGTH / 2));
  snake.push_back(Point(WIDTH / 2, HEIGTH / 2 - 1));
  snake.push_back(Point(WIDTH / 2, HEIGTH / 2 - 2));
}

void SnakeGame::ClearField() {
  for (int i = 0; i < HEIGTH; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      info.field[i][j] = 0;
    }
  }
}

void SnakeGame::ConnectSnakeAndField() {
  for (const auto& point : snake) {
    info.field[point.y][point.x] = 1;
  }
}

void SnakeGame::ConnectAppleAndField() { info.field[apple.y][apple.x] = 1; }

GameInfo_t SnakeGame::ConnectFiguresAndField() {
  ClearField();
  ConnectAppleAndField();
  ConnectSnakeAndField();
  return info;
}

bool IsOutsideBorders(int x, int y) {
  return (x < 0 || x >= WIDTH || y < 0 || y >= HEIGTH);
}

bool IsCollisionWithBody(int x, int y, const std::vector<Point>& snake) {
  return std::any_of(++(snake.begin()), snake.end(),
                     [&](const Point& p) { return p.x == x && p.y == y; });
}

bool SnakeGame::CheckCollision() {
  Point tmp = snake.front();
  return IsOutsideBorders(tmp.x, tmp.y) ||
         IsCollisionWithBody(tmp.x, tmp.y, snake);
}

void SnakeGame::UpdateInfo() {
  info.score++;
  if (info.high_score < info.score) {
    info.high_score = info.score;
  }
  if (info.level < 10) {
    info.level = info.score / 5;
  }
  info.speed = 600 - 30 * info.level;
}

void SnakeGame::MoveSnake() {
  if (!info.pause && start) {
    Point tmp{snake.front()};
    switch (next_direction) {
      case Direction::UpDirection:
        tmp.y -= 1;
        break;
      case Direction::DownDirection:
        tmp.y += 1;
        break;
      case Direction::RightDirection:
        tmp.x += 1;
        break;
      case Direction::LeftDirection:
        tmp.x -= 1;
        break;
      default:
        break;
    }
    snake.insert(snake.begin(), tmp);
    if (tmp.x == apple.x && tmp.y == apple.y) {
      state = StateSnake_t::Spawn;
      UpdateInfo();
    } else if (CheckCollision()) {
      ClearField();
      snake.clear();
      info.score = info.level = 0;
      info.speed = 600;
      state = StateSnake_t::StartGame;
      start = false;
    } else {
      state = StateSnake_t::Moving;
      snake.pop_back();
    }
    snake_direction = next_direction;
  }
  clock_gettime(CLOCK_REALTIME, &last_move_time);
  ConnectFiguresAndField();
}

}  // namespace s21
