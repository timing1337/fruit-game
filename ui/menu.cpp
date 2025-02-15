#include "menu.h"

using namespace fruit_game;

namespace fruit_ui {

	void menu::Initialize(Renderer* renderer)
	{
		TextureManager* texture_mgr = renderer->texture_mgr;

		TextureData* mainTitle = renderer->CreateText("CAT FRUIT", "Helvetica-Bold", 50, 255, 255, 255, 255);
		texture_mgr->textures["ui/main_title_text"] = mainTitle;
	}

	void menu::Render(Renderer* renderer)
	{
		TextureData* mainTitle = renderer->texture_mgr->textures["ui/main_title_text"];

		const int halfWidth = renderer->width / 2;
		const int halfHeight = renderer->height / 2;

		const int halfTitleWidth = mainTitle->width / 2;

		renderer->RenderTexture("ui/main_title_text", halfWidth - halfTitleWidth, 150);
	}
}	

