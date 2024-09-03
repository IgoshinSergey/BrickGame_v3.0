#include "cli.h"

void InitNcurses(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(50);
  curs_set(0);
}

void EndNcurses(void) { endwin(); }

void GameLoop(int game_id) {
  void *handle = GetHandle(game_id);
  UserInputFunc userInput = (UserInputFunc)dlsym(handle, "userInput");
  UpdateCurrentStateFunc updateCurrentState =
      (UpdateCurrentStateFunc)dlsym(handle, "updateCurrentState");
  FreeGameInfoFunc FreeGameInfo = NULL;

  if (game_id == 1) {
    InitAllInfoFunc InitAllInfo = (InitAllInfoFunc)dlsym(handle, "InitAllInfo");
    InitAllInfo();
  } else if (game_id == 3) {
    SelectGameFunc SelectGame = (SelectGameFunc)dlsym(handle, "SelectGame");
    SelectGame(3);
    FreeGameInfo = (FreeGameInfoFunc)dlsym(handle, "FreeGameInfo");
  }

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);

  clear();
  bkgd(COLOR_PAIR(2));

  WINDOW *field = newwin(22, 22, 0, 0);
  WINDOW *player = newwin(12, 18, 0, 24);

  while (true) {
    int symbol = getch();
    GameInfo_t display = updateCurrentState();
    InputProcessing(symbol, userInput);
    ShowGame(field, player, display);
    if (game_id == 3) {
      FreeGameInfo(display.field, display.next);
    }
    if (symbol == KEY_ESC) break;
  }
  delwin(field);
  delwin(player);

  if (game_id == 1) {
    DestroyGameFunc DestroyGame = (DestroyGameFunc)dlsym(handle, "DestroyGame");
    DestroyGame();
  }

  dlclose(handle);
}

void *GetHandle(int game_id) {
  void *handle = NULL;
  if (game_id == 1) {
    handle = dlopen("./libtetris.so", RTLD_LAZY);
  } else if (game_id == 2) {
    handle = dlopen("./libsnake.so", RTLD_LAZY);
  } else {
    handle = dlopen("./libclient.so", RTLD_LAZY);
  }
  return handle;
}

void ShowMainMenu() {
  mvprintw(1, 7, "BrickGame v3.0");
  mvprintw(3, 1, "Tetris - Enter \'T\' or \'t\' for start");
  mvprintw(4, 1, "Snake  - Enter \'S\' or \'s\' for start");
  mvprintw(5, 1, "Race   - Enter \'R\' or \'r\' for start");
  mvprintw(6, 1, "Exit   - Enter \'Esc\'");
}

void ShowGame(WINDOW *field, WINDOW *player, GameInfo_t game) {
  ShowField(field, game);
  ShowNext(player, game);
  ShowInfo(player, game);
}

void ShowField(WINDOW *field, GameInfo_t game) {
  werase(field);
  box(field, 0, 0);
  int **coordinates = game.field;
  if (coordinates) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        if (coordinates[i][j]) {
          wattron(field, COLOR_PAIR(1));
          mvwaddch(field, i + 1, j * 2 + 1, ' ');
          mvwaddch(field, i + 1, j * 2 + 2, ' ');
          wattroff(field, COLOR_PAIR(1));
        }
      }
    }
  }
  wrefresh(field);
}

void ShowNext(WINDOW *player, GameInfo_t game) {
  werase(player);
  box(player, 0, 0);
  int **next = game.next;
  if (next) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (next[i][j]) {
          wattron(player, COLOR_PAIR(1));
          mvwaddch(player, i + 2, j * 2 + 5, ' ');
          mvwaddch(player, i + 2, j * 2 + 6, ' ');
          wattroff(player, COLOR_PAIR(1));
        }
      }
    }
  }
}

void ShowInfo(WINDOW *player, GameInfo_t game) {
  mvwprintw(player, 1, 1, "NEXT FIGURE");
  mvwprintw(player, 6, 1, "SCORE %d", game.score);
  mvwprintw(player, 7, 1, "HIGH SCORE %d", game.high_score);
  mvwprintw(player, 8, 1, "LEVEL %d", game.level);
  mvwprintw(player, 9, 1, "SPEED %d", game.speed);
  if (game.pause) mvwprintw(player, 10, 1, "PAUSE");
  wrefresh(player);
}

void InputProcessing(int input, void (*userInput)(UserAction_t, bool)) {
  switch (input) {
    case KEY_UP:
      userInput(Up, false);
      break;
    case KEY_LEFT:
      userInput(Left, false);
      break;
    case KEY_RIGHT:
      userInput(Right, false);
      break;
    case KEY_DOWN:
      userInput(Down, false);
      break;
    case KEY_Z_LOWER:
    case KEY_Z_UPPER:
      userInput(Action, false);
      break;
    case KEY_START:
      userInput(Start, false);
      break;
    case KEY_ESC:
      userInput(Terminate, false);
      break;
    case KEY_P_LOWER:
    case KEY_P_UPPER:
      userInput(Pause, false);
      break;
    default:
      break;
  }
}
