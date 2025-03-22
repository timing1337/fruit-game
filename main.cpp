#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_timer.h"

#include "entities/entity_mgr.h"
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

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	Renderer::Initialize();

	Renderer* renderer = Renderer::getInstance();
	GameManager* game_mgr = GameManager::getInstance();
	AnimationManager* animation_mgr = AnimationManager::getInstance();
	EntityManager* entity_mgr = EntityManager::getInstance();

	game_mgr->lastUpdatedTicks = SDL_GetTicks();

	while (game_mgr->running) {
		Uint32 current = SDL_GetTicks();
		renderer->PreRender();
		game_mgr->Heartbeat();
		
		game_mgr->deltaTime = (current - game_mgr->lastUpdatedTicks);
		entity_mgr->Heartbeat(game_mgr->deltaTime);

		renderer->Render();

		animation_mgr->Heartbeat();

		renderer->UpdateRender();
		game_mgr->lastUpdatedTicks = current;
	}

	return 0;
}