// 
/**
* @file VectorTypes.h
* @brief A header file containing vector types, created for the TtEL GUI Notepad.
* @author Team theEeveeLovers Sean
* @date May 23rd, 2024
*/
#ifndef VECTOR_TYPES
#define VECTOR_TYPES
#include "../globals.h"

/**
 * @brief A vector type holding two integers.
 * It can be constructed using @code vector2_int(x, y) @endcode 
 * or just @code vector2_int() @endcode for a default containing 0's
 */
typedef struct vector2_int {
	int										x, y;
	constexpr vector2_int()					: x(0), y(0) { }
	constexpr vector2_int(int _x, int _y)	: x(_x), y(_y) { }
} vector2_int;
/**
 * @brief A vector type holding two floats.
 * It can be constructed using @code vector2_float(x, y) @endcode
 * or just @code vector2_float() @endcode for a default containing 0's
 */
typedef struct vector2_float {
	float									x, y;
	constexpr vector2_float() : x(0.f), y(0.f) { }
	constexpr vector2_float(float _x, float _y) : x(_x), y(_y) { }
} vector2_float;

/**
 * @brief A vector type holding three integers.
 * It can be constructed using @code vector3_int(x, y, z) @endcode
 * or just @code vector3_int() @endcode for a default containing 0's
 */
typedef struct vector3_int {
	int										x, y, z;
	constexpr vector3_int() : x(0), y(0), z(0) { }
	constexpr vector3_int(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }
} vector3_int;
/**
 * @brief A vector type holding three floats.
 * It can be constructed using @code vector3_float(x, y,z) @endcode
 * or just @code vector3_float() @endcode for a default containing 0's
 */
typedef struct vector3_float {
	float									x, y, z;
	constexpr vector3_float() : x(0.f), y(0.f), z(0.f) { }
	constexpr vector3_float(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
} vector3_float;

#endif 