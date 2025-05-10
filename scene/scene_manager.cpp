#include "scene_manager.h"

SceneManager* SceneManager::instancePtr = new SceneManager();

void SceneManager::Initialize() {
	GameTexture* backgroundTexture = Renderer::GetInstance()->GetTextureByName("background.png");
	GameTexture* backgroundBlurredTexture = Renderer::GetInstance()->GetTextureByName("background_blurred.png");

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

	this->scenes.push_back(mainMenu);
	this->scenes.push_back(mainStage);
	this->scenes.push_back(endStage);
	this->scenes.push_back(pauseScreen);
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
		if (scene->active) {
			scene->OnMouseClick(e);
		}
	}
}