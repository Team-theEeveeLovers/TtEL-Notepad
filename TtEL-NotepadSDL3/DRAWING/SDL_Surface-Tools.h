#pragma once
#include "../globals.h"
#include <SDL.h>
/**
* @file SDL_Surface-Tools.h
* @brief A header file of the TtEL SDL3 GUI Notepad, containing functions for dealing with SDL_Surface types.
* @author Team theEeveeLovers Sean
* @date June 8th, 2024
*/

Uint32 GetSurfacePixel32(Uint32 x, Uint32 y, SDL_Surface* surface) {
	Uint32* pixels = static_cast<Uint32*>(surface->pixels);
	Uint32 pitch = surface->pitch / 4;
	return pixels[(y * pitch) + x];
}