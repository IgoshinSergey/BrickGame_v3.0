#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_CLI_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_CLI_H_

#include <dlfcn.h>
#include <ncurses.h>

#include "../../brick_game/common/common.h"

#define KEY_Z_UPPER 'Z'
#define KEY_Z_LOWER 'z'
#define KEY_P_UPPER 'P'
#define KEY_P_LOWER 'p'
#define KEY_START '\n'
#define KEY_ESC 27

typedef GameInfo_t (*UpdateCurrentStateFunc)();
typedef void (*UserInputFunc)(UserAction_t, bool);
typedef void (*InitAllInfoFunc)();
typedef void (*DestroyGameFunc)();
typedef void (*SelectGameFunc)(int);
typedef void (*FreeGameInfoFunc)(int **, int **);

void InitNcurses(void);
void EndNcurses(void);

void GameLoop(int game_id);
void *GetHandle(int game_id);
void InputProcessing(int input, void (*userInput)(UserAction_t, bool));

void ShowMainMenu();
void ShowGame(WINDOW *field, WINDOW *player, GameInfo_t game);
void ShowField(WINDOW *field, GameInfo_t game);
void ShowNext(WINDOW *player, GameInfo_t game);
void ShowInfo(WINDOW *player, GameInfo_t game);

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_CLI_H_
