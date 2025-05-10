#include "button_element.h"

ButtonElement::ButtonElement(const char* id, vec2_t position, 
	const char* text, const char* fontId, const int size, const SDL_Color color, function<void(ButtonElement* button)> onClick,
	const int outlineSize, const SDL_Color outlineColor, const int boxPadding): Element(id, position){
	this->text = text;
	this->fontId = fontId;
	this->size = size;
	this->color = color;
	this->onClick = onClick;
	this->outlineSize = outlineSize;
	this->outlineColor = outlineColor;
	this->boxPadding = boxPadding;

	//For calculating bound
	this->Prepare();
	this->Release();

	this->alignment = Alignment::CENTER;
	this->alignmentVertical = AlignmentVertical::MIDDLE;
}

void ButtonElement::Prepare() {
	Renderer* renderer = Renderer::GetInstance();
	this->texture = renderer->CreateText(this->text, this->fontId, this->size, this->color, this->outlineSize, this->outlineColor);

	bound.x = this->texture->width + boxPadding * 2;
	bound.y = this->texture->height + boxPadding * 2;
}

void ButtonElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y, this->alignment, this->alignmentVertical);
}

void ButtonElement::OnHoveredRender() {
	Renderer* renderer = Renderer::GetInstance();
	SDL_SetTextureAlphaMod(this->texture->text->text, 200);
	renderer->RenderTexture(this->texture, this->position.x, this->position.y, this->alignment, this->alignmentVertical);
	SDL_SetTextureAlphaMod(this->texture->text->text, 255);

	int halfBoundX = this->bound.x / 2;
	int halfBoundY = this->bound.y / 2;

	int boxStartX = this->position.x;
	int boxStartY = this->position.y;

	switch (alignmentVertical) {
	case AlignmentVertical::TOP:
		break;
	case AlignmentVertical::MIDDLE:
		boxStartY -= this->bound.y / 2;
		break;
	case AlignmentVertical::BOTTOM:
		boxStartY -= this->bound.y;
		break;
	}

	switch (this->alignment) {
	case Alignment::LEFT:
		break;
	case Alignment::RIGHT:
		boxStartX -= this->bound.x;
		break;
	case Alignment::CENTER:
		boxStartX -= this->bound.x / 2;
		break;
	}

	SDL_Rect rect = { boxStartX, boxStartY, this->bound.x, this->bound.y };
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 150);
	SDL_RenderDrawRect(renderer->gRenderer, &rect);
}

void ButtonElement::Release() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->FreeGameTexture(this->texture);
	this->texture = nullptr;
}

void ButtonElement::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (IsHovered() && onClick != nullptr && !isClicked) {
		isClicked = true;
		onClick(this);
	}
}

bool ButtonElement::IsHovered() {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int halfBoundX = this->bound.x / 2;
	int halfBoundY = this->bound.y / 2;
	return mouseX >= position.x - halfBoundX && mouseX <= position.x + halfBoundX && mouseY >= position.y - halfBoundY && mouseY <= position.y + halfBoundY;
}