#include "renderer.h"
#include "game_manager.h"
#include "texture_manager.h"

using namespace fruit_game;

int main(int argc, char* args[])
{
	GameManager* game = new GameManager();
	Renderer* render = new Renderer();

	while (game->running)
	{
		game->Tick();
		render->Render(game);
	}

	return 0;
}