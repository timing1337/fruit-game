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

			fonts[dataPath.filename().replace_extension().string()] = TTF_OpenFont(dataPath.string().c_str(), 24);

			SDL_Log("Loading font: %s", dataPath.filename().replace_extension().string().c_str());
		}
	}

	FontManager::~FontManager() {

	}
}
