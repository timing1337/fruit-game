#pragma once

#include "render.h"
#include "base_scene.h"

#include "scene/types/main_menu.h"

#include <vector>

using namespace std;

class SceneManager
{
public:
	vector<BaseScene*> scenes;

	static SceneManager* instancePtr;

	static SceneManager* getInstance() {
		return instancePtr;
	}

	void Initialize();
	void Render();
	BaseScene* GetScene(SceneId sceneId);

	void OnMouseClick(SDL_MouseButtonEvent& e);
};

