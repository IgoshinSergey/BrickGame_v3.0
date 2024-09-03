#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_OBJECTS_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_OBJECTS_H_

#include <stdbool.h>
#include <time.h>

#include "../../common/common.h"

typedef enum StateTetris_t {
  StartGame,
  Spawn,
  Moving,
  Shifting,
  Attaching
} StateTetris_t;

typedef struct Figure_t {
  int **coordinates;
  int figure_id;
  int rotate_id;
  int length;
  int x, y;
} Figure_t;

typedef struct PlayerInfo_t {
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
  bool start;
} PlayerInfo_t;

typedef struct AllInfo_t {
  int **field;
  Figure_t *current;
  Figure_t *next;
  PlayerInfo_t player;
  struct timespec last_drop_time;
  int tacts;
} AllInfo_t;

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_OBJECTS_H_
