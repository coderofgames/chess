#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "PGKey.h"

#include <unordered_map>

using namespace std;



class HashEntry
{
public:

	//string fen;
	uint64 key;
	int hits;
};

#endif