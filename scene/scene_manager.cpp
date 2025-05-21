#include "scene_manager.h"

SceneManager* SceneManager::instancePtr = new SceneManager();

void SceneManager::Initialize() {
	GameTexture* backgroundTexture = Renderer::GetInstance()->GetTextureByName("background.png");
	GameTexture* backgroundBlurredTexture = Renderer::GetInstance()->GetTextureByName("background_blurred.png");
	GameTexture* cosmeticMenuTexture = Renderer::GetInstance()->GetTextureByName("cosmetic_background.png");

	MainMenu* mainMenu = new MainMenu();
	mainMenu->SetBackgroundTexture(backgroundTexture);
	mainMenu->SetActive(true);

	MainStage* mainStage = new MainStage();
	mainStage->SetBackgroundTexture(backgroundTexture);
	mainStage->SetActive(false);

	EndStage* endStage = new EndStage();
	endStage->SetBackgroundTexture(backgroundBlurredTexture);
	endStage->SetActive(false);

	PauseScreen* pauseScreen = new PauseScreen();
	pauseScreen->SetActive(false);

	CosmeticMenu* cosmeticMenu = new CosmeticMenu();
	cosmeticMenu->SetBackgroundTexture(cosmeticMenuTexture);
	cosmeticMenu->SetActive(false);

	Settings* settings = new Settings();
	settings->SetBackgroundTexture(backgroundTexture);
	settings->SetActive(false);

	this->currentScene = mainMenu;

	this->scenes.push_back(mainMenu);
	this->scenes.push_back(mainStage);
	this->scenes.push_back(endStage);
	this->scenes.push_back(cosmeticMenu);
	this->scenes.push_back(settings);
	this->scenes.push_back(pauseScreen);
}

void SceneManager::TransitionToScene(SceneId sceneId, std::function<void(SceneManager* scene_mgr)> callback) {
	Renderer* renderer = Renderer::GetInstance();

	if (lockInteraction) {
		return;
	}

	SDL_Log("Transitioning to scene %d", (int)sceneId);

	lockInteraction = true;

	renderer->PlayFadeTransition(
		[this, sceneId](TimerTask* self) {
			GameManager* game_mgr = GameManager::GetInstance();

			PauseScreen* pauseScreen = (PauseScreen*)this->GetScene(SceneId::PAUSE);

			this->currentScene->SetActive(false);

			//Really ulgy hacks
			// I hate myself
			

			//check if player is on the game menu and pausing the game,
			//this means player is moving to other scene (or settings),
			//temporary disable the overlay
			if (this->currentScene->sceneId == SceneId::GAME && game_mgr->state == PAUSED) {
				pauseScreen->SetActive(false);
			}

			this->currentScene = this->GetScene(sceneId);
			this->currentScene->SetActive(true);

			//Or.... player current scene is other scenes (aka settings) and trying to move back to game (paused)
			//turn back the over lay
			if (this->currentScene->sceneId == SceneId::GAME && game_mgr->state == PAUSED) {
				pauseScreen->SetActive(true);
			}

			//Or... player is on pause scene and IS trying to get back to menu, we need to turn off the overlay before transitioning
			if (this->currentScene->sceneId == SceneId::MAIN_MENU && pauseScreen->active) {
				pauseScreen->SetActive(false);

			}

			SDL_Log("Transitioned to scene %d", (int)sceneId);
		},
		[this, callback](TimerTask* self) {
			lockInteraction = false;
			if (callback != nullptr) {
				callback(this);
			}
			SDL_Log("Transition complete");
			SDL_Log("Current scene: %d", (int)this->currentScene->sceneId);
		});
}

BaseScene* SceneManager::GetScene(SceneId sceneId) {
	for (auto& scene : this->scenes) {
		if (scene->sceneId == sceneId) {
			return scene;
		}
	}
	return nullptr;
}

void SceneManager::Render() {
	for (auto& scene : this->scenes) {
		if (scene->active) {
			scene->Render();
		}
	}
}

void SceneManager::OnMouseClick(SDL_MouseButtonEvent& e) {
	for (auto& scene : this->scenes) {
		if (scene->active && !lockInteraction) {
			scene->OnMouseClick(e);
		}
	}
}