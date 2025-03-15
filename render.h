#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <filesystem>
#include <string>
#include <unordered_map>

#include "animation/animation.h"
#include "game.h"
#include "textures.h"

#include "ui/menu.h";
#include "ui/main_scene.h";

using namespace std;

namespace fs = std::filesystem;

enum Alignment{
	LEFT,
	CENTER,
	RIGHT
};

class Renderer {
private:
	static Renderer* instancePtr;
public:

	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;

	unordered_map<string, TTF_Font*> fonts;
	unordered_map<string, GameTexture*> textures;

	const int width = 1440;
	const int height = 810;

	const string game_data_path = fs::current_path().string() + "/game_data/";

	static void Initialize();

	static Renderer* getInstance() {
		return instancePtr;
	}

	void PreRender();
	void Render();
	void UpdateRender();

	void OnMouseClick(SDL_MouseButtonEvent& e);

	void OnStarting();
	void PlayTitleAnimation();

	bool LoadFontByName(const char* name);
	bool LoadTextureByName(const char* name);

	TTF_Font* GetFontByName(const char* name);
	GameTexture* GetTextureByName(const char* name);

	GameTexture* CreateText(const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 0xff });

	void RenderTextureBackground(const char* textureId);
	void SetBackgroundColor(const int r, const int g, const int b, const int a);

	void RenderText(const char* textureId, int x, int y, const Alignment align = Alignment::CENTER);
	void RenderText(GameTexture* texture, int x, int y, const Alignment align = Alignment::CENTER);
};