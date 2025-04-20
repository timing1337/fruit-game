#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "game.h"
#include "draw/surface_draw.h"
#include "mouse_path.h"
#include "task/task.h"
#include "task/types/timer_task.h"
#include "textures.h"

#include "ui/menu.h"
#include "ui/main_scene.h"
#include "ui/death_scene.h"

#include <functional>
#include <filesystem>
#include <string>
#include <unordered_map>

using namespace std;
namespace fs = std::filesystem;

enum Alignment{
	LEFT,
	CENTER,
	RIGHT
};

class BaseTask;
enum GameState;

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

	void Initialize();

	static Renderer* getInstance() {
		return instancePtr;
	}

	void PreRender();
	void Render();
	void UpdateRender();

	void OnMouseClick(SDL_MouseButtonEvent& e);
	void OnMousePathRecorded(MousePathRecord record);

	void PlayFadeTransition(function<void(TimerTask* self)> onTransitioned, function<void(TimerTask* self)> onComplete);
	void PlayTitleAnimationAndStartGame();

	bool LoadFontByName(const char* name);
	bool LoadTextureByName(const char* name);

	void FreeGameTexture(GameTexture* texture);

	TTF_Font* GetFontByName(const char* name);
	GameTexture* GetTextureByName(const char* name);

	GameTexture* CreateText(const char* text, const char* fontId, const int size, const SDL_Color color, const int outlineSize = 0, const SDL_Color outlineColor = { 0, 0, 0, 0xff });

	void RenderTextureBackground(const char* textureId);
	void SetBackgroundColor(const int r, const int g, const int b, const int a);

	void RenderTexture(const char* textureId, int x, int y, const Alignment align = Alignment::CENTER);
	void RenderTexture(GameTexture* texture, int x, int y, const Alignment align = Alignment::CENTER);
};