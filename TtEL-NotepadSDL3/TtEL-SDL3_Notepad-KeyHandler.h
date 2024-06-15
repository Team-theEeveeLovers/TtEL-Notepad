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
	// Keycode is a special character
	if (ME::thresholdInt(keyCode, 33, 47)) {
		// Iterate through screen buffer to find unpopulated space 
		for (int i = 0; i <= 256; i++) {
			// Is empty space
			if (text[i].letter[0] == '\0') {
				char specChar = static_cast<char>(keyCode); // turn keyCode to char
				text[i] = loadCharFromChar(&specChar); // add special character to the buffer
				break; // Leave this loop
			}
		}
	}
	// Keycode is a number
	if (ME::thresholdInt(keyCode, 48, 57)) {
		// Iterate through screen buffer to find unpopulated space 
		for (int i = 0; i <= 256; i++) {
			// Is empty space
			if (text[i].letter[0] == '\0') {
				char number = static_cast<char>(keyCode); // turn keyCode to char
				// number row special character
				if (capital) {
					switch (keyCode) {
					// SHIFT + 1
					case 49:
						number -= 0x10;
					}
				}
				text[i] = loadCharFromChar(&number); // add number to the buffer
				break; // Leave this loop
			}
		}
	}
	// Keycode is a letter
	else if (ME::thresholdInt(keyCode, 97, 122)) {
		// Iterate through screen buffer to find unpopulated space 
		for (int i = 0; i <= 256; i++) {
			// Is empty space
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
