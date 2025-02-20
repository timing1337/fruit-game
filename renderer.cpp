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

		texture_mgr = new TextureManager(gRenderer);
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

	void Renderer::RenderText(const char* textureId, int x, int y) {
		GameTexture* texture = texture_mgr->textures[textureId];
		if (texture == nullptr) {
			SDL_Log("Texture not found: %s\n", textureId);
			return;
		}
		if (texture->type != GameTextureType::TEXT) {
			SDL_Log("Texture is not text: %s\n", textureId);
			return;
		}
		RenderedText* text = texture->text;

		SDL_Rect rect = { x + text->outline, y + text->outline, text->width, text->height };
		SDL_RenderCopy(gRenderer, text->text, NULL, &rect);

		if (text->outline > 0) {
			SDL_Rect outlineRect = { x, y, text->width, text->height };
			SDL_RenderCopy(gRenderer, text->outlineText, NULL, &outlineRect);
		}
	}

	void Renderer::RenderSprite(const char* textureId, int x, int y) {
		GameTexture* texture = texture_mgr->textures[textureId];
		if (texture == nullptr) {
			SDL_Log("Texture not found: %s\n", textureId);
			return;
		}

		if (texture->type != GameTextureType::SPRITE) {
			SDL_Log("Texture is not a sprite: %s\n", textureId);
			return;
		}

		Sprite* sprite = texture->sprite;

		SDL_Rect rect = { x, y, sprite->width, sprite->height };
		SDL_RenderCopy(gRenderer, sprite->texture, NULL, &rect);
	}

	void Renderer::RenderTextureBackground(const char* textureId) {
		GameTexture* texture = texture_mgr->textures[textureId];
		if (texture == nullptr) {
			SDL_Log("Texture not found: %s\n", textureId);
			return;
		}

		if (texture->type != GameTextureType::SPRITE) {
			SDL_Log("Texture is not a sprite: %s\n", textureId);
			return;
		}

		Sprite* sprite = texture->sprite;

		SDL_RenderCopy(gRenderer, sprite->texture, NULL, NULL);
	}

	void Renderer::SetBackgroundColor(const int r, const int g, const int b, const int a) {
		SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
		SDL_RenderClear(gRenderer);
	}

	GameTexture* Renderer::CreateText(const char* text, const char* fontId, const int size, const SDL_Color color) {
		TTF_Font* font = font_mgr->fonts[fontId];
		if (font == nullptr) {
			SDL_Log("Font not found: %s\n", fontId);
			return NULL;
		}

		TTF_SetFontSize(font, size);

		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
		if (surface == nullptr) {
			SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			return NULL;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

		GameTexture* textureData = new GameTexture();
		textureData->type = GameTextureType::TEXT;

		textureData->text = new RenderedText();
		textureData->text->font = font;
		textureData->text->outline = 0;
		textureData->text->text = texture;
		textureData->text->outlineText = nullptr;
		textureData->text->width = surface->w;
		textureData->text->height = surface->h;

		SDL_FreeSurface(surface);

		return textureData;
	}

	GameTexture* Renderer::CreateOutlineText(const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize, const SDL_Color outlineColor){
		TTF_Font* font = font_mgr->fonts[fontId];
		if (font == nullptr) {
			SDL_Log("Font not found: %s\n", fontId);
			return NULL;
		}

		GameTexture* textureData = new GameTexture();
		textureData->type = GameTextureType::TEXT;

		TTF_SetFontSize(font, size);

		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
		if (surface == nullptr) {
			SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			return NULL;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

		TTF_SetFontOutline(font, outlineSize);

		SDL_Surface* outlineSurface = TTF_RenderText_Blended(font, text, outlineColor);

		if (outlineSurface == nullptr) {
			SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			return NULL;
		}

		SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(gRenderer, outlineSurface);

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		textureData->text = new RenderedText();
		textureData->text->font = font;
		textureData->text->text = texture;
		textureData->text->width = surface->w;
		textureData->text->height = surface->h;

		textureData->text->outlineText = outlineTexture;
		textureData->text->outline = outlineSize;

		SDL_FreeSurface(surface);
		SDL_FreeSurface(outlineSurface);

		return textureData;
	}

}