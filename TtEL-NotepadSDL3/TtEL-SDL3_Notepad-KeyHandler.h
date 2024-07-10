#pragma once
/**
* @file TtEL-SDL3_Notepad-KeyHandler.h
* @brief A header file of the TtEL SDL3 GUI Notepad, containing keyboard handling functions
* @author Team theEeveeLovers Sean
* @date June 14th, 2024
*/
#include "TtEL-SDL3_Notepad.hpp"

extern character* Dtext; // all the text in the 'document'
extern int textBufferSize; // the size of the textual screen buffer

/**
* @brief A function to handle a SDL Keycode.
* 
* @param keyCode - The SDL_Keycode to handle.
* @param capital - Bool describing if shift is pressed.
*
*/
void handleKey(SDL_Keycode keyCode, bool capital) {
	// Keycode is a special character
	if (ME::thresholdInt(keyCode, 33, 47) || ME::thresholdInt(keyCode, 58, 63) || ME::thresholdInt(keyCode, 91, 93) || keyCode == 96) {
		// Iterate through screen buffer to find unpopulated space 
		for (int i = 0; i <= textBufferSize; i++) {
			// Is empty space
			if (Dtext[i].isEmptyChar()) {
				char specChar = static_cast<char>(keyCode); // turn keyCode to char
				// shift special character
				if (capital) {
					switch (keyCode) {
					// SHIFT + '	(")
					case 39:
						specChar -= 5;
						break;
					// SHIFT + ,	(<)
					case 44:
						specChar += 0x10;
						break;
					// SHIFT + .	(>)
					case 46:
						specChar += 0x10;
						break;
					// SHIFT + /	(?)
					case 47:
						specChar += 0x10;
						break;
					// SHIFT + ;	(:)
					case 59:
						specChar--;
						break;
					// SHIFT + =	(+)
					case 61:
						specChar -= 0x12;
						break;
					}
				}
				Dtext[i] = loadCharFromChar(&specChar); // add special character to the buffer
				break; // Leave this loop
			} // We have reached the end of the file and more bytes need to be allocated
			else if (i == textBufferSize && Dtext[i].isFilledChar()) {
				// put the current text into a buffer
				character* tempBuffer = new character[textBufferSize];
				for (int i = 0; i < textBufferSize; i++) { tempBuffer[i] = Dtext[i]; }
				
				// Delete the old buffer
				delete[] Dtext;

				// store size of the old buffer
				int oldSize = textBufferSize;

				textBufferSize += FILE_PAD; // update the size variable
				// remake the buffer with the wanted size
				Dtext = new character[textBufferSize];

				// put the old text into the new buffer
				for (int i = 0; i < oldSize; i++) { Dtext[i] = tempBuffer[i]; }

				// delete the temporary buffer
				delete[] tempBuffer;

				// put the wanted key
				char specChar = static_cast<char>(keyCode); // turn keyCode to char
				// shift special character
				if (capital) {
					switch (keyCode) {
						// SHIFT + '	(")
					case 39:
						specChar -= 5;
						break;
						// SHIFT + ,	(<)
					case 44:
						specChar += 0x10;
						break;
						// SHIFT + .	(>)
					case 46:
						specChar += 0x10;
						break;
						// SHIFT + /	(?)
					case 47:
						specChar += 0x10;
						break;
						// SHIFT + ;	(:)
					case 59:
						specChar--;
						break;
						// SHIFT + =	(+)
					case 61:
						specChar -= 0x12;
						break;
					}
				}
				Dtext[i] = loadCharFromChar(&specChar); // add character to the buffer
				//Dtext[oldSize + 1].freeCharacter(); // stop character 'duplication'

				// log 
				SDL_Log("Buffer size increased from %d to %d!\n", oldSize, textBufferSize);

				break; // Leave this loop
			}
		}
	}
	// Keycode is a number
	if (ME::thresholdInt(keyCode, 48, 57)) {
		// Iterate through screen buffer to find unpopulated space 
		for (int i = 0; i <= textBufferSize; i++) {
			// Is empty space
			if (Dtext[i].isEmptyChar()) {
				char number = static_cast<char>(keyCode); // turn keyCode to char
				// number row special character
				if (capital) {
					switch (keyCode) {
					// SHIFT + 0	())
					case 0x30:
						number -= 7;
						break;
					// SHIFT + 9	(()
					case 0x39:
						number -= 0x11;
						break;
					// SHIFT + 1	(!)
					case 0x31:
						number -= 0x10;
						break;
					// SHIFT + 2	(@)
					case 0x32:
						number += 0xE;
						break;
					// SHIFT + 3	(#)
					case 0x33:
						number -= 0x10;
						break;
					// SHIFT + 4	($)
					case 0x34:
						number -= 0x10;
						break;
					// SHIFT + 5	(%)
					case 0x35:
						number -= 0x10;
						break;
					// SHIFT + 6	(^)
					case 0x36:
						number += 40;
						break;
					// SHIFT + 7	(&)
					case 0x37:
						number -= 0x11;
						break;
					// SHIFT + 8	(*)
					case 0x38:
						number -= 0xE;
						break;
					}
				}
				Dtext[i] = loadCharFromChar(&number); // add number to the buffer
				break; // Leave this loop
			} // We have reached the end of the file and more bytes need to be allocated
			else if (i == textBufferSize && Dtext[i].isFilledChar()) {
				// put the current text into a buffer
				character* tempBuffer = new character[textBufferSize];
				for (int i = 0; i < textBufferSize; i++) { tempBuffer[i] = Dtext[i]; }

				// Delete the old buffer
				delete[] Dtext;

				// store size of the old buffer
				int oldSize = textBufferSize;

				textBufferSize += FILE_PAD; // update the size variable
				// remake the buffer with the wanted size
				Dtext = new character[textBufferSize];

				// put the old text into the new buffer
				for (int i = 0; i < oldSize; i++) { Dtext[i] = tempBuffer[i]; }

				// delete the temporary buffer
				delete[] tempBuffer;

				// put the wanted key
				char number = static_cast<char>(keyCode); // turn keyCode to char
				// number row special character
				if (capital) {
					switch (keyCode) {
						// SHIFT + 0	())
					case 0x30:
						number -= 7;
						break;
						// SHIFT + 9	(()
					case 0x39:
						number -= 0x11;
						break;
						// SHIFT + 1	(!)
					case 0x31:
						number -= 0x10;
						break;
						// SHIFT + 2	(@)
					case 0x32:
						number += 0xE;
						break;
						// SHIFT + 3	(#)
					case 0x33:
						number -= 0x10;
						break;
						// SHIFT + 4	($)
					case 0x34:
						number -= 0x10;
						break;
						// SHIFT + 5	(%)
					case 0x35:
						number -= 0x10;
						break;
						// SHIFT + 6	(^)
					case 0x36:
						number += 40;
						break;
						// SHIFT + 7	(&)
					case 0x37:
						number -= 0x11;
						break;
						// SHIFT + 8	(*)
					case 0x38:
						number -= 0xE;
						break;
					}
				}
				Dtext[i] = loadCharFromChar(&number); // add character to the buffer
				//*Dtext[oldSize + 1].letter = '\0'; // stop character 'duplication'

				//Dtext[i] = Dtext[i + 1];
				//Dtext[i + 1].freeCharacter(); // stop character 'duplication'

				// log 
				SDL_Log("Buffer size increased from %d to %d!\n", oldSize, textBufferSize);

				break; // Leave this loop
			}
		}
	}
	// Keycode is a letter
	else if (ME::thresholdInt(keyCode, 97, 122)) {
		// Iterate through screen buffer to find unpopulated space 
		for (int i = 0; i <= textBufferSize; i++) {
			// Is empty space
			if (Dtext[i].isEmptyChar()) {
				char letter = static_cast<char>(keyCode); // turn keyCode to char
				if (capital) {
					letter -= ' '; // subtract space character from the letter to make it uppercase, ASCII is beautiful
				}
				Dtext[i] = loadCharFromChar(&letter); // add letter to the buffer
				break; // Leave this loop
			} // We have reached the end of the file and more bytes need to be allocated
			else if (i == textBufferSize && Dtext[i].isFilledChar()) {
				char letter = static_cast<char>(keyCode); // turn keyCode to char
				// put the current text into a buffer
				character* tempBuffer = new character[textBufferSize];
				for (int i = 0; i < textBufferSize; i++) { tempBuffer[i] = Dtext[i]; }

				// Delete the old buffer
				delete[] Dtext;

				// store size of the old buffer
				int oldSize = textBufferSize;

				textBufferSize += FILE_PAD; // update the size variable
				// remake the buffer with the wanted size
				Dtext = new character[textBufferSize];

				// put the old text into the new buffer
				for (int i = 0; i < oldSize; i++) { Dtext[i] = tempBuffer[i]; }

				// delete the temporary buffer
				delete[] tempBuffer;

				if (capital) {
					letter -= ' '; // subtract space character from the letter to make it uppercase, ASCII is beautiful
				}

				Dtext[i] = loadCharFromChar(&letter); // add letter to the buffer

				SDL_Log("Buffer size increased from %d to %d!\n", oldSize, textBufferSize);

				break; // Leave this loop
			}
		}
	}
}
