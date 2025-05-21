#pragma once

#include "audio/audio_mgr.h"
#include "render.h"
#include "scene/elements/basic_element.h"

class CheckboxElement : public Element
{
public:

	GameTexture* texture = nullptr;
	bool value = false;
	std::function<void(CheckboxElement* checkbox)> onClick;

	CheckboxElement(const char* id, vec2_t position, int boxSize, std::function<void(CheckboxElement* checkbox)> onClick) : Element(id, position)
	{
		this->bound.x = boxSize;
		this->bound.y = boxSize;
		this->onClick = onClick;
	}

	void Render() override;
	void OnHoveredRender() override;
	void OnMouseClick(SDL_MouseButtonEvent& e) override;
	bool IsHovered() override;

};
