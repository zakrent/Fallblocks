#pragma once

#include <stdint.h>

#include "common.h"

#define HIGH_SCORE_MAX 12
#define MAX_NAME_LENGTH 32

typedef struct{
	char name[MAX_NAME_LENGTH];
	u32 score;
} Highscore;

typedef struct{
	Highscore scores[HIGH_SCORE_MAX+1]; //One extra for adding and sorting
} Highscores;

void ReadScores(Highscores *highscores);
void SaveScores(Highscores *highscores);
void AddScore(Highscores *highscores, char* name, u32 score);