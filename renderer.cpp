#include "renderer.h"
#include "game_manager.h"

#include "ui/menu.h"

namespace fruit_game
{
	Renderer::Renderer() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}

		gWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

		if (gWindow == nullptr) {
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}

		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

		if (gRenderer == nullptr) {
			SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}

		int imageFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags)) { // what
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			SDL_Quit();
			return;
		}

		if (TTF_Init() == -1) {
			SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			SDL_Quit();
			return;
		}

		texture_mgr = new TextureManager(this->gRenderer);
		font_mgr = new FontManager();

		fruit_ui::menu::Initialize(this);
	}

	Renderer::~Renderer() {
		SDL_Log("Destroying renderer");
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		delete texture_mgr;
		delete font_mgr;
		SDL_Quit();
		IMG_Quit();
		TTF_Quit();
	}

	void Renderer::Render(GameManager* gameManager) {
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(this->gRenderer);
		if (gameManager->isInMenu) {
			fruit_ui::menu::Render(this);
		}
		else {
			//Render the game
		}
		SDL_RenderPresent(this->gRenderer);
		SDL_UpdateWindowSurface(this->gWindow);
	}

	// API WRAPPER

	void Renderer::RenderTexture(const char* textureId, int x, int y) {
		TextureData* texture = texture_mgr->textures[textureId];
		if (texture == nullptr) {
			SDL_Log("Texture not found: %s\n", textureId);
			return;
		}
		SDL_Rect destRect = { x, y, texture->width, texture->height };
		SDL_RenderCopy(gRenderer, texture->texture, NULL, &destRect);
	}

	void Renderer::RenderTextureBackground(const char* textureId) {
		TextureData* texture = texture_mgr->textures[textureId];
		if (texture == nullptr) {
			SDL_Log("Texture not found: %s\n", textureId);
			return;
		}
		SDL_RenderCopy(gRenderer, texture->texture, NULL, NULL);
	}

	TextureData* Renderer::CreateText(const char* text, const char* fontName, const int size, const SDL_Color color) {
		TTF_Font* font = font_mgr->fonts[fontName];
		if (font == nullptr) {
			SDL_Log("Font not found: %s\n", fontName);
			return NULL;
		}

		TTF_SetFontSize(font, size);

		SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
		if (surface == nullptr) {
			SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			return NULL;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

		TextureData* textureData = new TextureData();
		textureData->texture = texture;
		textureData->width = surface->w;
		textureData->height = surface->h;

		SDL_FreeSurface(surface);

		return textureData;
	}

	TextureData* Renderer::CreateText(const char* text, const char* fontName, const int size, const int r, const int g, const int b, const int a) {
		SDL_Color color = { r, g, b, a };
		return CreateText(text, fontName, size, color);
	}
}