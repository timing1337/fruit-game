#pragma once

#include "SDL.h"

#include "elements/basic_element.h"
#include "elements/types/checkbox_element.h"
#include "elements/types/button_element.h"
#include "elements/types/text_element.h"
#include "elements/types/image_element.h"
#include "elements/types/slider_element.h"
#include "scene_id.h"

#include <vector>



class BaseScene
{
public:
	SceneId sceneId;
	bool active = false;

	std::vector<Element*> elements;

	GameTexture* backgroundTexture = nullptr;

	BaseScene(SceneId sceneId);

	TextElement* AddText(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 255 });
	TextElement* AddText(const char* id, vec2_t position, std::string text, const char* fontId, const int size, const SDL_Color color, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 255 });
	ButtonElement* AddButton(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, std::function<void(ButtonElement* button)> onClick, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 255 }, const int boxPadding = 0);
	ImageElement* AddImage(const char* id, vec2_t position, const char* textureId, std::function<void(ImageElement* button)> onClick = nullptr, int boxPadding = 0, SDL_Color boxColor = {0, 0, 0, 255});
	ImageElement* AddImage(const char* id, vec2_t position, GameTexture* texture, std::function<void(ImageElement* button)> onClick = nullptr, int boxPadding = 0, SDL_Color boxColor = { 0, 0, 0, 255 });
	SliderElement* AddSlider(const char* id, vec2_t position, int height, int width, int maxSliderValue, std::function<void(SliderElement* slider)> onValueChange);
	CheckboxElement* AddCheckbox(const char* id, vec2_t position, int size, std::function<void(CheckboxElement* checkbox)> onClick);

	Element* GetElementById(const char* id);

	void SetActive(bool active);

	virtual void Prepare();
	virtual void Render();
	virtual void Release();

	void SetBackgroundTexture(GameTexture* texture);
	void OnMouseClick(SDL_MouseButtonEvent& e);
};

