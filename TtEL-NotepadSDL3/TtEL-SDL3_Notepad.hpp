#pragma once
#include <iostream>
using namespace std;
#include <SDL.h>

// If the person is using an older version of SDL3
#ifndef SDL_GetRendererName()

// emulate the API function
/**
 * Get the name of a renderer.
 *
 * The returned string follows the SDL_GetStringRule.
 * 
 * @deprecated THIS IS NOT AN API CALL, IT IS A FUNCTIONAL EMULATION OF ONE FROM A NEWER VERSION.
 * PLEASE UPDATE YOUR SDL3 VERSION.
 *
 * @param renderer the rendering context.
 * @returns the name of the selected renderer, or NULL if the renderer is
 *          invalid.
 *
 * @since This function is available since SDL 3.0.0.
 *
 * @sa SDL_CreateRenderer
 * @sa SDL_CreateRendererWithProperties
 */
const char *SDL_GetRendererName(SDL_Renderer *renderer) {
	SDL_RendererInfo renderer_info;
	if (SDL_GetRendererInfo(renderer, &renderer_info) < 0)
		return NULL;
	else
		return renderer_info.name;
}
#endif
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>


#include "TtEL-SDL3_Notepad-MATH.h"
// Wrapper for mathext
namespace ME = mathext;

#include "DRAWING/TtEL-SDL3_Notepad-DRAW.h"
// Wrapper for RectDraw
namespace RD = RectDraw;

#include "TtEL-SDL3_Notepad-LOGIC.h"

#include "globals.h"

#include "FILESYS/TtEL-SDL3_Notepad-FILELOADING.h"

extern SDL_Renderer* main_renderer;

extern int scr_wid;
extern int scr_hei;
extern float scr_floatwid;
extern float scr_floathei; 


#ifdef DRAW_DEBUG
extern bool DRAW_DBG;
#endif

extern SDL_Texture* textTextures[256];
extern vector2_int textTextureSizeVectors[256];
extern vector2_int textTextureSourceVectors[256];

character loadCharFromChar(char* chara, vector2_float position = vector2_float()) {
	character tempChar;
	tempChar.loadChar(chara);
	if (position.x != 0.f) {
		tempChar.x = position.x;
	}
	if (position.y != 0.f) {
		tempChar.y = position.y;
	}
	return tempChar;
	tempChar.freeCharacter();
}

#include "TtEL-SDL3_Notepad-KeyHandler.h"