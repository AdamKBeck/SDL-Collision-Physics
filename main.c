
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "main.h"
#include "Momentum.h"

/* This project is a basic GUI for collision detection and physics
 * Using objects created with the SDL API */
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
	                          WINDOW_WIDTH,					// width
	                          WINDOW_HEIGHT,				// height
	                          0								// flags
	                         );

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Vsync flag to refresh animations with monitor refresh rate
	gameState.renderer = renderer;

	loadGame(&gameState);

	/* The window is open: enter program loop (See SDL_PollEvent) */
	int done = 0;

	while (!done) { // event loop
		done = processEvents(window, &gameState); // check for events
		doRender(renderer, &gameState); // render display
	}

	SDL_DestroyTexture(gameState.star); // shutdown game and unload all memory

	/* Close and destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	/* Clean up*/
	SDL_Quit();
	return 0;
}

void doRender(SDL_Renderer *renderer, GameState *game) {
	// set drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	// clear the screen to blue
	SDL_RenderClear(renderer);

	// set drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255,255);

	// Rectangle uses man x and y coordinates
	SDL_Rect rect = {game->man.x, game->man.y, MAN_WIDTH, MAN_HEIGHT};
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

void loadGame(GameState *game){

	/* Initialize man struct from gameState, put man on a random coordinate on the window */
	game->man.x = (rand() % (WINDOW_WIDTH - MAN_WIDTH));
	game->man.y = (rand() % (WINDOW_HEIGHT - MAN_HEIGHT));
	game->man.speed = 0;

	SDL_Surface *starSurface = NULL;

	/* Load images and create rendering textures from them */
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

	// WASD and arrow key player movement
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_RETURN]){
		printf("<RETURN> is pressed.\n"); // Testing purposes only
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]){
		game->man.x -= MOVE;
	}
	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]){
		game->man.x += MOVE;
	}
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]){
		game->man.y -= MOVE;
	}
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]){
		game->man.y += MOVE;
	}

	/*	When 'T' is pressed apply gravity.
	 *	Turn off gravity when released */
	grav_trigger = false;
	if (state[SDL_SCANCODE_T]){
		grav_trigger = true;
		applyGravity(game);
	}

	/* To start speed back at 0 when pressed again, set to 0 when 'T' is released */
	if (!grav_trigger){
		game->man.speed = 0;
	}

	boundsCheck(game); 	// Before display is rendered, correct out of bounds movement
	return done;
}

void boundsCheck(GameState *game){
	/* Keeps man inside screen window */
	if (game->man.x < 0){
		game->man.x = 0;
	}
	if (game->man.x > (WINDOW_WIDTH - MAN_WIDTH)){
		game->man.x = WINDOW_WIDTH- MAN_WIDTH;
	}
	if (game->man.y < 0){
		game->man.y = 0;
	}
	if (game->man.y > (WINDOW_HEIGHT - MAN_HEIGHT)){
		game->man.y = WINDOW_HEIGHT - MAN_HEIGHT;
	}
}

/* Applies gravity to the square (TODO: apply to all objects)*/
void applyGravity(GameState *game){

	incVel(&(game->man.speed)); // Increments velocity of square
	game->man.y += (int)(game->man.speed);

	/* Flip direction when square hits edges */
	if (game->man.y >= WINDOW_HEIGHT - MAN_HEIGHT || game->man.y <= 0){
		game->man.speed = getVelocity(game->man.speed, 1, 0, 0);
	}

}
