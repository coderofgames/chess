#ifndef BB_DEFS
#define BB_DEFS


/*
Guest Source by Lucas Braesh posted on CCC
on Sat Jul 09 2011
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

typedef unsigned long long U64;

static inline int min(int x, int y) { return x < y ? x : y;  }
static inline int max(int x, int y) { return x > y ? x : y;  }

#define NB_SQUARE 64
enum {
   A1, B1, C1, D1, E1, F1, G1, H1,
   A2, B2, C2, D2, E2, F2, G2, H2,
   A3, B3, C3, D3, E3, F3, G3, H3,
   A4, B4, C4, D4, E4, F4, G4, H4,
   A5, B5, C5, D5, E5, F5, G5, H5,
   A6, B6, C6, D6, E6, F6, G6, H6,
   A7, B7, C7, D7, E7, F7, G7, H7,
   A8, B8, C8, D8, E8, F8, G8, H8,
   NoSquare
};

#define NB_RANK_FILE 8
enum { Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8 };
enum { FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH };

#define NB_PIECE 6
enum { Pawn, Knight, Bishop, Rook, Queen, King, NoPiece };

#define NB_COLOR 2
enum { White, Black, NoColor };

#define NB_PHASE 2
enum { MiddleGame, EndGame };

static inline int rank_file_ok(int r, int f) { return 0 <= r && r < NB_RANK_FILE && 0 <= f && f < NB_RANK_FILE; }
static inline int square_ok(int sq)      { return A1 <= sq && sq <= H8; }
static inline int color_ok(int color)   { return color == White || color == Black; }
static inline int piece_ok(int piece)   { return Pawn <= piece && piece < NoPiece; }

static inline int square(int r, int f) {
   assert(rank_file_ok(r, f));
   return 8 * r + f;
}
static inline int rank(int sq) {
   assert(square_ok(sq));
   return sq / 8;
}
static inline int file(int sq) {
   assert(square_ok(sq));
   return sq % 8;
}
static inline int opp_color(int color) {
   assert(color_ok(color));
   return color ^ 1;
}
static inline int is_slider(int piece) {
   assert(piece_ok(piece)); return Bishop <= piece && piece <= Queen;
}

#endif