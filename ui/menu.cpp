#include "menu.h"

using namespace fruit_game;

namespace fruit_ui {

	void menu::Initialize(Renderer* renderer)
	{
		TextureManager* texture_mgr = renderer->texture_mgr;

		GameTexture* mainTitle = renderer->CreateOutlineText("CAT FRUIT", "NoyhR-Black", 40, SDL_Color{ 255, 255, 255, 255 });
		texture_mgr->textures["ui/main_title_text"] = mainTitle;
	}

	void menu::Render(Renderer* renderer)
	{
		GameTexture* mainTitle = renderer->texture_mgr->textures["ui/main_title_text"];

		const int halfWidth = renderer->width / 2;
		const int halfHeight = renderer->height / 2;

		const int halfTitleWidth = mainTitle->text->width / 2;

		renderer->SetBackgroundColor(255, 236, 179, 255);
		renderer->RenderText("ui/main_title_text", halfWidth - halfTitleWidth, 50);
	}
}	

