
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_timer.h"
#include "SDL_mixer.h"

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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	Renderer* renderer = Renderer::GetInstance();
	GameManager* game_mgr = GameManager::GetInstance();
	TaskManager* animation_mgr = TaskManager::GetInstance();
	EntityManager* entity_mgr = EntityManager::GetInstance();
	SceneManager* scene_mgr = SceneManager::GetInstance();

	renderer->Initialize();
	entity_mgr->Initialize();
	scene_mgr->Initialize();

	Downsampling::Initialize();

	game_mgr->lastUpdatedTicks = SDL_GetTicks();

	srand(time(0));

	while (game_mgr->running) {
		Uint32 current = SDL_GetTicks();
		renderer->PreRender();
		game_mgr->deltaTime = (current - game_mgr->lastUpdatedTicks);

		entity_mgr->Heartbeat(game_mgr->deltaTime);

		//TODO: event bus, honestly just something minor if we have time for it
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type) {
			case SDL_QUIT:
				game_mgr->running = false;
				game_mgr->gameData->Save();
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
				case SDLK_ESCAPE:
					if (game_mgr->state == GameState::RUNNING) {
						PauseScreen* pauseScreen = (PauseScreen*)scene_mgr->GetScene(SceneId::PAUSE);
						pauseScreen->SetActive(true);
						game_mgr->FireStateChange(GameState::PAUSED);
					}
					else if (game_mgr->state == GameState::PAUSED) {
						PauseScreen* pauseScreen = (PauseScreen*)scene_mgr->GetScene(SceneId::PAUSE);
						pauseScreen->SetActive(false);
						game_mgr->FireStateChange(GameState::RUNNING);
					}

					if (scene_mgr->GetScene(SceneId::COSMETIC)->active) {
						renderer->PlayFadeTransition(
							[scene_mgr](TimerTask* self) {
								scene_mgr->GetScene(SceneId::COSMETIC)->SetActive(false);
								scene_mgr->GetScene(SceneId::MAIN_MENU)->SetActive(true);
							},
							[](TimerTask* self) {});
					}
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
