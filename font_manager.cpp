#include <filesystem>

#include "font_manager.h"

namespace fs = std::filesystem;

namespace fruit_game {
	FontManager::FontManager() {
		for (const auto& entry : fs::recursive_directory_iterator(fs::current_path() / "game_data" / "fonts")) {
			fs::path dataPath = entry.path();

			if (!fs::is_regular_file(dataPath)) {
				continue;
			}

			auto fontId = dataPath.filename().replace_extension().string();

			fonts[fontId] = TTF_OpenFont(dataPath.string().c_str(), 1);

			SDL_Log("Loading font: %s", fontId.c_str());
		}
	}

	FontManager::~FontManager() {
		SDL_Log("Destroying fonts");
		for (auto& font : fonts) {
			TTF_CloseFont(font.second);
		}
	}
}
