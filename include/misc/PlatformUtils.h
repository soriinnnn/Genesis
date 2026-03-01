#ifndef GENESIS_PLATFORM_UTILS_H
#define GENESIS_PLATFORM_UTILS_H

#if _WIN32
#define GENESIS_TEXT(string) L##string
#else
#define GENESIS_TEXT(string) string
#endif

#endif