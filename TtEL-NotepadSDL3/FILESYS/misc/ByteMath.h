#ifndef BYTE_MATH
#define BYTE_MATH
#include <SDL.h> // for Uint8

namespace ByteMath {
	bool thresholdByte(Uint8 byte, Uint8 lowerBound, Uint8 upperBound) {
		return lowerBound <= byte && byte <= upperBound;
	};
}



#endif