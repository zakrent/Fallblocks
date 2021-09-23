#pragma once

#include <SDL2/SDL.h>
#include <SDL2_TTF/SDL_ttf.h>

#include "common.h"
#include "highscore.h"

#define PX_PER_CELL 40

#define BOARD_W 10
#define BOARD_H 20

#define GRID_W (BOARD_W+2)
#define GRID_H (BOARD_H+1)

enum BoardState{
	STATE_EMPTY,
	STATE_CYAN,
	STATE_BLUE,
	STATE_ORANGE,
	STATE_YELLOW,
	STATE_GREEN,
	STATE_PURPLE,
	STATE_RED,
	COUNT_STATE
};

typedef struct{
	u8 destroyTimer;
} LineInfo;

enum GameState{
	STATE_START,
	STATE_PLAY,
	STATE_PAUSE,
	STATE_END,
	STATE_HIGHSCORE,
	STATE_OPTIONS,
	COUNT_GAME_STATE
};

typedef struct{
	i8 counter;
	i8 held;
	i8 input;
	i8 textCounter;
} MenuData;

typedef struct{
	bool notFirstFrame;
	u8 state;
	i8 shapeX;
	i8 shapeY;
	u8 fallCounter;
	u8 moveCounter;
	u8 forceFallCounter;
	u8 currentRotation;
	u8 currentShapeId;
	u8 nextShapeId;
	u8 currentShape[4][4];
	u8 level;
	bool rotationHeld;
	u32 score;
	u32 linesCleared;
	u32 frameNumber;
	TTF_Font *font;
	TTF_Font *bigFont;
	char name[MAX_NAME_LENGTH];
	LineInfo lineInfo[BOARD_H];
	u8 board[BOARD_W][BOARD_H];
	Highscores highscores;
	MenuData menuData;
} Game;

void InitGame(Game* game);
void Frame(Game* game, SDL_Renderer *renderer, bool *running);
