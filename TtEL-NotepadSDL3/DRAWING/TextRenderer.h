#ifndef TEXT_RENDERER
#define TEXT_RENDERER
#include "TtEL-SDL3_Notepad-DRAW.h"
extern int scr_wid;
extern int scr_hei;
extern float scr_floatwid;
extern float scr_floathei;
extern SDL_Renderer* main_renderer;
extern SDL_Texture* textTextures[256];
#include "VectorTypes.h"
extern vector2_int textTextureSizeVectors[256];
extern vector2_int textTextureSourceVectors[256];
#include "../globals.h"

class character {
private:
	// This is a magic number, it uses unused space in memory
	char magic[2] = { 'N', 'T' };
	// This is a magic number, it uses unused space in memory
	char magic2[4] = { 'C', 'h', 'a', 'r'};
	// This is padding between the magic number and the letter contained by the variable, it also uses unused memory space
	char padding[1] = { '\x20' };
	// The unused space is caused by the letter variable
public:
	// This is the letter contained by the character
	char letter[1] = "";
	// This is the texture of the letter
	SDL_Texture* letter_texture = NULL;
	// This is the x position of the character
	float x = 0.f;
	// This is the y position of the character
	float y = 0.f;
	// This is the width of the character
	Uint8 w = 0;	
	// This is the height of the character
	Uint8 h = 0;
	// This is the x texture source position of the character
	Uint8 sx = 0;
	// This is the y texture source position of the character
	Uint8 sy = 0;
	// This is the width of the source rectangle of the character
	Uint16 sw = 0;
	// This is the height of the source rectangle of the character
	Uint16 sh = 0;

	void drawCharacter();
	void freeCharacter();
	void loadChar(char chrctr[1], SDL_Texture* textTexture, vector2_int size = vector2_int(48, 48), vector2_int source = vector2_int(0, 0));
};

void character::drawCharacter() {
	SDL_FRect currentDraw = { x,y,static_cast<float>(w),static_cast<float>(h)};
	SDL_FRect currentSource = { static_cast<float>(sx), static_cast<float>(sy),static_cast<float>(w),static_cast<float>(h)};
	SDL_RenderTexture(main_renderer, textTextures[static_cast<int>(letter[0])], &currentSource, &currentDraw);
#ifdef DRAW_DEBUG
	SDL_SetRenderDrawColor(main_renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderRect(main_renderer, &currentDraw);
#endif
	//SDL_free(&currentDraw);
}

void character::freeCharacter() {
	char* magicptr[2];
	magicptr[0] = &magic[0];
	magicptr[1] = &magic[1];
#ifdef _NATIVE_NULLPTR_SUPPORTED
	magicptr[0] = nullptr;
	magicptr[1] = nullptr;
#else
	magicptr[0] = NULL;
	magicptr[1] = NULL;
#endif
	SDL_DestroyTexture(letter_texture);
}

void character::loadChar(char chrctr[1], SDL_Texture* textTexture, vector2_int size, vector2_int source) {
	letter[0] = chrctr[0];
	letter_texture = textTextures[static_cast<int>(letter[0])];
	w = size.x;
	h = size.y;
	sx = source.x;
	sy = source.y;
};
#endif
