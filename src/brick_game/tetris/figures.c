#include "inc/figures.h"

int figures[7][4][4][4] = {
    {{{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
     {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}},
    {{{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
    {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
    {{{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}}},
    {{{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}}},
    {{{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}};

Figure_t* RandomFigure() {
  Figure_t* figure = (Figure_t*)calloc(1, sizeof(Figure_t));
  figure->figure_id = rand() % 7;
  figure->rotate_id = 0;
  figure->coordinates = InitMatrix(4, 4);
  figure->length = 4;
  FillMatrix(figure->coordinates, figure->figure_id, figure->rotate_id);
  figure->x = 5 - figure->length / 2;
  figure->y = -figure->length;
  return figure;
}

void FreeFigure(Figure_t* figure) {
  FreeMatrix(figure->coordinates, 4);
  free(figure);
}

bool MoveLeft(AllInfo_t* info) {
  bool result = true;
  Figure_t* figure = info->current;
  figure->x -= 1;
  if (IsCollision(*info)) {
    figure->x += 1;
    result = false;
  }
  return result;
}

bool MoveRight(AllInfo_t* info) {
  bool result = true;
  Figure_t* figure = info->current;
  figure->x += 1;
  if (IsCollision(*info)) {
    figure->x -= 1;
    result = false;
  }
  return result;
}

bool MoveDown(AllInfo_t* info) {
  bool result = true;
  Figure_t* figure = info->current;
  figure->y += 1;
  if (IsCollision(*info)) {
    figure->y -= 1;
    result = false;
  }
  return result;
}

void Rotate(AllInfo_t* info) {
  Figure_t* figure = info->current;
  int current_rotation_id = figure->rotate_id;
  int next_rotation_id = (current_rotation_id + 1) % 4;
  FillMatrix(figure->coordinates, figure->figure_id, next_rotation_id);
  if (IsCollision(*info)) {
    FillMatrix(figure->coordinates, figure->figure_id, current_rotation_id);
  } else {
    figure->rotate_id = next_rotation_id;
  }
}

void ConnectFigure(AllInfo_t* info) {
  int** field = info->field;
  Figure_t* figure = info->current;
  int x = figure->x;
  int y = figure->y;

  for (int i = 0; i < figure->length; i++) {
    for (int j = 0; j < figure->length; j++) {
      if (figure->coordinates[i][j]) {
        field[y + i][x + j] = 1;
      }
    }
  }
}

void SpawnFigure(AllInfo_t* info) {
  if (info->current != NULL) {
    FreeFigure(info->current);
  }
  if (info->next == NULL) {
    info->next = RandomFigure();
  }
  info->current = info->next;
  info->next = RandomFigure();
}

void FillMatrix(int** matrix, int matrix_id, int rotation_id) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix[i][j] = figures[matrix_id][rotation_id][i][j];
    }
  }
}

int** InitMatrix(int rows, int cols) {
  int** matrix = (int**)calloc(rows, sizeof(int*));
  if (matrix) {
    for (int i = 0; i < rows; i++) {
      matrix[i] = (int*)calloc(cols, sizeof(int));
      if (!matrix[i]) exit(1);
    }
  } else {
    exit(1);
  }
  return matrix;
}

void FreeMatrix(int** matrix, int length) {
  for (int i = 0; i < length; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
