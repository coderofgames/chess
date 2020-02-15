#ifndef BITBOARD_H
#define BITBOARD_H

namespace BITBOARD_REPRESENTATION
{
	class Square
	{
	public:
		int rank;
		int file;
	};

	class Bitboard
	{
	public:

		enum{ WHITE, BLACK };

		enum{ WHITE_MASK, BLACK_MASK, OCCUPIED, PAWNS, KNIGHTS, BISHOPS, ROOKS, QUEENS, KINGS };

		__int64 pawns;
		__int64 knights;
		__int64 bishops;
		__int64 rooks;
		__int64 queens;
		__int64 kings;

		__int64 bitboards[9];

		__int64 black_mask;
		__int64 white_mask;
		__int64 occupied;

		void SetColor( int col, int file, int rank, int bitshift )
		{
			SetBit( col, (rank*8 + file) );	
		}

		inline void SetBit( int id, int bitshift )
		{
			bitboards[id] &= 1 << bitshift;
		}

		inline void SetPawn( int file, int rank, int col )
		{
			int bshift = rank*8 + file;
			SetBit( col, bshift );	
			SetBit( OCCUPIED, bshift );	
			SetBit( PAWNS, bshift );
		}

		inline void SetKnight( int file, int rank, int col )
		{
			int bshift = rank*8 + file;
			SetBit( col, bshift );	
			SetBit( OCCUPIED, bshift );	
			SetBit( KNIGHTS, bshift );
		}

		inline void SetBishop( int file, int rank, int col )
		{
			int bshift = rank*8 + file;
			SetBit( col, bshift );	
			SetBit( OCCUPIED, bshift );	
			SetBit( BISHOPS, bshift );
		}

		inline void SetRook( int file, int rank, int col )
		{
			int bshift = rank*8 + file;
			SetBit( col, bshift );	
			SetBit( OCCUPIED, bshift );	
			SetBit( ROOKS, bshift );
		}

		inline void SetQueen( int file, int rank, int col )
		{
			int bshift = rank*8 + file;
			SetBit( col, bshift );	
			SetBit( OCCUPIED, bshift );	
			SetBit( QUEENS, bshift );
		}

		inline void SetKing( int file, int rank, int col )
		{
			int bshift = rank*8 + file;
			SetBit( col, bshift );	
			SetBit( OCCUPIED, bshift );	
			SetBit( KINGS, bshift );
		}

		inline bool SquareOccupied( int file, int rank )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[OCCUPIED] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasPawn( int file, int rank )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[PAWNS] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasKnight( int file, int rank )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[KNIGHTS] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasBishop( int file, int rank )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[BISHOPS] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasRook( int file, int rank )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[ROOKS] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasQueen( int file, int rank )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[QUEENS] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasKing( int file, int rank )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[KINGS] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasWhitePiece( int file, int rank )
		{
			return ( (bitboards[WHITE_MASK] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareHasBlackPiece( int file, int rank )
		{
			return ( (bitboards[BLACK_MASK] & (1 << (rank*8 + file))) != 0 );
		}

		inline bool SquareOccupied( int bshift)
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[OCCUPIED] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasPawn( int bshift )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[PAWNS] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasKnight( int bshift )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[KNIGHTS] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasBishop( int bshift )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[BISHOPS] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasRook( int bshift )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[ROOKS] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasQueen( int bshift )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[QUEENS] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasKing( int bshift )
		{
			//int bshift = rank*8 + file;
			return ( (bitboards[KINGS] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasWhitePiece( int bshift )
		{
			return ( (bitboards[WHITE_MASK] & (1 << bshift)) != 0 );
		}

		inline bool SquareHasBlackPiece( int bshift )
		{
			return ( (bitboards[BLACK_MASK] & (1 << bshift)) != 0 );
		}


		int GetPieceOnSquare( int file, int rank )
		{
			int bitshift = (rank * 8) + file;

			if( !SquareOccupied( bitshift ) )
				return 0;

			int retval = 0;
			if( SquareHasPawn( file, rank ) )
				retval = 0;
			
			else if( SquareHasKnight( bitshift ))
				retval = 1;
			else if( SquareHasBishop( bitshift ))
				retval = 2;
			else if( SquareHasRook( bitshift ))
				retval = 3;
			else if( SquareHasQueen( bitshift ))
				retval = 4;
			else if( SquareHasKing( bitshift ))
				retval = 5;
			

			if( SquareHasWhitePiece(bitshift) )
				retval += 6;

			return retval;


		}




	
	};

};


#endif