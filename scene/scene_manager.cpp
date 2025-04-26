#include "scene_manager.h"

SceneManager* SceneManager::instancePtr = new SceneManager();

void SceneManager::Initialize() {
	MainMenu* mainMenu = new MainMenu();
	mainMenu->SetActive(true);
	this->scenes.push_back(mainMenu);

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
		if (scene->isActive) {
			scene->Render();
		}
	}
}

void SceneManager::OnMouseClick(SDL_MouseButtonEvent& e) {
	for (auto& scene : this->scenes) {
		if (scene->isActive) {
			scene->OnMouseClick(e);
		}
	}
}