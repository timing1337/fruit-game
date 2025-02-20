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

			SDL_Texture* texture = IMG_LoadTexture(renderer, dataPath.string().c_str());

			Sprite* sprite = new Sprite();
			sprite->texture = texture;

			SDL_QueryTexture(texture, NULL, NULL, &sprite->width, &sprite->height);

			GameTexture* textureData = new GameTexture();
			textureData->type = GameTextureType::SPRITE;
			textureData->sprite = sprite;

			textures[textureId] = textureData;
		}
	}

	TextureManager::~TextureManager()
	{
		SDL_Log("Destroying textures");
		for (auto& texture : textures) {
			GameTexture* textureData = texture.second;
			switch (textureData->type)
			{
				case GameTextureType::SPRITE:
					SDL_DestroyTexture(textureData->sprite->texture);
					delete textureData->sprite;
				break;
				case GameTextureType::TEXT:
					SDL_DestroyTexture(textureData->text->text);
					if (textureData->text->outline > 0) {
						SDL_DestroyTexture(textureData->text->outlineText);
					}
					delete textureData->text;
			default:
				break;
			}
			delete textureData;
		}
	}
}