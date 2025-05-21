#pragma once

#include "SDL.h"

#include "alignment.h"
#include "utils/math.h"

class Element
{
public:

	const char* id;

	vec2_t position;
	/* Depend on which kind of element, this will be changed on Prepare() */
	vec2_t bound;

	Alignment alignment = Alignment::LEFT;
	AlignmentVertical alignmentVertical = AlignmentVertical::TOP;

	bool active = true;

	Element(const char* id, vec2_t position) : id(id), position(position) {}

	void SetPosition(vec2_t position);
	void SetAlignment(Alignment alignment);
	void SetAlignmentVertical(AlignmentVertical alignmentVertical);
	void SetActive(bool active);

	vec2_t GetAlignedPosition();

	/* Preloading textures*/
	virtual void Prepare();
	virtual void Render();
	virtual void OnHoveredRender();
	/* Release textures */
	virtual void Release();

	virtual void OnMouseClick(SDL_MouseButtonEvent& e);

	virtual bool IsHovered();
};