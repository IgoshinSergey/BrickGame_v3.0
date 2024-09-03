PROJECT_NAME=BrickGame
CC=gcc
CXX=g++

STANDART_C=-std=c11
STANDART_CXX=-std=c++17
CFLAGS=-Wall -Werror -Wextra
DLFCN=-ldl
CURL=-lcurl
NCURSES=-lncurses
DYNAMIC_LIB=-shared -fPIC

CLI_FILES=gui/cli
DESKTOP_FILES=gui/desktop

COMMON_FILES=brick_game/common
TETRIS_FILES=brick_game/tetris
SNAKE_FILES=brick_game/snake
CLIENT_FILES=brick_game/client
SERVER_FILES=gui/server

MODEL_SNAKE_FILES=$(SNAKE_FILES)/model
CONTROLLER_SNAKE_FILES=$(SNAKE_FILES)/controller

TEST_FILES=tests

all: clean install

install: clean libtetris.so libsnake.so libclient.so cli desktop
	rm -rf build
	mkdir build	
	mv desktop_duild/BrickGame_desktop build
	mv cli_build/BrickGame_cli build
	rm -rf desktop_duild
	rm -rf cli_build

cli: libtetris.so libsnake.so libclient.so
	rm -rf cli_build
	mkdir cli_build
	cd cli_build && $(CC) $(STANDART_C) $(CFLAGS) -o $(PROJECT_NAME)_cli ../$(CLI_FILES)/*.c $(NCURSES) $(DLFCN)

desktop: libtetris.so libsnake.so libclient.so
	rm -rf desktop_duild
	mkdir desktop_duild
	cd desktop_duild && qmake ../$(DESKTOP_FILES)
	cd desktop_duild && make
	cd ..

server: libtetris.so libsnake.so libclient.so
	python3.12 $(SERVER_FILES)/server.py

test: clean
	python3.12 -m unittest -v $(TEST_FILES)/tests.py

coverage: clean
	coverage run -m unittest $(TEST_FILES)/tests.py
	coverage html

libsnake.so:
	$(CXX) $(CFLAGS) $(STANDART_CXX) $(DYNAMIC_LIB) -c $(MODEL_SNAKE_FILES)/*.cc $(CONTROLLER_SNAKE_FILES)/*.cc
	$(CXX) $(CFLAGS) $(STANDART_CXX) $(DYNAMIC_LIB) *.o -o libsnake.so
	rm -rf *.o

libtetris.so:
	$(CC) $(CFLAGS) $(STANDART_C) $(DYNAMIC_LIB) -c $(TETRIS_FILES)/*.c
	$(CC) $(CFLAGS) $(STANDART_C) $(DYNAMIC_LIB) *.o -o libtetris.so
	rm -rf *.o

libclient.so:
	$(CXX) $(CFLAGS) $(STANDART_CXX) $(DYNAMIC_LIB) $(CLIENT_FILES)/*.cc -o libclient.so $(CURL) 

unistall:
	rm -rf build

style:
	clang-format -i \
	$(COMMON_FILES)/*.* \
	$(CLIENT_FILES)/*.* \
	$(MODEL_SNAKE_FILES)/*.* \
	$(CONTROLLER_SNAKE_FILES)/*.* \
	$(TETRIS_FILES)/*.c \
	$(TETRIS_FILES)/inc/*.h \
	$(CLI_FILES)/*.* \
	$(DESKTOP_FILES)/*.cc \
	$(DESKTOP_FILES)/*.h

clean:
	@rm -rf \
	build desktop_duild cli_build htmlcov \
	tetris*.txt snake*.txt race*.txt \
	*.o *.so .coverage
