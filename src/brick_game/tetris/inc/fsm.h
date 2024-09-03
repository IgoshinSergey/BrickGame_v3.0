#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_FSM_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_FSM_H_

#include "objects.h"

GameInfo_t updateCurrentState(void);
void userInput(UserAction_t action, bool hold);

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_FSM_H_
