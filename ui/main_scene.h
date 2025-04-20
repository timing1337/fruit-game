#pragma once

#include "draw/post_processing.h"
#include "draw/surface_draw.h"
#include "entities/entity_mgr.h"
#include "render.h"

class MainScene
{
public:
	static SDL_Surface* gameCanvas;
	static GameTexture* scoreText;
	static GameTexture* comboText;

	static void Initialize();
	static void Show();
	static void UpdateScoreText();
	static void UpdateComboText();
};

