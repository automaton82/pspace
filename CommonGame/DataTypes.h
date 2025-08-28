//David Millman

#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "Platform.h"

#ifdef PLATFORM_WINDOWS
typedef unsigned __int64 Uint64;
typedef unsigned __int32 Uint32;
typedef unsigned __int16 Uint16;
typedef unsigned __int8 Uint8;
typedef unsigned int Uint;

typedef signed __int64 Sint64;
typedef signed __int32 Sint32;
typedef signed __int16 Sint16;
typedef signed __int8 Sint8;

typedef __int64 Int64;
typedef __int32 Int32;
typedef __int16 Int16;
typedef __int8 Int8;

typedef unsigned __int8 Byte;
typedef unsigned __int8 BYTE;
typedef unsigned __int16 Word;
typedef unsigned __int32 DWord;
typedef unsigned char Uchar;
#else // PLATFORM_LINUX
#include <stdint.h>

typedef uint64_t Uint64;
typedef uint32_t Uint32;
typedef uint16_t Uint16;
typedef uint8_t Uint8;
typedef unsigned int Uint;

typedef int64_t Sint64;
typedef int32_t Sint32;
typedef int16_t Sint16;
typedef int8_t Sint8;

typedef int64_t Int64;
typedef int32_t Int32;
typedef int16_t Int16;
typedef int8_t Int8;

typedef uint8_t Byte;
typedef uint8_t BYTE;
typedef uint16_t Word;
typedef uint32_t DWord;
typedef unsigned char Uchar;
#endif

typedef unsigned long Ulong;

#endif