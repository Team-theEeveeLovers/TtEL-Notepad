#ifndef VECTOR_TYPES
#define VECTOR_TYPES
typedef struct vector2_int {
	int										x, y;
	constexpr vector2_int()					: x(0), y(0) { }
	constexpr vector2_int(int _x, int _y)	: x(_x), y(_y) { }
} vector2_int;
#endif 