#pragma once

#include "render.h"
#include "base_scene.h"

#include "scene/types/main_menu.h"
#include "scene/types/main_stage.h"
#include "scene/types/end_stage.h"
#include "scene/types/pause_screen.h"
#include "scene/types/cosmetic_menu.h"
#include "scene/types/settings.h"

#include <vector>

class SceneManager
{
public:
	std::vector<BaseScene*> scenes;

	BaseScene* currentScene = nullptr;
	bool lockInteraction = false;

	static SceneManager* instancePtr;

	static SceneManager* GetInstance() {
		return instancePtr;
	}

	void Initialize();
	void Render();
	BaseScene* GetScene(SceneId sceneId);

	void TransitionToScene(SceneId sceneId, std::function<void(SceneManager* scene_mgr)> callback = nullptr);

	void OnMouseClick(SDL_MouseButtonEvent& e);
};

