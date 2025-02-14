#include "menu.h"

using namespace fruit_game;

namespace fruit_ui {

	void menu::Render(Renderer* renderer)
	{
		SDL_RenderCopy(renderer->gRenderer, renderer->texture_mgr->textures["fruits/peach"], NULL, NULL);
	}
}	

