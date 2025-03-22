#pragma once

#include "draw/surface_draw.h"
#include "entities/entity_mgr.h"
#include "render.h"

class MainScene
{
private:
	static GameTexture* scoreText;
public:
	static SDL_Surface* gameCanvas;

	static void Initialize();
	static void Show();
	static void UpdateScoreText();
};

