#pragma once

#include "constant.h"
#include "SDL.h"
#include "render.h"

class Downsampling
{
private:
	static SDL_Texture* downsampledTextures[6];

public:
	static void Initialize();
	static void Downsample(SDL_Texture* texture);
	static void Render();
	static void Free();
};

