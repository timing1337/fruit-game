#include "render.h"

Renderer::Renderer(GameManager* game_mgr) {
	gWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	this->game_mgr = game_mgr;

	MainMenu::Initialize(this);
}

void Renderer::PreRender() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(this->gRenderer);
}

void Renderer::Render() {
	switch (game_mgr->state) {
	case GameState::WAITING:
		MainMenu::Show(this);
		break;
	case GameState::STARTING:
		break;
	case GameState::RUNNING:
		break;
	case GameState::POSTGAME:
		break;
	}
}

void Renderer::OnMouseClick(SDL_MouseButtonEvent& e) {
	switch (game_mgr->state) {
	case GameState::WAITING:
		if (e.button != SDL_BUTTON_LEFT) {
			return;
		}

		const vec2_t center = vec2_t(width / 2, height / 2);

		vec2_t mousePos = vec2_t(e.x, e.y);

		GameTexture* playButton = GetTextureByName("ui/button_play");

		if (mousePos.x >= center.x - playButton->text->width / 2 && mousePos.x <= center.x + playButton->text->width / 2 &&
			mousePos.y >= center.y - playButton->text->height / 2 && mousePos.y <= center.y + playButton->text->height / 2) {
			game_mgr->FireStateChange(GameState::STARTING);
		}
		break;
	}
}

void Renderer::PostRender() {
	SDL_RenderPresent(this->gRenderer);
	SDL_UpdateWindowSurface(this->gWindow);
}

bool Renderer::LoadFontByName(const char* name) {
	//todo: hardcoded to ttf :p
	TTF_Font* font = TTF_OpenFont((game_data_path + "fonts/" + name + ".ttf").c_str(), 10);
	if (font == NULL) {
		SDL_Log(TTF_GetError());
		return false;
	}

	fonts[name] = font;
}

bool Renderer::LoadTextureByName(const char* name) {
	SDL_Texture* texture = IMG_LoadTexture(gRenderer, (game_data_path + "textures/" + name).c_str());
	if (texture == NULL) {
		SDL_Log(IMG_GetError());
		return false;
	}

	GameTexture* game_texture = new GameTexture();
	game_texture->type = GameTextureType::SPRITE;
	game_texture->sprite = new Sprite();
	game_texture->sprite->texture = texture;

	SDL_QueryTexture(texture, NULL, NULL, &game_texture->sprite->width, &game_texture->sprite->height);

	textures[name] = game_texture;
}

TTF_Font* Renderer::GetFontByName(const char* name) {
	if (fonts.find(name) == fonts.end()) {
		if (!LoadFontByName(name)) {
			return NULL;
		}
		return fonts[name];
	}
	return fonts[name];
}

GameTexture* Renderer::GetTextureByName(const char* name) {
	if (textures.find(name) == textures.end()) {
		if (!LoadTextureByName(name)) {
			return NULL;
		}
		return textures[name];
	}
	return textures[name];
}

GameTexture* Renderer::CreateText(const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize, const SDL_Color outlineColor) {
	TTF_Font* font = GetFontByName(fontId);

	TTF_SetFontSize(font, size);

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

	GameTexture* textureData = new GameTexture();
	textureData->type = GameTextureType::TEXT;

	textureData->text = new RenderedText();
	textureData->text->font = font;
	textureData->text->text = texture;
	textureData->text->width = surface->w;
	textureData->text->height = surface->h;
	textureData->text->outline = outlineSize;

	TTF_SetFontOutline(font, outlineSize);

	if (outlineSize != 0) {
		SDL_Surface* outlineSurface = TTF_RenderText_Blended(font, text, outlineColor);
		SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(gRenderer, outlineSurface);

		textureData->text->outlineText = outlineTexture;

		SDL_FreeSurface(outlineSurface);
	}

	SDL_FreeSurface(surface);

	return textureData;
}

void Renderer::RenderTextureBackground(const char* textureId) {
	GameTexture* texture = GetTextureByName(textureId);

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

void Renderer::RenderText(const char* textureId, int x, int y, const Alignment align) {
	GameTexture* texture = GetTextureByName(textureId);

	if (texture->type != GameTextureType::TEXT) {
		SDL_Log("Texture is not a text: %s\n", textureId);
		return;
	}

	RenderedText* text = texture->text;

	y -= text->height / 2;

	switch (align) {
	case Alignment::LEFT:
		x -= text->width;
		break;
	case Alignment::CENTER:
		x -= text->width / 2;
		break;
	case Alignment::RIGHT:
		break;
	}

	if (text->outline != 0) {
		SDL_Rect outlineRect = { x - text->outline, y - text->outline, text->width, text->height };
		SDL_RenderCopy(gRenderer, text->outlineText, NULL, &outlineRect);
	}

	SDL_Rect rect = { x, y, text->width, text->height };
	SDL_RenderCopy(gRenderer, text->text, NULL, &rect);
}