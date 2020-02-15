#ifndef PGKEY_H
#define PGKEY_H
#include <stdio.h>
#include <string.h>

#ifndef UINT64_DEFINED
#define UINT64_DEFINED
#ifdef _MSC_VER
  typedef unsigned __int64 uint64;
#else
  typedef unsigned long long int uint64;
#endif
#endif

  

#ifdef _MSC_VER
#  define U64(u) (u##ui64)
#else
#  define U64(u) (u##ULL)
#endif

uint64 hash_from_fen(char *fen);
#endif