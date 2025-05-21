#include "image_element.h"

ImageElement::ImageElement(const char* id, vec2_t position, GameTexture* texture, std::function<void(ImageElement* image)> onClick, int boxPadding, SDL_Color boxColor) : Element(id, position) {
	this->texture = texture;
	this->bound.x = texture->width + boxPadding;
	this->bound.y = texture->height + boxPadding;
	this->boxPadding = boxPadding;
	this->boxColor = boxColor;
	this->onClick = onClick;
}

void ImageElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y, this->alignment, this->alignmentVertical);

	if (boxPadding == 0) return;

	int halfBoundX = this->bound.x / 2;
	int halfBoundY = this->bound.y / 2;

	vec2_t position = GetAlignedPosition();

	SDL_Rect rect = { position.x, position.y, this->bound.x, this->bound.y };
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 150);
	SDL_RenderDrawRect(renderer->gRenderer, &rect);
}


void ImageElement::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (IsHovered() && onClick != nullptr) {
		onClick(this);
	}
}

bool ImageElement::IsHovered() {
	if (this->onClick == nullptr || this->boxPadding == 0) return false;
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int halfBoundX = this->bound.x / 2;
	int halfBoundY = this->bound.y / 2;
	return mouseX >= position.x - halfBoundX && mouseX <= position.x + halfBoundX && mouseY >= position.y - halfBoundY && mouseY <= position.y + halfBoundY;
}

void ImageElement::OnHoveredRender() {
	ImageElement::Render();
}