#include "game_manager.h"
#include "renderer.h"
#include <iostream>

namespace fruit_game
{
	void game_manager::Tick()
	{
		SDL_Event* e = &this->event;
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				this->running = false;
			}
		}
	}
}