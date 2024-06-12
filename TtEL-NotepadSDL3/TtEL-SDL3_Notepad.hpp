#pragma once
#include <iostream>
using namespace std;
#include <SDL.h>
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