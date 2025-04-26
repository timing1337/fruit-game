#pragma once

#include "SDL.h"

#include "utils/math.h"

class Element
{
public:

	const char* id;

	vec2_t position;
	/* Depend on which kind of element, this will be changed on Prepare() */
	vec2_t bound;

	bool allowHoveredRender = false;

	Element(const char* id, vec2_t position) : id(id), position(position) {}

	/* Preloading textures*/
	virtual void Prepare();
	virtual void Render();
	virtual void OnHoveredRender();
	/* Release textures */
	virtual void Release();

	virtual void OnMouseClick(SDL_MouseButtonEvent& e);

	virtual bool IsHovered();
};

