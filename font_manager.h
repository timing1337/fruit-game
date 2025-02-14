#pragma once

#include <unordered_map>
#include <string>

#include "SDL_ttf.h"

namespace fruit_game {
	class FontManager
	{
	public:
		std::unordered_map<std::string, TTF_Font*> fonts;

		FontManager();
		~FontManager();
	};
}