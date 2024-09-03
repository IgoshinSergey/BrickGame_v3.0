#ifndef AP2_BRICKGAME_V3_0_PYTHON_1_CLIENT_H_
#define AP2_BRICKGAME_V3_0_PYTHON_1_CLIENT_H_

#include <curl/curl.h>

#include <cstring>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../common/common.h"

using json = nlohmann::json;

#define BASE_URL "http://localhost:8080/api"

class BrickGameClient {
 public:
  BrickGameClient();
  ~BrickGameClient();

  bool SelectGamePost(int game_id);
  bool UserActionPost(UserAction_t action, bool hold);
  GameInfo_t UpdateCurrentStateGet();
  void FreeGameInfo(int **field, int **next);

 private:
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
};

extern "C" {
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
void SelectGame(int game_id);
void FreeGameInfo(int **field, int **next);
}

#endif  // AP2_BRICKGAME_V3_0_PYTHON_1_CLIENT_H_
