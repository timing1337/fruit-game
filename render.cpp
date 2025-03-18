#include "render.h"

Renderer* Renderer::instancePtr = new Renderer();

void Renderer::Initialize() {
	Renderer* renderer = getInstance();
	renderer->gWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer->width, renderer->height, SDL_WINDOW_OPENGL);
	renderer->gRenderer = SDL_CreateRenderer(renderer->gWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(renderer->gRenderer, SDL_BLENDMODE_BLEND);

	MainMenu::Initialize();
	MainScene::Initialize();
}

void Renderer::PreRender() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(this->gRenderer);
}

void Renderer::Render() {
	switch (GameManager::getInstance()->state) {
	case GameState::WAITING:
	case GameState::PREPARING:
		MainMenu::Show();
		break;
	case GameState::STARTING:
	case GameState::RUNNING:
		MainScene::Show();
		break;
	case GameState::POSTGAME:
		break;
	}
}

void Renderer::OnMouseClick(SDL_MouseButtonEvent& e) {
	switch (GameManager::getInstance()->state) {
	case GameState::WAITING:
		if (e.button != SDL_BUTTON_LEFT) {
			return;
		}

		const SDL_Point center{ width / 2, height / 2 };

		SDL_Point mousePos{ e.x, e.y };

		GameTexture* playButton = GetTextureByName("ui/button_play");

		if (mousePos.x >= center.x - playButton->text->width / 2 && mousePos.x <= center.x + playButton->text->width / 2 &&
			mousePos.y >= center.y - playButton->text->height / 2 && mousePos.y <= center.y + playButton->text->height / 2) {
			this->OnStarting();
		}
		break;
	}
}

void Renderer::OnStarting() {
	GameManager::getInstance()->FireStateChange(GameState::PREPARING);
	AnimationManager::getInstance()->Play(10 * 100, [this](Animation* self) {
		SDL_Rect fillRect = { 0, 0, this->width, this->height };
		int calculatedOpacity;
		if (self->current <= 500) {
			calculatedOpacity = (self->current * 255) / 500;
		}
		else {
			GameManager::getInstance()->FireStateChange(GameState::STARTING);
			calculatedOpacity = 255 - (((self->current - 500) * 255) / 500);
		}
		SDL_SetRenderDrawColor(this->gRenderer, 0, 0, 0, calculatedOpacity);
		SDL_RenderFillRect(this->gRenderer, &fillRect);
	},
	[this](Animation* self) {
			this->PlayTitleAnimation();
	});
}

void Renderer::PlayTitleAnimation() {
	AnimationManager::getInstance()->Play(4 * 500, [this](Animation* self) {
		int second = 3 - self->current / 500;
		GameTexture* texture;
		int calculatedOpacity = 255 - ((self->current % 500) * 255) / 500;
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
		this->RenderText(texture, this->width / 2, this->height / 2, Alignment::CENTER);
	},
	[this](Animation* self) {
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

	SDL_QueryTexture(texture, NULL, NULL, &game_texture->sprite->width, &game_texture->sprite->height);

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
	Sprite* sprite = texture->sprite;
	SDL_RenderCopy(gRenderer, sprite->texture, NULL, NULL);
}

void Renderer::SetBackgroundColor(const int r, const int g, const int b, const int a) {
	SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
	SDL_RenderClear(gRenderer);
}

void Renderer::RenderText(const char* textureId, int x, int y, const Alignment align) {
	GameTexture* texture = GetTextureByName(textureId);
	RenderText(texture, x, y, align);
}

void Renderer::RenderText(GameTexture* texture, int x, int y, const Alignment align) {
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

void Renderer::OnMousePathRecorded(MousePathRecord record) {
	AnimationManager::getInstance()->Play(1000,
		[this, record](Animation* self) {
			int calculatedOpacity = 255;
			if (self->current >= 500) {
				//Calculate opacity
				calculatedOpacity = 255 - (((self->current - 500) % 500) * 255) / 500;
			}

			SDL_SurfaceDrawLines(MainScene::gameCanvas, record.paths.data(), record.paths.size(), 255, 0, 0, calculatedOpacity, 1);
		},
		[](Animation* self) {
		}
	);
}