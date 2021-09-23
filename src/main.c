#include <SDL2/SDL.h>
#include <SDL2_TTF/SDL_ttf.h>

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "common.h"
#include "game.h"
#include "highscore.h"

int main(int argc, char *argv[]){
	srand ( time(NULL) );
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Tetris",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, GRID_H*PX_PER_CELL,
		NULL);
		
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	
	TTF_Init();
	
	Game* game = malloc(sizeof(Game));
	InitGame(game);
	
	bool running = true;
	real dt = 1.0f/60.0f*1000.0f;
	u32 target_trame_time = (u32)(dt);
	u32 frame_begin = 0; 
	SDL_Event e;
	while(running){
		frame_begin = SDL_GetTicks();
				
		while(SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_QUIT:
				{
					running = false;
				}
				break;
				default:
				break;
			}
		}
		
		Frame(game, renderer, &running);
	
		u32 frame_time = SDL_GetTicks() - frame_begin;
		if(frame_time < target_trame_time){
			SDL_Delay(target_trame_time-frame_time);
		}

	}
	
	return 0;
}