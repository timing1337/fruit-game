#pragma once

#include "draw/surface_draw.h"
#include "entities/entity_mgr.h"
#include "render.h"

class MainScene
{
public:
	static SDL_Surface* gameCanvas;
	static int killEffectFade;
	static GameTexture* scoreText;

	static void Initialize();
	static void Show();
	static void UpdateScoreText();
};

