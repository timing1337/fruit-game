#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "game_manager.h";

namespace fruit_game {
	class renderer
	{
	public:
		SDL_Window* gWindow;
		SDL_Surface* gScreenSurface;
		SDL_Renderer* gRenderer;

		renderer();
		~renderer();

		void Render(game_manager* game);
	};
}