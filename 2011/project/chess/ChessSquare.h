#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include <string>

using std::string;

class MyBigChessSquare
{
public:
	void Set(int x, int y)
	{
		file = x;
		rank = y;
	}
	int file;
	int rank;
};



class MyChessSquare
{
public:
	MyChessSquare(){}

	void Set(int x, int y)
	{
		file = x;
		rank = y;
	}

	inline bool operator==(MyChessSquare &other)
	{
		return packed == other.packed;
	}

	inline void operator=(MyChessSquare &other)
	{
		packed = other.packed;
	}
	union
	{
		struct 
		{
			unsigned char rank :3;
			unsigned char file :3;
			unsigned char pad : 2;
		};
		unsigned char packed;
	};
};

#define FILE_MASK 11110000
#define RANK_MASK 00001111

class CompressedSquare
{
public:
	CompressedSquare()
	{
	}

	int GetFile()
	{
		return (int)(sq>>4)<<4;
	}

	int GetRank()
	{
		return (int)(sq<<4)>>4;
	}

	void SetFile(int f)
	{
		sq &= (sq & FILE_MASK & f);
	}

	void SetRank(int r)
	{
		sq &= (sq & RANK_MASK & r);
	}

	void operator=(CompressedSquare &rhs)
	{
		sq = rhs.sq;
	}

	unsigned char sq;
	
};

//enum {promoQueen, promoRook, promoKnight, promoBishop, castle};

class MyChessMove
{
public:

	union
	{
		struct
		{
			MyChessSquare from;
			MyChessSquare to;
		};
		unsigned short int packed; 
	};

	int promo;
	int castle;

	MyChessMove()
	{
		promo = -1;
		castle = 0;
	}
	
	inline bool operator==(MyChessMove &rhs)
	{
		return (from.file == rhs.from.file) &&
			   (from.rank == rhs.from.rank) &&
			   (to.file == rhs.to.file) &&
			   (to.rank == rhs.to.rank) &&
			   (promo == rhs.promo);
		/*return ((packed == rhs.packed)&&
				(promo == rhs.promo));
				*/
	}

	inline void operator=(MyChessMove &rhs)
	{
		packed = rhs.packed;
		promo = rhs.promo;
	}

	void Compress()
	{
		if( promo!= -1 )
		{
			from.pad = promo;
			to.pad = 1;
		}
	}

	string toString()
	{
		char c1=0;
		switch( from.file )
		{
		case 0:c1 = 'a';break;
		case 1:c1 = 'b';break;
		case 2:c1 = 'c';break;
		case 3:c1 = 'd';break;
		case 4:c1 = 'e';break;
		case 5:c1 = 'f';break;
		case 6:c1 = 'g';break;
		case 7:c1 = 'h';break;
		default: c1 = '0'; break;
		}

		char c2=0;
		switch( from.rank )
		{
		case 0:c2 = '1';break;
		case 1:c2 = '2';break;
		case 2:c2 = '3';break;
		case 3:c2 = '4';break;
		case 4:c2 = '5';break;
		case 5:c2 = '6';break;
		case 6:c2 = '7';break;
		case 7:c2 = '8';break;
		default: c2 = '0'; break;
		}

		char c3=0;
		switch( to.file )
		{
		case 0:c3 = 'a';break;
		case 1:c3 = 'b';break;
		case 2:c3 = 'c';break;
		case 3:c3 = 'd';break;
		case 4:c3 = 'e';break;
		case 5:c3 = 'f';break;
		case 6:c3 = 'g';break;
		case 7:c3 = 'h';break;
		default: c3 = '0'; break;
		}

		char c4=0;
		switch( to.rank )
		{
		case 0:c4 = '1';break;
		case 1:c4 = '2';break;
		case 2:c4 = '3';break;
		case 3:c4 = '4';break;
		case 4:c4 = '5';break;
		case 5:c4 = '6';break;
		case 6:c4 = '7';break;
		case 7:c4 = '8';break;
		default: c4 = '0'; break;
		}

		char c5 = 0;
		switch( promo )
		{
		case 0: c5 = 'q'; break;
		case 1: c5 = 'b';break;
		case 2: c5 = 'n';break;
		case 3: c5= 'r'; break;
		}

		
		string out;
		out+=c1;
		out+=c2;
		out+=c3;
		out+=c4;
		if( promo != -1 )
		{
			out+=c5;
		}

		if( (c1 == '0') || (c2 == '0') || (c3 == '0') || (c4 == '0') )
		{
			return "";


		}
		else
		{
			return out;
		}

	}
};


class MyBigChessMove
{
public:

	MyChessMove toMove()
	{
		MyChessMove mv;
		mv.from.file = from.file;
		mv.from.rank = from.rank;
		mv.to.file= to.file;
		mv.to.rank = to.rank;
		mv.promo = promo;

		return mv;
	}

	inline bool operator==(MyBigChessMove &rhs)
	{
		return (from.file == rhs.from.file) &&
			   (from.rank == rhs.from.rank) &&
			   (to.file == rhs.to.file) &&
			   (to.rank == rhs.to.rank) &&
			   (promo == rhs.promo);
		/*return ((packed == rhs.packed)&&
				(promo == rhs.promo));
				*/
	}

	inline void operator=(MyBigChessMove &rhs)
	{
		from.file = rhs.from.file;
		from.rank = rhs.from.rank;
		to.file = rhs.to.file;
		to.rank = rhs.to.rank;
		promo = rhs.promo;
	}

	MyBigChessSquare from;
	MyBigChessSquare to;
	int promo;
	int castle;
};

#define PROMOTION 0
#define CASTLE 1

class CompressedChessMove
{
public:

	inline bool operator==(CompressedChessMove &rhs)
	{
		return ((from.sq==rhs.from.sq) && (to.sq==rhs.to.sq) && (flag == rhs.flag));
	}
	inline void operator=(CompressedChessMove &rhs)
	{
		from = rhs.from;
		to = rhs.to;
		flag = rhs.flag; 
	}
	CompressedSquare from;
	CompressedSquare to;
	unsigned char flag;
};

// graphics helper
class DirVec
{
public:
	float x,y;

	void Normalize()
	{
		x=x/sqrtf(x*x+y*y);
		y=y/sqrtf(x*x+y*y);
	}
};

#endif