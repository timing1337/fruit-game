#pragma once

#include "config/blade_color.h"
#include "game/game.h"
#include "scene/base_scene.h"

class CosmeticMenu : public BaseScene
{
public:
	CosmeticMenu();

	void OnSelectBlade(ImageElement* blade);
};

