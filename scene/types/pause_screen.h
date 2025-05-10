#pragma once

#include "constant.h"
#include "game/game.h"
#include "scene/base_scene.h"

class PauseScreen : public BaseScene
{
public:
	PauseScreen();
	void Render() override;

	void OnQuit();
};