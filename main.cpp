
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_timer.h"

#include "game/game.h"
#include "render.h"
#include "draw/downsampling.h"
#include "entities/entity_mgr.h"
#include "scene/scene_manager.h"

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

	Renderer* renderer = Renderer::GetInstance();
	GameManager* game_mgr = GameManager::GetInstance();
	TaskManager* animation_mgr = TaskManager::GetInstance();
	EntityManager* entity_mgr = EntityManager::GetInstance();
	SceneManager* scene_mgr = SceneManager::GetInstance();

	game_mgr->Initialize();
	renderer->Initialize();
	entity_mgr->Initialize();
	scene_mgr->Initialize();

	Downsampling::Initialize();

	game_mgr->lastUpdatedTicks = SDL_GetTicks();

	while (game_mgr->running) {
		Uint32 current = SDL_GetTicks();
		renderer->PreRender();
		game_mgr->deltaTime = (current - game_mgr->lastUpdatedTicks);

		entity_mgr->Heartbeat(game_mgr->deltaTime);

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type) {
			case SDL_QUIT:
				game_mgr->running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				game_mgr->OnMouseClick(e.button);
				scene_mgr->OnMouseClick(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				game_mgr->OnMouseRelease(e.button);
				break;
			case SDL_MOUSEMOTION:
				game_mgr->OnMouseMove(e.button);
				break;
			case SDL_KEYDOWN:

				switch (e.key.keysym.sym) {
				case SDLK_SPACE:
					game_mgr->running = false;
					break;
				}
				break;
			}
		}

		game_mgr->Heartbeat(game_mgr->deltaTime);
		
		scene_mgr->Render();

		animation_mgr->Heartbeat(game_mgr->deltaTime);

		renderer->UpdateRender();
		game_mgr->lastUpdatedTicks = current;

		entity_mgr->CleanUp();
	}

	return 0;
}
