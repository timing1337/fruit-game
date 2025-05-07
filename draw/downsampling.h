#pragma once

#include "constant.h"
#include "SDL.h"
#include "render.h"

class Downsampling
{
private:
	static SDL_Texture* downsampledTextures[5];

public:
	static void Initialize();
	static void Downsample(SDL_Texture* texture, SDL_Texture* fallbackTarget = nullptr);
	static void Render();
	static void Free();
};

