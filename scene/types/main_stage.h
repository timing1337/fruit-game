#pragma once


#include "draw/downsampling.h"
#include "draw/surface_draw.h"
#include "constant.h"
#include "game/game.h"
#include "scene/base_scene.h"
#include "entities/entity_mgr.h"
#include "render.h"
#include "utils/gradient.h"

class MainStage : public BaseScene
{
public:

	SDL_Surface* gameCanvas;

	MainStage();

	void Prepare() override;
	void Render() override;
	void Release() override;

	void DrawMousePathRecord(MousePathRecord* record);
};

