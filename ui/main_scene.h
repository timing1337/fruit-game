#pragma once

#include "entities/entity.h"
#include "render.h"
#include "draw/surface_draw.h"

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

