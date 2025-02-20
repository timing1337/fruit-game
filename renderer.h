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

		const int width = 928;
		const int height = 522;

		Renderer();
		~Renderer();

		void Render(GameManager* game);

		GameTexture* CreateText(const char* text, const char* fontId, const int size, const SDL_Color color);
		GameTexture* CreateOutlineText(const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize = 1, const SDL_Color outlineColor = { 0, 0, 0, 0xff });

		void RenderSprite(const char* textureId, const int x, const int y);
		void RenderTextureBackground(const char* textureId);
		void SetBackgroundColor(const int r, const int g, const int b, const int a);
		void RenderText(const char* textureId, const int x, const int y);
	};
}