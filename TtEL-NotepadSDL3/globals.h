#pragma once
#if defined(WIN32) && !defined(UNIX)
#define WINDOWS


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(UNIX) && !defined(WIN32)
#define POSIX
#endif
#define DRAW_DEBUG