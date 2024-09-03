#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_FIGURES_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_FIGURES_H_

#include <stdlib.h>
#include <time.h>

#include "objects.h"
#include "tetris.h"

// Function declarations
Figure_t* RandomFigure();
void FreeFigure(Figure_t* figure);
void FillMatrix(int** matrix, int matrix_id, int row_id);

bool MoveLeft(AllInfo_t* info);
bool MoveRight(AllInfo_t* info);
bool MoveDown(AllInfo_t* info);
void Rotate(AllInfo_t* info);

void ConnectFigure(AllInfo_t* info);
void SpawnFigure(AllInfo_t* info);

int** InitMatrix(int rows, int cols);
void FreeMatrix(int** matrix, int length);

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_FIGURES_H_
