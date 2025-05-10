#pragma once

#include "constant.h"
#include "game/game.h"
#include "scene/base_scene.h"

class EndStage : public BaseScene
{
public:
	EndStage();
	void Prepare() override;
	void Render() override;

	void PlayAgain(ButtonElement* button);
	void ReturnToMenu(ButtonElement* button);
};