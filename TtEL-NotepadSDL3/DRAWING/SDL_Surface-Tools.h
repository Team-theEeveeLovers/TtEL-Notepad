#pragma once
#include "../globals.h"
#include <SDL.h>

Uint32 GetSurfacePixel32(Uint32 x, Uint32 y, SDL_Surface* surface) {
	Uint32* pixels = static_cast<Uint32*>(surface->pixels);
	Uint32 pitch = surface->pitch / 4;
	return pixels[(y * pitch) + x];
}