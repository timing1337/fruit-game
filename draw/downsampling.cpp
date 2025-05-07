#include "downsampling.h"

SDL_Texture* Downsampling::downsampledTextures[5];

void Downsampling::Initialize() {
	Renderer* renderer = Renderer::GetInstance();

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, RENDERER_WIDTH, RENDERER_HEIGHT, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

	float steps[5] = { (float) 1/4, (float) 1/8, (float)1/16, (float)1/32, (float) 1/64};
	for (int i = 0; i < 5; i++) {
		downsampledTextures[i] = SDL_CreateTexture(renderer->gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, surface->w * steps[i], surface->h * steps[i]);
		SDL_SetTextureBlendMode(downsampledTextures[i], SDL_BLENDMODE_BLEND);
	}
}

void Downsampling::Downsample(SDL_Texture* texture, SDL_Texture* fallbackCanvas) {
	Renderer* renderer = Renderer::GetInstance();
	for (int i = 0; i < 5; i++) {
		SDL_SetRenderTarget(renderer->gRenderer, downsampledTextures[i]);
		SDL_SetRenderDrawColor(renderer->gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer->gRenderer);
		SDL_RenderCopy(renderer->gRenderer, texture, NULL, NULL);
	}
	SDL_SetRenderTarget(renderer->gRenderer, fallbackCanvas);
}

void Downsampling::Render() {
	Renderer* renderer = Renderer::GetInstance();
	SDL_Texture* final = SDL_CreateTexture(renderer->gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RENDERER_WIDTH * 1.5f, RENDERER_HEIGHT * 1.5f);
	SDL_SetTextureBlendMode(final, SDL_BLENDMODE_ADD);

	SDL_SetRenderTarget(renderer->gRenderer, final);
	SDL_SetRenderDrawColor(renderer->gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer->gRenderer);

	for (int i = 0; i < 5; i++) {
		SDL_RenderCopy(renderer->gRenderer, downsampledTextures[i], NULL, NULL);
	}

	SDL_SetRenderTarget(renderer->gRenderer, NULL);
	SDL_RenderCopy(renderer->gRenderer, final, NULL, NULL);
	SDL_DestroyTexture(final);
}

void Downsampling::Free() {
	for (int i = 0; i < 5; i++) {
		SDL_DestroyTexture(downsampledTextures[i]);
	}
}