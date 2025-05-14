#include "base_scene.h"

BaseScene::BaseScene(SceneId sceneId) : sceneId(sceneId) {}

ButtonElement* BaseScene::AddButton(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, function<void(ButtonElement* button)> onClick, const int outlineSize, const SDL_Color outlineColor, const int boxPadding) {
	ButtonElement* element = new ButtonElement(id, position, text, fontId, size, color, onClick, outlineSize, outlineColor, boxPadding);
	this->elements.push_back(element);
	return element;
}

TextElement* BaseScene::AddText(const char* id, vec2_t position, string text, const char* fontId, const int size, const SDL_Color color, const int outlineSize, const SDL_Color outlineColor) {
	TextElement* element = new TextElement(id, position, text, fontId, size, color, outlineSize, outlineColor);
	this->elements.push_back(element);
	return element;
}

TextElement* BaseScene::AddText(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize, const SDL_Color outlineColor) {
	TextElement* element = new TextElement(id, position, text, fontId, size, color, outlineSize, outlineColor);
	this->elements.push_back(element);
	return element;
}

ImageElement* BaseScene::AddImage(const char* id, vec2_t position, const char* textureId, function<void(ImageElement* button)> onClick, int boxPadding, SDL_Color boxColor) {
	GameTexture* texture = Renderer::GetInstance()->GetTextureByName(textureId);
	if (texture == nullptr) {
		printf("Error: Texture with id %s not found\n", textureId);
		return nullptr;
	}
	return BaseScene::AddImage(id, position, texture, onClick, boxPadding, boxColor);
}


ImageElement* BaseScene::AddImage(const char* id, vec2_t position, GameTexture* texture, function<void(ImageElement* button)> onClick, int boxPadding, SDL_Color boxColor) {
	ImageElement* element = new ImageElement(id, position, texture, onClick, boxPadding, boxColor);
	this->elements.push_back(element);
	return element;
}

Element* BaseScene::GetElementById(const char* id) {
	for (auto& element : this->elements) {
		if (strcmp(element->id, id) == 0) {
			return element;
		}
	}
	return nullptr;
}

void BaseScene::Prepare() {
	for (auto& element : this->elements) {
		if (element->active) {
			element->Prepare();
		}
	}
}

void BaseScene::Render() {
	Renderer* renderer = Renderer::GetInstance();

	if (this->backgroundTexture != nullptr) {
		SDL_RenderCopy(renderer->gRenderer, this->backgroundTexture->sprite->texture, NULL, NULL);
	}

	for (auto& element : this->elements) {
		if (element->active) {
			if (element->IsHovered()) {
				element->OnHoveredRender();
			}
			else {
				element->Render();
			}
		}
	}
}

void BaseScene::Release() {
	for (auto& element : this->elements) {
		if (element->active) {
			element->Release();
		}
	}
}

void BaseScene::SetBackgroundTexture(GameTexture* texture) {
	this->backgroundTexture = texture;
}

void BaseScene::SetActive(bool active) {
	if (active) {
		if(!this->active) this->Prepare();
	}
	else {
		if (this->active) this->Release();
	}
	this->active = active;
}

void BaseScene::OnMouseClick(SDL_MouseButtonEvent& e) {
	for (auto& element : this->elements) {
		if (element->IsHovered()) {
			element->OnMouseClick(e);
		}
	}
}