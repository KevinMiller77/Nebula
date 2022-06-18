#pragma once

#define DEBUG_LEVEL 5   //(Debugging level; 1) Error  2) Warning 3) Info
//                  4) Debug 5) Verbose 6)Too Much Info

#ifdef DEBUG_LEVEL
#include <stdio.h>
#endif

#ifdef DEBUG_LEVEL
#define LOG(type, ...) LOG_##type(__VA_ARGS__)
#define LOG_TAG(type, tag, ...) LOG_##type_TAG(tag, __VA_ARGS__)
#else
#define LOG(type, ...)
#define LOG_TAG(type, tag, ...)
#endif

#if DEBUG_LEVEL
#define LOG_NAKED(...) printf(__VA_ARGS__)
#define LOG_NAKED_TAG(tag, ...) printf(tag __VA_ARGS__)
#else
#define LOG_NAKED(...)
#define LOG_NAKED_TAG(tag, ...)
#endif

//Error Log
#if DEBUG_LEVEL > 0
#define LOG_ERR(...) printf("[Nebula] E: " __VA_ARGS__)
#define LOG_ERR_TAG(tag,...) printf("[Nebula]" tag " E: " __VA_ARGS__)
#else
#define LOG_ERR(...)
#endif

//Warning Log
#if DEBUG_LEVEL > 1
#define LOG_WRN(...) printf("[Nebula] W: " __VA_ARGS__)
#define LOG_WRN_TAG(tag,...) printf("[Nebula]" tag " W: " __VA_ARGS__)
#else
#define LOG_WRN(...)
#define LOG_WRN_TAG(tag,...)
#endif

//Info Log
#if DEBUG_LEVEL > 2
#define LOG_INF(...) printf("[Nebula] I: " __VA_ARGS__)
#define LOG_INF_TAG(tag,...) printf("[Nebula]" tag " I: " __VA_ARGS__)
#else
#define LOG_INF(...)
#define LOG_INF_TAG(tag,...)
#endif

//Debug Log
#if DEBUG_LEVEL > 3
#define LOG_DBG(...) printf("[Nebula] D: " __VA_ARGS__)
#define LOG_DBG_TAG(tag,...) printf("[Nebula]" tag " D: " __VA_ARGS__)
#else
#define LOG_DBG(...)
#define LOG_DBG_TAG(tag,...)
#endif

//Verbose Log
#if DEBUG_LEVEL > 4
#define LOG_VER(...)  printf("[Nebula] V: " __VA_ARGS__)
#define LOG_VER_TAG(tag,...) printf("[Nebula]" tag " V: " __VA_ARGS__)
#else
#define LOG_VER(...)
#define LOG_VER_TAG(tag,...)
#endif

//TMI Log
#if DEBUG_LEVEL > 5
#define LOG_TMI(...) printf("[Nebula] TMI: " __VA_ARGS__)
#define LOG_TMI_TAG(tag,...) printf("[Nebula]" tag " TMI: " __VA_ARGS__)
#else
#define LOG_TMI(...)
#define LOG_TMI_TAG(tag,...)
#endif
