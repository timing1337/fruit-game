#pragma once

#include "render.h"
#include "scene/elements/basic_element.h"

class SliderElement : public Element
{
public:
	int currentValue = 0;
	int maxValue;
	std::function<void(SliderElement* slider)> onValueChange;

	SliderElement(const char* id, vec2_t position, int height, int width, int maxSliderValue, std::function<void(SliderElement* slider)> onValueChange) : Element(id, position)
	{
		this->bound.x = width;
		this->bound.y = height;
		this->maxValue = maxSliderValue;
		this->currentValue = maxSliderValue;
		this->onValueChange = onValueChange;
	}

	void Update(int value);
	void Render() override;
	void OnMouseClick(SDL_MouseButtonEvent& e) override;
	bool IsHovered() override;
	void OnHoveredRender() override;
};

