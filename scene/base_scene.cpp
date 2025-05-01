#include "base_scene.h"

BaseScene::BaseScene(SceneId sceneId) : sceneId(sceneId) {}

ButtonElement* BaseScene::AddButton(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, function<void()> onClick, const int outlineSize, const SDL_Color outlineColor, const int boxPadding) {
	ButtonElement* element = new ButtonElement(id, position, text, fontId, size, color, onClick, outlineSize, outlineColor, boxPadding);
	this->elements.push_back(element);
	return element;
}

TextElement* BaseScene::AddText(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize, const SDL_Color outlineColor) {
	TextElement* element = new TextElement(id, position, text, fontId, size, color, outlineSize, outlineColor);
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