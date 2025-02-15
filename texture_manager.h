#pragma once

#include <unordered_map>
#include <string>

#include "SDL.h"

namespace fruit_game {
	struct TextureData {
		SDL_Texture* texture;
		int width;
		int height;
	};

	class TextureManager
	{
	public:
		std::unordered_map<std::string, TextureData*> textures;

		TextureManager(SDL_Renderer* renderer);
		~TextureManager();
	};
}

