#pragma once
#include <SDL.h> // for Uint8
#include "../globals.h"
#include "misc/ByteMath.h"
namespace BM = ByteMath;


namespace checkUTF8 {
	/**
	 * @brief Get the size of a codepoint based on a sample of bytes
	 * @param CodepointBytes a pointer to the bytes to get the size of
	 * @return 0 on error, size of codepoint on success
	 */
	Uint8 getCodepointSize(Uint8* CodepointBytes) {
		switch (sizeof(*CodepointBytes)) {
		case 0:
			return 0;
			DEBUG_BREAK();
			break;
		case 1: return 1; break;
		case 2:
			if (BM::thresholdByte(CodepointBytes[0], 0x00, 0x7F)) { return 1; }
			else { return 2; }
			break;
		case 3:
			if (BM::thresholdByte(CodepointBytes[0], 0x00, 0x7F)) { return 1; }
			else if (BM::thresholdByte(CodepointBytes[1], 0x00, 0x7F)) {}
			else { return 3; }
			break;
		default:
			return 0;
			DEBUG_BREAK();
			break;
		}
	};
	bool checkInvalidCodepoint(Uint8* CodepointBytes) {
		// Table
		if (sizeof(*CodepointBytes) > sizeof(Uint8)) {
			// Four bytes
			if (sizeof(*CodepointBytes >= 4 * sizeof(Uint8))) {
				if (BM::thresholdByte(CodepointBytes[0], 0x00, 0x7F)) {

					return true;
				}
				else {
					return false;
				}
			}
		}
	};
}