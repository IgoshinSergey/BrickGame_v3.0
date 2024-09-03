#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_COMMON_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_COMMON_H_

typedef enum UserAction_t {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_COMMON_H_
