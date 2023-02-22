#pragma once

/***************************************************[¿‡–Õ]***************************************************/
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef int64 llong;
typedef uint64 ullong;
typedef unsigned char uchar;
typedef int64 timestamp;

#ifdef _WIN32
#ifdef _WIN64
typedef uint64 ptr;
#else
typedef uint32 ptr;
#endif
#else
#ifdef __x86_64__
typedef uint64 ptr;
#else
typedef uint32 ptr;
#endif
#endif

