#include "text_element.h"

TextElement::TextElement(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color,
	const int outlineSize, const SDL_Color outlineColor): Element(id, position){
	this->fontId = fontId;
	this->size = size;
	this->color = color;
	this->outlineSize = outlineSize;
	this->outlineColor = outlineColor;
	this->text = new char[strlen(text) + 1];
	strcpy_s(this->text, strlen(text) + 1, text);
}

void TextElement::Prepare() {
	Renderer* renderer = Renderer::GetInstance();
	SDL_Log(this->id);
	SDL_Log("prepare");
	SDL_Log(this->text);
	this->texture = renderer->CreateText(this->text, this->fontId, this->size, this->color, this->outlineSize, this->outlineColor);

	bound.x = this->texture->width;
	bound.y = this->texture->height;
}

void TextElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y);
}

void TextElement::Release() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->FreeGameTexture(this->texture);
	this->bound = vec2_t(0, 0);
	this->texture = nullptr;
}

void TextElement::SetText(char* text) {
	this->text = text;
	Update();
}

void TextElement::SetText(const char* text) {
	if (this->text != nullptr) {
		delete[] this->text;
	}

	this->text = new char[strlen(text) + 1];
	strcpy_s(this->text, strlen(text) + 1, text);

	Update();
}

void TextElement::SetFont(const char* fontId) {
	this->fontId = fontId;
	Update();
}

void TextElement::SetSize(const int size) {
	this->size = size;
	Update();
}

void TextElement::SetColor(const SDL_Color color) {
	this->color = color;
	Update();
}

void TextElement::SetOutlineSize(const int outlineSize) {
	this->outlineSize = outlineSize;
	Update();
}

void TextElement::SetOutlineColor(const SDL_Color outlineColor) {
	this->outlineColor = outlineColor;
	Update();
}

void TextElement::Update() {
	if (this->active) {
		TextElement::Release();
		TextElement::Prepare();
	}
}

void TextElement::OnMouseClick(SDL_MouseButtonEvent& e) {
}
