#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "stdbool.h"

/* Make extern later if used between multiple .c files */
const double GRAVITY = 9.81; // positive is downward in GUI
const int MOVE = 10; // Amount of pixels moved for one keboard press
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const int MAN_WIDTH = 50;
const int MAN_HEIGHT = 50;
bool grav_trigger = false; //

typedef struct {
	int x, y;
	float speed;
	short life;
	char *name;
} Man;

typedef struct{
	int x, y;
} Star;

typedef struct {
	// Players
	Man man;

	// Stars
	Star stars[100];

	//Images
	SDL_Texture *star;
	SDL_Renderer *renderer;
} GameState;

void loadGame(GameState *game);
void boundsCheck(GameState *game);
void applyGravity(GameState *game);
int processEvents(SDL_Window *window, GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game);

#endif // MAIN_H_INCLUDED
