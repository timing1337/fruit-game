#include "basic_element.h"

void Element::Prepare() {}

void Element::Render() {}
void Element::OnHoveredRender() {}

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

vec2_t Element::GetAlignedPosition() {
	float x = this->position.x;
	float y = this->position.y;

	switch (alignmentVertical) {
	case AlignmentVertical::TOP:
		break;
	case AlignmentVertical::MIDDLE:
		y -= this->bound.y / 2;
		break;
	case AlignmentVertical::BOTTOM:
		y -= this->bound.y;
		break;
	}

	switch (alignment) {
	case Alignment::LEFT:
		break;
	case Alignment::RIGHT:
		x -= this->bound.x;
		break;
	case Alignment::CENTER:
		x -= this->bound.x / 2;
		break;
	}

	return vec2_t{ x, y };
}