#ifndef VECTOR_TYPES
#define VECTOR_TYPES
#include "../globals.h"

typedef struct vector2_int {
	int										x, y;
	constexpr vector2_int()					: x(0), y(0) { }
	constexpr vector2_int(int _x, int _y)	: x(_x), y(_y) { }
} vector2_int;
typedef struct vector2_float {
	int										x, y;
	constexpr vector2_float() : x(0.f), y(0.f) { }
	constexpr vector2_float(float _x, float _y) : x(_x), y(_y) { }
} vector2_float;

#endif 