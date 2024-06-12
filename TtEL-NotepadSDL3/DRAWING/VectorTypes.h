#ifndef VECTOR_TYPES
#define VECTOR_TYPES
#include "../globals.h"

typedef struct vector2_int {
	int										x, y;
	constexpr vector2_int()					: x(0), y(0) { }
	constexpr vector2_int(int _x, int _y)	: x(_x), y(_y) { }
} vector2_int;
typedef struct vector2_float {
	float									x, y;
	constexpr vector2_float() : x(0.f), y(0.f) { }
	constexpr vector2_float(float _x, float _y) : x(_x), y(_y) { }
} vector2_float;

typedef struct vector3_int {
	int										x, y, z;
	constexpr vector3_int() : x(0), y(0), z(0) { }
	constexpr vector3_int(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }
} vector3_int;
typedef struct vector3_float {
	float									x, y, z;
	constexpr vector3_float() : x(0.f), y(0.f), z(0.f) { }
	constexpr vector3_float(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
} vector3_float;

#endif 