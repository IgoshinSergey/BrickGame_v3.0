#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_TETRIS_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_TETRIS_H_

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include "figures.h"
#include "fsm.h"
#include "objects.h"

// Function declarations
StateTetris_t* GetState();
AllInfo_t* GetAllInfo();
GameInfo_t* GetGameInfo();

void InitAllInfo();
void DestroyGame();

GameInfo_t MakeGameInfo(AllInfo_t* info);
void UpdateAllInfo(AllInfo_t* info);

bool IsEndGame(AllInfo_t* info);
bool IsCollision(AllInfo_t info);

void SetTetrisScore(int high_score);
int GetTetrisScore();
int GetPoints(int** field);
int CalculatePoints(int count);
void EraseLine(int** field, int row);
bool CheckLine(int** field, int row);

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_TETRIS_H_
