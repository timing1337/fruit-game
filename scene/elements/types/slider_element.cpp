#include "slider_element.h"

void SliderElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	int x = this->position.x;
	int y = this->position.y - (this->bound.y / 2);

	int percentage = ((float)this->currentValue / this->maxValue) * (this->bound.x);

	SDL_Rect rect = { x, y, percentage, this->bound.y };
	SDL_Rect fullRect = { x, y, this->bound.x, this->bound.y };
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer->gRenderer, &fullRect);
	SDL_RenderFillRect(renderer->gRenderer, &rect);
}

void SliderElement::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (IsHovered()) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		int value = ((mouseX - this->position.x) / this->bound.x) * this->maxValue;
		Update(value);
	}
}

void SliderElement::Update(int value) {
	if (value < 0) {
		return;
	}

	if (value > this->maxValue) {
		return;
	}

	this->currentValue = value;
	if (this->onValueChange != nullptr) {
		this->onValueChange(this);
	}
}

bool SliderElement::IsHovered() {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return mouseX >= position.x && mouseX <= position.x + this->bound.x && mouseY >= position.y && mouseY <= position.y + this->bound.y;
}

void SliderElement::OnHoveredRender() {
	SliderElement::Render();
}