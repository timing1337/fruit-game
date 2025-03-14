#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "game.h"
#include "render.h"

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	int imageFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) & imageFlags)) {
		SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		SDL_Quit();
		return 0;
	}

	if (TTF_Init() == -1) {
		SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		SDL_Quit();
		return 0;
	}

	GameManager* game_mgr = new GameManager();
	Renderer* renderer = new Renderer(game_mgr);

	while(game_mgr->running){
		renderer->PreRender();

		game_mgr->Heartbeat();

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type) {
			case SDL_QUIT:
				game_mgr->running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				renderer->OnMouseClick(e.button);
				break;
			}
		}

		renderer->Render();

		renderer->PostRender();
	}

	return 0;
}