
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>

typedef struct {
	int x, y;
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

void loadGame(GameState *game){

	/* Initialize man struct from gameState */
	game->man.x = 320-40;
	game->man.y = 240-40;

	SDL_Surface *starSurface = NULL;

	// Load images and create rendering textures from them
	starSurface = IMG_Load("star.png");

	if (starSurface == NULL){
		printf("Cannot find star.png!!");
		SDL_Quit();
		exit(1); // terminate the program
	}

	game->star = SDL_CreateTextureFromSurface(game->renderer, starSurface );
    SDL_FreeSurface(starSurface); // Free surface once texture is created

	for (int i = 0; i < 100; i++){
		game->stars[i].x = rand() % 640;
		game->stars[i].y = rand() % 480;
	}
}

int processEvents(SDL_Window *window, GameState *game) {
	SDL_Event event;

	int done = 0;

	// Check for events to be processed, all of these are quitting events
	while(SDL_PollEvent(&event)) {

		switch(event.type) {

		case SDL_WINDOWEVENT_CLOSE:
			if (window) {
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
			break;

		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			case SDLK_ESCAPE:
				done = 1;
				break;
			}
			break;

		case SDL_QUIT:
			// quit out of the game
			done = 1;
			break;
		}

	}


	// player movement
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_RETURN]){
		printf("<RETURN> is pressed.\n");
	}
	if (state[SDL_SCANCODE_LEFT]){
		game->man.x -= 10;
	}
	if (state[SDL_SCANCODE_RIGHT]){
		game->man.x += 10;
	}
	if (state[SDL_SCANCODE_UP]){
		game->man.y -= 10;
	}
	if (state[SDL_SCANCODE_DOWN]){
		game->man.y += 10;
	}

	return done;
}

void doRender(SDL_Renderer *renderer, GameState *game) {
	// set drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	// clear the screen to blue
	SDL_RenderClear(renderer);

	// set drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255,255);

	// Rectangle uses man x and y coordinates
	SDL_Rect rect = {game->man.x, game->man.y, 200, 200};
	SDL_RenderFillRect(renderer, &rect);

	// Draw star image, not used now, may use later
//	for (int i = 0; i < 100; i++){
//		SDL_Rect starRect= {game->stars[i].x, game->stars[i].y, 64, 64};
//		SDL_RenderCopy(renderer, game->star, NULL, &starRect);
//	}


	// present to the screen what we've drawn
	SDL_RenderPresent(renderer);

	// don't burn up CPU, pause 100ms
	//SDL_Delay(10); // taken out because of vsync flag
}

int main(int argc, char* argv[]) {

	srand( (unsigned)time( NULL ) ); // Seed random numbers with current time


	GameState gameState;
	SDL_Window *window = NULL;		// Declare a window
	SDL_Renderer *renderer = NULL;	// Declare a renderer

	SDL_Init(SDL_INIT_VIDEO);	// Initialize SDL2


	/* Create an application window with the following settings: */
	window = SDL_CreateWindow("Game Window",				// Window title
	                          SDL_WINDOWPOS_UNDEFINED,		// x position
	                          SDL_WINDOWPOS_UNDEFINED,		// y position
	                          640,							// width
	                          480,							// height
	                          0								// flags
	                         );

	// Vsync flag to refresh animations with monitor refresh rate
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState.renderer = renderer;

	loadGame(&gameState);

	/* The window is open: enter program loop (See SDL_PollEvent) */
	int done = 0;

	// Event loop
	while (!done) {
		// Check for events
		done = processEvents(window, &gameState);

		// render display
		doRender(renderer, &gameState);
	}

	// Shutdown game and unload all memory
	SDL_DestroyTexture(gameState.star);

	/* Close and destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	/* Clean up*/
	SDL_Quit();
	return 0;
}
