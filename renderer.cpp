#include "renderer.h"
#include "game_manager.h"

#include "ui/menu.h";

namespace fruit_game
{
	renderer::renderer() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}

		gWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 928, 522, SDL_WINDOW_OPENGL);

		if (gWindow == nullptr) {
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}

		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

		if (gRenderer == nullptr) {
			SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			SDL_Quit();
			return;
		}

		if (TTF_Init() == -1) {
			SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			SDL_Quit();
			return;
		}
	}

	renderer::~renderer() {
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
	}

	void renderer::Render(game_manager* game_manager) {
		/*
		* Could have built an entire ui framework for this
		* but imo its kinda overkill.
		* well.... if i have time....
		* TODO: build an ui framework
		*/
		if (game_manager->isInMenu) {
			fruit_ui::menu::Render(this);
		}
		else {
		}
		SDL_RenderClear(this->gRenderer);
		SDL_RenderPresent(this->gRenderer);
		SDL_UpdateWindowSurface(this->gWindow);
	}
}