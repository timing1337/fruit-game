#include "render.h"

Renderer* Renderer::instancePtr = new Renderer();

void Renderer::Initialize() {
	Renderer* renderer = getInstance();
	renderer->gWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->width, renderer->height, SDL_WINDOW_OPENGL);
	renderer->gRenderer = SDL_CreateRenderer(renderer->gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	SDL_SetRenderDrawBlendMode(renderer->gRenderer, SDL_BLENDMODE_BLEND);

	MainMenu::Initialize();
	MainScene::Initialize();
	DeathScene::Initialize();
}

void Renderer::PreRender() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(this->gRenderer);
}

void Renderer::Render() {
	switch (GameManager::getInstance()->state) {
	case GameState::WAITING:
		MainMenu::Show();
		break;
	case GameState::STARTING:
	case GameState::RUNNING:
	case GameState::POSTGAME:
		MainScene::Show();
		break;
	case GameState::ENDGAME:
		DeathScene::Show();
		break;
	}
}

void Renderer::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (e.button != SDL_BUTTON_LEFT) {
		return;
	}
	switch (GameManager::getInstance()->state) {
	case GameState::WAITING:
		MainMenu::OnMouseClick(e);
		break;
	case GameState::ENDGAME:
		DeathScene::OnMouseClick(e);
		break;
	}
}

void Renderer::PlayFadeTransition(function<void(TimerTask* self)> onTransitioned, function<void(TimerTask* self)> onComplete) {
	TaskManager::getInstance()->RunTimerTask(500,
		[this](TimerTask* self) {
			SDL_Rect fillRect = { 0, 0, this->width, this->height };
			int calculatedOpacity = self->GetProgress() * 255;
			SDL_SetRenderDrawColor(this->gRenderer, 0, 0, 0, calculatedOpacity);
			SDL_RenderFillRect(this->gRenderer, &fillRect);
		}, [this, onTransitioned, onComplete](TimerTask* self) {
			onTransitioned(self);
			TaskManager::getInstance()->RunTimerTask(500,
				[this](TimerTask* self) {
					SDL_Rect fillRect = { 0, 0, this->width, this->height };
					int calculatedOpacity = 255 - (self->GetProgress() * 255);
					SDL_SetRenderDrawColor(this->gRenderer, 0, 0, 0, calculatedOpacity);
					SDL_RenderFillRect(this->gRenderer, &fillRect);
				},
				onComplete);
		});
}

void Renderer::PlayTitleAnimationAndStartGame() {
	TaskManager::getInstance()->RunTimerTask(4 * 500, [this](TimerTask* self) {
		int second = 3 - self->counter / 500;
		GameTexture* texture;
		//how many ticks left for fading
		int fadingTicks = self->counter % 500;
		float fadingProgress = (float)fadingTicks / 500;
		int calculatedOpacity = 255 - fadingProgress * 255;
		switch (second) {
		case 1:
			texture = this->GetTextureByName("ui/title_number_one");
			break;
		case 2:
			texture = this->GetTextureByName("ui/title_number_two");
			break;
		case 3:
			texture = this->GetTextureByName("ui/title_number_three");
			break;
		default:
			texture = this->GetTextureByName("ui/title_go");
			break;
		}
		SDL_SetTextureAlphaMod(texture->text->text, calculatedOpacity);
		this->RenderTexture(texture, this->width / 2, this->height / 2, Alignment::CENTER);
	},
	[this](TimerTask* self) {
			GameManager::getInstance()->FireStateChange(GameState::RUNNING);
	});
}

void Renderer::UpdateRender() {
	SDL_RenderPresent(this->gRenderer);
}

bool Renderer::LoadFontByName(const char* name) {
	//todo: hardcoded to ttf :p
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

	TTF_SetFontSize(font, size);

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

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

		textureData->text->outlineText = outlineTexture;

		SDL_FreeSurface(outlineSurface);
	}

	SDL_FreeSurface(surface);

	return textureData;
}

void Renderer::RenderTextureBackground(const char* textureId) {
	GameTexture* texture = GetTextureByName(textureId);
	Sprite* sprite = texture->sprite;
	SDL_RenderCopy(gRenderer, sprite->texture, NULL, NULL);
}

void Renderer::SetBackgroundColor(const int r, const int g, const int b, const int a) {
	SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
	SDL_RenderClear(gRenderer);
}

void Renderer::RenderTexture(const char* textureId, int x, int y, const Alignment align) {
	GameTexture* texture = GetTextureByName(textureId);
	RenderTexture(texture, x, y, align);
}

void Renderer::RenderTexture(GameTexture* texture, int x, int y, const Alignment align) {
	y -= texture->height / 2;

	switch (align) {
	case Alignment::LEFT:
		x -= texture->width;
		break;
	case Alignment::CENTER:
		x -= texture->width / 2;
		break;
	case Alignment::RIGHT:
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