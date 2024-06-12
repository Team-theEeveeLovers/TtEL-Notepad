#pragma once
#if defined(WIN32) && !defined(UNIX)
#define WINDOWS


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(UNIX) && !defined(WIN32)
#define POSIX
#endif
#define DRAW_DEBUG


#ifndef ASSERT
#include <assert.h>
#define ASSERT(_EXPR)            assert(_EXPR)                              
#endif

#ifndef DEBUG_BREAK
#if defined (_MSC_VER)
#define DEBUG_BREAK()    __debugbreak()
#elif defined(__clang__)
#define DEBUG_BREAK()    __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define DEBUG_BREAK()    __asm__ volatile("int3;nop")
#elif defined(__GNUC__) && defined(__thumb__)
#define DEBUG_BREAK()    __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define DEBUG_BREAK()    __asm__ volatile(".inst 0xe7f001f0")
#else
#define DEBUG_BREAK()    ASSERT(0)    // It is expected that you define DEBUG_BREAK() into something that will break nicely in a debugger!
#endif
#endif // #ifndef DEBUG_BREAK