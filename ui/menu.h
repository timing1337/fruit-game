#pragma once

#include "SDL.h"

#include "utils/math.h"
#include "render.h"

class Renderer;

class MainMenu
{
public:
	static void Initialize(Renderer* renderer);
	static void Show(Renderer* renderer);
};

