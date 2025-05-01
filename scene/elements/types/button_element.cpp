#include "button_element.h"

ButtonElement::ButtonElement(const char* id, vec2_t position, 
	const char* text, const char* fontId, const int size, const SDL_Color color, function<void()> onClick,
	const int outlineSize, const SDL_Color outlineColor, const int boxPadding): Element(id, position){
	this->text = text;
	this->fontId = fontId;
	this->size = size;
	this->color = color;
	this->onClick = onClick;
	this->outlineSize = outlineSize;
	this->outlineColor = outlineColor;
	this->boxPadding = boxPadding;
}

void ButtonElement::Prepare() {
	Renderer* renderer = Renderer::GetInstance();
	this->texture = renderer->CreateText(this->text, this->fontId, this->size, this->color, this->outlineSize, this->outlineColor);

	bound.x = this->texture->width + boxPadding * 2;
	bound.y = this->texture->height + boxPadding * 2;
}

void ButtonElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y);
}

void ButtonElement::OnHoveredRender() {
	Renderer* renderer = Renderer::GetInstance();
	SDL_SetTextureAlphaMod(this->texture->text->text, 200);
	renderer->RenderTexture(this->texture, this->position.x, this->position.y);
	SDL_SetTextureAlphaMod(this->texture->text->text, 255);

	int halfBoundX = this->bound.x / 2;
	int halfBoundY = this->bound.y / 2;

	SDL_Rect rect = { (int)(this->position.x - halfBoundX), (int)(this->position.y - halfBoundY), this->bound.x, this->bound.y };
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 200);
	SDL_RenderDrawRect(renderer->gRenderer, &rect);
}

void ButtonElement::Release() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->FreeGameTexture(this->texture);
	this->bound = vec2_t(0, 0);
	this->texture = nullptr;
}

void ButtonElement::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (IsHovered() && onClick != nullptr) onClick();
}

bool ButtonElement::IsHovered() {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int halfBoundX = this->bound.x / 2;
	int halfBoundY = this->bound.y / 2;
	return mouseX >= position.x - halfBoundX && mouseX <= position.x + halfBoundX && mouseY >= position.y - halfBoundY && mouseY <= position.y + halfBoundY;
}