#pragma once

#include "render.h"
#include "scene/elements/basic_element.h"

class TextElement : public Element
{
private:
	GameTexture* texture;
public:
	const char* text;
	const char* fontId;
	int size;
	SDL_Color color;
	int outlineSize;
	SDL_Color outlineColor;

	TextElement(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 255 });

	void SetText(const char* text);
	void SetFont(const char* fontId);
	void SetSize(int size);
	void SetColor(SDL_Color color);
	void SetOutlineSize(int outlineSize);
	void SetOutlineColor(SDL_Color outlineColor);

	void Update();

	void Prepare() override;
	void Render() override;
	void Release() override;
	void OnMouseClick(SDL_MouseButtonEvent& e) override;
};