#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_CONTROLLER_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_CONTROLLER_H_

#include "../../common/common.h"
#include "../model/model.h"

namespace s21 {

class Controller {
 public:
  SnakeGame *model;

  Controller(SnakeGame *model);
  ~Controller();

  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();
};

}  // namespace s21

extern "C" {
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
}

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_CONTROLLER_H_
