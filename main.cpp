#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_timer.h"

#include "game.h"
#include "render.h"

#include "animation/animation.h"

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

	Renderer::Initialize();

	Renderer* renderer = Renderer::getInstance();
	GameManager* game_mgr = GameManager::getInstance();
	AnimationManager* animation_mgr = AnimationManager::getInstance();

	while(game_mgr->running){
		renderer->PreRender();

		game_mgr->Heartbeat();
		renderer->Render();
		animation_mgr->Heartbeat();

		renderer->UpdateRender();
	}

	return 0;
}