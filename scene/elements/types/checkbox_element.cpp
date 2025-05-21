#include "checkbox_element.h"

void CheckboxElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	int x = this->position.x - (this->bound.x / 2);
	int y = this->position.y - (this->bound.y / 2);

	int endX = this->position.x + (this->bound.x / 2);
	int endY = this->position.y + (this->bound.y / 2);
	SDL_Rect rect = { x, y, this->bound.x, this->bound.y };
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer->gRenderer, &rect);

	if (this->value) {
		SDL_RenderDrawLine(renderer->gRenderer, endX, y, x, endY);
		SDL_RenderDrawLine(renderer->gRenderer, x, y, endX, endY);
	}
}

void CheckboxElement::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (IsHovered()) {
		this->value = !this->value;
		if (this->onClick != nullptr) {
			AudioManager::GetInstance()->PlaySound("button.wav");
			this->onClick(this);
		}
	}
}

bool CheckboxElement::IsHovered() {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int halfBoundX = this->bound.x / 2;
	int halfBoundY = this->bound.y / 2;
	return mouseX >= position.x - halfBoundX && mouseX <= position.x + halfBoundX && mouseY >= position.y - halfBoundY && mouseY <= position.y + halfBoundY;
}

void CheckboxElement::OnHoveredRender() {
	CheckboxElement::Render();
}