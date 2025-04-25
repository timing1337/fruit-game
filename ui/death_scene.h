#pragma once

#include "constant.h"
#include "render.h"

class DeathScene
{
private:
public:
	static void Initialize();
	static void Show();
	static void OnMouseClick(SDL_MouseButtonEvent& e);
};