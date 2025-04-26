#pragma once

#include "SDL.h"

#include "elements/basic_element.h"
#include "scene/elements/types/button_element.h"
#include "elements/types/text_element.h"
#include "scene_id.h"

#include <vector>

using namespace std;

class BaseScene
{
public:
	SceneId sceneId;
	/* Used for scene like pause, acting as an overlay over the main game scene */
	bool isActive = false;

	vector<Element*> elements;

	BaseScene(SceneId sceneId);

	void AddElement(Element* element);

	Element* AddText(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 255 });
	Element* AddButton(const char* id, vec2_t position, const char* text, const char* fontId, const int size, const SDL_Color color, function<void()> onClick, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 255 }, const int boxPadding = 0);

	Element* GetElementById(const char* id);

	void SetActive(bool active);

	virtual void Prepare();
	virtual void Render();
	virtual void Release();
	void OnMouseClick(SDL_MouseButtonEvent& e);
};

