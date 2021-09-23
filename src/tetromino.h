#pragma once

#include "game.h"

#define TETREMINO_COUNT 7

typedef struct{
	u8 boardState;
	u8 size;
	u8 shape[4][4];
} tetromino;

const tetromino tetrominos[TETREMINO_COUNT] = {
	{
		.boardState = STATE_CYAN,
		.size = 4,
		.shape = {
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},
	{
		.boardState = STATE_BLUE,
		.size = 3,
		.shape = {
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	},
	{
		.boardState = STATE_ORANGE,
		.size = 3,
		.shape = {
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},
	{
		.boardState = STATE_YELLOW,
		.size = 4,
		.shape = {
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	},
	{
		.boardState = STATE_GREEN,
		.size = 3,
		.shape = {
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{0,0,0,0}
		}
	},
	{
		.boardState = STATE_PURPLE,
		.size = 3,
		.shape = {
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	},
	{
		.boardState = STATE_RED,
		.size = 3,
		.shape = {
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		}
	},
};