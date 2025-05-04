#pragma once

#include "SDL.h"

#include "render.h"
#include "scene/elements/basic_element.h"

class ImageElement : public Element
{
private:
	GameTexture* texture;
public:

	ImageElement(const char* id, vec2_t position, GameTexture* texture);

	void Render() override;
};

