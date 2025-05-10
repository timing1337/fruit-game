#pragma once

#include "constant.h"
#include "game/game.h"
#include "scene/base_scene.h"

class MainMenu : public BaseScene
{
public:
	MainMenu();

	void OnStart(ButtonElement* button);
	void OnSettings(ButtonElement* button);
	void OnQuit(ButtonElement* button);
};

