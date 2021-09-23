#include "game.h"

#include <stdlib.h>
#include <stdio.h>

#include "tetromino.h"

void ResetGame(Game *game){
	char name[MAX_NAME_LENGTH];
	strncpy(name, game->name, MAX_NAME_LENGTH);
	TTF_Font *temp = game->font;
	TTF_Font *temp2 = game->bigFont;
	*game = (Game){0};
	game->font = temp;
	game->bigFont = temp2;
	
	game->menuData.held = 1;
	strncpy(game->name, name, MAX_NAME_LENGTH);
}

void InitGame(Game *game){
	ResetGame(game);

	game->shapeX = 5;
	game->font = TTF_OpenFont("joystix monospace.ttf", 25);
	game->bigFont = TTF_OpenFont("joystix monospace.ttf", 80);
	char name[] = "Player";
	strncpy(game->name, name, MAX_NAME_LENGTH);
}
	
void BoardStateColor(SDL_Renderer *renderer, u8 state){
	switch(state){
		case STATE_CYAN:
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			break;
		case STATE_BLUE:
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			break;		
		case STATE_ORANGE:
			SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
			break;		
		case STATE_YELLOW:
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			break;		
		case STATE_GREEN:
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			break;		
		case STATE_PURPLE:
			SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
			break;		
		case STATE_RED:
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			break;		
		default:
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			break;
	}
}

void DrawPlayArea(Game *game, SDL_Renderer *renderer){
	SDL_Rect rect;
	rect.w = PX_PER_CELL;
	rect.h = PX_PER_CELL;
	
	//Board 
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	
	for(u8 y = 0; y < BOARD_H; y++){
		rect.y = y*PX_PER_CELL;
		for(u8 x = 0; x < BOARD_W; x++){
			rect.x = (x+1)*PX_PER_CELL;
			if(game->lineInfo[y].destroyTimer){
				if(game->lineInfo[y].destroyTimer % 10 > 5){
					SDL_SetRenderDrawColor(renderer, 0, 127, 0, 255);
					SDL_RenderFillRect(renderer, &rect);
				}
			}
			else if(game->board[x][y]){
				BoardStateColor(renderer, game->board[x][y]);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
	
	//Shape
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for(u8 y = 0; y < 4; y++){
		rect.y = (game->shapeY+y)*PX_PER_CELL;
		for(u8 x = 0; x < 4; x++){
			rect.x = (game->shapeX+1+x)*PX_PER_CELL;
			if(game->currentShape[x][y]){
				BoardStateColor(renderer, tetrominos[game->currentShapeId].boardState);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
	
	//Borders
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	
	rect.y = (GRID_H-1)*PX_PER_CELL;
	for(u8 x = 0; x < GRID_W; x++){
		rect.x = x*PX_PER_CELL;
		SDL_RenderFillRect(renderer, &rect);
	}
	
	rect.x = 0;
	for(u8 y = 0; y < GRID_H; y++){
		rect.y = y*PX_PER_CELL;
		SDL_RenderFillRect(renderer, &rect);
	}
	
	rect.x = (GRID_W-1)*PX_PER_CELL;
	for(u8 y = 0; y < GRID_H; y++){
		rect.y = y*PX_PER_CELL;
		SDL_RenderFillRect(renderer, &rect);
	}
	
	//Grid
	SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
	for(u8 x = 0; x <= GRID_W; x++){
		SDL_RenderDrawLine(renderer, x*PX_PER_CELL, 0, x*PX_PER_CELL, GRID_H*PX_PER_CELL);
	}
	for(u8 y = 0; y <= GRID_H; y++){
		SDL_RenderDrawLine(renderer, 0, y*PX_PER_CELL, GRID_W*PX_PER_CELL, y*PX_PER_CELL);
	}
	
}


void DrawTextEx(Game *game, SDL_Renderer *renderer, char* text, i32 x, i32 y, bool centered, bool big){
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface *surface = 0;
	if(!big){
		surface = TTF_RenderText_Solid(game->font, text, color);
	}else{
		surface = TTF_RenderText_Solid(game->bigFont, text, color);
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	i32 texW = 0;
	i32 texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { x, y, texW, texH };
	if(centered){
		dstrect.x -= texW/2;
	}
	
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

//Very slow
void DrawText(Game *game, SDL_Renderer *renderer, char* text, i32 x, i32 y){
	DrawTextEx(game, renderer, text, x, y, false, false);
}

void DrawUI(Game *game, SDL_Renderer *renderer){
	SDL_Rect rect;
	char buf[32];
	rect.w = PX_PER_CELL;
	rect.h = PX_PER_CELL;
	
	i32 ui_y = 0;
	
	i32 ui_x = 525;
	
	//Score
	snprintf(buf, 32, "Score: %u", game->score);
	DrawText(game, renderer, buf, ui_x, ui_y);
	ui_y += 25;
	
	snprintf(buf, 32, "Level: %u", game->level);
	DrawText(game, renderer, buf, ui_x, ui_y);
	ui_y += 25;
	
	snprintf(buf, 32, "Lines: %u", game->linesCleared);
	DrawText(game, renderer, buf, ui_x, ui_y);
	ui_y += 25;
	
	//Next shape
	DrawText(game, renderer, "Next shape:", ui_x, ui_y);
	ui_y += 50;
	
	for(u8 y = 0; y < 4; y++){
		rect.y = ui_y+(y)*PX_PER_CELL;
		for(u8 x = 0; x < 4; x++){
			rect.x = ui_x+x*PX_PER_CELL;
			if(tetrominos[game->nextShapeId].shape[x][y]){
				BoardStateColor(renderer, tetrominos[game->nextShapeId].boardState);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
	
}

bool CheckCollision(Game *game, i8 shapeX, i8 shapeY){
	for(i8 y = 0; y < 4; y++){
		for(i8 x = 0; x < 4; x++){
			if(game->currentShape[x][y]){
				if(shapeY < 0){
					if(shapeX+x >= BOARD_W || shapeX+x < 0){
						return true;
					}
				}
				else if(game->board[shapeX+x][shapeY+y] || shapeX+x >= BOARD_W || shapeY+y >= BOARD_H || shapeX+x < 0){
					return true;
				}
			}
		}
	}
	return false;
}

void UpdateCurrentShape(Game *game){
	for(u8 y = 0; y < 4; y++){
		for(u8 x = 0; x < 4; x++){
			game->currentShape[x][y] = 0;
		}
	}
	
	u8 size = tetrominos[game->currentShapeId].size;
	for(u8 y = 0; y < size; y++){
		for(u8 x = 0; x < size; x++){
			u8 srcX, srcY;
			switch(game->currentRotation){
				case 0:
					srcX = x;
					srcY = y;
					break;
				case 1:
					srcX = y;
					srcY = size-1-x;
					break;
				case 2:
					srcX = size-1-x;
					srcY = size-1-y;
					break;
				case 3:
					srcX = size-1-y;
					srcY = x;
					break;
			}
			game->currentShape[x][y] = tetrominos[game->currentShapeId].shape[srcX][srcY];
		}
	}
}

void CompleteLine(Game *game, u8 y0){
	for(u8 x = 0; x < BOARD_W; x++){
		game->board[x][y0] = STATE_EMPTY;
	}
	
	for(u8 y = y0; y > 0; y--){
		for(u8 x = 0; x < BOARD_W; x++){
			game->board[x][y] = game->board[x][y-1];
		}
	}
}

void SpawnTetermino(Game *game){
	game->shapeY = -2;
	game->shapeX = BOARD_W/2-2;
	game->currentShapeId = game->nextShapeId;
	game->nextShapeId = rand() % TETREMINO_COUNT;
	UpdateCurrentShape(game);
}

void DrawMenuEntry(Game *game, SDL_Renderer *renderer, char* text, u32 *uiY, bool active){
	DrawTextEx(game, renderer, text, 400, *uiY+active*5*sinf(game->frameNumber*2*M_PI*1.0f/60.0f), 1, 0);
	*uiY += 50;
}

bool ProcessMenu(Game *game, u8 *state, u8 menuMin, u8 menuMax){
	if(game->menuData.input){
		return true;
	}
	
	if(state[SDL_SCANCODE_RETURN]){
		if(!game->menuData.held){
			game->menuData.held = 1;
			return true;
		}
	}else if(state[SDL_SCANCODE_W]){
		if(!game->menuData.held){
			game->menuData.counter = game->menuData.counter-1;
		}
		game->menuData.held = 1;
	}else if(state[SDL_SCANCODE_S]){
		if(!game->menuData.held){
			game->menuData.counter = game->menuData.counter+1;
		}
		game->menuData.held = 1;
	}
	else{
		game->menuData.held = 0;
	}

	game->menuData.counter = CLAMP(game->menuData.counter, menuMin, menuMax);
	return false;
}

bool MenuTextInput(Game *game, u8 *state, char* buf, u32 bufSize){
	if(!game->menuData.input){
		game->menuData.input=1;
		game->menuData.textCounter = strlen(buf);
		game->menuData.textCounter = CLAMP(game->menuData.textCounter, 0, bufSize-2);
	}
	
	u8 anyHeld = 0;
	
	if(state[SDL_SCANCODE_RETURN]){
		if(!game->menuData.held){
			buf[game->menuData.textCounter] = 0;
			game->menuData.input = 0;
			game->menuData.held = 1;
			return true;
		}
		anyHeld = 1;
	}
		
	char name[2];
	name[1] = 0;
	
	for(char c = 'A'; c <= 'Z'; c++){
		name[0] = c;
		
		if(!anyHeld && state[SDL_GetScancodeFromName(name)]){
			if(!game->menuData.held){
				buf[game->menuData.textCounter++] = c;
			}
			anyHeld = 1;
		}
	}
	
	for(char c = '0'; c <= '9'; c++){
		name[0] = c;
		
		if(!anyHeld && state[SDL_GetScancodeFromName(name)]){
			if(!game->menuData.held){
				buf[game->menuData.textCounter++] = c;
			}
			anyHeld = 1;
		}
	}
	
	if(state[SDL_SCANCODE_SPACE]){
		if(!anyHeld && !game->menuData.held){
			buf[game->menuData.textCounter++] = ' ';
		}
		anyHeld = 1;
	}
	
	game->menuData.textCounter = CLAMP(game->menuData.textCounter, 0, bufSize-2);
	
	if(game->frameNumber % 60 > 30){
		buf[game->menuData.textCounter] = '_';
	}
	else{
		buf[game->menuData.textCounter] = ' ';
	}
	
	if(state[SDL_SCANCODE_BACKSPACE]){
		if(!game->menuData.held && game->menuData.textCounter > 0){
			buf[game->menuData.textCounter] = 0;
			buf[--game->menuData.textCounter] = 0;
		}
		anyHeld = 1;
	}

	game->menuData.held = anyHeld;
	
	return false;
}

void FrameStart(Game *game, SDL_Renderer *renderer, bool *running){
	game->frameNumber++;
	
	u8 *state = SDL_GetKeyboardState(NULL);
	
	if(ProcessMenu(game, state, 0, 3)){
		switch(game->menuData.counter){
			case 0:
				game->state = STATE_PLAY;
				break;
			case 1:
				game->state = STATE_HIGHSCORE;
				break;
			case 2:
				game->state = STATE_OPTIONS;
				break;
			case 3:
				*running = 0;
				break;
			default:
				break;
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	DrawTextEx(game, renderer, "FALLBLOCKS", 400, 200, 1, 1);

	u32 uiY = 400;
	DrawMenuEntry(game, renderer, "START GAME",  &uiY, game->menuData.counter == 0);
	DrawMenuEntry(game, renderer, "HIGH SCORES", &uiY, game->menuData.counter == 1);
	DrawMenuEntry(game, renderer, "OPTIONS",     &uiY, game->menuData.counter == 2);
	DrawMenuEntry(game, renderer, "EXIT",        &uiY, game->menuData.counter == 3);
	
	ReadScores(&game->highscores);
	
	SDL_RenderPresent(renderer);
}

void FramePlay(Game *game, SDL_Renderer *renderer){
	if(!game->notFirstFrame){
		SpawnTetermino(game);
		game->notFirstFrame = 1;
	}
	
	u8 *state = SDL_GetKeyboardState(NULL);
	
	bool fall = false;
	
	//ROTATION
	
	if(!state[SDL_SCANCODE_Q] && !state[SDL_SCANCODE_E]){
		game->rotationHeld = 0;
	}
	else{
		u8 oldRotation = game->currentRotation;
		
		if(state[SDL_SCANCODE_Q] && !game->rotationHeld){
			game->currentRotation = ((game->currentRotation+1) % 4);
			game->rotationHeld = 1;
		}
	
		if(state[SDL_SCANCODE_E] && !game->rotationHeld){
			game->currentRotation = game->currentRotation-1;
			game->currentRotation = game->currentRotation % 4;
			game->rotationHeld = 1;
		}
		
		UpdateCurrentShape(game);
		
		if(CheckCollision(game, game->shapeX, game->shapeY)){
			game->currentRotation = oldRotation;
			UpdateCurrentShape(game);
		}
	}

	//MOVEMENT
		
	if(game->moveCounter != 0){
		game->moveCounter--;
	}
	
	if(state[SDL_SCANCODE_A] && game->moveCounter == 0){
		if(!CheckCollision(game, game->shapeX-1, game->shapeY)){
			game->shapeX -=1;
		}
	}
	
	if(state[SDL_SCANCODE_D] && game->moveCounter == 0){
		if(!CheckCollision(game, game->shapeX+1, game->shapeY)){
			game->shapeX +=1;
		}
	}
	
	if(!state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D]){
		game->moveCounter = 0;
	}
	else{
		if(game->moveCounter == 0){
			game->moveCounter = 5;
		}
	}
	
	if(state[SDL_SCANCODE_S]){
		if(game->forceFallCounter == 0){
			fall = 1;
			game->forceFallCounter = 1;
		}
		else{
			game->forceFallCounter--;
		}
	}
	else{
		game->forceFallCounter = 0;
	}
	
	//
	u8 linesClearedThisFrame = 0;
	for(u8 y = 0; y < BOARD_H; y++){
		if(game->lineInfo[y].destroyTimer){
			game->lineInfo[y].destroyTimer--;
			if(!game->lineInfo[y].destroyTimer){
				CompleteLine(game, y);
				linesClearedThisFrame++;
			}
		}
	}
	
	game->linesCleared += linesClearedThisFrame;
	
	switch(linesClearedThisFrame){
		case 1:
			game->score += 40*(game->level+1);
			break;
		case 2:
			game->score += 100*(game->level+1);
			break;
		case 3:
			game->score += 300*(game->level+1);
			break;
		case 4:
			game->score += 1200*(game->level+1);
			break;
		default:
			break;
	}
	
	game->level = game->linesCleared/5;
	
	for(u8 y = 0; y < BOARD_H; y++){
		if(game->lineInfo[y].destroyTimer) continue;
		bool rowFull = true;
		for(u8 x = 0; x < BOARD_W; x++){
			if(!game->board[x][y]){
				rowFull = false;
				break;
			}
		}
		if(rowFull){
			game->lineInfo[y].destroyTimer = 40;
		}
	}
	
	//FALL
	game->fallCounter++;
	if(game->fallCounter == MAX(5, 20-game->level)){
		game->fallCounter = 0;
		fall = true;
	}	

	if(fall){
		if(!CheckCollision(game, game->shapeX, game->shapeY+1)){
			game->shapeY += 1;
		}
		else if(game->shapeY < 0){
			game->state = STATE_END;
			AddScore(&game->highscores, game->name, game->score);
			SaveScores(&game->highscores);
		}
		else{
			for(i8 y = 0; y < 4; y++){
				for(i8 x = 0; x < 4; x++){
					if(game->currentShape[x][y]){
						game->board[game->shapeX+x][game->shapeY+y] = tetrominos[game->currentShapeId].boardState;
					}
				}
			}
			SpawnTetermino(game);
		}
	}
	
	//Pause menu
	if(state[SDL_SCANCODE_ESCAPE]){
		game->state = STATE_PAUSE;
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	DrawPlayArea(game, renderer);
	DrawUI(game, renderer);

	SDL_RenderPresent(renderer);
}

void FramePause(Game *game, SDL_Renderer *renderer){
	game->frameNumber++;
	
	u8 *state = SDL_GetKeyboardState(NULL);

	if(ProcessMenu(game, state, 0, 1)){
		switch(game->menuData.counter){
			case 0:
				game->state = STATE_PLAY;
				break;
			case 1:
				ResetGame(game);
				break;
			default:
				break;
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	u32 uiY = GRID_H*PX_PER_CELL/2-100;
	DrawTextEx(game, renderer, "PAUSED", 400, uiY, 1, 1);
	uiY += 100;

	char buf[32];
	snprintf(buf, 32, "Your score is: %u!", game->score);
	DrawTextEx(game, renderer, buf, 400, uiY, 1, 0);
	
	uiY += 50;

	DrawMenuEntry(game, renderer, "RESUME",     &uiY, game->menuData.counter == 0);
	DrawMenuEntry(game, renderer, "MAIN MENU", &uiY,  game->menuData.counter == 1);
	
	SDL_RenderPresent(renderer);
}

void FrameEnd(Game *game, SDL_Renderer *renderer){
	game->frameNumber++;
	
	u8 *state = SDL_GetKeyboardState(NULL);

	if(ProcessMenu(game, state, 0, 1)){
		switch(game->menuData.counter){
			case 0:
				ResetGame(game);
				game->state = STATE_PLAY;
				break;
			case 1:
				ResetGame(game);
				break;
			default:
				break;
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	u32 uiY = GRID_H*PX_PER_CELL/2-100;
	DrawTextEx(game, renderer, "YOU LOST!", 400, uiY, 1, 1);
	uiY += 100;

	char buf[32];
	snprintf(buf, 32, "Your score is: %u!", game->score);
	DrawTextEx(game, renderer, buf, 400, uiY, 1, 0);
	
	uiY += 50;

	DrawMenuEntry(game, renderer, "RETRY",     &uiY, game->menuData.counter == 0);
	DrawMenuEntry(game, renderer, "MAIN MENU", &uiY, game->menuData.counter == 1);
	
	SDL_RenderPresent(renderer);
}

void FrameHighscore(Game *game, SDL_Renderer *renderer){
	game->frameNumber++;
	
	u8 *state = SDL_GetKeyboardState(NULL);
	
	if(state[SDL_SCANCODE_RETURN]){
		if(!game->menuData.held){
			game->state = STATE_START;
		}
		game->menuData.held = 1;
	}
	else{
		game->menuData.held = 0;
	}
		
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	u32 uiY = 60;
	
	DrawTextEx(game, renderer, "HIGHSCORES:", 400, uiY, 1, 0);
	uiY += 50;
	
	char buf[MAX_NAME_LENGTH+32];
	
	for(int i = 0; i < HIGH_SCORE_MAX; i++){
		snprintf(buf, MAX_NAME_LENGTH+32, "%2u. %s, %u", i+1, game->highscores.scores[i].name, game->highscores.scores[i].score);
		DrawTextEx(game, renderer, buf, 200, uiY, 0, 0);
		uiY += 50;
	}
	
	DrawMenuEntry(game, renderer, "BACK",  &uiY, 1);

	SDL_RenderPresent(renderer);
}

void FrameOptions(Game *game, SDL_Renderer *renderer){
	game->frameNumber++;
	
	u8 *state = SDL_GetKeyboardState(NULL);

	if(ProcessMenu(game, state, 0, 1)){
		switch(game->menuData.counter){
			case 0:
				MenuTextInput(game, state, game->name, MAX_NAME_LENGTH);
				break;
			case 1:
				game->state = STATE_START;
				break;
			default:
				break;
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	u32 uiY = 400;
	
	char buf[MAX_NAME_LENGTH+32];
	
	snprintf(buf, MAX_NAME_LENGTH+32, "NAME:%s", game->name);
	
	DrawMenuEntry(game, renderer, buf, &uiY, game->menuData.counter == 0);
	DrawMenuEntry(game, renderer, "BACK",  &uiY, game->menuData.counter == 1);
	
	SDL_RenderPresent(renderer);
}

void Frame(Game* game, SDL_Renderer *renderer, bool *running){
	switch(game->state){
		case STATE_START:
			FrameStart(game, renderer, running);
			break;
		case STATE_PLAY:
			FramePlay(game, renderer);
			break;
		case STATE_PAUSE:
			FramePause(game, renderer);
			break;
		case STATE_END:
			FrameEnd(game, renderer);
			break;
		case STATE_HIGHSCORE:
			FrameHighscore(game, renderer);
			break;
		case STATE_OPTIONS:
			FrameOptions(game, renderer);
			break;
		default:
			game->state = STATE_START;
			break;
	}
}


