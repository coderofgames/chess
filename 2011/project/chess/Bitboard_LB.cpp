#include "bitboard_LB.h"

/*
Guest Source by Lucas Braesh posted on CCC
on Sat Jul 09 2011
*/

U64 Bit[NB_SQUARE],
   Rank[NB_RANK_FILE], File[NB_RANK_FILE],
   Between[NB_SQUARE][NB_SQUARE],
    Direction[NB_SQUARE][NB_SQUARE],
   KAttacks[NB_SQUARE], NAttacks[NB_SQUARE],
    PAttacks[NB_COLOR][NB_SQUARE],
   PInitialRank[NB_COLOR], PPromotionRank[NB_COLOR];

static void init_bit();
static void init_rank_file();
static void init_KNP_attacks();
static void init_rays();

U64 BPseudoAttacks[NB_SQUARE], RPseudoAttacks[NB_SQUARE];

static U64 symetric(U64 b);
static U64 rotate_a1h8(U64 b);
static U64 rotate_a8h1(U64 b);
static void init_rank_file_slides();
static void init_diag_slides();
static void init_BR_pseudo_attacks();

void init_bitboard()
{
   static int done = 0;
   if (done) return; else done = 1;
   
   init_bit();
   init_rank_file();
   init_KNP_attacks();
   init_rays();
   
   init_rank_file_slides();
   init_diag_slides();
   init_BR_pseudo_attacks();
}

static void init_bit()
{
   int i;
   for (i = A1; i <= H8; i++)
      Bit[i] = 1ULL << i;
}

static void init_rank_file()
{
   int i;
   for (i = 0; i < NB_RANK_FILE; i++) {
      Rank[i] = 0xFFULL << (8*i);
      File[i] = 0x0101010101010101ULL << i;
   }
   
   PInitialRank[White] = Rank[Rank2];
   PInitialRank[Black] = Rank[Rank7];
   
   PPromotionRank[White] = Rank[Rank8];
   PPromotionRank[Black] = Rank[Rank1];
}

static void init_KNP_attacks()
{
   const int K_dir[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
   const int N_dir[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
   const int P_dir[NB_COLOR][2][2] = { {{1,-1},{1,1}}, {{-1,-1},{-1,1}} };

   int sq, i, c;
   for (sq = A1; sq <= H8; sq++) {
      int r = rank(sq), f = file(sq);
      KAttacks[sq] = NAttacks[sq] = 0ULL;

      for (i = 0; i < 8; i++) {
         int dr = K_dir[i][0], df = K_dir[i][1];
         if (rank_file_ok(r+dr,f+df))
            set_bit(&KAttacks[sq], square(r+dr,f+df));

         dr = N_dir[i][0], df = N_dir[i][1];
         if (rank_file_ok(r+dr,f+df))
            set_bit(&NAttacks[sq], square(r+dr,f+df));
      }

      PAttacks[White][sq] = PAttacks[Black][sq] = 0ULL;
      for (i = 0; i < 2; i++)
         for (c = White; c <= Black; c++) {
            int dr = P_dir[c][i][0], df = P_dir[c][i][1];
            if (rank_file_ok(r+dr,f+df))
               set_bit(&PAttacks[c][sq], square(r+dr,f+df));
         }
   }
}

static void init_rays()
{
   const int dir[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
   memset(Between, 0, sizeof(Between));
   memset(Direction, 0, sizeof(Direction));

   int sq, i;
   for (sq = A1; sq <= H8; sq++) {
      int r = rank(sq), f = file(sq);

      for (i = 0; i < 8; i++) {
         int dr = dir[i][0], df = dir[i][1];
         int _r, _f, _sq;
         U64 mask;

         for (_r=r+dr,_f=f+df,mask=0ULL; rank_file_ok(_r,_f); _r+=dr,_f+=df) {
            _sq = square(_r,_f);
            set_bit(&mask, _sq);
            Between[sq][_sq] = mask;
         }

         U64 direction = mask;

         while (mask) {
            _sq = next_bit(&mask);
            Direction[sq][_sq] = direction;
         }
      }
   }
}

/* Bitboard primitives */

int count_bit(U64 b)
{
   b -= ((b>>1) & 0x5555555555555555ULL);
   b = ((b>>2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
   b = ((b>>4) + b) & 0x0F0F0F0F0F0F0F0FULL;
   b *= 0x0101010101010101ULL;
   return b >> 56;
}

int count_bit_max15(U64 b)
{
   b -= (b>>1) & 0x5555555555555555ULL;
   b = ((b>>2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
   b *= 0x1111111111111111ULL;
   return b >> 60;
}

const int BitTable[NB_SQUARE] = {
   0, 1, 2, 7, 3, 13, 8, 19, 4, 25, 14, 28, 9, 34, 20, 40, 5, 17, 26, 38, 15,
   46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57, 63, 6, 12, 18, 24, 27, 33, 39,
   16, 37, 45, 47, 30, 53, 49, 56, 62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43,
   51, 60, 42, 59, 58
};

/* Symetric and Rotated Bitboard code */

static const int diag_shift[] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 43, 49, 54, 58, 61, 63};
static const U64 diag_mask[] = {1, 3, 7, 15, 31, 63, 127, 255, 127, 63, 31, 15, 7, 3, 1};

static const int rot_a1h8[NB_SQUARE] = {
   A1, C1, F1, B2, G2, E3, D4, D5,
   B1, E1, A2, F2, D3, C4, C5, C6,
   D1, H1, E2, C3, B4, B5, B6, A7,
   G1, D2, B3, A4, A5, A6, H6, F7,
   C2, A3, H3, H4, H5, G6, E7, B8,
   H2, G3, G4, G5, F6, D7, A8, E8,
   F3, F4, F5, E6, C7, H7, D8, G8,
   E4, E5, D6, B7, G7, C8, F8, H8
};

static const int rot_a8h1[NB_SQUARE] = {
   D5, C6, A7, F7, B8, E8, G8, H8,
   D4, C5, B6, H6, E7, A8, D8, F8,
   E3, C4, B5, A6, G6, D7, H7, C8,
   G2, D3, B4, A5, H5, F6, C7, G7,
   B2, F2, C3, A4, H4, G5, E6, B7,
   F1, A2, E2, B3, H3, G4, F5, D6,
   C1, E1, H1, D2, A3, G3, F4, E5,
   A1, B1, D1, G1, C2, H2, F3, E4
};

static U64 symetric(U64 b)
// returns the symetric of b with respect to a1h8
{
   static const int sym[] = {
      A1, A2, A3, A4, A5, A6, A7, A8,
      B1, B2, B3, B4, B5, B6, B7, B8,
      C1, C2, C3, C4, C5, C6, C7, C8,
      D1, D2, D3, D4, D5, D6, D7, D8,
      E1, E2, E3, E4, E5, E6, E7, E8,
      F1, F2, F3, F4, F5, F6, F7, F8,
      G1, G2, G3, G4, G5, G6, G7, G8,
      H1, H2, H3, H4, H5, H6, H7, H8
   };
   int i;
   U64 result = 0;

   for (i = 0; i < 64; i++)
      if (test_bit(b, sym[i]))
         set_bit(&result, i);

   return result;
}

static U64 rotate_a1h8(U64 b)
{   
   int i;
   U64 result = 0;

   for (i = 0; i < 64; i++)
      if (test_bit(b, i)) set_bit(&result, rot_a1h8[i]);

   return result;
}

static U64 rotate_a8h1(U64 b)
{   
   int i;
   U64 result = 0;

   for (i = 0; i < 64; i++)
      if (test_bit(b, i)) set_bit(&result, rot_a8h1[i]);

   return result;
}

static U64 rank_slides[NB_RANK_FILE][0x100], file_slides[NB_RANK_FILE][0x100],
    diag_a1h8_slides[NB_SQUARE][0x100], diag_a8h1_slides[NB_SQUARE][0x100];

static void init_rank_file_slides()
{
   memset(rank_slides, 0, sizeof(rank_slides));
   memset(file_slides, 0, sizeof(file_slides));

   int i, occ;
   for (i = 0; i < NB_RANK_FILE; i++)
      for (occ = 0; occ < 0x100; occ++) {
         U64 mask = 0;
         int f;

         // calculate rank_slides
         for (f = i-1; f >= 0; f--) {
            set_bit(&mask, f);
            if (test_bit(occ, f)) break;
         }
         for (f = i+1; f < 8; f++) {
            set_bit(&mask, f);
            if (test_bit(occ, f)) break;
         }
         rank_slides[i][occ] = mask;

         // calculate file_slides (just symetric)
         file_slides[i][occ] = symetric(mask);
      }
}

static void init_diag_slides()
{
   memset(diag_a1h8_slides, 0, sizeof(diag_a1h8_slides));
   memset(diag_a8h1_slides, 0, sizeof(diag_a8h1_slides));

   int sq, occ, i;
   for (sq = A1; sq <= H8; sq++) {
      int f = file(sq), r = rank(sq);

      // calculate diag_a1h8_slides
      for (occ = 0; occ <= diag_mask[f + r]; occ++) {
         int _sq = r + f < 8            // first square of the diag a1h8 containing sq
         ? square(r + f, 0)
         : square(7, f + r - 7);
         int pos = r + f < 8 ? f : 7 - r;   // pos on the diag: top left -> bottom right
         U64 mask = rank_slides[pos][occ] & diag_mask[f + r];   // copy this onto the diag

         for (i = 0; i < 8; i++, _sq -= 7)
            if (test_bit(mask, i))
               set_bit(&diag_a1h8_slides[sq][occ], _sq);
      }

      // calculate diag_a8h1_slides
      for (occ = 0; occ <= diag_mask[f + 7-r]; occ++) {
         int _sq = 7-r + f < 8         // first square of the diag a8h1 containing sq
         ? square(7, f + 7-r)
         : square(r + 7-f, 7);
         int pos = 7-r + f < 8 ? 7-r : 7-f;   // pos on the diag: top right -> bottom left
         U64 mask = rank_slides[pos][occ] & diag_mask[f + 7-r];   // copy this onto the diag

         for (i = 0; i < 8; i++, _sq -= 9)
            if (test_bit(mask, i))
               set_bit(&diag_a8h1_slides[sq][occ], _sq);
      }
   }
}

static void init_BR_pseudo_attacks()
{
   occ_t occ;
   memset(&occ, 0, sizeof(occ_t));

   int sq;
   for (sq = A1; sq <= H8; sq++) {
      BPseudoAttacks[sq] = bishop_attack(&occ, sq);
      RPseudoAttacks[sq] = rook_attack(&occ, sq);
   }
}

/* Occupancy primitives */

void clear_occ(occ_t *occ, int sq)
{
   assert(square_ok(sq));
   clear_bit(&occ->all, sq);
   clear_bit(&occ->all_sym, square(file(sq), rank(sq)));
   clear_bit(&occ->all_a1h8, rot_a1h8[sq]);
   clear_bit(&occ->all_a8h1, rot_a8h1[sq]);
}

void set_occ(occ_t *occ, int sq)
{
   assert(square_ok(sq));
   set_bit(&occ->all, sq);
   set_bit(&occ->all_sym, square(file(sq), rank(sq)));
   set_bit(&occ->all_a1h8, rot_a1h8[sq]);
   set_bit(&occ->all_a8h1, rot_a8h1[sq]);
}

U64 rook_attack(const occ_t *occ, int sq)
{
   assert(square_ok(sq));
   int r = rank(sq), f = file(sq);

   return (rank_slides[f][(occ->all >> (r * 8)) & 0xFF] << (r * 8))
          | (file_slides[r][(occ->all_sym >> (f * 8)) & 0xFF] << f);
}

U64 bishop_attack(const occ_t *occ, int sq)
{
   assert(square_ok(sq));
   int r = rank(sq), f = file(sq),
       diagno_a1h8 = r + f,
       diagno_a8h1 = 7-r + f;

   U64 occ_a1h8 = (occ->all_a1h8 >> diag_shift[diagno_a1h8]) & diag_mask[diagno_a1h8],
       occ_a8h1 = (occ->all_a8h1 >> diag_shift[diagno_a8h1]) & diag_mask[diagno_a8h1];

   return diag_a1h8_slides[sq][occ_a1h8] | diag_a8h1_slides[sq][occ_a8h1];
}

void print_bitboard(U64 b)
{
   int r, f;
   for (r = Rank8; r >= Rank1; r--) {
      for (f = FileA; f <= FileH; f++) {
         int sq = square(r, f);
         char c = test_bit(b, sq)
            ? 'X'
            : '.';
         printf(" %c", c);
      }
      printf("\n");
   }
} 

/*
move_t *gen_piece_moves(const Board *B, U64 targets, move_t *mlist, int king_moves)
// Generates piece moves, when the board is not in check. Uses targets to filter the tss, eg.
// targets = ~friends (all moves), empty (quiet moves only), enemies (captures only). 
{
   assert(!king_moves || !board_is_check(B));
   const int us = B->turn, them = opp_color(us);
   const int last_piece = king_moves ? King : Queen;
   assert(!(targets & B->all[us]));
   U64 fss, tss;
   
   for (int piece = Bishop; piece <= last_piece; piece++) {
      fss = B->b[us][piece];

      while (fss) {
         int fsq = next_bit(&fss);
         U64 tss = piece_attack(&B->occ, piece, fsq) & targets;

         if (piece == King)   // filter direct self checks
            tss &= ~B->st->attacks;

         while (tss) {
            int tsq = next_bit(&tss);
            mlist = serialize_moves(B, fsq, tsq, mlist);
         }
      }
   }

   return mlist;
} 

static inline move_t *serialize_moves(const Board *B, int fsq, int tsq, move_t *mlist)
// * Centralise the move generation here: given (fsq,tsq) the rest follows. We handle here all the
// * annoying cases (indirect self-check through fsq, or through the ep captured square). Note that
// * direct self-checks aren't generated, so we don't check them here. In other words, we never put
// * our King on squares attacked by the enemy before calling this function 
{
   assert(square_ok(fsq) && square_ok(tsq));
   const int us = B->turn, them = opp_color(us),
      kpos = B->king_pos[us];

   if ((test_bit(B->st->pinned, fsq))         // pinned piece
   &&   (!test_bit(Direction[kpos][fsq], tsq)))   // moves out of pin ray
      return mlist;   // illegal move by indirect self check

   move_t m;
   m.fsq = fsq;
   m.tsq = tsq;
   m.piece = B->piece_on[fsq];
   m.capture = B->piece_on[tsq];
   m.ep = Pawn == m.piece && tsq == B->st->epsq;

   if (m.ep) {
      occ_t occ = B->occ;
      // play the ep capture on occ
      clear_occ(&occ, m.fsq);
      clear_occ(&occ, m.tsq + (us ? 8 : -8));   // remove the ep captured enemy pawn
      set_occ(&occ, m.tsq);
      // test for check by a sliding enemy piece      
      if ((get_RQ(B, them) & RPseudoAttacks[kpos] & rook_attack(&occ, kpos))
      || (get_BQ(B, them) & BPseudoAttacks[kpos] & bishop_attack(&occ, kpos)))
         return mlist;   // illegal move by indirect self check (through the ep captured pawn)
   }

   // promotions
   if (Pawn == m.piece && test_bit(PPromotionRank[us], tsq)) {
      m.promotion = Knight; *mlist++ = m;
      m.promotion = Bishop; *mlist++ = m;
      m.promotion = Rook;     *mlist++ = m;
      m.promotion = Queen;  *mlist++ = m;
   }
   // all other moves
   else {
      m.promotion = NoPiece;
      *mlist++ = m;
   }

   return mlist;
} 
*/