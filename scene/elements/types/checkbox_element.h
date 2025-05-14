#pragma once

#include "scene/elements/basic_element.h"

class CheckboxElement : public Element
{
public:

	GameTexture* texture = nullptr;
	bool isActive = false;

	CheckboxElement(const char* id, vec2_t position, int boxSize) : Element(id, position)
	{

	}
};

