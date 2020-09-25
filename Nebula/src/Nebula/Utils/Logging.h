#pragma once

#define DEBUG_LEVEL 5   //(Debugging level; 1) Error  2) Warning 3) Info
//                  4) Debug 5) Verbose 6)Too Much Info

#ifdef DEBUG_LEVEL
#include <stdio.h>
#endif

#ifdef DEBUG_LEVEL
#define LOG(type, ...) LOG_##type(__VA_ARGS__)
#else
#define LOG(type, ...)
#endif

//Error Log
#if DEBUG_LEVEL > 0
#define LOG_ERR(...) printf("[Nebula] E: " __VA_ARGS__)
#else
#define LOG_ERR(...)
#endif

//Warning Log
#if DEBUG_LEVEL > 1
#define LOG_WRN(...) printf("[Nebula] W: " __VA_ARGS__)
#else
#define LOG_WRN(...)
#endif

//Info Log
#if DEBUG_LEVEL > 2
#define LOG_INF(...) printf("[Nebula] I: " __VA_ARGS__)
#else
#define LOG_INF(...)
#endif

//Debug Log
#if DEBUG_LEVEL > 3
#define LOG_DBG(...) printf("[Nebula] D: " __VA_ARGS__)
#else
#define LOG_DBG(...)
#endif

//Verbose Log
#if DEBUG_LEVEL > 4
#define LOG_VER(...)  printf("[Nebula] V: " __VA_ARGS__)
#else
#define LOG_VER(...)
#endif

//TMI Log
#if DEBUG_LEVEL > 5
#define LOG_TMI(...) printf("[Nebula] TMI: " __VA_ARGS__)
#else
#define LOG_TMI(...)
#endif
