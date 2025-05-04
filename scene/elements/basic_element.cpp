#include "basic_element.h"

void Element::Prepare() {}

void Element::Render() {}
void Element::OnHoveredRender(){}

void Element::Release() {}

void Element::OnMouseClick(SDL_MouseButtonEvent& e) {}

bool Element::IsHovered() {
	return false;
}

void Element::SetPosition(vec2_t position) {
	this->position = position;
}

void Element::SetAlignment(Alignment alignment) {
	this->alignment = alignment;
}

void Element::SetAlignmentVertical(AlignmentVertical alignmentVertical) {
	this->alignmentVertical = alignmentVertical;
}

void Element::SetActive(bool active) {
	if (active) {
		if (!this->active) this->Prepare();
	}
	else {
		if (this->active) this->Release();
	}
	this->active = active;
}