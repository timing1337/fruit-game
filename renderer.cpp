#include "renderer.h"
#include "game_manager.h"

#include "ui/menu.h"

namespace fruit_game
{
	Renderer::Renderer() {
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

		int imageFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags)) { // what
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			SDL_Quit();
			return;
		}

		if (TTF_Init() == -1) {
			SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			SDL_Quit();
			return;
		}

		texture_mgr = new TextureManager(this->gRenderer);
		font_mgr = new FontManager();
	}

	Renderer::~Renderer() {
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		IMG_Quit();
		TTF_Quit();
	}

	void Renderer::Render(GameManager* GameManager) {
		/*
		* Could have built an entire ui framework for this
		* but imo its kinda overkill.
		* well.... if i have time....
		* TODO: build an ui framework
		*/
		//Reset the frame
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(this->gRenderer);
		if (GameManager->isInMenu) {
			fruit_ui::menu::Render(this);
		}
		else {
			//Render the game
		}
		SDL_RenderPresent(this->gRenderer);
		SDL_UpdateWindowSurface(this->gWindow);
	}
}