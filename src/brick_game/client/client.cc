#include "client.h"

BrickGameClient::BrickGameClient() {}

BrickGameClient::~BrickGameClient() {}

size_t BrickGameClient::WriteCallback(void *contents, size_t size, size_t nmemb,
                                      void *userp) {
  strncat(static_cast<char *>(userp), static_cast<char *>(contents),
          size * nmemb);
  return size * nmemb;
}

bool BrickGameClient::SelectGamePost(int game_id) {
  CURL *curl;
  CURLcode res;
  char url[256];
  char response[1024];
  snprintf(url, sizeof(url), "%s/games/%d", BASE_URL, game_id);

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res == CURLE_OK;
  }
  return false;
}

bool BrickGameClient::UserActionPost(UserAction_t action, bool hold) {
  CURL *curl;
  CURLcode res;
  char url[256];
  char response[1024];

  snprintf(url, sizeof(url), "%s/actions", BASE_URL);

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    json json_data;
    json_data["action_id"] = action;
    json_data["hold"] = hold;
    std::string json_string = json_data.dump();

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
  }
  return false;
}

GameInfo_t BrickGameClient::UpdateCurrentStateGet() {
  CURL *curl;
  CURLcode res;
  char url[256];
  char response[2048] = "";
  GameInfo_t game_info = {nullptr, nullptr, 0, 0, 0, 0, false};

  snprintf(url, sizeof(url), "%s/state", BASE_URL);

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res == CURLE_OK) {
      json json_data = json::parse(response);
      if (json_data.contains("field")) {
        const auto &field = json_data["field"];
        if (field.is_array()) {
          game_info.field = new int *[20];
          for (size_t i = 0; i < 20; i++) {
            const auto &row = field[i];
            game_info.field[i] = new int[10];
            for (size_t j = 0; j < 10; j++) {
              game_info.field[i][j] = row[j].get<bool>() ? 1 : 0;
            }
          }
        }
      }

      if (json_data.contains("next")) {
        const auto &next = json_data["next"];
        if (next.is_array()) {
          game_info.next = new int *[4];
          for (size_t i = 0; i < 4; i++) {
            const auto &row = next[i];
            game_info.next[i] = new int[4];
            for (size_t j = 0; j < 4; j++) {
              game_info.next[i][j] = row[j].get<bool>() ? 1 : 0;
            }
          }
        }
      }

      game_info.score = json_data["score"].get<int>();
      game_info.high_score = json_data["high_score"].get<int>();
      game_info.level = json_data["level"].get<int>();
      game_info.speed = json_data["speed"].get<int>();
      game_info.pause = json_data["pause"].get<bool>();
    }
  }
  return game_info;
}

void BrickGameClient::FreeGameInfo(int **field, int **next) {
  if (field) {
    for (int i = 0; i < 20; i++) {
      delete[] field[i];
    }
    delete[] field;
  }

  if (next) {
    for (int i = 0; i < 4; i++) {
      delete[] next[i];
    }
    delete[] next;
  }
}

static BrickGameClient client;

void userInput(UserAction_t action, bool hold) {
  client.UserActionPost(action, hold);
}

GameInfo_t updateCurrentState() { return client.UpdateCurrentStateGet(); }

void SelectGame(int game_id) { client.SelectGamePost(game_id); }

void FreeGameInfo(int **field, int **next) { client.FreeGameInfo(field, next); }
