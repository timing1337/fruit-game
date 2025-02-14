#pragma once

#include <unordered_map>
#include <string>

#include "SDL.h"

namespace fruit_game {
	class TextureManager
	{
	public:
		std::unordered_map<std::string, SDL_Texture*> textures;

		TextureManager(SDL_Renderer* renderer);
		~TextureManager();
	};
}

