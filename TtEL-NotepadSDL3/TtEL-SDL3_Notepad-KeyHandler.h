#pragma once
/**
* @file TtEL-SDL3_Notepad-KeyHandler.h
* @brief A header file of the TtEL SDL3 GUI Notepad, containing keyboard handling functions
* @author Team theEeveeLovers Sean
* @date June 14th, 2024
*/
#include "TtEL-SDL3_Notepad.hpp"

extern character text[256]; // all the text in the 'document'

/**
* @brief A function to handle a SDL Keycode.
* 
* @param keyCode - The SDL_Keycode to handle.
* @param capital - Bool describing if shift is pressed.
*
*/
void handleKey(SDL_Keycode keyCode, bool capital) {
	// Keycode is a letter
	if (ME::thresholdInt(keyCode, 97, 122)) {
		// Iterate through screen buffer to find unpopulated space 
		for (int i = 0; i <= 256; i++) {
			if (text[i].letter[0] == '\0') {
				char letter = static_cast<char>(keyCode); // turn keyCode to char
				if (capital) {
					letter -= ' '; // subtract space character from the letter to make it uppercase, ASCII is beautiful
				}
				text[i] = loadCharFromChar(&letter); // add letter to the buffer
				break; // Leave this loop
			}
		}
	}
}