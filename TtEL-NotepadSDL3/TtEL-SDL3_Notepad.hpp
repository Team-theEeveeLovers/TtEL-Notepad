#include <iostream>
using namespace std;
#include <SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>


#ifndef ASSERT
#include <assert.h>
#define ASSERT(_EXPR)            assert(_EXPR)                              
#endif

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

#ifndef DEBUG_BREAK
#if defined (_MSC_VER)
#define DEBUG_BREAK()    __debugbreak()
#elif defined(__clang__)
#define DEBUG_BREAK()    __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define DEBUG_BREAK()    __asm__ volatile("int3;nop")
#elif defined(__GNUC__) && defined(__thumb__)
#define DEBUG_BREAK()    __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define DEBUG_BREAK()    __asm__ volatile(".inst 0xe7f001f0")
#else
#define DEBUG_BREAK()    ASSERT(0)    // It is expected that you define DEBUG_BREAK() into something that will break nicely in a debugger!
#endif
#endif // #ifndef DEBUG_BREAK

extern SDL_Texture* textTextures[256];
extern vector2_int textTextureSizeVectors[256];

character loadCharFromChar(char* chara) {
	character tempChar;
	int charaNum = static_cast<int>(*chara);
	tempChar.loadChar(chara, textTextures[charaNum], textTextureSizeVectors[charaNum]);
	return tempChar;
	tempChar.freeCharacter();
}