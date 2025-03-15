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

	std::srand(std::time(0));

	Renderer::Initialize();

	Renderer* renderer = Renderer::getInstance();
	GameManager* game_mgr = GameManager::getInstance();

	while(game_mgr->running){
		game_mgr->Heartbeat();

		renderer->PreRender();
		renderer->Render();

		//is this bad? yes
		//Do i have any idea how to approach this correctly? no
		while (true) {
			bool allReady = true;
			for (auto& animation : AnimationManager::getInstance()->animations) {
				if (!animation.second->isReadyForRender) {
					allReady = false;
					break;
				}
			}
			if (allReady) {
				break;
			}
		}
		renderer->UpdateRender();

		//Resetting their state.
		for (auto& animation : AnimationManager::getInstance()->animations) {
			animation.second->isReadyForRender = false;
		}
	}

	return 0;
}