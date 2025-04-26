#include "basic_element.h"

void Element::Prepare() {}

void Element::Render() {}
void Element::OnHoveredRender(){}

void Element::Release() {}

void Element::OnMouseClick(SDL_MouseButtonEvent& e) {}

bool Element::IsHovered() {
	return false;
}