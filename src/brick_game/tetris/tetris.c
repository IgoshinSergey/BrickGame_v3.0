#include "inc/tetris.h"

StateTetris_t* GetState() {
  static StateTetris_t state = 0;
  return &state;
}

AllInfo_t* GetAllInfo() {
  static AllInfo_t all_info = {NULL, NULL, NULL, {0}, {0}, 0};
  return &all_info;
}

GameInfo_t* GetGameInfo() {
  static GameInfo_t game_info = {NULL, NULL, 0, 0, 0, 0, 0};
  return &game_info;
}

GameInfo_t MakeGameInfo(AllInfo_t* info) {
  GameInfo_t* display = GetGameInfo();
  StateTetris_t* state = GetState();
  if (display->field != NULL) {
    FreeMatrix(display->field, 20);
  }
  display->field = InitMatrix(20, 10);
  int** disp = display->field;
  int** g = info->field;
  Figure_t* figure = info->current;

  if (figure && *state != StartGame && *state != Spawn) {
    int** f = figure->coordinates;
    int x_f = figure->x;
    int y_f = figure->y;
    for (int i = 0; i < 20; i++) {
      int len_f = figure->length;
      for (int j = 0; j < 10; j++) {
        bool flag = false;
        if (i >= y_f && j >= x_f && (i - y_f < len_f) && (j - x_f < len_f)) {
          if (f[i - y_f][j - x_f]) {
            flag = true;
          } else {
            flag = (g[i][j]) ? true : false;
          }
        } else {
          flag = (g[i][j]) ? true : false;
        }
        if (flag) {
          disp[i][j] = 1;
        }
      }
    }
  } else if (g) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        if (g[i][j]) {
          disp[i][j] = 1;
        }
      }
    }
  }

  if (info->next) {
    display->next = info->next->coordinates;
  } else {
    display->next = NULL;
  }

  display->field = disp;
  display->high_score = info->player.high_score;
  display->level = info->player.level;
  display->pause = info->player.pause;
  display->score = info->player.score;
  display->speed = info->player.speed;
  return *display;
}

int GetTetrisScore() {
  int high_score = 0;
  FILE* f = fopen("./tetris_score.txt", "r");
  if (f != NULL) {
    int res = fscanf(f, "%d", &high_score);
    if (res) {
      fclose(f);
    }
  }
  return high_score;
}

void SetTetrisScore(int high_score) {
  FILE* f = fopen("./tetris_score.txt", "w");
  if (f != NULL) {
    fprintf(f, "%d", high_score);
    fclose(f);
  }
}

int GetPoints(int** field) {
  int num_row = 19;
  int count_full_lines = 0;
  while (num_row > 0) {
    if (CheckLine(field, num_row)) {
      EraseLine(field, num_row);
      count_full_lines++;
    } else {
      num_row--;
    }
  }
  int points = CalculatePoints(count_full_lines);
  return points;
}

int CalculatePoints(int count) {
  int points = 0;
  switch (count) {
    case 1:
      points = 100;
      break;
    case 2:
      points = 300;
      break;
    case 3:
      points = 700;
      break;
    case 4:
      points = 1500;
      break;
    default:
      break;
  }
  return points;
}

bool IsEndGame(AllInfo_t* info) {
  bool result = false;
  Figure_t* cur = info->current;
  int** coordinates = cur->coordinates;
  int len = cur->length;
  int y = cur->y;

  if (y <= 0) {
    for (int i = 0; i < len; i++) {
      if (coordinates[-y][i]) {
        result = true;
        break;
      }
    }
  }
  return result;
}

void EraseLine(int** field, int row) {
  for (int i = row; i > 0; i--) {
    for (int j = 0; j < 10; j++) {
      field[i][j] = field[i - 1][j];
    }
  }
}

bool CheckLine(int** field, int row) {
  bool flag = true;
  for (int j = 0; j < 10; j++) {
    if (!field[row][j]) {
      flag = false;
      break;
    }
  }
  return flag;
}

void InitAllInfo() {
  AllInfo_t* info = GetAllInfo();
  if (info->field) {
    FreeMatrix(info->field, 20);
  }
  info->field = InitMatrix(20, 10);
  srand(time(NULL));
  SpawnFigure(info);
  clock_gettime(CLOCK_REALTIME, &info->last_drop_time);
  info->player.high_score = GetTetrisScore();
  info->player.level = 0;
  info->player.pause = 0;
  info->player.score = 0;
  info->player.start = 0;
  info->player.speed = 800 - 50 * info->player.level;
}

void UpdateAllInfo(AllInfo_t* info) {
  info->player.score += GetPoints(info->field);
  if (info->player.high_score < info->player.score) {
    info->player.high_score = info->player.score;
  }
  if (info->player.level < 10) {
    info->player.level = info->player.score / 600;
    if (info->player.level > 10) {
      info->player.level = 10;
    }
  }
  info->player.speed = 800 - 50 * info->player.level;
}

void DestroyGame() {
  AllInfo_t* info = GetAllInfo();
  GameInfo_t* display = GetGameInfo();
  if (display->field) {
    FreeMatrix(display->field, 20);
    display->field = NULL;
  }
  if (info->field) {
    FreeMatrix(info->field, 20);
    info->field = NULL;
  }
  if (info->current) {
    FreeFigure(info->current);
    info->current = NULL;
  }
  if (info->next) {
    FreeFigure(info->next);
    info->next = NULL;
  }
  *GetState() = StartGame;
}

bool IsCollision(AllInfo_t info) {
  int** field = info.field;
  int** figure = info.current->coordinates;
  int x = info.current->x;
  int y = info.current->y;
  int len = info.current->length;
  bool flag = false;

  for (int i = 0; i < len && !flag; i++) {
    for (int j = 0; j < len && !flag; j++) {
      int val_figure = figure[i][j];
      if (y + i >= 0) {
        if (x + j >= 0 && x + j < 10 && y + i < 20) {
          int val_field = field[i + y][j + x];
          flag = (val_field == 1 && val_figure == 1) ? true : false;
        } else {
          flag = (val_figure == 1) ? true : false;
        }
      } else if (x + j < 0 || x + j >= 10) {
        flag = true;
      }
    }
  }
  return flag;
}
