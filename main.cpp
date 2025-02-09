#include "renderer.h"
#include "game_manager.h"

using namespace fruit_game;

int main(int argc, char* args[])
{
	game_manager* game = new game_manager();
	renderer* render = new renderer();

	while (game->running)
	{
		game->Tick();
		render->Render(game);
	}

	return 0;
}