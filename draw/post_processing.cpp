#include "post_processing.h"

Kernel* kernel = new Kernel(3);

SDL_Surface* SDL_Downsampling(SDL_Surface* surface, float size) {
	SDL_Renderer* gRenderer = Renderer::getInstance()->gRenderer;
	int w, h;
	SDL_Surface* downsampled = SDL_CreateRGBSurfaceWithFormat(0, surface->w * size, surface->h * size, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_SetSurfaceBlendMode(downsampled, SDL_BLENDMODE_BLEND);
	SDL_BlitScaled(surface, NULL, downsampled, NULL);
	return downsampled;
}