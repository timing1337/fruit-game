#pragma once

#include "draw/downsampling.h"
#include "draw/surface_draw.h"
#include "entities/entity_mgr.h"
#include "render.h"
#include "utils/gradient.h"

class MainScene
{
public:
	static SDL_Surface* gameCanvas;
	static SDL_Surface* downsampledGameCanvas;
	static GameTexture* scoreText;
	static GameTexture* comboText;

	static void Initialize();
	static void Show();
	static void UpdateScoreText();
	static void UpdateComboText();
	static void DrawMousePathRecord(MousePathRecord* record);
};

