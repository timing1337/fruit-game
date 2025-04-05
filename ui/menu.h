#pragma once

#include "SDL.h"

#include "utils/math.h"
#include "render.h"

class MainMenu
{
public:
	static void Initialize();
	static void Show();
	static void OnMouseClick(SDL_MouseButtonEvent& e);
};

