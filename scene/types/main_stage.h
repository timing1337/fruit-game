#pragma once


#include "draw/downsampling.h"
#include "constant.h"
#include "game/game.h"
#include "scene/base_scene.h"
#include "entities/entity_mgr.h"
#include "render.h"
#include "utils/gradient.h"

class MainStage : public BaseScene
{
public:

	//Used for glow.
	SDL_Texture* glowCanvas;
	//seperate canvas so we can apply shakes :D
	SDL_Texture* gameCanvas;

	int shakeIntensity = 0;
	float shakeFrequency = 0;
	int redColorOverlayOpacity = 0;

	MainStage();

	void Prepare() override;
	void Render() override;
	void Release() override;

	void DrawMousePathRecord(MousePathRecord* record);
};

