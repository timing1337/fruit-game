#include "base_scene.h"

BaseScene::BaseScene(SceneId sceneId) : sceneId(sceneId) {}

Element* BaseScene::AddButton(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, function<void()> onClick, const int outlineSize, const SDL_Color outlineColor, const int boxPadding) {
	Element* element = new ButtonElement(id, position, text, fontId, size, color, onClick, outlineSize, outlineColor, boxPadding);
	this->elements.push_back(element);
	return element;
}

Element* BaseScene::AddText(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize, const SDL_Color outlineColor) {
	Element* element = new TextElement(id, position, text, fontId, size, color, outlineSize, outlineColor);
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
		element->Prepare();
	}
}

void BaseScene::Render() {
	for (auto& element : this->elements) {
		if (element->IsHovered()) {
			element->OnHoveredRender();
		}
		else {
			element->Render();
		}
	}
}

void BaseScene::Release() {
	for (auto& element : this->elements) {
		element->Release();
	}
}

void BaseScene::SetActive(bool active) {
	this->isActive = active;
	if (active) {
		this->Prepare();
	}
	else {
		this->Release();
	}
}

void BaseScene::OnMouseClick(SDL_MouseButtonEvent& e) {
	for (auto& element : this->elements) {
		if (element->IsHovered()) {
			element->OnMouseClick(e);
		}
	}
}