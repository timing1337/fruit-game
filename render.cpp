#include "render.h"

Renderer* Renderer::instancePtr = new Renderer();

void Renderer::Initialize() {
	Renderer* renderer = GetInstance();
	renderer->gWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RENDERER_WIDTH, RENDERER_HEIGHT, SDL_WINDOW_OPENGL);
	renderer->gRenderer = SDL_CreateRenderer(renderer->gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	SDL_SetRenderDrawBlendMode(renderer->gRenderer, SDL_BLENDMODE_BLEND);
}

void Renderer::PreRender() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(this->gRenderer);
}

void Renderer::SetBackgroundColor(const int r, const int g, const int b, const int a) {
	SDL_Rect fillRect = { 0, 0, RENDERER_WIDTH, RENDERER_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
	SDL_RenderFillRect(gRenderer, &fillRect);
}

void Renderer::PlayFadeTransition(function<void(TimerTask* self)> onTransitioned, function<void(TimerTask* self)> onComplete) {
	TaskManager::GetInstance()->RunTimerTask(FADING_OUT_TRANSITION_TICKS,
		[this](TimerTask* self) {
			int calculatedOpacity = self->GetProgress() * 255;
			SetBackgroundColor(0, 0, 0, self->GetProgress() * 255);
		}, [this, onTransitioned, onComplete](TimerTask* self) {
			onTransitioned(self);
			TaskManager::GetInstance()->RunTimerTask(FADING_IN_TRANSITION_TICKS,
				[this](TimerTask* self) {
					SetBackgroundColor(0, 0, 0, 255 - (self->GetProgress() * 255));
				},
				onComplete);
		});
}

void Renderer::UpdateRender() {
	SDL_RenderPresent(this->gRenderer);
}

bool Renderer::LoadFontByName(const char* name) {
	TTF_Font* font = TTF_OpenFont((game_data_path + "fonts/" + name + ".ttf").c_str(), 10);
	if (font == NULL) {
		SDL_Log(TTF_GetError());
		return false;
	}

	fonts[name] = font;
	return true;
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

	SDL_QueryTexture(texture, NULL, NULL, &game_texture->width, &game_texture->height);

	textures[name] = game_texture;
	return true;
}

TTF_Font* Renderer::GetFontByName(const char* name) {
	for (auto& font : fonts) {
		if (strcmp(font.first, name) == 0) {
			return font.second;
		}
	}
	if (!LoadFontByName(name)) {
		return NULL;
	}
	return fonts[name];
}

GameTexture* Renderer::GetTextureByName(const char* name) {
	for (auto& texture : textures) {
		if (strcmp(texture.first, name) == 0) {
			return texture.second;
		}
	}
	if (!LoadTextureByName(name)) {
		return NULL;
	}
	return textures[name];
}

void Renderer::FreeGameTexture(GameTexture* texture) {
	switch (texture->type) {
	case GameTextureType::SPRITE:
		SDL_DestroyTexture(texture->sprite->texture);
		delete texture->sprite;
		break;
	case GameTextureType::TEXT:
		SDL_DestroyTexture(texture->text->text);
		if (texture->text->outline != 0)
			SDL_DestroyTexture(texture->text->outlineText);
		delete texture->text;
		break;
	}
	delete texture;
}

GameTexture* Renderer::CreateText(const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize, const SDL_Color outlineColor) {
	TTF_Font* font = GetFontByName(fontId);

	if (font == NULL) {
		SDL_Log("Font not found: %s", fontId);
		return NULL;
	}

	TTF_SetFontSize(font, size);

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	GameTexture* textureData = new GameTexture();
	textureData->type = GameTextureType::TEXT;
	textureData->width = surface->w;
	textureData->height = surface->h;

	textureData->text = new RenderedText();
	textureData->text->font = font;
	textureData->text->text = texture;
	textureData->text->outline = outlineSize;

	TTF_SetFontOutline(font, outlineSize);

	if (outlineSize != 0) {
		SDL_Surface* outlineSurface = TTF_RenderText_Blended(font, text, outlineColor);
		SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(gRenderer, outlineSurface);
		SDL_SetTextureBlendMode(outlineTexture, SDL_BLENDMODE_BLEND);

		textureData->text->outlineText = outlineTexture;

		SDL_FreeSurface(outlineSurface);
	}

	SDL_FreeSurface(surface);

	TTF_SetFontOutline(font, 0);

	return textureData;
}

void Renderer::RenderTextureBackground(const char* textureId) {
	GameTexture* texture = GetTextureByName(textureId);
	Sprite* sprite = texture->sprite;
	SDL_RenderCopy(gRenderer, sprite->texture, NULL, NULL);
}

void Renderer::RenderTexture(const char* textureId, int x, int y, const Alignment align, const AlignmentVertical alignmentVertical) {
	GameTexture* texture = GetTextureByName(textureId);
	RenderTexture(texture, x, y, align, alignmentVertical);
}

void Renderer::RenderTexture(GameTexture* texture, int x, int y, const Alignment align, const AlignmentVertical alignmentVertical) {

	switch (alignmentVertical) {
	case AlignmentVertical::TOP:
		break;
	case AlignmentVertical::MIDDLE:
		y -= texture->height / 2;
		break;
	case AlignmentVertical::BOTTOM:
		y -= texture->height;
		break;
	}

	switch (align) {
	case Alignment::LEFT:
		break;
	case Alignment::RIGHT:
		x -= texture->width;
		break;
	case Alignment::CENTER:
		x -= texture->width / 2;
		break;
	}

	SDL_Rect rect = { x, y, texture->width, texture->height };

	switch (texture->type) {
		case GameTextureType::SPRITE:
			SDL_RenderCopy(gRenderer, texture->sprite->texture, NULL, &rect);
		break;
		case GameTextureType::TEXT:
			RenderedText* text = texture->text;

			if (text->outline != 0) {
				SDL_Rect outlineRect = { x - text->outline, y - text->outline, texture->width, texture->height };
				SDL_RenderCopy(gRenderer, text->outlineText, NULL, &outlineRect);
			}

			SDL_RenderCopy(gRenderer, text->text, NULL, &rect);
		break;
	}
}