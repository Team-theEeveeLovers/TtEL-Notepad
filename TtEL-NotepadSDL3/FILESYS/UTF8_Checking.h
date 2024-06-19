#pragma once
#include <SDL.h> // for Uint8
#include "../globals.h"
#include "misc/ByteMath.h"
namespace BM = ByteMath;


namespace checkUTF8 {

	Uint8 getCodepointSize(Uint8* CodepointBytes) {
		switch (sizeof(*CodepointBytes)) {
		case 0:
			DEBUG_BREAK();
		default:
			DEBUG_BREAK();
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