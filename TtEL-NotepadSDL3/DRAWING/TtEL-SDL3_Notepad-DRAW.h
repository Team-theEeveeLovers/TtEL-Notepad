/**
* @file TtEL-SDL3_Notepad-DRAW.h
* @brief A header file of the TtEL SDL3 GUI Notepad, containing drawing functions
* @author Team theEeveeLovers Sean
* @date May 23rd, 2024
*/

#ifndef TTEL_SDL3_NOTEPAD_DRAW
#define TTEL_SDL3_NOTEPAD_DRAW
#include <SDL.h>
#include "TextRenderer.h"
#include "VectorTypes.h"
#include "../globals.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "SDL_Surface-Tools.h"

#include <string>
#include <iostream>
using namespace std;
extern SDL_Renderer* main_renderer;
extern SDL_Texture* textTextures[256];
extern vector2_int textTextureSizeVectors[256];
extern vector2_int textTextureSourceVectors[256];


#ifdef DRAW_DEBUG
extern bool DRAW_DBG;
#endif

extern int scr_wid;
extern int scr_hei;
extern float scr_floatwid;
extern float scr_floathei;

/**
 * @brief A custom rectangle type defined using edge values.\n
 * It is defined using left edge x value, a right edge x value, a top edge y value, and a bottom edge y value.
 */
typedef struct NOTEPAD_RECT {
	int lx; // Left Edge X Value
	int rx; // Right Edge X Value
	int ty; // Top Edge Y Value
	int by; // Bottom Edge Y Value

	SDL_FRect toSDL_FRect() const;
} NOTEPAD_RECT;

// Converts the NOTEPAD_RECT to an SDL3 SDL_FRect
SDL_FRect NOTEPAD_RECT::toSDL_FRect() const {
	float x = static_cast<float>(lx);
    float y = static_cast<float>(ty);
    float w = static_cast<float>(rx)-x;
    float h = static_cast<float>(by)-y;
	SDL_FRect outRect = { x,y,w,h };
	return outRect;
}
/*
// Converts the NOTEPAD_RECT to an SDL3 SDL_FRect
SDL_FRect NOTEPAD_RECT::toSDL_FRect() const {
    int x = static_cast<float>(lx);
    int y = ty;
    int w = rx - x;
    int h = by - y;
    SDL_FRect outRect = { x,y,w,h };
    return outRect;
}*/

/**
* @brief A function that loads an <SDL_Texture*> from a text string.
* @param font - The font to render with
* @param text - The text to render
* @param textColor - The color to render the text with. It defaults to black.
* @param height - Optional pointer to a variable to store the height of the texture.
* @param width - Optional pointer to a variable to store the width of the texture.
*
* @return The loaded <SDL_Texture*>, or NULL upon error.
*/
SDL_Texture* loadTextureFromText(TTF_Font* font, string text, SDL_Color textColor = { 0x0,0x0,0x0 }, int* height = NULL, int* width = NULL) {
    // declare temporary variable
    SDL_Texture* mTex = NULL;


    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor); // Render text to a surface
    if (textSurface == NULL)
    {
        cout << "Unable to render text to a surface. SDL_ttf Error: " << TTF_GetError() << endl;
        return NULL;
    }
    else {
        mTex = SDL_CreateTextureFromSurface(main_renderer, textSurface); // Create a texture from the text surface
        if (height != NULL) {
            *height = textSurface->h;
        }
        if (width != NULL) {
            *width = textSurface->w;
        }
        SDL_DestroySurface(textSurface);
        if (mTex == NULL) {
            cout << "Unable to create a texture from rendered text surface. SDL_error: " << SDL_GetError() << endl;
            return mTex;
        }
        else {
            return mTex;
        }
    }
}

/**
* @brief A function that loads an <SDL_Texture*> from a text string of char class.
* @param font - The font to render with
* @param text - The text to render
* @param textColor - The color to render the text with. It defaults to black.
* @param height - Optional pointer to a variable to store the height of the texture.
* @param width - Optional pointer to a variable to store the width of the texture.
*
* @return The loaded <SDL_Texture*>, or NULL upon error.
*/
SDL_Texture* loadTextureFromChar(TTF_Font* font, char text, SDL_Color textColor = { 0x0,0x0,0x0 }, int* height = NULL, int* width = NULL) {
    // declare temporary variable
    SDL_Texture* mTex = NULL;


    SDL_Surface* textSurface = TTF_RenderText_Blended(font, &text, textColor); // Render text to a surface
    if (textSurface == NULL)
    {
        cout << "Unable to render text to a surface. SDL_ttf Error: " << TTF_GetError() << endl;
        return NULL;
    }
    else {
        /*if (crop) {
            Uint32 bg = GetSurfacePixel32(0, 0, textSurface);
            int startY = 0;
            for (int y = 0; y < textSurface->h; y++) {
                for (int x = 0; x < textSurface->w; x++) {
                    Uint32 currentPixel = GetSurfacePixel32(x, y, textSurface);
                    if (currentPixel != bg) {
                        startY = y;
                        break;
                    }
                }
            }
        }*/
        mTex = SDL_CreateTextureFromSurface(main_renderer, textSurface); // Create a texture from the text surface
        if (height != NULL) {
            *height = textSurface->h;
        }
        if (width != NULL) {
            *width = textSurface->w;
        }
        SDL_DestroySurface(textSurface);
        if (mTex == NULL) {
            cout << "Unable to create a texture from rendered text surface. SDL_error: " << SDL_GetError() << endl;
            return mTex;
        }
        else {
            return mTex;
        }
    }
}
/**
* @brief A function that loads an <SDL_Texture*> from a UNICODE codepoint.
* @param font - The font to render with
* @param text - The codepoint to render
* @param textColor - The color to render the text with. It defaults to black.
* @param height - Optional pointer to a variable to store the height of the texture.
* @param width - Optional pointer to a variable to store the width of the texture. 
*
* @return The loaded <SDL_Texture*>, or NULL upon error.
*/

SDL_Texture* loadTextureFromUnicodeCodepoint(TTF_Font* font, Uint16 text, SDL_Color textColor = { 0x0,0x0,0x0 }, int* height = NULL, int* width = NULL) {
    // declare temporary variable
    SDL_Texture* mTex = NULL;

    Uint16* text_ptr = &text;
    SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(font, text_ptr, textColor); // Render text to a surface
    if (textSurface == NULL)
    {
        cout << "Unable to render text to a surface. SDL_ttf Error: " << TTF_GetError() << endl;
        return NULL;
    }
    else {
        mTex = SDL_CreateTextureFromSurface(main_renderer, textSurface); // Create a texture from the text surface
        if (height != NULL) {
            *height = textSurface->h;
        }
        if (width != NULL) {
            *width = textSurface->w;
        }
        SDL_DestroySurface(textSurface);
        if (mTex == NULL) {
            cout << "Unable to create a texture from rendered text surface. SDL_error: " << SDL_GetError() << endl;
            return mTex;
        }
        else {
            return mTex;
        }
    }
}






// Namespace for drawing rectangles
namespace RectDraw {
    /**
    * @brief A function that draws and fills a rectangle using input position and size.
    * @param x - X position of the rectangle in pixels, where the origin is at the left edge of the screen.
    * @param y - Y position of the rectangle in pixels, where the origin is at the top edge of the screen.
    * @param w - Width of the rectangle in pixels.
    * @param h - Height of the rectangle in pixels.
    *
    * @return 0 on success, or -1 on error.
    */
    int FillFRectFromInput(float x, float y, float w, float h) {
        SDL_FRect drawRect = { x, y, w, h };
        return SDL_RenderFillRect(main_renderer, &drawRect);
        SDL_free(&drawRect);
    }   
    /**
    * @brief A function that draws and fills an input rectangle.
    * @param drawRect - A nonpointer to a rectangle to draw.
    * @return 0 on success, or -1 on error.
    */
    int FillFRectFromInputRect(SDL_FRect drawRect) {
        return SDL_RenderFillRect(main_renderer, &drawRect);
    }
    /**
    * @brief A function that draws an unfilled rectangle using input position and size.
    * @param x - X position of the rectangle in pixels, where the origin is at the left edge of the screen.
    * @param y - Y position of the rectangle in pixels, where the origin is at the top edge of the screen.
    * @param w - Width of the rectangle in pixels.
    * @param h - Height of the rectangle in pixels.
    *
    * @return 0 on success, or -1 on error.
    */
    int StrokeFRectFromInput(float x, float y, float w, float h) {
        SDL_FRect drawRect = { x, y, w, h };
        return SDL_RenderRect(main_renderer, &drawRect);
        SDL_free(&drawRect);
    }
    /**
    * @brief A function that outlines an input rectangle.
    * @param drawRect - A nonpointer to a rectangle to draw.
    * @return 0 on success, or -1 on error.
    */
    int StrokeFRectFromInputRect(SDL_FRect drawRect) {
        return SDL_RenderRect(main_renderer, &drawRect);
    }
}









#endif