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

#ifdef DRAW_DEBUG
extern bool DRAW_DBG;
#endif

#define MAX_FILESIZE 4096*16 // 64 MB
#define FILE_PAD 16 // 16 B

class character {
private:
	// This is a magic number, it uses unused space in memory
	char magic[2] = { 'N', 'C' };
	// This is padding between the magic number and the letter contained by the variable, it also uses unused memory space
	char padding[1] = { '\x20' };
	// The unused space is caused by the letter variable
public:
	// This is the letter contained by the character
	char letter[1] = { '\0' };
	// This is the x position of the character
	float x = 0.f;
	// This is the y position of the character
	float y = 0.f;
	// This is the width of the character
	Uint16 w = 0;	
	// This is the height of the character
	Uint16 h = 0;

	void drawCharacter(vector2_float posOffset = vector2_float(), vector4_float srcOffset = vector4_float(), vector2_float sizeOffset = vector2_float());
	void freeCharacter();
	void destroyCharacter();
	void loadChar(char chrctr[1]);

	// does the char contain a letter?
	inline bool isFilledChar() const { return letter[0] != '\0'; };

	// does the char contain unfilled space?
	inline bool isEmptyChar() const { return letter[0] == '\0'; };

	// is the bool a newline (line feed) character?
	inline bool isNewline() const {return letter[0] == '\n'; // sorry CRLF fans, adding CR would add an additional overhead, so we just check for LF here
	};
};

/**
 * @brief Draws the character on the screen
 * @param posOffset - an offset modifying the position of drawing on the screen, positive y values are downward
 * @param srcOffset - an offset modifying the source rectangle for drawing the text texture partially (cropping), positive y values are downward. The z value is width and the w value is height (negative values subtract and positive values add).
 * @param sizeOffset - an offset modifying the render size of the texture for stretching the texture or adjusting for source offsets. X is width and y is height. (negative values subtract and positive values add)
 */
void character::drawCharacter(vector2_float posOffset, vector4_float srcOffset, vector2_float sizeOffset) {
	if (magic[0] != 'N' || magic[1] != 'C') {
		//ASSERT("TextRenderer.h: Not a valid character! ");
		//ASSERT("Header was invalid!");
		ASSERT(magic[0] == 'N' && magic[1] == 'C' && "Invalidated header!"); // The character has an invalid header
		DEBUG_BREAK(); // Break in case assert is unavailable or just doesn't want to abort the application
	}
	else if (padding[0] != '\x20') {
		//ASSERT("TextRenderer.h: Not a valid character! ");
		//ASSERT("Header Padding was invalid!");
		ASSERT(padding[0] == '\x20' && "Invalidated header padding!"); // The padding between the header and data was invalid
		DEBUG_BREAK(); // Break in case assert is unavailable or just doesn't want to abort the application
	}
	else {
		// Unsigned so we don't index before an array
		Uint8 tableIDX = static_cast<Uint8>(letter[0]);
		// IF UNKNOWN CHARACTER
		if (textTextures[tableIDX] == NULL)
			tableIDX = 0; // REPLACE WITH REPLACEMENT CHARACTER
		float width = static_cast<float>(w), height = static_cast<float>(h);
		SDL_FRect currentDraw = { 
			x + posOffset.x,y + posOffset.y,
			width,height 
		};

		if (sizeOffset.x != 0.f)
			currentDraw.w += sizeOffset.x; // add the offset

		if (sizeOffset.y != 0.f)
			currentDraw.h += sizeOffset.y; // add the offset

		// initalize source rect variables
		float sourceX = static_cast<float>(textTextureSourceVectors[tableIDX].x);
		float sourceY = static_cast<float>(textTextureSourceVectors[tableIDX].y);
		float sourceW = width;
		float sourceH = height;

		if (srcOffset.x != 0.f)
			sourceX += srcOffset.x; // add the offset

		if (srcOffset.y != 0.f)
			sourceY += srcOffset.y; // add the offset

		if (srcOffset.z != 0.f)
			sourceW += srcOffset.z; // add the offset

		if (srcOffset.w != 0.f)
			sourceH += srcOffset.w; // add the offset
		

		SDL_FRect currentSource = { 
			sourceX,sourceY,
			sourceW,sourceH
		};
		/*if (textTextures[tableIDX] == NULL) {
			// OLD DEBUG THINGS
			//ASSERT("TextRenderer.h: Character has no letter texture!");
			int printNumb = static_cast<int>(letter[0]) << 24; // shift left 3 bytes (8x3 bits) to cut off the leading 0xFFFFFF
			cout << "Missing Texture for letter '" << letter[0] << "'! (0x";
			cout << hex << printNumb << ") " << endl;
			ASSERT(textTextures[tableIDX] != NULL && "Missing texture for letter" && letter[0]);
			
			DEBUG_BREAK(); // Break in case assert is unavailable or just doesn't want to abort the application

		}
		else {
			SDL_RenderTexture(main_renderer, textTextures[tableIDX], &currentSource, &currentDraw);
		}*/
		SDL_RenderTexture(main_renderer, textTextures[tableIDX], &currentSource, &currentDraw);
#ifdef DRAW_DEBUG
		if (DRAW_DBG) {
			SDL_SetRenderDrawColor(main_renderer, 0xFF, 0x00, 0x00, 0x66);
			SDL_RenderRect(main_renderer, &currentDraw);
			// if the sourceRect isn't the whole texture
			if (currentSource.x != 0.f || currentSource.y != 0.f || currentSource.w != currentDraw.w || currentSource.h != currentDraw.h) {
				SDL_FRect debugSourceRect = {
					currentSource.x + currentDraw.x,
					currentSource.y + currentDraw.y,
					currentSource.w,
					currentSource.h,
				};
				SDL_SetRenderDrawColor(main_renderer, 0x33, 0xFF, 0x33, 0x66);
				SDL_RenderRect(main_renderer, &debugSourceRect);
			}
		}
#endif
		//SDL_free(&currentDraw);
	}
}

void character::freeCharacter() {
	letter[0] = '\0'; // nullify letter
	x = 0.f; y = 0.f; // zeroify position
	w = 0; h = 0; // zeroify size
}
void character::destroyCharacter() {
	magic[0] = '\0'; // nullify magic
	magic[1] = '\0'; // nullify magic
	*magic = NULL; // null magic
	padding[0] = '\0'; // nullify padding
	*padding = NULL; // null padding
	letter[0] = '\0'; // nullify letter
	*letter = NULL; // null letter
	x = 0.f; y = 0.f; // zeroify position
	w = 0; h = 0; // zeroify size
}

void character::loadChar(char chrctr[1]) {
	letter[0] = chrctr[0];
	// Unsigned so we don't index before an array
	Uint8 tableIDX = static_cast<Uint8>(letter[0]);
	// IF UNKNOWN CHARACTER
	if (textTextures[tableIDX] == NULL)
		tableIDX = 0; // REPLACE WITH REPLACEMENT CHARACTER
	w = textTextureSizeVectors[tableIDX].x;
	h = textTextureSizeVectors[tableIDX].y;
};


void drawFromTextBuffer(character* textBuffer, int textBufferSize, SDL_FRect TextBackground, float lineSpacing, float Scroll, float RightTextMargin) {
	for (int i = 0; i <= textBufferSize - 1; i++) {
		if (textBuffer[i].isFilledChar()) {

			if (i == 0) {
				textBuffer[i].x = TextBackground.x + 10.f;
				textBuffer[i].y = TextBackground.y + 14.f;
			}
			else {
				textBuffer[i].x = (textBuffer[i - 1].x + textBuffer[i - 1].w) + 0.3125f;
				textBuffer[i].y = textBuffer[i - 1].y;
				if (textBuffer[i].isNewline())
					textBuffer[i].y += lineSpacing + 10.f;
			}


			if (textBuffer[i].x + textBuffer[i].w > RightTextMargin) {
				while (textBuffer[i].x + textBuffer[i].w > RightTextMargin) {
					textBuffer[i].x = textBuffer[i].x - (RightTextMargin - 20.f);
					textBuffer[i].y = textBuffer[i].y + lineSpacing;
				}
			}
			if (i > 0) {
				if (textBuffer[i - 1].y < textBuffer[i].y) {
					textBuffer[i].x = (TextBackground.x + 10.f);
				}
			}
			// If the text got scrolled above the textbox (with added 20.f because of the empty space)
			if (textBuffer[i].y - Scroll + 20.f < TextBackground.y) {
				// how far above are we?
				float above = textBuffer[i].y + Scroll;
				float below = textBuffer[i].y - Scroll;
				// If we are completely above
				if (textBuffer[i].y - Scroll + textBuffer[i].h + 20.f < TextBackground.y) {
					// allan please add code
				}
				else {
					textBuffer[i].drawCharacter(vector2_float(0.f, 0.f - Scroll + below), vector4_float(0.f, 0.f - ((above / 3.5f)), 0.f, 0.f - below + textBuffer[i].y), vector2_float(0.f, 0.f - below));

					// keep this for scrolling text from below the textbox
					// float above = textBuffer[i].y - Scroll;
					// textBuffer[i].drawCharacter(vector2_float(0.f, 0.f - Scroll), vector4_float(0.f, 0.f-(above/2.f), 0.f, 0.f-above), vector2_float(0.f, 0.f-above));

				}
			}
			else {
				textBuffer[i].drawCharacter(vector2_float(0.f, 0.f - Scroll));
			}
		}
		else {
			// If this is the second instance of NULLness
			if (i > 1 && textBuffer[i - 1].isEmptyChar())
				break; // Break out of the for loop
		}
	}
}





#endif
