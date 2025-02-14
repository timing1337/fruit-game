#include <filesystem>
#include "SDL_image.h"
#include "texture_manager.h"

namespace fs = std::filesystem;

namespace fruit_game {
	TextureManager::TextureManager(SDL_Renderer* renderer)
	{
		for (const auto& entry : fs::recursive_directory_iterator(fs::current_path() / "game_data" / "textures")) {
			fs::path dataPath = entry.path();

			if (!fs::is_regular_file(dataPath)) {
				continue;
			}

			std::string textureId = dataPath.filename().replace_extension().string();

			fs::path curFolder = dataPath.parent_path();

			while (curFolder.filename() != "textures") {
				textureId = curFolder.filename().string() + "/" + textureId;
				curFolder = curFolder.parent_path();
			}

			SDL_Log("Loading texture: %s", textureId.c_str());

			textures[textureId] = IMG_LoadTexture(renderer, dataPath.string().c_str());
		}
	}

	TextureManager::~TextureManager()
	{
		for (auto& texture : textures) {
			SDL_DestroyTexture(texture.second);
		}
	}
}