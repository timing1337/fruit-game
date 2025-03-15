#pragma once

#include "entity.h"
#include "render.h"

class MainScene
{
private:
	static GameTexture* scoreText;
public:
	static void Initialize();
	static void Show();
	static void UpdateScoreText();
};

