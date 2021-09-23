#include "highscore.h"

#include <stdlib.h>
#include <stdio.h>

void ReadScores(Highscores *highscores){
	FILE *pFile = fopen ("scores.dat" , "r");
	
	if(pFile == NULL)
		return;
	
	u8 highScoreIndex = 0;
	u8 bufIndex = 0;
	u8 tokenCounter = 0;
	
	char buf[MAX_NAME_LENGTH] = {0};
	int characterEx = fgetc(pFile);
	char character = 0;
	while(characterEx != EOF){
		character = (char)characterEx;
		if(character == ';'){
			if(tokenCounter == 0){
				strncpy(highscores->scores[highScoreIndex].name, buf, MAX_NAME_LENGTH);
				bufIndex = 0;
				memset(buf,0,MAX_NAME_LENGTH);
				tokenCounter = 1;
			}
			else{
				highscores->scores[highScoreIndex].score = (u32)atoi(buf);
				bufIndex = 0;
				memset(buf,0,MAX_NAME_LENGTH);
				tokenCounter = 0;
				highScoreIndex++;
			}
		}
		else if(character >= '0'){
			buf[bufIndex] = character;
			if(bufIndex != MAX_NAME_LENGTH-1){
				bufIndex++;
			}
		}
		
		characterEx = fgetc(pFile);
	}
	
	fclose(pFile);
}

void SaveScores(Highscores *highscores){
	FILE *pFile = fopen("scores.dat" , "w");
	
	if(pFile == NULL)
		return;
	
	for(int i = 0; i < HIGH_SCORE_MAX; i++){
		fprintf(pFile, "%s;%u;", highscores->scores[i].name, highscores->scores[i].score);
	}
	
	fclose(pFile);
}

int CompareScores(const void *a, const void *b){
	return ((Highscore*)b)->score-((Highscore*)a)->score;
}

void AddScore(Highscores *highscores, char* name, u32 score){
	Highscore highscore = (Highscore){.score = score};
	strncpy(highscore.name, name, MAX_NAME_LENGTH);
	highscores->scores[HIGH_SCORE_MAX] = highscore;
	
	qsort(highscores->scores, HIGH_SCORE_MAX+1, sizeof(Highscore), CompareScores);
}