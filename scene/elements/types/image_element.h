#pragma once

#include "SDL.h"

#include "render.h"
#include "scene/elements/basic_element.h"

class ImageElement : public Element
{
private:
	GameTexture* texture = nullptr;
	int boxPadding;
	SDL_Color boxColor;
	std::function<void(ImageElement* image)> onClick = nullptr;
public:
	ImageElement(const char* id, vec2_t position, GameTexture* texture, std::function<void(ImageElement* image)> onClick = nullptr, int boxPadding = 0, SDL_Color boxColor = {0, 0, 0, 255});
	void Render() override;
	void OnMouseClick(SDL_MouseButtonEvent& e) override;
	bool IsHovered() override;
	void OnHoveredRender() override;
};

