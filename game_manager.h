#pragma once

#include "SDL.h"

namespace fruit_game {
	class GameManager
	{
	public:
		bool running = true;
		bool isInMenu = true;

		SDL_Event event;

		void Tick();
	};
}