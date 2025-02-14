#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "font_manager.h"
#include "game_manager.h"
#include "texture_manager.h"

namespace fruit_game {
	class Renderer
	{
	public:
		SDL_Window* gWindow;
		SDL_Surface* gScreenSurface;
		SDL_Renderer* gRenderer;
		
		TextureManager* texture_mgr;
		FontManager* font_mgr;

		Renderer();
		~Renderer();

		void Render(GameManager* game);
	};
}