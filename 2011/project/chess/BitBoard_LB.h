#ifndef BITBOARD_LB_H
#define BITBOARD_LB_H

/*
Guest Source by Lucas Braesh posted on CCC
on Sat Jul 09 2011
*/
#include "bitboard_defs.h"

void init_bitboard();

extern U64 Bit[NB_SQUARE],
   Rank[NB_RANK_FILE], File[NB_RANK_FILE],
   Between[NB_SQUARE][NB_SQUARE],
    Direction[NB_SQUARE][NB_SQUARE],
   KAttacks[NB_SQUARE], NAttacks[NB_SQUARE],
    PAttacks[NB_COLOR][NB_SQUARE],
   PInitialRank[NB_COLOR], PPromotionRank[NB_COLOR];

extern U64 BPseudoAttacks[NB_SQUARE], RPseudoAttacks[NB_SQUARE];
   
void print_bitboard(U64 b);

static inline void set_bit(U64 *b, int i)   { assert(square_ok(i)); *b |= Bit[i]; }
static inline void clear_bit(U64 *b, int i)   { assert(square_ok(i)); *b &= ~Bit[i]; }
static inline U64 test_bit(U64 b, int i)   { assert(square_ok(i)); return b & Bit[i]; }
static inline U64 shift_bit(U64 b, int i)   { return i > 0 ? b << i : b >> -i;  }

int count_bit(U64 b);
int count_bit_max15(U64 b);

extern const int BitTable[NB_SQUARE];

static inline int first_bit(U64 b) {
   return BitTable[((b & -b) * 0x218a392cd3d5dbfULL) >> 58];
}
static inline int next_bit(U64 *b) {
   U64 _b = *b;
   *b &= *b - 1;
   return BitTable[((_b & -_b) * 0x218a392cd3d5dbfULL) >> 58];
}

typedef struct {
   U64 all, all_sym, all_a1h8, all_a8h1;
} occ_t;

void set_occ(occ_t *occ, int sq);
void clear_occ(occ_t *occ, int sq);
U64 rook_attack(const occ_t *occ, int sq);
U64 bishop_attack(const occ_t *occ, int sq);

static inline U64 piece_attack(const occ_t *occ, int piece, int sq)
{
   assert(Knight <= piece && piece <= King && square_ok(sq));
   
   if (piece == Knight)
      return NAttacks[sq];
   else if (piece == Bishop)
      return bishop_attack(occ, sq);
   else if (piece == Rook)
      return rook_attack(occ, sq);
   else if (piece == Queen)
      return bishop_attack(occ, sq) | rook_attack(occ, sq);
   else   // if (piece == King)
      return KAttacks[sq];
}

#endif