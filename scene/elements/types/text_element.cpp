#include "text_element.h"

TextElement::TextElement(const char* id, vec2_t position, std::string text, const char* fontId, const int size, const SDL_Color color,
	const int outlineSize, const SDL_Color outlineColor) : Element(id, position) {
	this->fontId = fontId;
	this->size = size;
	this->color = color;
	this->outlineSize = outlineSize;
	this->outlineColor = outlineColor;
	this->text = text.c_str();

	//For calculating bound
	this->Prepare();
	this->Release();
}

void TextElement::Prepare() {
	if (this->texture != nullptr) { //?
		this->Release();
	}
	Renderer* renderer = Renderer::GetInstance();
	this->texture = renderer->CreateText(this->text.c_str(), this->fontId, this->size, this->color, this->outlineSize, this->outlineColor);

	bound.x = this->texture->width;
	bound.y = this->texture->height;
}

void TextElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y, this->alignment, this->alignmentVertical);
}

void TextElement::Release() {
	if (this->texture != nullptr) {
		Renderer* renderer = Renderer::GetInstance();
		renderer->FreeGameTexture(this->texture);
		this->texture = nullptr;
	}
}

void TextElement::SetText(const char* text) {
	this->text = std::string(text);
	Update();
}

void TextElement::SetText(std::string text) {
	this->text = text.c_str();
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
