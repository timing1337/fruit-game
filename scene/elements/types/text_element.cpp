#include "text_element.h"

TextElement::TextElement(const char* id, vec2_t position, 
	const char* text, const char* fontId, const int size, const SDL_Color color,
	const int outlineSize, const SDL_Color outlineColor): Element(id, position){
	this->text = text;
	this->fontId = fontId;
	this->size = size;
	this->color = color;
	this->outlineSize = outlineSize;
	this->outlineColor = outlineColor;
}

void TextElement::Prepare() {
	Renderer* renderer = Renderer::getInstance();
	this->texture = renderer->CreateText(this->text, this->fontId, this->size, this->color, this->outlineSize, this->outlineColor);

	bound.x = this->texture->width;
	bound.y = this->texture->height;
}

void TextElement::Render() {
	Renderer* renderer = Renderer::getInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y);
}

void TextElement::Release() {
	Renderer* renderer = Renderer::getInstance();
	renderer->FreeGameTexture(this->texture);
	this->bound = vec2_t(0, 0);
	this->texture = nullptr;
}

void TextElement::OnMouseClick(SDL_MouseButtonEvent& e) {
}
