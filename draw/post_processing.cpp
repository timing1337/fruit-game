#include "post_processing.h"

void SDL_PostGaussianBlur(SDL_Surface* surface, float radius) {
	SDL_LockSurface(surface);

	int pitch = surface->pitch / sizeof(unsigned int);

	Uint32* pixels = (Uint32*)surface->pixels;

	float sigma = std::max(radius * 0.5f, 1.0f);
	int kernelWidth = (radius * 2) + 1;
	float sum = 0;

	vector<vector<float>> kernel(kernelWidth, vector<float>(kernelWidth));

	for (int x = -radius; x <= radius; x++)
	{
		for (int y = -radius; y <= radius; y++)
		{
			float exponentNumerator = (float)(-(x * x + y * y));
			float exponentDenominator = (2.0f * sigma * sigma);
			float eExpression = pow(M_E, exponentNumerator / exponentDenominator);
			float kernelValue = (eExpression / (2 * M_PI * sigma * sigma));

			kernel[x + radius][y + radius] = kernelValue;
			sum += kernelValue;
		}
	}

	//Normalize
	for (int x = 0; x < kernelWidth; x++) {
		for (int y = 0; y < kernelWidth; y++) {
			kernel[x][y] /= sum;
		}
	}

	for (int x = radius; x < surface->w - radius; x++) {
		for (int y = radius; y < surface->h - radius; y++) {
			float r = 0.0f, g = 0.0f, b = 0.0f;
			for (int kernelX = -radius; kernelX <= radius; kernelX++)
			{
				for (int kernelY = -radius; kernelY <= radius; kernelY++)
				{
					float kernelValue = kernel[kernelX + radius][kernelY + radius];

					int pixelPosition = (y - kernelY) * pitch + (x - kernelX);

					Uint8 currentR, currentG, currentB;
					SDL_GetRGB(pixels[pixelPosition], surface->format, &currentR, &currentG, &currentB);

					r += currentR * kernelValue;
					g += currentG * kernelValue;
					b += currentB * kernelValue;
				}
			}

			int pixelPosition = y * pitch + x;

			Uint8 finalR = static_cast<Uint8>(std::min(std::max(r, 0.0f), 255.0f));
			Uint8 finalG = static_cast<Uint8>(std::min(std::max(g, 0.0f), 255.0f));
			Uint8 finalB = static_cast<Uint8>(std::min(std::max(b, 0.0f), 255.0f));

			//Blend with the original pixel
			Uint8 originalR, originalG, originalB;
			SDL_GetRGB(pixels[pixelPosition], surface->format, &originalR, &originalG, &originalB);

			finalR = (finalR + originalR) / 2;
			finalG = (finalG + originalG) / 2;
			finalB = (finalB + originalB) / 2;

			pixels[pixelPosition] = SDL_MapRGBA(surface->format, finalR, finalG, finalB, 255);
		}
	}

	SDL_UnlockSurface(surface);
}

void SDL_PostBrighten(SDL_Surface* surface) {
	SDL_LockSurface(surface);
	int pitch = surface->pitch / sizeof(unsigned int);
	Uint32* pixels = (Uint32*)surface->pixels;
	for (int x = 0; x < surface->w; x++) {
		for (int y = 0; y < surface->h; y++) {
			int pixelPosition = y * pitch + x;
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixels[pixelPosition], surface->format, &r, &g, &b, &a);

			float rF = r / 255.0f;
			float gF = g / 255.0f;
			float bF = b / 255.0f;

			float brightness = rF * 0.2126f + gF * 0.7152f + bF * 0.0722f;

			float brightR, brightG, brightB;

			if (brightness > 1.0f) {
				brightR = r;
				brightG = g;
				brightB = b;
			}
			else {
				brightR = 0;
				brightG = 0;
				brightB = 0;
			}
		}
	}
	SDL_UnlockSurface(surface);
}