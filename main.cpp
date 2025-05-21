
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_timer.h"
#include "SDL_mixer.h"

#include "audio/audio_mgr.h"
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
	AudioManager* audio_mgr = AudioManager::GetInstance();

	renderer->Initialize();
	game_mgr->Initialize();
	entity_mgr->Initialize();
	scene_mgr->Initialize();
	audio_mgr->Initialize();

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
					if (scene_mgr->lockInteraction) break;

					BaseScene* currentScene = scene_mgr->currentScene;

					if (currentScene->sceneId == SceneId::GAME) {
						if (game_mgr->state == GameState::RUNNING) {
							PauseScreen* pauseScreen = (PauseScreen*)scene_mgr->GetScene(SceneId::PAUSE);
							pauseScreen->SetActive(true);
							game_mgr->FireStateChange(GameState::PAUSED);
							audio_mgr->PlaySound("button.wav");

						}
						else if (game_mgr->state == GameState::PAUSED) {
							PauseScreen* pauseScreen = (PauseScreen*)scene_mgr->GetScene(SceneId::PAUSE);
							pauseScreen->SetActive(false);
							game_mgr->FireStateChange(GameState::RUNNING);
							audio_mgr->PlaySound("button.wav");
						}
					}
					else {
						//Ulgy hacks #2

						//ok player is trying to revert back to previous scene, prob is on settings and wanna go back
						if (game_mgr->state == GameState::PAUSED) {
							scene_mgr->TransitionToScene(SceneId::GAME);
						}
						else {
							//or player is just chilling in menu
							if (currentScene->sceneId != SceneId::MAIN_MENU) {
								scene_mgr->TransitionToScene(SceneId::MAIN_MENU);
								audio_mgr->PlaySound("button.wav");
							}
						}
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
