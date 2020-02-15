#ifndef PGLOOKUP_H
#define PGLOOKUP_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#ifndef UINT64_DEFINED
#define UINT64_DEFINED
#ifdef _MSC_VER
  typedef unsigned __int64 uint64;
#else
  typedef unsigned long long int uint64;
#endif
#endif

#include <string>

struct entry_t {
	std::string san;
    uint64 key;	
    uint16 move;
    uint16 weight;
    uint16 learn; // was 32
	uint16 n; // never existed
};

int int_from_file(FILE *f, int l, uint64 *r);
int entry_from_file(FILE *f, entry_t *entry);
int find_key(FILE *f, uint64 key, entry_t *entry);
void move_to_string(char move_s[6], uint16 move);

#endif