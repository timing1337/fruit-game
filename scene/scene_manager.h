#pragma once

#include "render.h"
#include "base_scene.h"

#include "scene/types/main_menu.h"
#include "scene/types/main_stage.h"
#include "scene/types/end_stage.h"
#include "scene/types/pause_screen.h"

#include <vector>

using namespace std;

class SceneManager
{
public:
	vector<BaseScene*> scenes;

	static SceneManager* instancePtr;

	static SceneManager* GetInstance() {
		return instancePtr;
	}

	void Initialize();
	void Render();
	BaseScene* GetScene(SceneId sceneId);

	void OnMouseClick(SDL_MouseButtonEvent& e);
};

