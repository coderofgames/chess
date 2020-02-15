#ifndef BYTEBOARD_H
#define BYTEBOARD_H


//#include <gl/glew.h>
//#include "wx/glcanvas.h"
//#include "Image.h"
//#include "ARB_Multisample.h"
//#include "Drawable.h"


#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif


/*#include "GLFont.h"
*/
#include "PgnLoad.h"
//#include "ChessPiece.h"
#include <ctype.h>
enum{ KINGSAFE, CHECK, CHECKMATE, STALEMATE, DRAW_BY_REPETITION, DRAW_BY_50_MOVE, DRAW_BY_INSUFFICIENT_MATERIAL, PLAYING };


/*
#ifdef __DARWIN__
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif
*/

#include <vector>

using namespace std;

#include "ChessSquare.h"

class MyChessPiece;


class ChessSquare 
{
public: 
	ChessSquare(){}
	~ChessSquare(){}

	MyChessSquare sq;

	MyChessPiece *piece;
};

/*
MyChessSquare c1a3[3] = { {2,0}, {1,1}, {0,2} };
MyChessSquare e1a5[5] = { {4,0}, {3,1}, {2,2}, {1,3}, {0,4} };
MyChessSquare g1a7[7] = { {6,0}, {5,1}, {4,2}, {3,3}, {2,4}, {1,5}, {0,6} };
MyChessSquare h2b8[7] = { {7,1}, {6,2}, {5,3}, {4,4}, {3,5}, {2,6}, {1,7} };
MyChessSquare h4d8[5] = { {7,3}, {6,4}, {5,5}, {4,6}, {3,7} };
MyChessSquare h6f8[3] = { {7,5}, {6,6}, {5,7} };

MyChessSquare g1h2[2] = { {6,0}, {7,1} };
MyChessSquare e1h4[4] = { {4,0}, {5,1}, {6,2}, {7,3} };
MyChessSquare c1h6[6] = { {2,0}, {3,1}, {4,2}, {5,3}, {6,4}, {7,5} };
MyChessSquare a1h8[8] = { {0,0}, {1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}, {7,7} };
MyChessSquare a3f8[6] = { {0,2}, {1,3}, {2,4}, {3,5}, {4,6}, {5,7} };
MyChessSquare a5d8[4] = { {0,4}, {1,5}, {2,6}, {3,7} };
MyChessSquare a7b8[2] = { {0,6}, {1,7} };


MyChessSquare b1h7[7] = { {1,0}, {2,1}, {3,2}, {4,3}, {5,4}, {6,5}, {7,6} };
MyChessSquare d1h5[5] = { {3,0}, {4,1}, {5,2}, {6,3}, {7,4} };
MyChessSquare f1h3[3] = { {5,0}, {6,1}, {7,2} };
MyChessSquare a2g8[7] = { {0,1}, {1,2}, {2,3}, {3,4}, {4,5}, {5,6}, {6,7} };
MyChessSquare a4e8[5] = { {0,3}, {1,4}, {2,5}, {3,6}, {4,7} };
MyChessSquare a6c8[3] = { {0,5}, {1,6}, {2,7} };

MyChessSquare b1a2[2] = { {1,0}, {0,1} };
MyChessSquare d1a4[4] = { {3,0}, {2,1}, {1,2}, {0,3} };
MyChessSquare f1a6[6] = { {5,0}, {4,1}, {3,2}, {2,3}, {1,4}, {0,5} };
MyChessSquare h1a8[8] = { {7,0}, {6,1}, {5,2}, {4,3}, {3,4}, {2,5}, {1,6}, {0,7} };
MyChessSquare h3c8[6] = { {7,2}, {6,3}, {5,4}, {4,5}, {3,6}, {2,7} };
MyChessSquare h5e8[4] = { {7,4}, {6,5}, {5,6}, {4,7} };
MyChessSquare h7g8[2] = { {7,6}, {6,7} };
*/



#ifndef WHITE
enum {WHITE, BLACK};
#endif
enum {Pawn, Knight, Bishop, Rook, Queen, King};

class MyChessPiece
{
public:
	MyChessPiece()
	{
		bdark_squared = false;
		bCaptured = false;
		bHasMoved = false;
	}

	// special case of bishop
	void SetDarkSquare(bool b)
	{
		bdark_squared = true;
	}

	void SetSquare(int x1, int y1)
	{
		x=x1; y = y1;
	}
	void SetColor( int col )
	{
		color = col;
	}
	void SetType(int Type)
	{
		type = Type;
	}

	void SetCaptured(bool bcap)
	{
		bCaptured = bcap;
	}

	int type;
	int color;
	bool bdark_squared;
	MyChessSquare sq;
	int x;
	int y;
	bool bCaptured;
	bool bHasMoved;
	ChessSquare *currSquare;
	int PinFromX;
	int PinFromY;
};




class ChessBoard
{
public:

	enum {A=0,B,C,D,E,F,G,H};
	unsigned char board[8][8];
	unsigned char old_board[8][8];
	
	//unsigned char mask[8][8];
	//char cboard[64];
	bool postReset;

	enum{ bPawn=1, bKnight, bBishop, bRook, bQueen, bKing,
		wPawn, wKnight, wBishop, wRook, wQueen, wKing};

	enum {BPawn=1, BKnight, BBishop, BRook, BQueen, BKing, WPawn, WKnight, WBishop, WRook, WQueen, WKing};

	
//	ChessSquare squares[8][8];
	

	//enum {WHITE, BLACK};

	//Drawable *textures[12]; // indexed by textures[ board[ i ][ j ] - 1 ];

	wxBitmap *svgTextures[12];

	float board_display_size;

	enum{a=0,b,c,d,e,f,g,h};

	MyChessPiece pieces[32];
	//MyChessPiece *white_pieces[16];
	//MyChessPiece *black_pieces[16];
	vector< MyChessPiece* > whitePcs;
	vector< MyChessPiece* > blackPcs;

	bool extraWhiteRooks;
	bool extraWhiteKnights;
	bool extraWhiteBishops;
	bool extraWhiteQueens;

	bool extraBlackRooks;
	bool extraBlackKnights;
	bool extraBlackBishops;
	bool extraBlackQueens;

	bool whiteCastled;
	bool blackCastled;


	ChessSquare empassantTarget;
	bool canEmpasse;

	int fiftyMov;
	int movCount;

	MyChessMove currMove;

	int GameStatus;



	ChessBoard()
	{
		
		postReset = false;


		mov = 0;
		GameStatus = PLAYING;


		// clear the board
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{
				board[i][j] = 0;
				//cboard[i*8+j] = 0;
			}
		}

		for (int i=0; i< 12; i++)
		{
			svgTextures[i] = NULL;
		}

	
		SetInitialPosition();


	}

	~ChessBoard()
	{
		for (int i=0; i< 12; i++)
		{
			/*if( textures[i] )
			{
				delete textures[i];
				textures[i]=NULL;
			}*/
			if( svgTextures[i] )
			{
				delete svgTextures[i];
				svgTextures[i] = NULL;
			}
		}
	}

	inline bool FiftyMoveTest(){return this->fiftyMov >= 100;}

	inline int GetPieceIDForTargetSquare( MyChessMove* target )
	{
		return board[target->to.file][target->to.rank];
	}

	inline char PieceCodeToChar( int pieceCode )
	{
		switch( pieceCode )
		{
		case wPawn: return 'P';
		case wRook: return 'R';
		case wKnight: return 'N';
		case wBishop: return 'B';
		case wQueen: return 'Q';
		case wKing: return 'K';
		case bPawn: return 'p';
		case bRook:return 'r';
		case bKnight:return 'n';
		case bBishop:return 'b';
		case bQueen:return 'q';
		case bKing:return 'k';
		};

		return '0';
	}

	void InitPieces()
	{
		for( int i = 0; i < whitePcs.size(); i++ )
			whitePcs.pop_back();
		whitePcs.clear();
		for( int i = 0; i < blackPcs.size(); i++ )
			blackPcs.pop_back();
		blackPcs.clear();

		pieces[0].type = Rook;
		pieces[0].color = WHITE;
		pieces[0].SetSquare(0,0);
		whitePcs.push_back(&pieces[0]);
		//whiteRooks.push_back(&pieces[0]);

		pieces[1].type = Knight;
		pieces[1].color = WHITE;
		pieces[1].SetSquare(1,0);
		whitePcs.push_back(&pieces[1]);

		pieces[2].type = Bishop;
		pieces[2].color = WHITE;
		pieces[2].SetSquare(2,0);
		//pieces[2].SetDarkSquare(true);
		whitePcs.push_back(&pieces[2]);

		pieces[3].type = Queen;
		pieces[3].color = WHITE;
		pieces[3].SetSquare(3,0);
		whitePcs.push_back(&pieces[3]);

		pieces[4].type = King;
		pieces[4].color = WHITE;
		pieces[4].SetSquare(4,0);
		whitePcs.push_back(&pieces[4]);

		pieces[5].type = Bishop;
		pieces[5].color = WHITE;
		pieces[5].SetSquare(5,0);
		pieces[5].SetDarkSquare(true);
		whitePcs.push_back(&pieces[5]);

		pieces[6].type = Knight;
		pieces[6].color = WHITE;
		pieces[6].SetSquare(6,0);
		//pieces[6].SetDarkSquare(false);
		whitePcs.push_back(&pieces[6]);

		pieces[7].type = Rook;
		pieces[7].color = WHITE;
		pieces[7].SetSquare(7,0);
		whitePcs.push_back(&pieces[7]);


		for( int i = 0; i < 8; i++ )
		{
			pieces[8+i].type = Pawn;
			pieces[8+i].color = WHITE;
			pieces[8+i].SetSquare(i,1);
			whitePcs.push_back(&pieces[8+i]);
		}

		pieces[16].type = Rook;
		pieces[16].color = BLACK;
		pieces[16].SetSquare(0,7);
		blackPcs.push_back(&pieces[16]);

		pieces[17].type = Knight;
		pieces[17].color = BLACK;
		pieces[17].SetSquare(1,7);
		blackPcs.push_back(&pieces[17]);

		pieces[18].type = Bishop;
		pieces[18].color = BLACK;
		pieces[18].SetSquare(2,7);
		pieces[18].SetDarkSquare(true);
		blackPcs.push_back(&pieces[18]);

		pieces[19].type = Queen;
		pieces[19].color = BLACK;
		pieces[19].SetSquare(3,7);
		blackPcs.push_back(&pieces[19]);

		pieces[20].type = King;
		pieces[20].color = BLACK;
		pieces[20].SetSquare(4,7);
		blackPcs.push_back(&pieces[20]);

		pieces[21].type = Bishop;
		pieces[21].color = BLACK;
		pieces[21].SetSquare(5,7);
		//pieces[21].SetDarkSquare(true);
		blackPcs.push_back(&pieces[21]);

		pieces[22].type = Knight;
		pieces[22].color = BLACK;
		pieces[22].SetSquare(6,7);
		//pieces[22].SetDarkSquare(false);
		blackPcs.push_back(&pieces[22]);

		pieces[23].type = Rook;
		pieces[23].color = BLACK;
		pieces[23].SetSquare(7,7);	
		blackPcs.push_back(&pieces[23]);

		for( int i = 0; i < 8; i++ )
		{
			pieces[24+i].type = Pawn;
			pieces[24+i].color = BLACK;
			pieces[24+i].SetSquare(i,6);
			blackPcs.push_back(&pieces[24+i]);
		}
	}

	void SetPosition2Kings()
	{

		ClearBoard();
		fiftyMov = 0;
		movCount = 0;
		mov=0;
		postReset = false;
		
		InitPieces();

		/*
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 2; j < 7; j++ )
			{
				SetSquare( i, j, 0);
			}
			SetSquare( i, 2, wPawn);
			SetSquare( i, 7, bPawn);
		}*/

		extraWhiteRooks = false;
		extraWhiteKnights = false;
		extraWhiteBishops = false;
		extraWhiteQueens = false;

		extraBlackRooks = false;
		extraBlackKnights = false;
		extraBlackBishops = false;
		extraBlackQueens = false;

		whiteCastled = false;
		blackCastled = false;
		canEmpasse = false;

		GameStatus = PLAYING;

		for( int i = 0; i < 32; i++)
		{
			pieces[i].bCaptured = true;
			//pieces[i].
			pieces[i].bHasMoved = true;
		}


		//SetSquare( a, 1, wRook );
		//SetSquare( b, 1, wKnight );
		//SetSquare( c, 1, wBishop );
		//SetSquare( d, 1, wQueen );
		SetSquare( e, 1, wKing );
		//SetSquare( f, 1, wBishop );
		//SetSquare( g, 1, wKnight );
		//SetSquare( h, 1,  wRook);

		//SetSquare( a, 8, bRook );
		//SetSquare( b, 8, bKnight );
		//SetSquare( c, 8, bBishop );
		//SetSquare( d, 8, bQueen );
		SetSquare( e, 8, bKing );
		//SetSquare( f, 8, bBishop );
		//SetSquare( g, 8, bKnight );
		//SetSquare( h, 8,  bRook );

		/*for( int i = 0; i < 8; i++ )
		{
			SetSquare( i, 2, wPawn);
			SetSquare( i, 7, bPawn);
		}*/

		//memcpy( (void*)old_board, (void*)board, sizeof(unsigned char)*64);
	}

	void DropPiece(int texID, int X, int Y)
	{
		board[X][Y] = texID;
		//pnbrqk pnbrqk
	/*	switch( texID )
		{
		case 0:	board[X][Y] = wPawn; break;
		case 1: board[X][Y] = wKnight; break;
		case 2: board[X][Y] = wBishop; break;
		case 3: board[X][Y] = wRook; break;
		case 4: board[X][Y] = wQueen; break;
		case 5: board[X][Y] = wKing; break;
		case 6: board[X][Y] = bPawn; break;
		case 7: board[X][Y] = bKnight; break;
		case 8: board[X][Y] = bBishop; break;
		case 9: board[X][Y] = bRook; break;
		case 10: board[X][Y] = bQueen; break;
		case 11: board[X][Y] = bKing; break;

		};
		*/
	}

	void SetupPositionSetBlackCanCastleQueenside()
	{
		this->blackPcs[4]->bHasMoved = false;
		this->blackPcs[0]->bHasMoved = false;
	}

	void SetupPositionSetBlackCanCastleKingside()
	{
		this->blackPcs[4]->bHasMoved = false;
		this->blackPcs[7]->bHasMoved = false;
	}

	void SetupPositionSetWhiteCanCastleQueenside()
	{
		this->whitePcs[4]->bHasMoved = false;
		this->whitePcs[0]->bHasMoved = false;
	}

	void SetupPositionSetWhiteCanCastleKingside()
	{
		this->whitePcs[4]->bHasMoved = false;
		this->whitePcs[7]->bHasMoved = false;
	}


	void SetupPositionSetBlackCan_not_CastleQueenside()
	{
		this->blackPcs[4]->bHasMoved = true;
	//	this->blackPcs[0]->bHasMoved = false;
	}

	void SetupPositionSetBlackCan_not_CastleKingside()
	{
		this->blackPcs[4]->bHasMoved = true;
	//	this->blackPcs[7]->bHasMoved = false;
	}

	void SetupPositionSetWhiteCan_not_CastleQueenside()
	{
		this->whitePcs[4]->bHasMoved = true;
		//this->whitePcs[0]->bHasMoved = false;
	}

	void SetupPositionSetWhiteCan_not_CastleKingside()
	{
		this->whitePcs[4]->bHasMoved = true;
		//this->whitePcs[7]->bHasMoved = false;
	}

	void SetInitialPosition()
	{
		fiftyMov = 0;
		movCount = 0;
		mov=0;
		postReset = false;
		
		InitPieces();


		for( int i = 0; i < 8; i++ )
		{
			for( int j = 2; j < 7; j++ )
			{
				SetSquare( i, j, 0);
			}
			SetSquare( i, 2, wPawn);
			SetSquare( i, 7, bPawn);
		}

		extraWhiteRooks = false;
		extraWhiteKnights = false;
		extraWhiteBishops = false;
		extraWhiteQueens = false;

		extraBlackRooks = false;
		extraBlackKnights = false;
		extraBlackBishops = false;
		extraBlackQueens = false;

		whiteCastled = false;
		blackCastled = false;
		canEmpasse = false;

		GameStatus = PLAYING;

		for( int i = 0; i < 32; i++)
		{
			pieces[i].bCaptured = false;
			//pieces[i].
			pieces[i].bHasMoved = false;
		}


		SetSquare( a, 1, wRook );
		SetSquare( b, 1, wKnight );
		SetSquare( c, 1, wBishop );
		SetSquare( d, 1, wQueen );
		SetSquare( e, 1, wKing );
		SetSquare( f, 1, wBishop );
		SetSquare( g, 1, wKnight );
		SetSquare( h, 1,  wRook);

		SetSquare( a, 8, bRook );
		SetSquare( b, 8, bKnight );
		SetSquare( c, 8, bBishop );
		SetSquare( d, 8, bQueen );
		SetSquare( e, 8, bKing );
		SetSquare( f, 8, bBishop );
		SetSquare( g, 8, bKnight );
		SetSquare( h, 8,  bRook );

		for( int i = 0; i < 8; i++ )
		{
			SetSquare( i, 2, wPawn);
			SetSquare( i, 7, bPawn);
		}

		memcpy( (void*)old_board, (void*)board, sizeof(unsigned char)*64);
	}

	string Fen()
	{
		string output;
		int blankSquares = 0;
		
		for( int y = 7; y> -1; y-- )
		{
			for( int x = 0; x <8; x++ )
			{
				int index = y*8+x;
				if( board[x][y]!= 0)
				{
					if( blankSquares > 0 )
					{
						std::stringstream blanks;
						blanks << blankSquares;
						output += blanks.str();
						//output += "/";
						blankSquares = 0;
					}

					if( board[x][y] == wPawn )
					{
						output+='P';
					}
					if( board[x][y] == wRook )
					{
						output+='R';
					}
					if( board[x][y] == wKnight )
					{
						output+='N';
					}
					if( board[x][y] == wBishop )
					{
						output+='B';
					}
					if( board[x][y] == wQueen )
					{
						output+='Q';
					}
					if( board[x][y] == wKing )
					{
						output+='K';
					}
					if( board[x][y] == bPawn )
					{
						output+='p';
					}
					if( board[x][y] == bRook )
					{
						output+='r';
					}
					if( board[x][y] == bKnight )
					{
						output+='n';
					}
					if( board[x][y] == bBishop )
					{
						output+='b';
					}
					if( board[x][y] == bQueen )
					{
						output+='q';
					}
					if( board[x][y] == bKing )
					{
						output+='k';
					}

				}
				else
				{
					blankSquares++;
				}

				if( index % 8 == 7 )
				{
					if (blankSquares > 0)
					{
						std::stringstream blanks;
						blanks << blankSquares;
						output += blanks.str();
						output += "/";
						blankSquares = 0;
					}
					else
					{
						if (x > 0 && x != 63)
						{
							output += "/";
						}
					}
				}
			}
		}

		if( output[output.length()-1] == '/' )
		{
			output[output.length()-1] = ' ';
		}

		if( mov == 0 || mov == 2)
		{
			output += "w ";
		}
		else
		{
			output += "b ";
		}

		string spacer;
		bool canCastle = false;
		if( canWhiteCastleShort() )
		{
			output+="K";
			spacer = " ";
			canCastle = true;
		}
		if( canWhiteCastleLong() )
		{
			output+="Q";
			spacer = " ";
			canCastle = true;
		}
		if( canBlackCastleShort() )
		{
			output+="k";
			spacer = " ";
			canCastle = true;
		}
		if( canBlackCastleLong() )
		{
			output+="q";
			spacer = " ";
			canCastle = true;
		}
		
		if( !canCastle )
		{
			output+="-";
			spacer = " ";
		}

		if( canEmpasse )
		{
			output += spacer;
			switch(empassantTarget.sq.file)
			{
			case 0: output+= "a"; break;
			case 1: output+= "b"; break;
			case 2: output+= "c"; break;
			case 3: output+= "d"; break;
			case 4: output+= "e"; break;
			case 5: output+= "f"; break;
			case 6: output+= "g"; break;
			case 7: output+= "h"; break;
			}
			//output+= this->empassantTarget.sq.rank + 1;
			std::stringstream blanks;
			blanks << (empassantTarget.sq.rank + 1);
			output += blanks.str();
		}
		else
		{
			output += spacer + "-";
		}

		std::stringstream fiftyMovInfoStrStr;

		if( mov == 1 )
			fiftyMovInfoStrStr << (fiftyMov);
		else
			fiftyMovInfoStrStr << (fiftyMov);

		output+= " "+ fiftyMovInfoStrStr.str()+ " ";
		//movInfo.flush();
		
		if( mov == 1 && movCount == 0 )
		{
			std::stringstream movCountInfoStrStr;
			movCountInfoStrStr << (1);
			output+= movCountInfoStrStr.str();
		}
		else
		{
			std::stringstream movCountInfoStrStr;
			movCountInfoStrStr << (movCount);
			output+= movCountInfoStrStr.str();
		}

		return output;
	}



	bool MaterialDraw()
	{
	
		int numKnightsWhite=0, numBishopsWhite=0, numQueensWhite=0, numRooksWhite=0, numPawnsWhite=0;
		int numKnightsBlack=0, numBishopsBlack=0, numQueensBlack=0, numRooksBlack=0, numPawnsBlack=0;
		bool bDarkWhite = false;
		bool bDarkBlack = false;
		for( int i = 0; i < 16; i++ )
		{
			if( !whitePcs[i]->bCaptured )
			{
				switch( whitePcs[i]->type )
				{
				case Pawn: return false;
				case Bishop: bDarkWhite = whitePcs[i]->bdark_squared; numBishopsWhite++; break;
				case Rook: return false;
				case Knight: numKnightsWhite++; break;
				case Queen : return false;
				case King: break;
				}
			}
			if( !blackPcs[i]->bCaptured )
			{
				switch( blackPcs[i]->type )
				{
				case Pawn: return false;
				case Bishop: bDarkBlack = blackPcs[i]->bdark_squared; numBishopsBlack++; break;
				case Rook: return false;
				case Knight: numKnightsBlack++; break;
				case Queen : return false;
				case King: break;
				}
			}
		}

		if( (numBishopsBlack == 2) || (numBishopsWhite == 2) ) return false;
		if( (numKnightsBlack == 2) || (numKnightsWhite == 2) ) return false;
		if( ((numKnightsBlack ==1)&&  (numBishopsBlack == 1)) || ((numKnightsWhite ==1)&&  (numBishopsWhite == 1)) ) return false;
		if( (numBishopsBlack == 1) && (numKnightsWhite == 1)) return false;
		if( (numBishopsWhite == 1) && (numKnightsBlack == 1)) return false;
		if( (numBishopsWhite == 1) && (numBishopsBlack == 1)) 
		{
			if( bDarkWhite != bDarkBlack )
				return false;
		}
		if( (numKnightsBlack == 1) && (numKnightsWhite == 1)) return false;
		
		return true;
	}

	int TimeMaterialTest(int col)
	{
	
		int numKnightsWhite=0, numBishopsWhite=0, numQueensWhite=0, numRooksWhite=0, numPawnsWhite=0;
		int numKnightsBlack=0, numBishopsBlack=0, numQueensBlack=0, numRooksBlack=0, numPawnsBlack=0;
		bool bDarkWhite = false;
		bool bDarkBlack = false;
		int numPiecesBlack=0, numPiecesWhite=0;
		for( int i = 0; i < 16; i++ )
		{
			if( !whitePcs[i]->bCaptured )
			{
				if( i!= 4 ) numPiecesWhite++;

				switch( whitePcs[i]->type )
				{
				case Pawn: break;
				case Bishop: bDarkWhite = whitePcs[i]->bdark_squared; numBishopsWhite++; break;
				case Rook: break;
				case Knight: numKnightsWhite++; break;
				case Queen : break;
				case King: break;
				}
			}
			if( !blackPcs[i]->bCaptured )
			{
				if( i!= 4 ) numPiecesBlack++;
				switch( blackPcs[i]->type )
				{
				case Pawn: break;
				case Bishop: bDarkBlack = blackPcs[i]->bdark_squared; numBishopsBlack++; break;
				case Rook: break;
				case Knight: numKnightsBlack++; break;
				case Queen : break;
				case King: break;
				};
			}
		}

		if( col != 1 )
		{
			if( numPiecesBlack > 0 )
			{
				if( numPiecesWhite > 0 )
				{
					if( (numBishopsWhite == 1) && (numBishopsBlack == 1) &&
						(numPiecesWhite == 1) && (numPiecesBlack==1)) 
					{
						if( bDarkWhite != bDarkBlack )
							return 0;
						else return 1;
					}
					else return 1;
				}
				else
				{
					if( (numPiecesBlack == 1) && ((numKnightsBlack == 1)||(numBishopsBlack==1)))
					{
						return 0;
					}
					else return 1;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if( numPiecesWhite > 0 )
			{
				if( numPiecesBlack > 0 )
				{
					if( (numBishopsBlack == 1) && (numBishopsWhite == 1) &&
						(numPiecesWhite == 1) && (numPiecesBlack==1)) 
					{
						if( bDarkWhite != bDarkBlack )
							return 0;
						else return 1;
					}
					else return 1;
				}
				else
				{
					if( (numPiecesWhite == 1) && ((numKnightsWhite == 1)||(numBishopsWhite==1)))
					{
						return 0;
					}
					else return 1;
				}
			}
			else
			{

				return 0;
			}
		}

		
	}



	bool DoesBlackHaveAnyPieces()
	{
		int count = 0;
		//int matCount = 0;
		for( int i = 0; i < 16; i++ )
		{
			if( i != 4 )
			{
				if( !blackPcs[i]->bCaptured )
				{
					count++;
					if( ((blackPcs[i]->type != Knight) &&
						(blackPcs[i]->type != Bishop)) || (count > 1))
					{
						//if 
						return true;
					}
				}
			}
		}

		return false;
	}

	bool DoesWhiteHaveAnyPieces()
	{
		int count = 0;
		//int matCount = 0;
		for( int i = 0; i < 16; i++ )
		{
			if( i != 4 )
			{
				if( !whitePcs[i]->bCaptured )
				{
					count++;
					if( ((whitePcs[i]->type != Knight) &&
						(whitePcs[i]->type != Bishop)) || 
						(count > 1))
					{
						return true;
					}
				}
			}
		}

		return false;
	}


	void ClearBoard()
	{

		fiftyMov = 0;
		movCount = 0;
		mov=0;
		postReset = false;

		extraWhiteRooks = false;
		extraWhiteKnights = false;
		extraWhiteBishops = false;
		extraWhiteQueens = false;

		extraBlackRooks = false;
		extraBlackKnights = false;
		extraBlackBishops = false;
		extraBlackQueens = false;

		whiteCastled = false;
		blackCastled = false;
		canEmpasse = false;
		GameStatus = PLAYING;
		
		InitPieces();

		for( int i = 0; i < blackPcs.size(); i++ )
		{
			blackPcs[i]->bCaptured = true;
			blackPcs[i]->bHasMoved = false;
		}

		for( int i = 8; i < 16; i++ )
		{
			blackPcs[i]->bCaptured = true;
		}

		for( int i = 0; i < whitePcs.size(); i++ )
		{
			whitePcs[i]->bCaptured = true;
			whitePcs[i]->bHasMoved = false;
		}
		for( int i = 8; i < 16; i++ )
		{
			whitePcs[i]->bCaptured = true;
		}

		for( int i = 0; i < 8; i++ )
		{
			for( int j = 1; j < 9; j++ )
			{
				SetSquare( i, j, 0);
			}
		}
	}


	void SetBoard(string fenstr) 
	{
		if( fenstr.length() == 0 )
			return; // return integer empty string

		int index = 0;
		int spc = 0;
		int numWhitePawns = 0;
		int numBlackPawns = 0;
		int numWhiteKnights = 0;
		int numBlackKnights = 0;
		int numWhiteBishops = 0;
		int numBlackBishops = 0;
		int numWhiteRooks = 0;
		int numBlackRooks = 0;
		int numWhiteQueens = 0;
		int numBlackQueens = 0;
		
		
		int spacers = 0;

		ClearBoard();

		whiteCastled = true;
		blackCastled = true;
		whitePcs[7]->bHasMoved =true;
		whitePcs[0]->bHasMoved =true;
		blackPcs[7]->bHasMoved =true;
		blackPcs[0]->bHasMoved =true;

		vector<string> vecFen;
		istringstream iss(fenstr);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter<vector<string> >(vecFen));

		if( vecFen.size() > 6 )
		{
			return; // return an integer.
		}
		else
		{
			// look at the strings for validity early out

		}
		// hopefully this will fix loading on buggy fens.
		if( vecFen.size() == 4 )
		{
			vecFen.push_back("1");
			vecFen.push_back("25");
		}


		/*for( int i = 0; i < vecFen.size(); i++ )
		{
			cout<< vecFen[i] << endl;
		}*/

		int rank = 0;
		int file = 0;
		for( int i = 0; i < vecFen[0].length(); i++ )
		{
			char c = vecFen[0][i];

			if( c== '/' )
			{
				rank++;
				file = 0;
				continue;
			}
			if( (c >= '1') && (c <= '8') )
			{
				file += c-'0';
				continue;
			}

			if( isalpha(c) )
			{
				
				switch(c)
				{
				case 'P':board[file][7-rank] = wPawn;break;
				case 'N':board[file][7-rank] = wKnight;break;
				case 'B':board[file][7-rank] = wBishop;break;
				case 'R':board[file][7-rank] = wRook;break;
				case 'Q':board[file][7-rank] = wQueen;break;
				case 'K':board[file][7-rank] = wKing;break;
				case 'p':board[file][7-rank] = bPawn;break;
				case 'n':board[file][7-rank] = bKnight;break;
				case 'b':board[file][7-rank] = bBishop;break;
				case 'r':board[file][7-rank] = bRook;break;
				case 'q':board[file][7-rank] = bQueen;break;
				case 'k':board[file][7-rank] = bKing;break;
				}
				file++;
				continue;
			}
		}

		if( vecFen[1][0] == 'b' )mov = BLACK;

		if( vecFen[1][0] == 'w' ) mov = WHITE;


		if( vecFen[2][0] == '-' ) 
		{
			//cout<<"No Castling"<<endl;
			whitePcs[7]->bHasMoved =true;
			whitePcs[0]->bHasMoved =true;
			blackPcs[7]->bHasMoved =true;
			blackPcs[0]->bHasMoved =true;
			whiteCastled = true;
			blackCastled = true;

		}
		else
		{

			for( int idx=0; idx < vecFen[2].length(); idx++ )
			{
				// temporary solution?
				//if( blackCastled == false)
				//	break;



				char c = vecFen[2][idx];
				switch(c)
				{
				case 'K': 
					{
						cout<< "white can castle king side" <<endl;
						//whitePcs[7]->bCaptured =false;
						whitePcs[7]->bHasMoved =false;
						whitePcs[4]->bHasMoved = false;
						whiteCastled = false;
						break;
					}
						
				case 'Q': //cout<<"white can castle Queen side"<<endl;break;
					{
						//cout<< "white can castle king side" <<endl;
						//whitePcs[0]->bCaptured =false;
						whitePcs[0]->bHasMoved =false;
						whitePcs[4]->bHasMoved = false;
						whiteCastled = false;
						break;
					}
				case 'k': //cout<<"black can castle king side"<<endl;break;
					{
						//cout<< "white can castle king side" <<endl;
						//blackPcs[7]->bCaptured =false;
						blackPcs[7]->bHasMoved =false;
						blackPcs[4]->bHasMoved = false;
						blackCastled = false;
						break;
					}
				case 'q': //cout<<"black can castle queen side"<<endl;break;
					{
						//cout<< "white can castle king side" <<endl;
						//blackPcs[0]->bCaptured =false;
						blackPcs[0]->bHasMoved =false;
						blackPcs[4]->bHasMoved = false;
						blackCastled = false;
						break;
					}

				}
			}
		}

		if( vecFen[3][0] == '-' )
		{
			this->canEmpasse = false;
		}
		else
		{
			//cout<< "file: "<< vecFen[3][0]-'a' <<" File text: "<<vecFen[3][0]<< "rank: "<<vecFen[3][1]-'1'<<" rank text: "<<vecFen[3][1] <<endl;
			empassantTarget.sq.file = vecFen[3][0]-'a';
			empassantTarget.sq.rank = vecFen[3][1]-'1';	
			this->canEmpasse = true;
		}

		fiftyMov = atoi(vecFen[4].c_str());
		//fiftyMov /= 2;
		movCount = atoi(vecFen[5].c_str());


		// for each square, allocate a piece
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{
				if( board[i][j] != 0 )
				{
					if( board[i][j] == wQueen )
					{
						if( whitePcs[3]->bCaptured == true )
						{
							whitePcs[3]->bCaptured = false;
							whitePcs[3]->SetSquare(i,j);
						}
						else
						{
							for( int k = 8; k < 16; k++ )
							{
								if( whitePcs[k]->bCaptured == true )
								{
									whitePcs[k]->bCaptured = false;
									whitePcs[k]->SetSquare(i,j);
									whitePcs[k]->SetType(Queen);
									whitePcs[k]->SetColor(WHITE);

									extraWhiteQueens = true;
									break;
								}
							}
						}
					}
					if( board[i][j] == wKing )
					{
						whitePcs[4]->bCaptured = false;
						whitePcs[4]->SetSquare(i,j);
					}
					if( board[i][j] == wRook )
					{
						if( whitePcs[0]->bCaptured == true )
						{
							whitePcs[0]->bCaptured = false;
							whitePcs[0]->SetSquare(i,j);
						}
						else if( whitePcs[7]->bCaptured == true )
						{
							whitePcs[7]->bCaptured = false;
							whitePcs[7]->SetSquare(i,j);
						}
						else
						{
							for( int k = 8; k < 16; k++ )
							{
								if( whitePcs[k]->bCaptured == true )
								{
									whitePcs[k]->bCaptured = false;
									whitePcs[k]->SetSquare(i,j);
									whitePcs[k]->SetType(Rook);
									extraWhiteRooks = true;
									break;
								}
							}
						}
					}
					if( board[i][j] == wKnight )
					{
						if( whitePcs[1]->bCaptured == true )
						{
							whitePcs[1]->bCaptured = false;
							whitePcs[1]->SetSquare(i,j);
						}
						else if( whitePcs[6]->bCaptured == true )
						{
							whitePcs[6]->bCaptured = false;
							whitePcs[6]->SetSquare(i,j);
						}
						else
						{
							for( int k = 8; k < 16; k++ )
							{
								if( whitePcs[k]->bCaptured == true )
								{
									whitePcs[k]->bCaptured = false;
									whitePcs[k]->SetSquare(i,j);
									whitePcs[k]->SetType(Knight);
									extraWhiteKnights = true;
									break;
								}
							}
						}
					}
					if( board[i][j] == wBishop )
					{
						
						if( IsSquareDark(i,j ))
						{
							/*if(whitePcs[2]->bdark_squared && whitePcs[2]->bCaptured)
							{
								whitePcs[2]->bCaptured = false;
								whitePcs[2]->SetSquare(i,j);
							}*/
							if( whitePcs[5]->bCaptured)
							{
								whitePcs[5]->bCaptured = false;
								whitePcs[5]->SetSquare(i,j);
							}
							else
							{
								for( int k = 8; k < 16; k++ )
								{
									if( whitePcs[k]->bCaptured == true )
									{
										whitePcs[k]->bCaptured = false;
										whitePcs[k]->SetSquare(i,j);
										whitePcs[k]->SetType(Bishop);
										whitePcs[k]->SetDarkSquare(true);
										extraWhiteBishops= true;
										break;
									}
								}
							}
						}
						else
						{
							if( whitePcs[2]->bCaptured)
							{
								whitePcs[2]->bCaptured = false;
								whitePcs[2]->SetSquare(i,j);
							}
				
							else
							{
								for( int k = 8; k < 16; k++ )
								{
									if( whitePcs[k]->bCaptured == true )
									{
										whitePcs[k]->bCaptured = false;
										whitePcs[k]->SetSquare(i,j);
										whitePcs[k]->SetType(Bishop);
										extraWhiteBishops= true;
										break;
									}
								}
							}
						}
					}
					if( board[i][j] == wPawn )
					{
						for( int k = 8; k < 16; k++ )
						{
							if( whitePcs[k]->bCaptured == true )
							{
								whitePcs[k]->bCaptured = false;
								if( j == 1 )
									whitePcs[k]->bHasMoved = false;
								else
									whitePcs[k]->bHasMoved = true;

								whitePcs[k]->SetSquare(i,j);
								break;
							}
							
						}
					}


					//BLACK

					if( board[i][j] == bQueen )
					{
						if( blackPcs[3]->bCaptured == true )
						{
							blackPcs[3]->bCaptured = false;
							blackPcs[3]->SetSquare(i,j);
						}
						else
						{
							for( int k = 8; k < 16; k++ )
							{
								if( blackPcs[k]->bCaptured == true )
								{
									blackPcs[k]->bCaptured = false;
									blackPcs[k]->SetSquare(i,j);
									blackPcs[k]->SetType(Queen);
									extraBlackQueens= true;
									break;
								}
							}
						}
					}
					if( board[i][j] == bKing )
					{
						blackPcs[4]->bCaptured = false;
						blackPcs[4]->SetSquare(i,j);
					}
					if( board[i][j] == bRook )
					{
						if( blackPcs[0]->bCaptured == true )
						{
							blackPcs[0]->bCaptured = false;
							blackPcs[0]->SetSquare(i,j);
						}
						else if( blackPcs[7]->bCaptured == true )
						{
							blackPcs[7]->bCaptured = false;
							blackPcs[7]->SetSquare(i,j);
						}
						else
						{
							for( int k = 8; k < 16; k++ )
							{
								if( blackPcs[k]->bCaptured == true )
								{
									blackPcs[k]->bCaptured = false;
									blackPcs[k]->SetSquare(i,j);
									blackPcs[k]->SetType(Rook);
									extraBlackRooks= true;
									break;
								}
							}
						}
					}
					if( board[i][j] == bKnight )
					{
						if( blackPcs[1]->bCaptured == true )
						{
							blackPcs[1]->bCaptured = false;
							blackPcs[1]->SetSquare(i,j);
						}
						else if( blackPcs[6]->bCaptured == true )
						{
							blackPcs[6]->bCaptured = false;
							blackPcs[6]->SetSquare(i,j);
						}
						else
						{
							for( int k = 8; k < 16; k++ )
							{
								if( blackPcs[k]->bCaptured == true )
								{
									blackPcs[k]->bCaptured = false;
									blackPcs[k]->SetSquare(i,j);
									blackPcs[k]->SetType(Knight);
									extraBlackKnights= true;
									break;
								}
							}
						}
					}
					if( board[i][j] == bBishop )
					{
						

						if( IsSquareDark(i,j ))
						{
							if( blackPcs[2]->bCaptured )
							{
								blackPcs[2]->bCaptured = false;
								blackPcs[2]->SetSquare(i,j);
							}
							else
							{
								for( int k = 8; k < 16; k++ )
								{
									if( blackPcs[k]->bCaptured == true )
									{
										blackPcs[k]->bCaptured = false;
										blackPcs[k]->SetSquare(i,j);
										blackPcs[k]->SetType(Bishop);
										blackPcs[k]->SetDarkSquare(true);
										extraBlackBishops= true;
										break;
									}
								}
							}
						}
						else
						{

							if(blackPcs[5]->bCaptured) 
							{
								
								blackPcs[5]->bCaptured = false;
								blackPcs[5]->SetSquare(i,j);
								
							}
					
							else
							{
								for( int k = 8; k < 16; k++ )
								{
									if( blackPcs[k]->bCaptured == true )
									{
										blackPcs[k]->bCaptured = false;
										blackPcs[k]->SetSquare(i,j);
										blackPcs[k]->SetType(Bishop);
										extraBlackBishops= true;
										break;
									}
								}
							}
						}
					}
					if( board[i][j] == bPawn )
					{
						for( int k = 8; k < 16; k++ )
						{
							if( blackPcs[k]->bCaptured == true )
							{
								blackPcs[k]->bCaptured = false;
								if( j == 6 )
									blackPcs[k]->bHasMoved = false;
								else
									blackPcs[k]->bHasMoved = true;
								blackPcs[k]->SetSquare(i,j);
								break;
							}
							
						}
					}
				}
			}
		}


		GameStatus = PLAYING;
	}


	void GenLegalMovesKnight(MyChessPiece *N, vector<MyChessMove>* legalMoves)
	{

		MyChessMove moves[8];

		moves[0].to.file = N->x + 1;
		moves[0].to.rank = N->y + 2;
		
		moves[1].to.file = N->x + 1;
		moves[1].to.rank = N->y - 2;

		moves[2].to.file = N->x - 1;
		moves[2].to.rank = N->y + 2;

		moves[3].to.file = N->x - 1;
		moves[3].to.rank = N->y - 2;

		moves[4].to.file = N->x + 2;
		moves[4].to.rank = N->y + 1;

		moves[5].to.file = N->x + 2;
		moves[5].to.rank = N->y - 1;

		moves[6].to.file = N->x - 2;
		moves[6].to.rank = N->y + 1;

		moves[7].to.file = N->x - 2;
		moves[7].to.rank = N->y - 1;

		for( int i = 0; i < 8; i++ )
		{
			if( moves[i].to.file > -1 && moves[i].to.file < 8 &&
				moves[i].to.rank > -1 && moves[i].to.rank < 8 )
			{
				if( N->color == WHITE )
				{
					if( board[moves[i].to.file][moves[i].to.rank] != wPawn &&
						board[moves[i].to.file][moves[i].to.rank] != wRook &&
						board[moves[i].to.file][moves[i].to.rank] != wKnight &&
						board[moves[i].to.file][moves[i].to.rank] != wBishop &&
						board[moves[i].to.file][moves[i].to.rank] != wQueen &&
						board[moves[i].to.file][moves[i].to.rank] != wKing )
					{
						legalMoves->push_back(moves[i]);
					}
				}
				else
				{
					if( board[moves[i].to.file][moves[i].to.rank] != bPawn &&
						board[moves[i].to.file][moves[i].to.rank] != bRook &&
						board[moves[i].to.file][moves[i].to.rank] != bKnight &&
						board[moves[i].to.file][moves[i].to.rank] != bBishop &&
						board[moves[i].to.file][moves[i].to.rank] != bQueen &&
						board[moves[i].to.file][moves[i].to.rank] != bKing )
					{
						legalMoves->push_back(moves[i]);
					}
				}
			}

		}
	}

	void GenLegalMovesBishop(MyChessPiece *B, vector<MyChessMove>* legalMoves)
	{
		int x1 = B->x;
		int y1 = B->y;

		for( int x=x1+1, y=y1+1; (y<8)&&(x<8); x++, y++)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( B->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}
		
		
		for( int x=x1+1, y = y1-1; (y>-1)&&(x<8); x++, y--)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( B->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}
		
		for( int y=y1+1, x = x1-1; (x>-1)&&(y<8); y++, x--)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( B->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}

		for( int y=y1-1, x = x1-1; (x>-1)&&(y>-1); y--, x--)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( B->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}
	}

	void GenLegalMovesRook(MyChessPiece *R, vector<MyChessMove>* legalMoves)
	{
		int X1 = R->x;
		int Y1 = R->y;
		int x;
		int y=R->y;

		for( x=X1+1; x<8; x++ )
		{
			if( R->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}
		for( x=X1-1; x>-1; x-- )
		{
			if( R->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}

		x = R->x;

		for( y=Y1+1; y<8; y++ )
		{
			if( R->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}

		for( y=Y1-1; y>-1; y-- )
		{
			if( R->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = R->x;
					move.from.rank = R->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}
	}

	void GenLegalMovesQueen(MyChessPiece *Q, vector<MyChessMove>* legalMoves)
	{
		int x1 = Q->x;
		int y1 = Q->y;

		for( int x=x1+1, y=y1+1; (y<8)&&(x<8); x++, y++)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}
		
		
		for( int x=x1+1, y = y1-1; (y>-1)&&(x<8); x++, y--)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}
		
		for( int y=y1+1, x = x1-1; (x>-1)&&(y<8); y++, x--)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}

		for( int y=y1-1, x = x1-1; (x>-1)&&(y>-1); y--, x--)
		{
			MyChessMove move;
			move.from.file = x1;
			move.from.rank = y1;
			move.to.file = x;
			move.to.rank = y;
			
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					legalMoves->push_back(move);
				}
			}
		}

		int X1 = Q->x;
		int Y1 = Q->y;
		int x;
		int y=Q->y;

		for( x=X1+1; x<8; x++ )
		{
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}
		for( x=X1-1; x>-1; x-- )
		{
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}

		x = Q->x;

		for( y=Y1+1; y<8; y++ )
		{
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}

		for( y=Y1-1; y>-1; y-- )
		{
			if( Q->color == WHITE )
			{
				if( board[x][y] != wPawn && 
					board[x][y] != wRook &&
					board[x][y] != wKnight &&
					board[x][y] != wBishop &&
					board[x][y] != wQueen &&
					board[x][y] != wKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
			else
			{
				if( board[x][y] != bPawn && 
					board[x][y] != bRook &&
					board[x][y] != bKnight &&
					board[x][y] != bBishop &&
					board[x][y] != bQueen &&
					board[x][y] != bKing )
				{
					MyChessMove move;
					move.from.file = Q->x;
					move.from.rank = Q->y;
					move.to.file = x;
					move.to.rank = y;
					legalMoves->push_back(move);
				}
			}
		}
	}

	void GenLegalMovesKing(MyChessPiece *K, vector<MyChessMove>* legalMoves)
	{
		int numSquares = 0;
		MyChessSquare king_squares[8];
		king_squares[0].Set( K->x-1, K->y-1);
		king_squares[1].Set( K->x-1, K->y);
		king_squares[2].Set( K->x-1, K->y+1);
		king_squares[3].Set( K->x, K->y-1);
		king_squares[4].Set( K->x, K->y+1);
		king_squares[5].Set( K->x+1, K->y-1);
		king_squares[6].Set( K->x+1, K->y);
		king_squares[7].Set( K->x+1, K->y+1);
		if( K->color == WHITE )
		{
			for( int i = 0; i < 8; i++ )
			{
				if( (king_squares[i].file >= 0) && 
					(king_squares[i].rank >= 0) &&
					(king_squares[i].rank < 8) && 
					(king_squares[i].file < 8) )
				{
					if( (board[king_squares[i].file][king_squares[i].rank] != wPawn) &&
						(board[king_squares[i].file][king_squares[i].rank] != wRook) &&
						(board[king_squares[i].file][king_squares[i].rank] != wKnight) &&
						(board[king_squares[i].file][king_squares[i].rank] != wBishop) &&
						(board[king_squares[i].file][king_squares[i].rank] != wQueen) )
					{
						if( !IsSquareDirectlyAttacked(BLACK, king_squares[i].file, king_squares[i].rank) )
						{
							if( !IsSquareDirectlyAttackedIgnoringKing(BLACK, king_squares[i].file, king_squares[i].rank ))
							{
								MyChessMove move;
								move.from.file = K->x;
								move.from.rank = K->y;
								move.to.file = king_squares[i].file;
								move.to.rank = king_squares[i].rank;
								legalMoves->push_back( move);
							}
							// we got a square
							
						}
					}
				}
			}
			if( this->canWhiteCastleLong() )
			{
				if( !this->isRankSpanBlocked(K->x, 0, 0) )
				{
					if( !this->IsSquareDirectlyAttacked(BLACK, K->x-1, 0) )
					{
						MyChessMove move;
						move.from.file = K->x;
						move.from.rank = K->y;
						move.to.file = 2;
						move.to.rank = 0;
						legalMoves->push_back( move);
					}
				}
			}
			if( this->canWhiteCastleShort() )
			{
				if( !this->isRankSpanBlocked(K->x, 0, 0) )
				{
					if( !this->IsSquareDirectlyAttacked(BLACK, K->x+1, 0) )
					{
						MyChessMove move;
						move.from.file = K->x;
						move.from.rank = K->y;
						move.to.file = 6;
						move.to.rank = 0;
						legalMoves->push_back( move);
					}
				}
			}

		}
		else
		{
			for( int i = 0; i < 8; i++ )
			{
				if( (king_squares[i].file >= 0) && 
					(king_squares[i].rank >= 0) &&
					(king_squares[i].rank < 8) && 
					(king_squares[i].file < 8) )
				{
					if( (board[king_squares[i].file][king_squares[i].rank] != bPawn) &&
						(board[king_squares[i].file][king_squares[i].rank] != bRook) &&
						(board[king_squares[i].file][king_squares[i].rank] != bKnight) &&
						(board[king_squares[i].file][king_squares[i].rank] != bBishop) &&
						(board[king_squares[i].file][king_squares[i].rank] != bQueen) )
					{
						if( !IsSquareDirectlyAttacked(WHITE, king_squares[i].file, king_squares[i].rank) )
						{
							// we got a square
							if( !IsSquareDirectlyAttackedIgnoringKing(WHITE, king_squares[i].file, king_squares[i].rank ))
							{
								MyChessMove move;
								move.from.file = K->x;
								move.from.rank = K->y;
								move.to.file = king_squares[i].file;
								move.to.rank = king_squares[i].rank;
								legalMoves->push_back( move);
							}
						}
					}
				}
			}
			if( this->canBlackCastleLong() )
			{
				if( !this->isRankSpanBlocked(K->x, 0, 0) )
				{
					if( !this->IsSquareDirectlyAttacked(WHITE, K->x-1, 7) )
					{
						MyChessMove move;
						move.from.file = K->x;
						move.from.rank = K->y;
						move.to.file = 2;
						move.to.rank = 7;
						legalMoves->push_back( move);
					}
				}
			}
			if( this->canBlackCastleShort() )
			{
				if( !this->isRankSpanBlocked(K->x, 0, 0) )
				{
					if( !this->IsSquareDirectlyAttacked(WHITE, K->x+1, 7) )
					{
						MyChessMove move;
						move.from.file = K->x;
						move.from.rank = K->y;
						move.to.file = 6;
						move.to.rank = 7;
						legalMoves->push_back( move);
					}
				}
			}
		}

		
	
	}

	void GenLegalMovesPawn(MyChessPiece *P, vector<MyChessMove>* legalMoves)
	{
		if( P->color == WHITE )
		{
			if( P->y == 1 )
			{
				MyChessMove move2;
				move2.from.file = P->x;
				move2.from.rank = P->y;
				move2.to.file = P->x;
				move2.to.rank = P->y +2;
				if( CanPawnReachSquare(P, move2.to.file, move2.to.rank) )
				{
					legalMoves->push_back(move2);
				}
			}
			MyChessMove moves[3];
			moves[0].to.file = P->x + 1;
			moves[0].to.rank = P->y + 1;

			moves[1].to.file = P->x;
			moves[1].to.rank = P->y + 1;

			moves[2].to.file = P->x - 1;
			moves[2].to.rank = P->y + 1;
			for( int i = 0; i < 3; i++ )
			{
				moves[i].from.file = P->x;
				moves[i].from.rank = P->y;
				if( CanPawnReachSquare(P, moves[i].to.file, moves[i].to.rank) )
				{
					legalMoves->push_back(moves[i]);
				}
			}


		}
		else
		{
			if( P->y == 6 )
			{
				MyChessMove move2;
				move2.from.file = P->x;
				move2.from.rank = P->y;
				move2.to.file = P->x;
				move2.to.rank = P->y +2;
				if( CanPawnReachSquare(P, move2.to.file, move2.to.rank) )
				{
					legalMoves->push_back(move2);
				}
			}
			MyChessMove moves[3];
			moves[0].to.file = P->x + 1;
			moves[0].to.rank = P->y + 1;

			moves[1].to.file = P->x;
			moves[1].to.rank = P->y + 1;

			moves[2].to.file = P->x - 1;
			moves[2].to.rank = P->y + 1;
			for( int i = 0; i < 3; i++ )
			{
				moves[i].from.file = P->x;
				moves[i].from.rank = P->y;
				if( CanPawnReachSquare(P, moves[i].to.file, moves[i].to.rank) )
				{
					legalMoves->push_back(moves[i]);
				}
			}
		}
	}

	void GenMoves(vector<MyChessMove>* legalMoves)
	{
		if( mov != BLACK )
		{
			for( int i = 0; i < whitePcs.size(); i++ )
			{
				if( !whitePcs[i]->bCaptured )
				{
					if( !isPiecePinned(whitePcs[i] ))
					{
						switch(whitePcs[i]->type)
						{
						case Pawn:
							GenLegalMovesPawn(whitePcs[i], legalMoves);
							break;
						case Knight:
							GenLegalMovesKnight(whitePcs[i], legalMoves);
							break;
						case Bishop:
							GenLegalMovesBishop(whitePcs[i], legalMoves);
							break;
						case Rook:
							GenLegalMovesRook(whitePcs[i], legalMoves);
							break;
						case Queen:
							GenLegalMovesQueen(whitePcs[i], legalMoves);
							break;
						};
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < blackPcs.size(); i++ )
			{
				if( !blackPcs[i]->bCaptured )
				{
					if( !isPiecePinned(blackPcs[i] )) // note, else move to square on same diagonal or rank as sliding attacker !
					{
						switch(blackPcs[i]->type)
						{
						case Pawn:
							GenLegalMovesPawn(blackPcs[i], legalMoves);
							break;
						case Knight:
							GenLegalMovesKnight(blackPcs[i], legalMoves);
							break;
						case Bishop:
							GenLegalMovesBishop(blackPcs[i], legalMoves);
							break;
						case Rook:
							GenLegalMovesRook(blackPcs[i], legalMoves);
							break;
						case Queen:
							GenLegalMovesQueen(blackPcs[i], legalMoves);
							break;
						};
					}
				}
			}
		}
	}

	int GenLegalMoves(vector<MyChessMove>* legalMoves)
	{
		
		vector<MyChessPiece*> attackers;
		if( mov != BLACK )
		{
			MyChessPiece *K = whitePcs[4];
			GenLegalMovesKing(K, legalMoves);

			if( GetPiecesAttackingSquare(BLACK, K->x, K->y, &attackers) )
			{
				int numSquares = GetNumSquaresAvailableToKing(K);
				if( attackers.size() > 1 )
				{
						
					if( numSquares == 0 )
					{
						return CHECKMATE;
					}
					else return CHECK;
				}
				else
				{
					if( attackers.size() == 1 )
					{
						if( numSquares != 0 )
						{
							return CHECK;
						}
						else 
						{
							vector<MyChessMove> defensiveMoves;
							AddPiecesPreventingCheck(K, &defensiveMoves, attackers[0]->x, attackers[0]->y);
							if( defensiveMoves.size() == 0 )
							{
								return CHECKMATE;
							}
							else
							{
								for( int i = 0; i < defensiveMoves.size(); i++ )
								{
									legalMoves->push_back(defensiveMoves[i]);
								}
								return CHECK;
							}	
						}
					}
				}
			}
			else
			{
				GenMoves(legalMoves);

				if( legalMoves->size() == 0 )
					return STALEMATE;
				else return KINGSAFE;
			}
		}
		else
		{
			MyChessPiece *K = blackPcs[4];
			GenLegalMovesKing(K, legalMoves);

			if( GetPiecesAttackingSquare(WHITE, K->x, K->y, &attackers) )
			{
				int numSquares = GetNumSquaresAvailableToKing(K);
				if( attackers.size() > 1 )
				{	
					if( numSquares == 0 )
					{
						return CHECKMATE;
					}
					else return CHECK;
				}
				else
				{
					if( attackers.size() == 1 )
					{
						if( numSquares != 0 )
						{
							return CHECK;
						}
						else 
						{
							vector<MyChessMove> defensiveMoves;
							AddPiecesPreventingCheck(K, &defensiveMoves, attackers[0]->x, attackers[0]->y);
							if( defensiveMoves.size() == 0 )
							{
								return CHECKMATE;
							}
							else
							{
								for( int i = 0; i < defensiveMoves.size(); i++ )
								{
									legalMoves->push_back(defensiveMoves[i]);
								}
								return CHECK;
							}	
						}
					}
				}
			}
			else
			{
				GenMoves(legalMoves);

				if( legalMoves->size() == 0 )
					return STALEMATE;
				else return KINGSAFE;
			}
			
		}
		
		return KINGSAFE;
	}


	void AddPiecesPreventingCheck(MyChessPiece* K, vector<MyChessMove>*legalMoves, int fromX, int fromY)
	{
		for( int i = 0; i < 16; i++ )
		{
			if( i == 4 ) 
				continue;
			if( K->x == fromX )
			{
				if( K->color == WHITE )
				{
					if( !whitePcs[i]->bCaptured )
					{
						int toX;
						int toY;
						if( CanPieceIntercedeFileSpan(whitePcs[i], K->x, K->y, fromY, &toX, &toY ) )
							if( !isPiecePinned(whitePcs[i]))
							{
								MyChessMove mv;
								mv.to.file = toX;
								mv.to.rank = toY;
								mv.from.file = whitePcs[i]->x;
								mv.from.rank = whitePcs[i]->y;
								legalMoves->push_back(mv);
								return ;
							}
					}
				}
				else
				{
					if( !blackPcs[i]->bCaptured )
					{
						int toX;
						int toY;
						if( CanPieceIntercedeFileSpan(blackPcs[i], K->x, K->y, fromY, &toX, &toY ) )
							if( !isPiecePinned(blackPcs[i]))
							{
								MyChessMove mv;
								mv.to.file = toX;
								mv.to.rank = toY;
								mv.from.file = blackPcs[i]->x;
								mv.from.rank = blackPcs[i]->y;
								legalMoves->push_back(mv);
								return ;
							}
					}
				}
			}
			else if( K->y == fromY )
			{
				if( K->color == WHITE )
				{
					if( !whitePcs[i]->bCaptured )
					{
						int toX;
						int toY;
						if( CanPieceIntercedeRankSpan(whitePcs[i], K->x, fromX, fromY, &toX, &toY ) )
							if( !isPiecePinned(whitePcs[i]))
							{
								MyChessMove mv;
								mv.to.file = toX;
								mv.to.rank = toY;
								mv.from.file = whitePcs[i]->x;
								mv.from.rank = whitePcs[i]->y;
								legalMoves->push_back(mv);
								return ;
							}
					}
				}
				else
				{
					if( !blackPcs[i]->bCaptured )
					{
						int toX;
						int toY;
						if( CanPieceIntercedeRankSpan(blackPcs[i], K->x, fromX, fromY, &toX, &toY ) )
							if( !isPiecePinned(blackPcs[i]))
							{
								MyChessMove mv;
								mv.to.file = toX;
								mv.to.rank = toY;
								mv.from.file = blackPcs[i]->x;
								mv.from.rank = blackPcs[i]->y;
								legalMoves->push_back(mv);
								return ;
							}
					}
				}	
			}
			else if( abs(K->x - fromX) == abs(K->y - fromY))
			{
				if( K->color == WHITE )
				{
					if( !whitePcs[i]->bCaptured )
					{
						int toX;
						int toY;
						if( CanPieceIntercedeDiagonalSpan(whitePcs[i], K->x, K->y, fromX, fromY, &toX, &toY ))
							if( !isPiecePinned(whitePcs[i]))
							{
								MyChessMove mv;
								mv.to.file = toX;
								mv.to.rank = toY;
								mv.from.file = whitePcs[i]->x;
								mv.from.rank = whitePcs[i]->y;
								legalMoves->push_back(mv);
								return ;
							}
					}
				}
				else
				{
					if( !blackPcs[i]->bCaptured )
					{
						int toX;
						int toY;
						if( CanPieceIntercedeDiagonalSpan(blackPcs[i], K->x, K->y, fromX, fromY, &toX, &toY ))
							if( !isPiecePinned(blackPcs[i]))
							{
								MyChessMove mv;
								mv.to.file = toX;
								mv.to.rank = toY;
								mv.from.file = blackPcs[i]->x;
								mv.from.rank = blackPcs[i]->y;
								legalMoves->push_back(mv);
								return ;
							}
					}
				}	
			}
			else 
			{
				if( K->color == WHITE )
				{
					vector<MyChessPiece*> attackers;
					if( GetPiecesAttackingSquare(WHITE, fromX, fromY, &attackers ))
					{
						for( int i = 0; i < attackers.size(); i++ )
						{
							if( !attackers[i]->bCaptured )
								if( !isPiecePinned(attackers[i]))
								{
									MyChessMove mv;
									mv.to.file = attackers[i]->x;
									mv.to.rank = attackers[i]->y;
									mv.from.file = fromX;
									mv.from.rank = fromY;
									legalMoves->push_back(mv);
									return ;
								}
						}
						return ;
					}
				}
				else
				{
					vector<MyChessPiece*> attackers;
					if( GetPiecesAttackingSquare(BLACK, fromX, fromY, &attackers ))
					{
						for( int i = 0; i < attackers.size(); i++ )
						{
							if( !attackers[i]->bCaptured )
								if( !isPiecePinned(attackers[i]))
								{
									MyChessMove mv;
									mv.to.file = attackers[i]->x;
									mv.to.rank = attackers[i]->y;
									mv.from.file = fromX;
									mv.from.rank = fromY;
									legalMoves->push_back(mv);
									return ;
								}
						}

						return ;
					}
				}
			}
		}

		return ;
	}

	bool CanKnightReachSquare(MyChessPiece *N, int toFile, int toRank )
	{
		if( (N->x == toFile) && (N->y == toRank) ) 
			return false;

		if( toFile == N->x + 1 ||
			toFile == N->x - 1)
		{
			if( toRank == N->y + 2 ||
				toRank == N->y - 2 )
			{
			//	if( !isPiecePinned(N) )
					return true;
			}
		}
		else if ( toFile == N->x + 2 ||
				  toFile == N->x - 2)
		{
			if( toRank == N->y + 1 ||
				toRank == N->y - 1 )
			{
			//	if( !isPiecePinned(N) )
					return true;
			}
		}

		return false;
	}



	bool CanBishopReachSquare(MyChessPiece *B, int toFile, int toRank )
	{
		if( (B->x == toFile) && (B->y == toRank) ) 
			return false;

		int mod1 = (toRank + toFile) % 2;
		bool bdark = false;
		if( mod1 == 0) bdark = true;
		

		if( bdark )
		{
			if( !B->bdark_squared )
			{
				if( abs(B->x-toFile) == abs(B->y-toRank) )
				{
					if( IsDiagonalSpanClear(B->x, B->y, toFile, toRank ))
					{
						return true;
					}
				}
			}
		}
		else
		{
			if( B->bdark_squared )
			{
				if( abs(B->x-toFile) == abs(B->y-toRank) )
				{
					if( IsDiagonalSpanClear(B->x, B->y, toFile, toRank ))
					{
						return true;
					}
				}
			}
		}
		
		return false;
	}

	bool CanQueenReachSquare(MyChessPiece *Q, int toFile, int toRank )
	{
		if( (Q->x == toFile) && (Q->y == toRank) ) 
			return false;
		
		if( Q->x == toFile )
		{
			return !isFileSpanBlocked(Q->x, Q->y, toRank);
		}
		else if( Q->y == toRank )
		{
			return !isRankSpanBlocked(Q->x, toFile, Q->y);
		}
		else if( abs(Q->x-toFile) == abs(Q->y-toRank) )
		{
			return IsDiagonalSpanClear(Q->x, Q->y, toFile, toRank );
		}

		return false;
	}

	bool CanKingAttackSquare(MyChessPiece *K, int toFile, int toRank )
	{
		if( (K->x == toFile) && (K->y == toRank) ) 
			return false;
		
		if( toFile == K->x +1 || toFile == K->x-1 || toFile == K->x )
		{
			if( toRank == K->y+1 || toRank == K->y-1 || toRank == K->y )
			{
				return true;
			}
		}

		return false;
	}

	bool CanRookReachSquare(MyChessPiece* R, int toFile, int toRank )
	{
		if( (R->x == toFile) && (R->y == toRank) ) 
			return false;

		if( R->x == toFile )
		{
			return !isFileSpanBlocked(R->x, R->y, toRank);
		}
		else if( R->y == toRank )
		{
			return !isRankSpanBlocked(R->x, toFile, R->y);
		}

		return false;
	}


	bool CanBishopReachSquareCheckXray(MyChessPiece *B, int toFile, int toRank )
	{
		if( (B->x == toFile) && (B->y == toRank) ) 
			return false;

		int mod1 = (toRank + toFile) % 2;
		bool bdark = false;
		if( mod1 == 0) bdark = true;
		
		if( bdark )
		{
			if( !B->bdark_squared )
			{
				if( abs(B->x-toFile) == abs(B->y-toRank) )
				{
					int col;
					if( B->color == WHITE )
						col = BLACK;
					else col = WHITE;
					if( IsDiagonalSpanClearIgnoringKing(col, B->x, B->y, toFile, toRank ))
					{
						return true;
					}
				}
			}
		}
		else
		{
			if( B->bdark_squared )
			{
				if( abs(B->x-toFile) == abs(B->y-toRank) )
				{
					int col;
					if( B->color == WHITE )
						col = BLACK;
					else col = WHITE;
					if( IsDiagonalSpanClearIgnoringKing(col, B->x, B->y, toFile, toRank ))
					{
						return true;
					}
				}
			}
		}
		

		return false;
	}

	bool CanQueenReachSquareCheckXray(MyChessPiece *Q, int toFile, int toRank )
	{
		int col;
		if( Q->color == WHITE )
			col = BLACK;
		else col = WHITE;
		
		if( (Q->x == toFile) && (Q->y == toRank) ) 
			return false;
		
		if( Q->x == toFile )
		{
			return !isFileSpanBlockedIgnoringKing(col, Q->x, Q->y, toRank);
		}
		else if( Q->y == toRank )
		{
			return !isRankSpanBlockedIgnoringKing(col, Q->x, toFile, Q->y);
		}
		else if( abs(Q->x-toFile) == abs(Q->y-toRank) )
		{
			return IsDiagonalSpanClearIgnoringKing(col, Q->x, Q->y, toFile, toRank );
		}

		return false;
	}

	bool CanRookReachSquareCheckXray(MyChessPiece* R, int toFile, int toRank )
	{
		int col;
		if( R->color == WHITE )
			col = BLACK;
		else col = WHITE;

		if( (R->x == toFile) && (R->y == toRank) ) 
			return false;

		if( R->x == toFile )
		{
			return !isFileSpanBlockedIgnoringKing(col, R->x, R->y, toRank);
		}
		else if( R->y == toRank )
		{
			return !isRankSpanBlockedIgnoringKing(col, R->x, toFile, R->y);
		}

		return false;
	}

	bool CanKingReachSquare(MyChessPiece* K, int toFile, int toRank)
	{
		if( (K->x == toFile) && (K->y == toRank) ) 
			return false;

		bool inRange = false;
		if( (K->x == toFile+1) || (K->x == toFile-1) )
		{
			if( ((K->y == toRank+1) || (K->y == toRank-1)) || (K->y == toRank) )
			{
				inRange = true;
			}
		}
		else if( (K->y == toRank+1) || (K->y == toRank-1) )
		{
			if( (K->x == toFile) )
			{
				inRange = true;
			}
		}

		bool bCastleShort = false;
		bool bCastleLong = false;
		if( K->x == 4  && toRank == K->y)
		{
			if( toFile == 6  )
			{
				if( K->color == WHITE )
				{
					if( canWhiteCastleShort() == true )
					{
						if( board[6][0] == 0 &&
							board[5][0] == 0 )
						{
							bCastleShort = true;
							inRange = true;
						}
					}
				}
				else
				{
					if( canBlackCastleShort() == true )
					{
						if( board[6][7] == 0 &&
							board[5][7] == 0 )
						{
							bCastleShort = true;
							inRange = true;
						}
					}
				}
			}
			if( toFile == 2 )
			{
				if( K->color == WHITE )
				{
					if( canWhiteCastleLong() == true )
					{
						if( board[3][0] == 0 &&
							board[2][0] == 0 &&
							board[1][0] == 0 )
						{
							bCastleLong = true;
							inRange = true;
						}
					}
				}
				else
				{
					if( canBlackCastleLong() == true )
					{
						if( board[3][7] == 0 &&
							board[2][7] == 0 &&
							board[1][7] == 0 )
						{
							bCastleLong = true;
							inRange = true;
						}
					}
				}
			}
		}

		if( inRange )
		{
			if( K->color == WHITE )
			{	
				int fromX = -1; int fromY = -1;
				if( !IsSquareDirectlyAttacked(BLACK, toFile, toRank, &fromX, &fromY))
				{
					// X-Ray's
					for( int i = 0; i < 16; i++ )
					{
						if( (blackPcs[i]->type != Knight) &&
							(blackPcs[i]->type != King) &&
							(blackPcs[i]->type != Pawn) && 
							!blackPcs[i]->bCaptured )
						{
							if( PieceAttacksSquareIgnoringKing(blackPcs[i], toFile, toRank ) )
							{
								return false;
							}
						}
					}

					if( bCastleShort )
					{
						if( !IsSquareDirectlyAttacked(BLACK, toFile-1, toRank, &fromX, &fromY)) 
						{
							for( int i = 0; i < 16; i++ )
							{
								if( (blackPcs[i]->type != Knight) &&
									(blackPcs[i]->type != King) &&
									(blackPcs[i]->type != Pawn) &&
									!blackPcs[i]->bCaptured)
								{

									if( PieceAttacksSquareIgnoringKing(blackPcs[i], toFile-1, toRank ) )
									{
										return false;
									}
								}
							}
						}
						else return false;
					}
					if( bCastleLong )
					{
						if( !IsSquareDirectlyAttacked(BLACK, toFile+1, toRank, &fromX, &fromY)) 
						{
							for( int i = 0; i < 16; i++ )
							{
								if( (blackPcs[i]->type != Knight) &&
									(blackPcs[i]->type != King) &&
									(blackPcs[i]->type != Pawn) &&
									!blackPcs[i]->bCaptured)
								{
									if( PieceAttacksSquareIgnoringKing(blackPcs[i], toFile+1, toRank ) )
									{
										return false;
									}		
								}
							}
						}
						else return false;
					}
					return true;
				}
				else return false;
					
			}
			else
			{
				int fromX = -1; int fromY = -1;
				if( !IsSquareDirectlyAttacked(WHITE, toFile, toRank, &fromX, &fromY))
				{
					//if( from
					for( int i = 0; i < 16; i++ )
					{
						// X-Ray's
						if( (whitePcs[i]->type != Knight)&&
							(whitePcs[i]->type != King)&&
							(whitePcs[i]->type != Pawn)&&
							!whitePcs[i]->bCaptured )
						{
							if( PieceAttacksSquareIgnoringKing(whitePcs[i], toFile, toRank ) )
								return false;	
						}
					}

					if( bCastleShort )
					{
						if( !IsSquareDirectlyAttacked(WHITE, toFile-1, toRank, &fromX, &fromY)) 
						{
							for( int i = 0; i < 16; i++ )
							{
								if( (whitePcs[i]->type != Knight)&&
									(whitePcs[i]->type != King)&&
									(whitePcs[i]->type != Pawn)&&
									!whitePcs[i]->bCaptured )
								{
									if( PieceAttacksSquareIgnoringKing(whitePcs[i], toFile-1, toRank ) )
									{
										return false;
									}
								}
							}
						}
						else return false;
					}
					if( bCastleLong )
					{
						if( !IsSquareDirectlyAttacked(WHITE, toFile+1, toRank, &fromX, &fromY)) 
						{
							for( int i = 0; i < 16; i++ )
							{
								if( (whitePcs[i]->type != Knight)&&
									(whitePcs[i]->type != King)&&
									(whitePcs[i]->type != Pawn)&&
									!whitePcs[i]->bCaptured )
								{
									if( PieceAttacksSquareIgnoringKing(whitePcs[i], toFile+1, toRank ) )
									{
										return false;
									}
								}
							}
						}
						else return false;
					}
					return true;
					
				}
				else return false;
			}
		}

		return false;
	}

	bool CanPawnReachSquare(MyChessPiece* P, int toFile, int toRank)
	{
		if( (P->x == toFile) && (P->y == toRank) ) 
			return false;
		
		bool almost = false;
		if( P->color == WHITE )
		{
			if( P->y == toRank-1 )
			{
				if( P->x == toFile )
				{
					if( board[P->x][toRank] == 0 )
						almost = true;
				}
				else if( (P->x == toFile-1)|| (P->x ==toFile+1))
				{
					if( toFile == this->empassantTarget.sq.file && 
						toRank == this->empassantTarget.sq.rank && 
						this->empassantTarget.sq.rank == 5 )
					{
						almost = true;
					}
					else if( board[toFile][toRank] != 0 )
					{
						if( (board[toFile][toRank] != wPawn) &&
							(board[toFile][toRank] != wRook) &&
							(board[toFile][toRank] != wBishop) &&
							(board[toFile][toRank] != wKnight) &&
							(board[toFile][toRank] != wQueen) &&
							(board[toFile][toRank] != wKing))
						{
							almost = true;
						}
					}
				}
			}
			else if( P->y == 1 )
			{
				if( P->y == toRank-2 )
				{
					if( P->x == toFile )
					{
						if( board[toFile][toRank] == 0 )
							almost = true;
					}
				}
			}
		}
		else 
		{
			if( P->y == toRank+1 )
			{
				if( P->x == toFile )
				{
					if( board[P->x][toRank] == 0 )
						almost = true;
				}
				else if( (P->x == toFile-1)|| (P->x ==toFile+1) )
				{
					if( toFile == this->empassantTarget.sq.file && 
						toRank == this->empassantTarget.sq.rank &&
						this->empassantTarget.sq.rank == 2)
					{
						almost = true;
					}
					else if( board[toFile][toRank] != 0 )
					{
						if( (board[toFile][toRank] != bPawn) &&
							(board[toFile][toRank] != bRook) &&
							(board[toFile][toRank] != bBishop) &&
							(board[toFile][toRank] != bKnight) &&
							(board[toFile][toRank] != bQueen) &&
							(board[toFile][toRank] != bKing))
						{
							almost = true;
						}
					}
				}
			}
			else if( P->y == 6  )
			{
				if( P->y == toRank+2 )
				{
					if( P->x == toFile )
					{
						if( board[toFile][toRank] == 0 )
							almost = true;
					}
				}
			}
		}

		return almost;
	}

	
	int GetGameStatus()
	{
		return GameStatus;
	}

	string EPN_Move_to_PGN_Move(string epn, bool promote, int typeForPromotion)
	{
		int fromFile = epn[0]-'a';
		int fromRank = epn[1]-'0';
		int toFile = epn[2]-'a';
		int toRank = epn[3]-'0';

		char buf[4];
		itoa(fromRank+1, buf, 10);
		string epn1 = buf;

		//char buf[4];
		itoa(toRank+1, buf, 10);
		string epn3 = buf;
		
		string out = "";

		int white_black_check;


		char c = board[fromFile][fromRank];
		switch(c)
		{
		case bPawn: 
			{
				if( board[toFile][toRank] != 0 || (toFile != fromFile))
				{
					out += epn[0];
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;

				if( toRank == 0 )
				{
					if( epn.length() == 5 )
					{
						out+='=';
						out+=toupper(epn[4]);
					}
				}
				white_black_check = WHITE;
				break;
			}
		case bKnight:
			{
				out+='N'; 
				white_black_check = WHITE;
				if( extraBlackKnights == false )
				{
					//MyChessSquare sq= findBlackKnight(toFile, toRank);
					MyChessPiece *N1 = blackPcs[1];
					MyChessPiece *N2 = blackPcs[6];
					if( CanKnightReachSquare(N1, toFile, toRank ))
					{
						if( CanKnightReachSquare(N2, toFile, toRank ))
						{
							if( (N1->x == fromFile) && 
								(N1->y == fromRank) )
							{
								if( !N2->bCaptured && !isPiecePinned(N2))
								{
									if( N1->x == N2->x )
									{
										out+=epn1;
									}
									else //else if( N1->y == N2->y )
									{
										out+=epn[0];
									}
								}
							}
							else if( (N2->x == fromFile) &&
									 (N2->y == fromRank))
							{
								if( !N1->bCaptured && !isPiecePinned(N1))
								{
									if( N1->x == N2->x )
									{
										out+=epn1;
									}
									else
									{
										out+=epn[0];
									}
								}
							}
						}
					}
				}
				else
				{
					MyChessPiece* pc = NULL;
					bool sameFile = false;
					bool sameRank = false;
					bool moreThanOne = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( blackPcs[i]->type == Knight )
						{
							if( blackPcs[i]->bCaptured == false &&
								!isPiecePinned(blackPcs[i] ))
							{
	
								if( blackPcs[i]->x == fromFile )
								{
									if( blackPcs[i]->y == fromRank )
									{
										pc = blackPcs[i];
										count++;
									}
									else
									{
										bool res = CanKnightReachSquare(blackPcs[i], toFile, toRank);
										if( res ) {
											if( sameFile == false )
												sameFile = true;
											count++;
										}

									}
								}
								else if( blackPcs[i]->y == fromRank )
								{
									
									bool res = CanKnightReachSquare(blackPcs[i], toFile, toRank);
									if( res )
									{
										
										if( sameRank == false )
											sameRank = true;

										count++;
									}
										
								}
								else if( CanKnightReachSquare(blackPcs[i], toFile, toRank) )
								{
										moreThanOne = true;
										count++;
								}						
							}
						}
					}

					if( sameRank && sameFile )
					{
						out+=epn[0];
						out+=epn1;
					}
					else if( sameRank )
					{
						out+=epn[0];
					}
					else if( sameFile )
					{
						out+=epn1;
					}
					else if( moreThanOne )
					{
						out+=epn[0];
					}
				}
				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;
				break;
			}
		case bBishop: 
			{
				out+='B';
				white_black_check = WHITE;
				if( extraBlackBishops == false )
				{
					//MyChessSquare sq = findBlackBishop(toFile, toRank );

				}
				else
				{
					MyChessPiece* pc = NULL;
					bool sameFile = false;
					bool sameRank = false;
					bool moreThanOne = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( blackPcs[i]->type == Bishop )
						{
							if( !blackPcs[i]->bCaptured )
							{
								if( CanBishopReachSquare(blackPcs[i], toFile, toRank) )
								{
									if( fromFile == blackPcs[i]->x )
									{
										if( fromRank == blackPcs[i]->y )
										{
											pc = blackPcs[i];
										}
										else 
										{
											sameFile = true;
										}
									}
									else if( fromRank == blackPcs[i]->y )
									{
										sameRank = true;
									}
									else
									{
										count++;
									}								
								}
							}
						}
					}

					if( count == 1 )
					{
						out+=epn[0];
					}
					if( moreThanOne && (count >1) )
					{
						out+=epn[0];
						out+=epn1;
					}
					else if( sameRank )
					{
						out+=epn[0];
					}
					else if( sameFile )
					{
						out+=epn1;
					}
				}
				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;
				break;
			}
		case bRook:
			{
				out+='R';
				white_black_check = WHITE;
				if( extraBlackRooks == false )
				{
					MyChessPiece *R1 = blackPcs[0];
					MyChessPiece *R2 = blackPcs[7];

					bool canReachR1 =CanRookReachSquare(R1, toFile, toRank );
					bool canReachR2 = CanRookReachSquare(R2, toFile, toRank );

					
					if( canReachR1 && !R1->bCaptured )
					{
						if( canReachR2 && !R2->bCaptured )
						{
							if( R1->x == R2->x )
							{
								out+=epn1;
							}
							else
							{
								out+=epn[0];
							}
						}
					}
				}
				else
				{
					bool sameFile = false;
					bool sameRank = false;
					bool moreThanOne = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( blackPcs[i]->type == Rook )
						{
							if( !blackPcs[i]->bCaptured )
							{
								bool canReach = CanRookReachSquare(blackPcs[i], toFile, toRank );
								if( (isPiecePinned(blackPcs[i])) && 
									(blackPcs[i]->PinFromX != toFile) &&
									(blackPcs[i]->PinFromY != toRank) )
									canReach = false;

								if( canReach )
								{
									if( fromFile == blackPcs[i]->x )
									{
										if( fromRank == blackPcs[i]->y )
										{

										}
										else
										{
											sameFile = true;
											count++;
										}
									}
									else if( fromRank == blackPcs[i]->y )
									{
										sameRank = true;
										count++;
									}
									else moreThanOne = true;
								}
							}
						}
					}

					if( sameRank )
					{
						out+=epn[0];
					}
					else if( sameFile )
					{
						out+=epn1;
					}
					else if( moreThanOne )
					{
						out+=epn[0];
					}
				}
				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;
				break;
			}
		case bQueen:
			{
				out+='Q';
				white_black_check = WHITE;
				if( extraBlackQueens == false )
				{

				}
				else
				{
					bool sameFile = false;
					bool sameRank = false;
					bool sameAsDestFile = false;
					bool sameAsDestRank = false;
					bool sameDiagonal = false;
					bool moreThanOne = false;
					bool notsameFileOrRank = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( blackPcs[i]->type == Queen )
						{
							if( !blackPcs[i]->bCaptured )
							{
								bool canReach = CanQueenReachSquare(blackPcs[i], toFile, toRank );

								if( isPiecePinned( blackPcs[i] ) && 
									(blackPcs[i]->PinFromX != toFile) &&
									(blackPcs[i]->PinFromY != toRank) )
								{
									canReach = false;
									if( (abs(blackPcs[4]->x - blackPcs[i]->x) == 
										abs(blackPcs[4]->y - blackPcs[i]->y)) &&
										(abs(blackPcs[i]->PinFromX- blackPcs[4]->x) ==
										abs(blackPcs[i]->PinFromY - blackPcs[4]->y)))
									{
									
									}
								}
								if( canReach)
								{
									if( blackPcs[i]->x == fromFile )
									{
										if( fromRank == blackPcs[i]->y )
										{
											//count++;
										}
										else
										{
											sameFile = true;
											count++;
										}
									}
									else if( blackPcs[i]->y == fromRank )
									{
										sameRank = true;
										count++;
									}
									else if( abs(blackPcs[i]->x-toFile) == abs(blackPcs[i]->y-toRank) )
									{
										notsameFileOrRank = true;
										count++;
									}
									else if( blackPcs[i]->x == toFile )
									{
										notsameFileOrRank = true;
										sameAsDestFile = true;
										count++;
									}
									else if( blackPcs[i]->y == toRank )
									{
										notsameFileOrRank = true;
										sameAsDestRank = true;
										count++;
									}
									//else if( whitePcs[i]
								}
							}
						}
					}
					if( count > 1 )
					{
						if( !sameRank && sameFile )
						{
							out+=epn1;
						}
						else if( !sameFile && sameRank )
						{
							out+=epn[0];
							
						}
						else if( sameFile && sameRank )
						{
							out+=epn[0];
							out+=epn1;
						}
						else if( !sameFile && !sameRank )
						{
							out+=epn[0];
						}
						/*else if( sameFile && notsameFileOrRank )
						{
							//out+=epn[0];
							out+=epn1;
						}
						else*/

					}
					else if( count == 1 )
					{
						if( !sameRank && sameFile )
						{
							
							out+=epn1;
							
						}
						else if( !sameFile && sameRank )
						{
							out+=epn[0];
						}
						else if( !sameFile && !sameRank )
						{
							out+=epn[0];
						}

					}
					
					/*else
					{
						out+=epn[0];
					}*/
				}
				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;
				break;
			}
		case bKing: 
			{
				
				white_black_check = WHITE;

				if( epn[0]==epn[2]-2)
				{
					out += "O-O";
					break;
				}
				else if( epn[0] == epn[2]+2)
				{
					out += "O-O-O";
					break;
				}
				out+='K';
				if( board[toFile][toRank] != 0 )
				{
					//out += epn[0];
					out += 'x';
				}
				out+=epn[2];
				out+=epn3;
				break;
			}
		case wPawn: 
				{
					if( (board[toFile][toRank] != 0) || (toFile != fromFile))
					{
						out += epn[0];
						out += 'x';
					}

					out+=epn[2];
					out+=epn3;

					if( toRank == 7 )
					{
						if( epn.length() == 5 )
						{
							out+='=';
							out+=toupper(epn[4]);
						}
					}
					white_black_check = BLACK;
				break;
				}
		case wKnight: 
			{
				out+='N'; 
				white_black_check = BLACK;
				if( extraWhiteKnights == false )
				{
						//MyChessSquare sq= findBlackKnight(toFile, toRank);
					MyChessPiece *N1 = whitePcs[1];
					MyChessPiece *N2 = whitePcs[6];
					if( CanKnightReachSquare(N1, toFile, toRank ))
					{
						if( CanKnightReachSquare(N2, toFile, toRank ))
						{
							if( (N1->x == fromFile) && 
								(N1->y == fromRank) )
							{
								if( !N2->bCaptured && !isPiecePinned(N2))
								{
									if( N1->x == N2->x )
									{
										out+=epn1;
									}
									else //else if( N1->y == N2->y )
									{
										out+=epn[0];
									}
								}
							}
							else if( (N2->x == fromFile) &&
									 (N2->y == fromRank))
							{
								if( !N1->bCaptured && !isPiecePinned(N1))
								{
									if( N1->x == N2->x )
									{
										out+=epn1;
									}
									else
									{
										out+=epn[0];
									}
								}
							}
						}
					}
				}
				else
				{
					MyChessPiece* pc = NULL;
					bool sameFile = false;
					bool sameRank = false;
					bool moreThanOne = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( whitePcs[i]->type == Knight )
						{
							if( whitePcs[i]->bCaptured == false && !isPiecePinned(whitePcs[i]) )
							{
								if( whitePcs[i]->x == fromFile )
								{
									if( whitePcs[i]->y == fromRank )
									{
										pc = whitePcs[i];
										count++;
									}
									else
									{
										bool res = CanKnightReachSquare(whitePcs[i], toFile, toRank);
										if( res && !isPiecePinned(whitePcs[i])) {
											if( sameFile == false )
												sameFile = true;
											count++;
										}

									}
								}
								else if( whitePcs[i]->y == fromRank )
								{
									
									bool res = CanKnightReachSquare(whitePcs[i], toFile, toRank);
									if( res && !isPiecePinned(whitePcs[i]) )
									{
										if( sameRank == false )
											sameRank = true;

										count++;
									}
										
								}
								else if( CanKnightReachSquare(whitePcs[i], toFile, toRank) )
								{
									if( !isPiecePinned(whitePcs[i] ))
									{
										moreThanOne = true;
										count++;
									}
								}
							}
						}
					}

					if( sameRank && sameFile )
					{
						out+=epn[0];
						out+=epn1;
					}
					else if( sameRank )
					{
						out+=epn[0];
					}
					else if( sameFile )
					{
						out+=epn1;
					}
					else if( moreThanOne )
					{
						out+=epn[0];
					}
				}

				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;
				break;
			}
		case wBishop:
			{
				out+='B';
				white_black_check = BLACK;
				if( extraWhiteBishops == false )
				{
					//MyChessSquare sq = findBlackBishop(toFile, toRank );
				}
				else
				{
					MyChessPiece* pc = NULL;
					bool sameFile = false;
					bool sameRank = false;
					bool moreThanOne = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( whitePcs[i]->type == Bishop )
						{
							if( !whitePcs[i]->bCaptured )
							{
								bool canReach = true;
								if( isPiecePinned(whitePcs[i]))
								{
									if(!(abs(toFile - whitePcs[4]->x) == abs(toRank-whitePcs[4]->y)) ||
										!(abs(whitePcs[i]->PinFromX - whitePcs[4]->x)== abs(whitePcs[i]->PinFromY - whitePcs[4]->y)))
										canReach = false;
								}
								
								
								if( canReach && CanBishopReachSquare(whitePcs[i], toFile, toRank) )
								{
									if( fromFile == whitePcs[i]->x )
									{
										if( fromRank == whitePcs[i]->y )
										{
											pc = whitePcs[i];
										}
										else 
										{
											sameFile = true;
										}
									}
									else if( fromRank == whitePcs[i]->y )
									{
										sameRank = true;
									}
									else
									{
										count++;
									}							
								}
							}
						}
					}

					if( count == 1 )
					{
						out+=epn[0];
					}
					if( moreThanOne && (count >1) )
					{
						out+=epn[0];
						out+=epn1;
					}
					else if( sameRank )
					{
						out+=epn[0];
					}
					else if( sameFile )
					{
						out+=epn1;
					}
				}

				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;

				break;
			}
		case wRook:
			{
				out+='R';
				white_black_check = BLACK;
				if( extraWhiteRooks == false )
				{
					MyChessPiece *R1 = whitePcs[0];
					MyChessPiece *R2 = whitePcs[7];

					bool canReachR1 =CanRookReachSquare(R1, toFile, toRank );
					bool canReachR2 = CanRookReachSquare(R2, toFile, toRank );

					if( canReachR1 && !R1->bCaptured)
					{
						if( canReachR2 && !R2->bCaptured )
						{
							if( R1->x == R2->x )
							{
								out+=epn1;
							}
							else
							{
								out+=epn[0];
							}
						}
					}
				}
				else
				{
					bool sameFile = false;
					bool sameRank = false;
					bool moreThanOne = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( whitePcs[i]->type == Rook )
						{
							if( !whitePcs[i]->bCaptured )
							{
								bool canReach = CanRookReachSquare(whitePcs[i], toFile, toRank );
								if( (isPiecePinned(whitePcs[i])) && 
									(whitePcs[i]->PinFromX != toFile) &&
									(whitePcs[i]->PinFromY != toRank) )
									canReach = false;

								if( canReach )
								{
									if( fromFile == whitePcs[i]->x )
									{
										if( fromRank == whitePcs[i]->y )
										{

										}
										else
										{
											sameFile = true;
											count++;
										}
									}
									else if( fromRank == whitePcs[i]->y )
									{
										sameRank = true;
										count++;
									}
									else moreThanOne = true;
								}
							}
						}
					}

					if( sameRank )
					{
						out+=epn[0];
					}
					else if( sameFile )
					{
						out+=epn1;
					}
					else if( moreThanOne )
					{
						out+=epn[0];
					}
					/*else if( count > 1 )
					{
						out+=epn[0];
						out+=epn[1];
					}*/


				}

				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;
				break;
			}
		case wQueen:
			{
				out+='Q';
				white_black_check = BLACK;
				if( extraWhiteQueens == false )
				{

				}
				else
				{
					bool sameFile = false;
					bool sameRank = false;
					bool sameDiagonal = false;
					bool moreThanOne = false;
					bool notsameFileOrRank = false;
					int count = 0;
					for( int i = 0; i < 16; i++ )
					{
						if( whitePcs[i]->type == Queen )
						{
							if( !whitePcs[i]->bCaptured )
							{
								bool canReach = CanQueenReachSquare(whitePcs[i], toFile, toRank );

								if( isPiecePinned( whitePcs[i] ) && 
									(whitePcs[i]->PinFromX != toFile) &&
									(whitePcs[i]->PinFromY != toRank) )
								{
									canReach = false;
									if( (abs(whitePcs[4]->x - whitePcs[i]->x) == 
										abs(whitePcs[4]->y - whitePcs[i]->y)) &&
										(abs(whitePcs[i]->PinFromX- whitePcs[4]->x) ==
										abs(whitePcs[i]->PinFromY - whitePcs[4]->y)))
									{
										//if( abs(whitePcs[i]->PinFromX - whitePcs[i]->x) ==
										//	abs(whitePcs[4]->x - toFile)
									}
								}
								if(canReach ) 
								{
									if( whitePcs[i]->x == fromFile )
									{
										if( fromRank == whitePcs[i]->y )
										{
											//count++;
										}
										else
										{
											sameFile = true;
											count++;
										}
									}
									else if( whitePcs[i]->y == fromRank )
									{
										sameRank = true;
										count++;
									}
									else if( abs(whitePcs[i]->x-toFile) == abs(whitePcs[i]->y-toRank) )
									{
										notsameFileOrRank = true;
										count++;
									}
									else if( whitePcs[i]->x == toFile )
									{
										notsameFileOrRank = true;
										count++;
									}
									else if( whitePcs[i]->y == toRank )
									{
										notsameFileOrRank = true;
										count++;
									}
									//else if( whitePcs[i]
								}
							}
						}
					}
					if( count > 1 )
					{
						if( !sameRank && sameFile )
						{
							out+=epn1;
						}
						else if( !sameFile && sameRank )
						{
							out+=epn[0];
							
						}
						else if( sameFile && sameRank )
						{
							out+=epn[0];
							out+=epn1;
						}
						else if( !sameFile && !sameRank )
						{
							out+=epn[0];
						}
						/*else if( sameFile && notsameFileOrRank )
						{
							//out+=epn[0];
							out+=epn1;
						}
						else*/

					}
					else if( count == 1 )
					{
						if( !sameRank && sameFile )
						{
							
							out+=epn1;
							
						}
						else if( !sameFile && sameRank )
						{
							out+=epn[0];
						}
						else if( !sameFile && !sameRank )
						{
							out+=epn[0];
						}

					}
				}
				if( board[toFile][toRank] != 0 )
				{
					out += 'x';
				}

				out+=epn[2];
				out+=epn3;

				break;
			}
		case wKing: 
			{
				white_black_check = BLACK;
				if( epn[0]==epn[2]-2)
				{
					out += "O-O";
					break;
				}
				else if( epn[0] == epn[2]+2)
				{
					out += "O-O-O";
					break;
				}
				out+='K';
				
				if( board[toFile][toRank] != 0 )
				{
					//out += epn[0];
					out += 'x';
				}
				out+=epn[2];
				out+=epn3;
				break;
			}
		}

		if( epn.length() == 5 )
		{
			promote = true;

			if( epn[4] == 'q' )
				typeForPromotion = 0;
			if( epn[4] == 'b' )
				typeForPromotion = 1;
			if( epn[4] == 'n' )
				typeForPromotion = 2;
			if( epn[4] == 'r' )
				typeForPromotion = 3;
		}
		

		makemove2(fromFile, fromRank, toFile, toRank, promote, typeForPromotion);

		if( promote && !(epn.length() == 5))
		{
			out+= "=";
			switch( typeForPromotion )
			{
			case 0: out+= "Q"; break;
			case 1: out+= "B"; break;
			case 2: out+= "N"; break;
			case 3: out+= "R"; break;
			}
		}

		if( white_black_check == WHITE )
		{
			int kingStatus = IsKingInCheckMate(whitePcs[4]);
			if( kingStatus == CHECKMATE )
			{
				out += '#';
				GameStatus = CHECKMATE;
			}
			if( kingStatus == CHECK )
			{
				out+='+';
				GameStatus = CHECK;
			}
			if( kingStatus == STALEMATE )
			{
				GameStatus = STALEMATE;
			}
		}
		else
		{
			int kingStatus = IsKingInCheckMate(blackPcs[4]);
			if( kingStatus == CHECKMATE )
			{
				out += '#';
				GameStatus = CHECKMATE;
			}
			if( kingStatus == CHECK )
			{
				out+='+';
				GameStatus = CHECK;
			}
			if( kingStatus == STALEMATE )
			{
				GameStatus = STALEMATE;
			}
		}

		return out;
	}

	

	bool PieceAttacksSquare(MyChessPiece *Pc, int X, int Y)
	{
		switch( Pc->type )
		{
		case Rook: return this->CanRookReachSquare(Pc, X, Y );
		case Knight: return this->CanKnightReachSquare(Pc, X, Y );
		case Bishop: return this->CanBishopReachSquare(Pc, X, Y );
		case Queen: return this->CanQueenReachSquare(Pc, X, Y );
		case King: return this->CanKingAttackSquare(Pc, X, Y );
		}
		return false;
	}


	

	bool BlackPawnAttacksSquare(int x, int y,int *fromX=0, int *fromY= 0  )
	{
		for( int i = 8; i < 16; i++ )
		{
			if( !blackPcs[i]->bCaptured )
			{
				if( blackPcs[i]->type != Pawn )
				{
					if( PieceAttacksSquare(blackPcs[i], x,y))
					{
						if( fromX != NULL )
						{
							*fromX = blackPcs[i]->x;
							*fromY = blackPcs[i]->y;
						}
						return true;
					}
				}
				else if( blackPcs[i]->y == y +1)
				{
					if( ((blackPcs[i]->x == x - 1) ||
						(blackPcs[i]->x == x + 1)))
					{
						if( fromX != NULL )
						{
							*fromX = blackPcs[i]->x;
							*fromY = blackPcs[i]->y;
						}
							return true;
					}
				}
			}
		}

		return false;
	}

	bool BlackPawnAttacksSquareIgnoringKing(int x, int y,int *fromX=0, int *fromY= 0  )
	{
		for( int i = 8; i < 16; i++ )
		{
			if( !blackPcs[i]->bCaptured )
			{
				if( blackPcs[i]->type != Pawn )
				{
					if( PieceAttacksSquareIgnoringKing(blackPcs[i], x,y))
					{
						if( fromX != NULL )
						{
							*fromX = blackPcs[i]->x;
							*fromY = blackPcs[i]->y;
						}
						return true;
					}
				}
				else if( blackPcs[i]->y == y +1)
				{
					if( ((blackPcs[i]->x == x - 1) ||
						(blackPcs[i]->x == x + 1)))
					{
						if( fromX != NULL )
						{
							*fromX = blackPcs[i]->x;
							*fromY = blackPcs[i]->y;
						}
							return true;
					}
				}
			}
		}

		return false;
	}

	bool WhitePawnAttacksSquare(int x, int y, int *fromX=0, int *fromY= 0 )
	{
		for( int i = 8; i < 16; i ++ )
		{
			if( !whitePcs[i]->bCaptured )
			{
				if( whitePcs[i]->type != Pawn )
				{
					if( PieceAttacksSquare(whitePcs[i], x,y))
					{
						if( fromX != NULL )
						{
							*fromX = whitePcs[i]->x;
							*fromY = whitePcs[i]->y;
						}
						return true;
					}
				}
				else if( whitePcs[i]->y == y -1)
				{
					if( ((whitePcs[i]->x == x - 1) ||
						(whitePcs[i]->x == x + 1)))
					{
						if( fromX != NULL )
						{
							*fromX = whitePcs[i]->x;
							*fromY = whitePcs[i]->y;
						}
							return true;
					}
				}
			}
		}

		return false;
	}

	bool WhitePawnAttacksSquareIgnoringKing(int x, int y, int *fromX=0, int *fromY= 0 )
	{
		for( int i = 8; i < 16; i ++ )
		{
			if( !whitePcs[i]->bCaptured )
			{
				if( whitePcs[i]->type != Pawn )
				{
					if( PieceAttacksSquareIgnoringKing(whitePcs[i], x,y))
					{
						if( fromX != NULL )
						{
							*fromX = whitePcs[i]->x;
							*fromY = whitePcs[i]->y;
						}
						return true;
					}
				}
				else if( whitePcs[i]->y == y -1)
				{
					if( ((whitePcs[i]->x == x - 1) ||
						(whitePcs[i]->x == x + 1)))
					{
						if( fromX != NULL )
						{
							*fromX = whitePcs[i]->x;
							*fromY = whitePcs[i]->y;
						}
							return true;
					}
				}
			}
		}

		return false;
	}

	bool PieceAttacksSquareIgnoringKing( MyChessPiece *PC, int X, int Y)
	{
		//this->CanP
		
		switch( PC->type )
		{
		case Rook: return this->CanRookReachSquareCheckXray(PC, X, Y );
		
		case Bishop: return this->CanBishopReachSquareCheckXray(PC, X, Y );
		case Queen: return this->CanQueenReachSquareCheckXray(PC, X, Y );
		}


		return false;
	
	}

	bool IsSquareDirectlyAttackedIgnoringKing(int color, int x, int y, int *fromX=0, int *fromY = 0)
	{
		if( color == BLACK )
		{
			for( int i = 0; i < 8; i++ )
			{
				if( !blackPcs[i]->bCaptured )
				{
					if( PieceAttacksSquareIgnoringKing(blackPcs[i],x,y) )
					{
						if( fromX != NULL )
						{
							*fromX = blackPcs[i]->x;
							*fromY = blackPcs[i]->y;
						}
						return true;
					}
				}
			}
		
			if( BlackPawnAttacksSquareIgnoringKing( x, y, fromX, fromY))
				return true;
			
		}
		else 
		{
			for( int i = 0; i < 8; i++ )
			{
				if( !whitePcs[i]->bCaptured )
				{
					if( PieceAttacksSquareIgnoringKing(whitePcs[i],x,y) )
					{
						if( fromX != NULL )
						{
							*fromX = whitePcs[i]->x;
							*fromY = whitePcs[i]->y;
						}
						return true;
					}
				}
			}
			if( WhitePawnAttacksSquareIgnoringKing( x, y, fromX, fromY))
				return true;
		}
		return false;
	}

	bool IsSquareDirectlyAttacked(int color, int x, int y, int *fromX=0, int *fromY = 0)
	{
		if( color == BLACK )
		{
			for( int i = 0; i < 8; i++ )
			{
				if( !blackPcs[i]->bCaptured )
				{
					if( PieceAttacksSquare(blackPcs[i],x,y) )
					{
						if( fromX != NULL )
						{
							*fromX = blackPcs[i]->x;
							*fromY = blackPcs[i]->y;
						}
						return true;
					}
				}
			}
		
			if( BlackPawnAttacksSquare( x, y, fromX, fromY))
				return true;
			
		}
		else 
		{
			for( int i = 0; i < 8; i++ )
			{
				if( !whitePcs[i]->bCaptured )
				{
					if( PieceAttacksSquare(whitePcs[i],x,y) )
					{
						if( fromX != NULL )
						{
							*fromX = whitePcs[i]->x;
							*fromY = whitePcs[i]->y;
						}
						return true;
					}
				}
			}
			if( WhitePawnAttacksSquare( x, y, fromX, fromY))
				return true;
		}
		return false;
	}

	bool GetPiecesAttackingSquare(int color, int x, int y, vector<MyChessPiece*> *attackers)
	{
		if( color == BLACK )
		{
			for( int i = 0; i < 8; i++ )
			{
				if( !blackPcs[i]->bCaptured )
					if( PieceAttacksSquare(blackPcs[i],x,y) )
					{
						attackers->push_back(blackPcs[i]);
					}
			}
		
			for( int i = 8; i < 16; i++ )
			{
				if( !blackPcs[i]->bCaptured )
				{
					if( blackPcs[i]->type != Pawn )
					{
						if( PieceAttacksSquare(blackPcs[i], x,y))
						{
							attackers->push_back(blackPcs[i]);
							//return true;
						}
					}
					else if( blackPcs[i]->y == y +1)
					{
						if( ((blackPcs[i]->x == x - 1) ||
							(blackPcs[i]->x == x + 1)))
						{
							attackers->push_back(blackPcs[i]);
								//return true;
						}
					}
				}
			}			
		}
		else 
		{
			for( int i = 0; i < 8; i++ )
			{
				if( !whitePcs[i]->bCaptured )
					if( PieceAttacksSquare(whitePcs[i],x,y) )
					{
						attackers->push_back(whitePcs[i]);
						//return true;
					}
			}
			for( int i = 8; i < 16; i ++ )
			{
				if( !whitePcs[i]->bCaptured )
				{
					if( whitePcs[i]->type != Pawn )
					{
						if( PieceAttacksSquare(whitePcs[i], x,y))
						{
							attackers->push_back(whitePcs[i]);
						}
					}
					else if( whitePcs[i]->y == y -1)
					{
						if( ((whitePcs[i]->x == x - 1) ||
							(whitePcs[i]->x == x + 1)))
						{
							attackers->push_back(whitePcs[i]);
						}
					}
				}
			}
		}
		if( attackers->size() > 0 )
			return true;
		else return false;
	}

	bool IsKingInCheck(int color, int *fromX = 0, int *fromY = 0)
	{

		if (color == WHITE )
		{
			return IsSquareDirectlyAttacked( BLACK, whitePcs[4]->x, whitePcs[4]->y, fromX, fromY);
		}
		else
		{
			return IsSquareDirectlyAttacked( WHITE, blackPcs[4]->x, blackPcs[4]->y, fromX, fromY);
		}
	}



	bool IsPieceDefended(MyChessPiece* PC)
	{
		if( PC->color == WHITE )
		{
			for( int i = 0; i < 8; i++ )
			{
				if( PC != whitePcs[i] )
				{
					if( !whitePcs[i]->bCaptured )
						if( PieceAttacksSquare(whitePcs[i],PC->x,PC->y) )
							return true;
				}
			}
			for( int i = 8; i < 16; i ++ )
			{
				if( PC != whitePcs[i] )
				{
					if( !whitePcs[i]->bCaptured )
					{
						if( whitePcs[i]->type != Pawn )
						{
							if( PieceAttacksSquare(whitePcs[i], PC->x,PC->y))
							{
								return true;
							}
						}
						else if( whitePcs[i]->y == PC->y -1)
						{
							if( ((whitePcs[i]->x == PC->x - 1) ||
								(whitePcs[i]->x == PC->x + 1)))
							{
									return true;
							}
						}
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < 8; i++ )
			{
				if( PC != blackPcs[i] )
				{
					if( !blackPcs[i]->bCaptured )
						if( PieceAttacksSquare(blackPcs[i],PC->x,PC->y) )
							return true;
				}
			}
			for( int i = 8; i < 16; i ++ )
			{
				if( PC != blackPcs[i] )
				{
					if( !blackPcs[i]->bCaptured )
					{
						if( blackPcs[i]->type != Pawn )
						{
							if( PieceAttacksSquare(blackPcs[i], PC->x,PC->y))
							{
								return true;
							}
						}
						else if( blackPcs[i]->y == PC->y -1)
						{
							if( ((blackPcs[i]->x == PC->x - 1) ||
								(blackPcs[i]->x == PC->x + 1)))
							{
									return true;
							}
						}
					}
				}
			}
		}
	}

	void SetSquare(int x, int y, unsigned char val)
	{
		board[x][y-1] = val;
		//cboard[(y-1)*8+x] = val;
	}


	bool IsSquareChecked( int color, int x, int y, int *fromX = 0, int *fromY = 0 )
	{
		int colA;
		int colB;
		if( color == WHITE )
		{
			colA = BLACK;
			colB = WHITE;
		}
		else
		{
			colA = WHITE;
			colB = BLACK;
		}
		if( !IsSquareDirectlyAttacked(colA, x, y ) )
		{
			if( board[x][y] != 0 )
			{
				if( PieceColor(x, y) != colB )
				{
					return false;
				}
				else 
				{
					return true;
				}
			}
			else
			{
				
				if( *fromX == x )
				{
					if( !isFileSpanBlockedIgnoringKing(colB, x, y, *fromY ))
					{
						return true;
					}
				}
				else if( *fromY == y )
				{
					if( !isRankSpanBlockedIgnoringKing(colB, x, *fromX, y ))
					{
						return true;
					}
				}
				else if( abs(*fromX-x) == abs(*fromY-y))
				{
					if( IsDiagonalSpanClearIgnoringKing(colB, *fromX, *fromY,x,y ))
					{
						return true;
					}
				}
				return false;
			}
		}
		return true;
	}

	int GetNumSquaresAvailableToKing(MyChessPiece *K)
	{
		int numSquares = 0;
		MyChessSquare king_squares[8];
		king_squares[0].Set( K->x-1, K->y-1);
		king_squares[1].Set( K->x-1, K->y);
		king_squares[2].Set( K->x-1, K->y+1);
		king_squares[3].Set( K->x, K->y-1);
		king_squares[4].Set( K->x, K->y+1);
		king_squares[5].Set( K->x+1, K->y-1);
		king_squares[6].Set( K->x+1, K->y);
		king_squares[7].Set( K->x+1, K->y+1);
		if( K->color == WHITE )
		{
			for( int i = 0; i < 8; i++ )
			{
				if( (king_squares[i].file >= 0) && 
					(king_squares[i].rank >= 0) &&
					(king_squares[i].rank < 8) && 
					(king_squares[i].file < 8) )
				{
					if( (board[king_squares[i].file][king_squares[i].rank] != wPawn) &&
						(board[king_squares[i].file][king_squares[i].rank] != wRook) &&
						(board[king_squares[i].file][king_squares[i].rank] != wKnight) &&
						(board[king_squares[i].file][king_squares[i].rank] != wBishop) &&
						(board[king_squares[i].file][king_squares[i].rank] != wQueen) )
					{
						if( !IsSquareDirectlyAttacked(BLACK, king_squares[i].file, king_squares[i].rank) )
						{
							if( !IsSquareDirectlyAttackedIgnoringKing(BLACK, king_squares[i].file, king_squares[i].rank ))
							{
								numSquares++;
							}
							// we got a square
							
						}
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < 8; i++ )
			{
				if( (king_squares[i].file >= 0) && 
					(king_squares[i].rank >= 0) &&
					(king_squares[i].rank < 8) && 
					(king_squares[i].file < 8) )
				{
					if( (board[king_squares[i].file][king_squares[i].rank] != bPawn) &&
						(board[king_squares[i].file][king_squares[i].rank] != bRook) &&
						(board[king_squares[i].file][king_squares[i].rank] != bKnight) &&
						(board[king_squares[i].file][king_squares[i].rank] != bBishop) &&
						(board[king_squares[i].file][king_squares[i].rank] != bQueen) )
					{
						if( !IsSquareDirectlyAttacked(WHITE, king_squares[i].file, king_squares[i].rank) )
						{
							// we got a square
							if( !IsSquareDirectlyAttackedIgnoringKing(WHITE, king_squares[i].file, king_squares[i].rank ))
							{
								numSquares++;
							}
						}
					}
				}
			}
		}
		return numSquares;
	}

	
	
	bool CanPieceIntercedeFileSpan(MyChessPiece* PC, int x, int y1, int y2, int *XX=0, int *YY=0 )
	{
		int Y1 = y1;
		int Y2 = y2;


		if( Y1 < Y2 )
		{
			for( int Y = Y1+1; Y <= Y2; Y++ )
			{
				switch( PC->type )
				{
				case Bishop: 
					{
						if( CanBishopReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Knight: 
					{
						if( CanKnightReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Rook: 
					{
						if( CanRookReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Queen: 
					{
						if( CanQueenReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Pawn: 
					{
						if( CanPawnReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				}
			}
		}

		if( Y1 > Y2 )
		{
			Y1 = y2;
			Y2 = y1;

			for( int Y = Y1; Y < Y2; Y++ )
			{
				switch( PC->type )
				{
				case Bishop: 
					{
						if( CanBishopReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Knight: 
					{
						if( CanKnightReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Rook: 
					{
						if( CanRookReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Queen: 
					{
						if( CanQueenReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				case Pawn: 
					{
						if( CanPawnReachSquare(PC, x, Y) )
						{
							if( XX != 0 && YY != 0 )
							{
								*XX = x;
								*YY = Y;
							}
							return true; 
						}
					}
					break;
				}
			}
		}


		return false;
	}

	bool CanPieceIntercedeRankSpan(MyChessPiece* PC, int x1, int x2, int y, int *XX=0, int *YY=0 )
	{
		int X1 = x1;
		int X2 = x2;

		if( X1 < X2 )
		{
			for( int X = X1+1; X <= X2; X++ )
			{
				switch( PC->type )
				{
				case Bishop: 
					{
						if( CanBishopReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Knight: 
					{
						if( CanKnightReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Rook: 
					{
						if( CanRookReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Queen: 
					{
						if( CanQueenReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Pawn: 
					{
						if( CanPawnReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				}
			}
		}

		if( X1 > X2 )
		{
			X1 = x2;
			X2 = x1;
			for( int X = X1; X < X2; X++ )
			{
				switch( PC->type )
				{
				case Bishop: 
					{
						if( CanBishopReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Knight: 
					{
						if( CanKnightReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Rook: 
					{
						if( CanRookReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Queen: 
					{
						if( CanQueenReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				case Pawn: 
					{
						if( CanPawnReachSquare(PC, X, y) )
						{
							if( XX!=0 && YY!=0)
							{
								*XX=X;
								*YY=y;
							}
							return true; 
						}
					}
					break;
				}
			}
		}


		return false;
	}

	bool CanPieceReachSquare(MyChessPiece *PC, int X, int Y)
	{
		switch(PC->type)
		{
		case Bishop: return CanBishopReachSquare(PC, X, Y );
		case Knight:return CanKnightReachSquare(PC, X,Y);
		case Pawn:return CanPawnReachSquare(PC, X,Y);
		case Rook: return CanRookReachSquare(PC,X,Y);
		case Queen:return CanQueenReachSquare(PC,X,Y);
		}
	}

	bool CanPieceIntercedeDiagonalSpan(MyChessPiece *PC, int x1, int y1, int x2, int y2, int *XX=0, int *YY=0 )
	{
		if((x1 < x2) && (y1 < y2))
		{
			for( int x=x1+1, y=y1+1; (y<=y2)&&(x<=x2); x++, y++)
			{
				if( CanPieceReachSquare(PC, x,y))
				{
					if( XX!=0 && YY!=0 )
					{
						*XX = x;
						*YY = y;
					}
					return true;
				}
			}
		}
		if( (x1 < x2) && (y1 > y2) )
		{
			for( int x=x1+1, y = y1-1; (y>=y2)&&(x<=x2); x++, y--)
			{
				if( CanPieceReachSquare(PC, x,y))
				{
					if( XX!=0 && YY!=0 )
					{
						*XX = x;
						*YY = y;
					}
					return true;
				}
			}
		}
		if((x1 > x2) && (y1 < y2))
		{
			for( int y=y1+1, x = x1-1; (x>=x2)&&(y<=y2); y++, x--)
			{
				if( CanPieceReachSquare(PC, x,y))
				{
					if( XX!=0 && YY!=0 )
					{
						*XX = x;
						*YY = y;
					}
					return true;
				}
			}
		}
		if((x1 > x2) && (y1 > y2))
		{
			for( int x=x2, y=y2; (y<y1)&&(x<x1); x++, y++)
			{
				if( CanPieceReachSquare(PC, x,y))
				{
					if( XX!=0 && YY!=0 )
					{
						*XX = x;
						*YY = y;
					}
					return true;
				}
			}
		}
		return false;
	}

	bool CanPiecePreventCheck(MyChessPiece *K, int fromX, int fromY )
	{
		for( int i = 0; i < 16; i++ )
		{
			if( i == 4 ) 
				continue;
			if( K->x == fromX )
			{
				if( K->color == WHITE )
				{
					if( !whitePcs[i]->bCaptured )
					{
						if( CanPieceIntercedeFileSpan(whitePcs[i], K->x, K->y, fromY ) )
							if( !isPiecePinned(whitePcs[i]))
								return true;
					}
				}
				else
				{
					if( !blackPcs[i]->bCaptured )
					{
						if( CanPieceIntercedeFileSpan(blackPcs[i], K->x, K->y, fromY ) )
							if( !isPiecePinned(blackPcs[i]))
								return true;
					}
				}
			}
			else if( K->y == fromY )
			{
				if( K->color == WHITE )
				{
					if( !whitePcs[i]->bCaptured )
					{
						if( CanPieceIntercedeRankSpan(whitePcs[i], K->x, fromX, fromY ) )
							if( !isPiecePinned(whitePcs[i]))
								return true;
					}
				}
				else
				{
					if( !blackPcs[i]->bCaptured )
					{
						if( CanPieceIntercedeRankSpan(blackPcs[i], K->x, fromX, fromY ) )
							if( !isPiecePinned(blackPcs[i]))
								return true;
					}
				}	
			}
			else if( abs(K->x - fromX) == abs(K->y - fromY))
			{
				if( K->color == WHITE )
				{
					if( !whitePcs[i]->bCaptured )
					{
						if( CanPieceIntercedeDiagonalSpan(whitePcs[i], K->x, K->y, fromX, fromY ))
							if( !isPiecePinned(whitePcs[i]))
								return true;
					}
				}
				else
				{
					if( !blackPcs[i]->bCaptured )
					{
						if( CanPieceIntercedeDiagonalSpan(blackPcs[i], K->x, K->y, fromX, fromY ))
							if( !isPiecePinned(blackPcs[i]))
								return true;
					}
				}	
			}
			else 
			{
				if( K->color == WHITE )
				{
					vector<MyChessPiece*> attackers;
					if( GetPiecesAttackingSquare(WHITE, fromX, fromY, &attackers ))
					{
						for( int i = 0; i < attackers.size(); i++ )
						{
							if( !attackers[i]->bCaptured )
								if( !isPiecePinned(attackers[i]))
									return true;
						}
						return false;
					}
				}
				else
				{
					vector<MyChessPiece*> attackers;
					if( GetPiecesAttackingSquare(BLACK, fromX, fromY, &attackers ))
					{
						for( int i = 0; i < attackers.size(); i++ )
						{
							if( !attackers[i]->bCaptured )
								if( !isPiecePinned(attackers[i]))
									return true;
						}

						return false;
					}
				}
			}
		}

		return false;
	}


	bool CanKnightMove(MyChessPiece* N)
	{
		MyChessSquare knightMoves[8];
		knightMoves[0].Set(N->x -2, N->y - 1);
		knightMoves[1].Set(N->x -2, N->y + 1);
		knightMoves[2].Set(N->x -1, N->y + 2);
		knightMoves[3].Set(N->x +1, N->y + 2);
		knightMoves[4].Set(N->x +2, N->y + 1);
		knightMoves[5].Set(N->x +2, N->y - 1);
		knightMoves[6].Set(N->x +1, N->y -2 );
		knightMoves[7].Set(N->x -1, N->y -2);

		for( int i = 0; i < 8; i++ )
		{
			if( (knightMoves[i].file >= 0) && (knightMoves[i].file < 8) &&
				(knightMoves[i].rank >= 0) && (knightMoves[i].rank < 8))
			{
				if( CanKnightReachSquare(N, knightMoves[i].file, knightMoves[i].rank ) )
				{
					if( N->color == WHITE )
					{
						if( board[knightMoves[i].file][knightMoves[i].rank] == 0 )
						{
							return true;
						}
						else 
						{
							if( (board[knightMoves[i].file][knightMoves[i].rank] != wPawn) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != wRook) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != wBishop) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != wQueen) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != wKing) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != wKnight))
							{
								return true;
							}
						}	
					}	
					else
					{
						if( board[knightMoves[i].file][knightMoves[i].rank] == 0 )
						{
							return true;
						}
						else 
						{
							if( (board[knightMoves[i].file][knightMoves[i].rank] != bPawn) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != bRook) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != bBishop) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != bQueen) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != bKing) &&
								(board[knightMoves[i].file][knightMoves[i].rank] != bKnight))
							{
								return true;
							}
						}	
					}
				}
			}

		}

		return false;
	}

	bool CanBishopMove(MyChessPiece *B)
	{
		MyChessSquare surroundBishop[4];
		surroundBishop[0].Set( B->x-1, B->y-1);
		surroundBishop[1].Set( B->x+1, B->y-1);
		surroundBishop[2].Set( B->x-1, B->y+1);
		surroundBishop[3].Set( B->x+1, B->y+1);
		for( int i = 0; i < 4; i++ )
		{
			if( (surroundBishop[i].file >=0) &&
				(surroundBishop[i].file < 8) &&
				(surroundBishop[i].rank >=0) &&
				(surroundBishop[i].rank < 8))
			{
				if( B->color == WHITE )
				{
					if( board[surroundBishop[i].file][surroundBishop[i].rank] == 0 )
					{
						return true;
					}
					else
					{
						if( (board[surroundBishop[i].file][surroundBishop[i].rank] != wPawn) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != wRook) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != wKnight) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != wBishop) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != wQueen) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != wKing))
						{
							return true;
						}

					}
				}
				else
				{
					if( board[surroundBishop[i].file][surroundBishop[i].rank] == 0 )
					{
						return true;
					}
					else
					{
						if( (board[surroundBishop[i].file][surroundBishop[i].rank] != bPawn) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != bRook) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != bKnight) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != bBishop) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != bQueen) &&
							(board[surroundBishop[i].file][surroundBishop[i].rank] != bKing))
						{
							return true;
						}

					}
				}
			}
		}
		return false;
	}

	bool CanRookMove(MyChessPiece* R)
	{
		MyChessSquare rmvs[4];
		rmvs[0].Set(R->x+1, R->y);
		rmvs[1].Set(R->x-1, R->y);
		rmvs[2].Set(R->x, R->y+1);
		rmvs[3].Set(R->x, R->y-1);


		for( int i = 0; i < 4; i++ )
		{
			if( (rmvs[i].file >=0) &&
				(rmvs[i].file < 8) &&
				(rmvs[i].rank >=0) &&
				(rmvs[i].rank < 8))
			{
				if( R->color == WHITE )
				{
					if( board[rmvs[i].file][rmvs[i].rank] == 0 )
					{
						return true;
					}
					else
					{
						if( (board[rmvs[i].file][rmvs[i].rank] != wPawn) &&
							(board[rmvs[i].file][rmvs[i].rank] != wRook) &&
							(board[rmvs[i].file][rmvs[i].rank] != wKnight) &&
							(board[rmvs[i].file][rmvs[i].rank] != wBishop) &&
							(board[rmvs[i].file][rmvs[i].rank] != wQueen) &&
							(board[rmvs[i].file][rmvs[i].rank] != wKing))
						{
							return true;
						}
					}
				}
				else
				{
					if( board[rmvs[i].file][rmvs[i].rank] == 0 )
					{
						return true;
					}
					else
					{
						if( (board[rmvs[i].file][rmvs[i].rank] != bPawn) &&
							(board[rmvs[i].file][rmvs[i].rank] != bRook) &&
							(board[rmvs[i].file][rmvs[i].rank] != bKnight) &&
							(board[rmvs[i].file][rmvs[i].rank] != bBishop) &&
							(board[rmvs[i].file][rmvs[i].rank] != bQueen) &&
							(board[rmvs[i].file][rmvs[i].rank] != bKing))
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	bool CanQueenMove(MyChessPiece *Q)
	{
		MyChessSquare qmvs[8];
		qmvs[0].Set( Q->x-1, Q->y-1);
		qmvs[1].Set( Q->x-1, Q->y);
		qmvs[2].Set( Q->x-1, Q->y+1);
		qmvs[3].Set( Q->x, Q->y-1);
		qmvs[4].Set( Q->x, Q->y+1);
		qmvs[5].Set( Q->x+1, Q->y-1);
		qmvs[6].Set( Q->x+1, Q->y);
		qmvs[7].Set( Q->x+1, Q->y+1);

		for( int i = 0; i < 8; i++ )
		{
			if( (qmvs[i].file >=0) &&
				(qmvs[i].file < 8) &&
				(qmvs[i].rank >=0) &&
				(qmvs[i].rank < 8))
			{
				if( Q->color == WHITE )
				{
					if( board[qmvs[i].file][qmvs[i].rank] == 0 )
					{
						return true;
					}
					else
					{
						if( (board[qmvs[i].file][qmvs[i].rank] != wPawn) &&
							(board[qmvs[i].file][qmvs[i].rank] != wRook) &&
							(board[qmvs[i].file][qmvs[i].rank] != wKnight) &&
							(board[qmvs[i].file][qmvs[i].rank] != wBishop) &&
							(board[qmvs[i].file][qmvs[i].rank] != wQueen) &&
							(board[qmvs[i].file][qmvs[i].rank] != wKing))
						{
							return true;
						}
					}
				}
				else
				{
					if( board[qmvs[i].file][qmvs[i].rank] == 0 )
					{
						return true;
					}
					else
					{
						if( (board[qmvs[i].file][qmvs[i].rank] != bPawn) &&
							(board[qmvs[i].file][qmvs[i].rank] != bRook) &&
							(board[qmvs[i].file][qmvs[i].rank] != bKnight) &&
							(board[qmvs[i].file][qmvs[i].rank] != bBishop) &&
							(board[qmvs[i].file][qmvs[i].rank] != bQueen) &&
							(board[qmvs[i].file][qmvs[i].rank] != bKing))
						{
							return true;
						}
					}
				}
			}
		}
		return false;

	}

	bool CanPawnMove(MyChessPiece *P)
	{
		if( P->color == WHITE )
		{
			if( board[P->x][P->y+1] == 0 )
				return true;
			else
			{
				MyChessSquare pmvs[2];
				pmvs[0].Set(P->x+1,P->y+1); 
				pmvs[1].Set(P->x-1,P->y+1);

				for( int i =0; i <2; i++ )
				{
					if( (pmvs[i].file >= 0) && 
						(pmvs[i].file < 8) &&
						(board[pmvs[i].file][pmvs[i].rank] != 0) &&
						(board[pmvs[i].file][pmvs[i].rank] != wPawn) &&
						(board[pmvs[i].file][pmvs[i].rank] != wRook) &&
						(board[pmvs[i].file][pmvs[i].rank] != wKnight) &&
						(board[pmvs[i].file][pmvs[i].rank] != wBishop) &&
						(board[pmvs[i].file][pmvs[i].rank] != wQueen) &&
						(board[pmvs[i].file][pmvs[i].rank] != wKing))
					{
						return true;
					}
				}

				if( this->canEmpasse )
				{
					if( this->empassantTarget.sq.file == pmvs[0].file &&
						this->empassantTarget.sq.rank == pmvs[0].rank )
					{
						return true;
					}
					if( this->empassantTarget.sq.file == pmvs[1].file &&
						this->empassantTarget.sq.rank == pmvs[1].rank )
					{
						return true;
					}
					
				}
				return false;
				
			}
			return false;
		}
		else
		{
			if( board[P->x][P->y-1] == 0 )
				return true;
			else
			{
				MyChessSquare pmvs[2];
				pmvs[0].Set(P->x+1,P->y-1); 
				pmvs[1].Set(P->x-1,P->y-1);

				for( int i =0; i <2; i++ )
				{
					if( (pmvs[i].file >= 0) && 
						(pmvs[i].file < 8) &&
						(board[pmvs[i].file][pmvs[i].rank] != 0) &&
						(board[pmvs[i].file][pmvs[i].rank] != bPawn) &&
						(board[pmvs[i].file][pmvs[i].rank] != bRook) &&
						(board[pmvs[i].file][pmvs[i].rank] != bKnight) &&
						(board[pmvs[i].file][pmvs[i].rank] != bBishop) &&
						(board[pmvs[i].file][pmvs[i].rank] != bQueen) &&
						(board[pmvs[i].file][pmvs[i].rank] != bKing))
					{
						return true;
					}
				}

				if( this->canEmpasse )
				{
					if( this->empassantTarget.sq.file == pmvs[0].file &&
						this->empassantTarget.sq.rank == pmvs[0].rank )
					{
						return true;
					}
					if( this->empassantTarget.sq.file == pmvs[1].file &&
						this->empassantTarget.sq.rank == pmvs[1].rank )
					{
						return true;
					}
					
				}
				return false;
				
			}
			return false;
		}
	}

	

	int IsKingInCheckMate(MyChessPiece* K)
	{
		
		//if( numSquares == 0 )
		{
			vector<MyChessPiece*> attackers;
			if( K->color == WHITE )
			{
				if( GetPiecesAttackingSquare(BLACK, K->x, K->y, &attackers) )
				{
					int numSquares = GetNumSquaresAvailableToKing(K);
					if( attackers.size() > 1 )
					{
						
						if( numSquares == 0 )
						{
							return CHECKMATE;
						}
						else return CHECK;
					}
					else
					{
						if( attackers.size() == 1 )
						{
							// findPieceToInterCede
							if( numSquares != 0 )
							{
								return CHECK;
							}
							else if( CanPiecePreventCheck(K, attackers[0]->x, attackers[0]->y))
							{
								return CHECK;
							}
							else return CHECKMATE;

						}
					}
				}
				else
				{
					int numPcs = 0;
					int numSquares = GetNumSquaresAvailableToKing(K);

					if( numSquares != 0 ) return KINGSAFE;
					// find any movable piece
					for( int i = 0; i < 16; i++ )
					{
						if( (!whitePcs[i]->bCaptured) && (i!=4))
						{
							if( !this->isPiecePinned(whitePcs[i]) )
							{
								switch(whitePcs[i]->type)
								{
								case Bishop: 
									{
										if( CanBishopMove(whitePcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								case Knight:
									{
										if( CanKnightMove(whitePcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								case Rook:
									{
										if( CanRookMove(whitePcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;

								case Queen:
									{
										if( CanQueenMove(whitePcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								case Pawn:
									{
										if( CanPawnMove(whitePcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								}
							}
						}
					}	
					return STALEMATE;
				}
			}
			else
			{
				if( GetPiecesAttackingSquare(WHITE, K->x, K->y, &attackers) )
				{
					int numSquares = GetNumSquaresAvailableToKing(K);
					if( attackers.size() > 1 )
					{
						
						if( numSquares == 0 )
							return CHECKMATE;
						else return CHECK;
					}
					else
					{
						if( attackers.size() == 1 )
						{
							// findPieceToInterCede
							if( numSquares != 0 )
							{
								return CHECK;
							}
							else if( CanPiecePreventCheck(K, attackers[0]->x, attackers[0]->y))
							{
								return CHECK;
							}
							else return CHECKMATE;

						}
					}
				}
				else
				{
					int numPcs = 0;
					int numSquares = GetNumSquaresAvailableToKing(K);

					if( numSquares != 0 ) return KINGSAFE;
					// find any movable piece
					for( int i = 0; i < 16; i++ )
					{
						
						if( (!blackPcs[i]->bCaptured) && (i!=4))
						{
							if( !this->isPiecePinned(blackPcs[i]) )
							{
								switch(blackPcs[i]->type)
								{
								case Bishop: 
									{
										if( CanBishopMove(blackPcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								case Knight:
									{
										if( CanKnightMove(blackPcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								case Rook:
									{
										if( CanRookMove(blackPcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;

								case Queen:
									{
										if( CanQueenMove(blackPcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								case Pawn:
									{
										if( CanPawnMove(blackPcs[i]) )
										{
											return KINGSAFE;
										}
									}
									break;
								}
							}
						}

						
					}	
					return STALEMATE;
				}
			}
		}
		return KINGSAFE;
	}

	bool IsCheckMate(MyChessPiece* K)
	{
		//if( K->color == WHITE )
		{
			int fromX = 0; int fromY = 0;
			if( IsKingInCheck(K->color, &fromX, &fromY) )
			{
				if( K->x == 0 )
				{
					if( K->y == 0 )
					{
						if( !IsSquareChecked(K->color, K->x+1, K->y ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x+1, K->y+1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x, K->y+1 ) )
						{
							return false;
						}
					}
					else if( K->y == 7 )
					{
						if( !IsSquareChecked(K->color, K->x+1, K->y ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x+1, K->y-1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x, K->y-1 ) )
						{
							return false;
						}
					}
					else
					{
						if( !IsSquareChecked(K->color, K->x, K->y-1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x+1, K->y-1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x+1, K->y ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x+1, K->y+1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x, K->y+1 ) )
						{
							return false;
						}
					}
				}
				else if( K->x == 7 )
				{
					if( K->y == 0 )
					{
						if( !IsSquareChecked(K->color, K->x, K->y+1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x-1, K->y+1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x-1, K->y ) )
						{
							return false;
						}
					}
					else if( K->y == 7 )
					{
						if( !IsSquareChecked(K->color, K->x, K->y-1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x-1, K->y-1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x-1, K->y ) )
						{
							return false;	
						}
					}
					else
					{
						if( !IsSquareChecked(K->color, K->x, K->y-1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x-1, K->y-1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x-1, K->y ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x-1, K->y+1 ) )
						{
							return false;
						}
						if( !IsSquareChecked(K->color, K->x, K->y+1 ) )
						{
							return false;
						}
					}
				}
				else
				{
					if( !IsSquareChecked(K->color, K->x-1, K->y-1 ) )
					{
						return false;
					}
					if( !IsSquareChecked(K->color, K->x-1, K->y ) )
					{
						return false;
					}
					if( !IsSquareChecked(K->color, K->x-1, K->y+1 ) )
					{
						return false;
					}
					if( !IsSquareChecked(K->color, K->x, K->y-1 ) )
					{
						return false;
					}
					if( !IsSquareChecked(K->color, K->x, K->y+1 ) )
					{
						return false;
					}
					if( !IsSquareChecked(K->color, K->x+1, K->y-1 ) )
					{
						return false;
					}
					if( !IsSquareChecked(K->color, K->x+1, K->y ) )
					{
						return false;
					}
					if( !IsSquareChecked(K->color, K->x+1, K->y+1 ) )
					{
						return false;
					}
						
				}
				
			}
		}

		// if we got this far we need to look for things that can get in the way


		return false;
	}




	bool canWhiteCastleLong()
	{
		if( !whiteCastled && 
			(whitePcs[0]->bHasMoved == false) && 
			(whitePcs[0]->bCaptured == false) &&
			(whitePcs[4]->bHasMoved == false))
		{	
			if((whitePcs[4]->x == 4) && (whitePcs[4]->y == 0) &&
			   (whitePcs[0]->x == 0) && (whitePcs[0]->y == 0))
				return true;
		}
		return false;
	}

	bool canBlackCastleLong()
	{
		if( !blackCastled && 
			(blackPcs[0]->bHasMoved == false) && 
			(blackPcs[0]->bCaptured == false) &&
			(blackPcs[4]->bHasMoved == false))
		{
			if((blackPcs[4]->x == 4) && (blackPcs[4]->y == 7) &&
			   (blackPcs[0]->x == 0) && (blackPcs[0]->y == 7))
				return true;
		}
		return false;
	}

	bool canWhiteCastleShort()
	{
		if( !whiteCastled && 
			(whitePcs[7]->bHasMoved == false) && 
			(whitePcs[7]->bCaptured == false) &&
			(whitePcs[4]->bHasMoved == false))
		{	
			if((whitePcs[4]->x == 4) && (whitePcs[4]->y == 0) &&
			   (whitePcs[7]->x == 7) && (whitePcs[7]->y == 0))
				return true;
			
		}
		return false;
	}

	bool canBlackCastleShort()
	{
		if( !blackCastled && 
			(blackPcs[7]->bHasMoved == false) && 
			(blackPcs[7]->bCaptured == false) &&
			(blackPcs[4]->bHasMoved == false))
		{
			if((blackPcs[4]->x == 4) && (blackPcs[4]->y == 7) && 
			   (blackPcs[7]->x == 7) && (blackPcs[7]->y == 7))
				return true;
			
		}
		return false;
	}

	bool IsSquareEmpty( int x1, int y1 )
	{
		return board[x1][y1] == 0;
	}



	bool IsKingInCheck()
	{
		return false;
	}

	bool IsSquareDark(int x, int y)
	{
		return (((y + x) % 2)!=0);
	}

	
	MyChessMove *GetLastMove()
	{
		return &currMove;
	}

	bool CompareMoveToLast(MyChessMove &mv)
	{
		return ((mv.from.file == currMove.from.file)&&
			(mv.from.rank == currMove.from.rank) &&
			(mv.to.file == currMove.to.file) &&
			(mv.to.rank == currMove.to.rank) &&
			(mv.promo == currMove.promo));
	}

	
	void makemove2( int x1, int y1, int x2, int y2, bool promote, int type )
	{

		bool empassed = false;
		//canEmpasse = false;

		//fiftyMov++;
		if( mov == 1 )// || mov==2)
		{
			
			movCount++;
		}

		if( x1 >= 8 || x1 < 0 || y1 >= 8 || y1 < 0 || 
			x2 >= 8 || x2 < 0 || y2 >= 8 || y2 < 0)
			return;

		if( x1 == x2 && y1 == y2 ) return;

		
		currMove.from.file = x1;
		currMove.from.rank = y1;
		currMove.to.file = x2;
		currMove.to.rank = y2;
		if( promote )
		{
			currMove.promo = type;
		}
		else currMove.promo = -1;

		/*char temp = cboard[y1*8+x1];

		cboard[y2*8+x2] = temp;
		cboard[y1*8+x1] = 0;*/

		MyChessPiece *pc = NULL;
		MyChessPiece *pc2 = NULL;

		bool whitePiece = false;

		//memcpy( (void*)old_board, (void*)board, sizeof(unsigned char)*64);

		for( int i = 0; i < whitePcs.size(); i++ )
		{
			if( whitePcs[i]->x == x1 && whitePcs[i]->y == y1 )
			{
				if( !whitePcs[i]->bCaptured )
				{
					pc = whitePcs[i];
					whitePiece = true;
					break;
				}
			} 
			else if( whitePcs[i]->x == x2 && whitePcs[i]->y == y2 )
			{
				if( !whitePcs[i]->bCaptured )
				{
					pc2 = whitePcs[i];
					break;
				//whitePiece = true;
				}
			}			
		}
		for( int i = 0; i < blackPcs.size(); i++ )
		{
			if( blackPcs[i]->x == x1 && blackPcs[i]->y == y1 )
			{
				if( !blackPcs[i]->bCaptured )
				{
					pc = blackPcs[i];
					whitePiece = false;
					break;
				}
			} 
			else if( blackPcs[i]->x == x2 && blackPcs[i]->y == y2 )
			{
				if( !blackPcs[i]->bCaptured )
				{
					pc2 = blackPcs[i];
					break;
				}
			}			
		}


		if( pc!= NULL )
		{
			if( promote )
			{
				switch( type )
				{
				case 0: 
					{
						pc->type = Queen; 
						break;
					}
				case 1:
					{
						pc->type = Bishop; 
						pc->bdark_squared = IsSquareDark(x2,y2);
						break;
					}
				case 2: 
					{
						pc->type = Knight;
						break;
					}
				case 3: 
					{
						pc->type = Rook;
						break;
					}
				}
			}

			if( pc->type == Pawn )
			{
				fiftyMov = 0;
				if( abs(y2-y1)==2)
				{
					if( y2 > y1)
					{
						int empasse = y2-1;
						canEmpasse = true;
						empassantTarget.sq.file = x1;
						empassantTarget.sq.rank = empasse;
						
					}
					else
					{
						int empasse = y2+1;
						canEmpasse = true;
						empassantTarget.sq.file = x1;
						empassantTarget.sq.rank = empasse;
						
					}
				}
				else if( canEmpasse )
				{
					canEmpasse = false;
					if( (y2 == empassantTarget.sq.rank) &&
						(x2 == empassantTarget.sq.file))
					{
						if( y2 == 5 )
						{
							for( int j = 8; j < 16; j++ )
							{
								if( (blackPcs[j]->x == empassantTarget.sq.file) &&
									(blackPcs[j]->y == empassantTarget.sq.rank-1) &&
									(pc->color == WHITE))
								{
									pc2 = blackPcs[j];
									board[x2][y2-1] = 0;
									//pc2->bCaptured = true;
									break;
								}
							}
						}
						else if( y2 == 2 )
						{
							for( int j = 8; j < 16; j++ )
							{
								if( (whitePcs[j]->x == empassantTarget.sq.file) &&
									(whitePcs[j]->y == empassantTarget.sq.rank+1) &&
									(pc->color == BLACK))
								{
									pc2 = whitePcs[j];
									board[x2][y2+1] = 0;
									//pc2->bCaptured = true;
									//pc2->SetSquare(-1,-1);

									break;
								}
							}
						}
					}

				}
				else canEmpasse = false;
			}
			else if( pc->type == King )
			{
				if( pc->color == WHITE )
					whiteCastled = true;
				if( pc->color == BLACK )
					blackCastled = true;
				
				if( x2 == x1+2 )
				{
					if( pc->color == WHITE )
					{
						//castle short
						MyChessPiece *R = whitePcs[7];
						R->SetSquare(x1+1, y1);
						board[7][0]= 0;
						board[5][0]= wRook;
						whiteCastled = true;
						currMove.castle = 1;

					}
					else
					{
						MyChessPiece *R = blackPcs[7];
						R->SetSquare(x1+1, y1);
						board[7][7]= 0;
						board[5][7]= bRook;
						blackCastled = true;
						currMove.castle = 1;
					}
				}
				else if( x2 == x1-2 )
				{
					if( pc->color == WHITE )
					{
						MyChessPiece *R = whitePcs[0];
						R->SetSquare(x1-1, y1);
						board[0][0]= 0;
						board[3][0]= wRook;
						whiteCastled = true;
						currMove.castle = 2;
					}
					else
					{
						MyChessPiece *R = blackPcs[0];
						R->SetSquare(x1-1, y1);
						board[0][7]= 0;
						board[3][7]= bRook;
						blackCastled = true;
						currMove.castle = 2;
					}
				}
				fiftyMov++;
			}
			else
			{
				canEmpasse = false;
				
				//if( (mov == 2)|| (mov==0))
				{
					fiftyMov++;
					if( fiftyMov >= 100 )
						GameStatus = DRAW_BY_50_MOVE;
				}
			}

			

			pc->SetSquare(x2,y2);
			pc->bHasMoved = true;
		}

		if( pc2 != NULL )
		{
			pc2->SetSquare(-1,-1);
			pc2->SetCaptured(true);
			fiftyMov = 0;
		}

		if( promote )
		{
			if( mov==WHITE)
			{
				switch( type )
				{
				case 0:
					{
						board[x2][y2] = wQueen;
						extraWhiteQueens = true;
						break;
					}
				case 1:
					{
						board[x2][y2] = wBishop; 
						extraWhiteBishops = true;
						break;
					}
				case 2:
					{
						board[x2][y2] = wKnight; 
						extraWhiteKnights = true;
						break;
					}
				case 3:
					{
						board[x2][y2] = wRook; 
						extraWhiteRooks = true;
						break;
					}

				}
			}
			else
			{
				switch( type )
				{
				case 0:
					{
						board[x2][y2] = bQueen;
						extraBlackQueens = true;
						break;
					}
				case 1:
					{
						board[x2][y2] = bBishop; 
						//pc->bdark_squared = IsSquareDark(x2,y2);
						extraBlackBishops = true;
						break;
					}
				case 2:
					{
						board[x2][y2] = bKnight; 
						extraBlackKnights = true;
						break;
					}
				case 3:
					{
						board[x2][y2] = bRook; 
						extraBlackRooks = true;
						break;
					}

				}
			}
		}
		else
		{
			
			board[x2][y2] = board[x1][y1];
		}
		// = pc->type;
		
		board[x1][y1]= 0;
		mov++;
		if( mov == 2 )
			mov = 0;
		//board[x2][y2] = temp2;
	}

	void makemove( int x1, int y1, int x2, int y2, bool promote, int type, bool isCastle = false )
	{
		if( (x1 >= 8) || (x1 < 0) || (y1 >= 8) || (y1 < 0) || 
			(x2 >= 8) || (x2 < 0) || (y2 >= 8) || (y2 < 0))
			return;



		if( (x1 == x2) && (y1 == y2) ) return;

		canEmpasse = false;

		currMove.from.file = x1;
		currMove.from.rank = y1;
		currMove.to.file = x2;
		currMove.to.rank = y2;
		if( isCastle )
			currMove.castle = 1;

		if( promote )
		{
			currMove.promo = type;
		}
		else currMove.promo = -1;

		
		if( mov == 1)// || mov==2)
		{
			if( !isCastle )
				movCount++;
		}

		

		/*char temp = cboard[y1*8+x1];

		cboard[y2*8+x2] = temp;
		cboard[y1*8+x1] = 0;*/

		MyChessPiece *pc = NULL;
		MyChessPiece *pc2 = NULL;

		bool whitePiece = false;

		//memcpy( (void*)old_board, (void*)board, sizeof(unsigned char)*64);

		for( int i = 0; i < whitePcs.size(); i++ )
		{
			if( whitePcs[i]->x == x1 && whitePcs[i]->y == y1 )
			{
				if( !whitePcs[i]->bCaptured )
				{
					pc = whitePcs[i];
					whitePiece = true;
				}
			} 
			else if( whitePcs[i]->x == x2 && whitePcs[i]->y == y2 )
			{
				if( !whitePcs[i]->bCaptured )
				{
					pc2 = whitePcs[i];
				//whitePiece = true;
				}
			}			
		}
		for( int i = 0; i < blackPcs.size(); i++ )
		{
			if( blackPcs[i]->x == x1 && blackPcs[i]->y == y1 )
			{
				if( !blackPcs[i]->bCaptured )
				{
					pc = blackPcs[i];
					whitePiece = false;
				}
			} 
			else if( blackPcs[i]->x == x2 && blackPcs[i]->y == y2 )
			{
				if( !blackPcs[i]->bCaptured )
				{
					pc2 = blackPcs[i];
				}
			}			
		}


		if( pc!= NULL )
		{
			if( promote )
			{
				switch( type )
				{
				case 0: 
					{
						pc->type = Queen; 
						break;
					}
				case 1:
					{
						pc->type = Bishop; 
						pc->bdark_squared = IsSquareDark(x2,y2);
						break;
					}
				case 2: 
					{
						pc->type = Knight;
						break;
					}
				case 3: 
					{
						pc->type = Rook;
						break;
					}
				}
			}
			if( pc->type == Pawn )
			{
				fiftyMov = 0;
				if( abs(y2-y1)==2)
				{
					if( y2 > y1)
					{
						int empasse = y2-1;
						canEmpasse = true;
						empassantTarget.sq.file = x1;
						empassantTarget.sq.rank = empasse;
						
					}
					else
					{
						int empasse = y2+1;
						canEmpasse = true;
						empassantTarget.sq.file = x1;
						empassantTarget.sq.rank = empasse;
						
					}
				}
				else canEmpasse = false;
			}
			else
			{
				//if( (mov == 2)|| (mov==0))
				fiftyMov++;
			}
			pc->SetSquare(x2,y2);
			pc->bHasMoved = true;
		}

		if( pc2 != NULL )
		{
			pc2->SetSquare(-1,-1);
			pc2->SetCaptured(true);
			fiftyMov = 0;
		}

		if( promote )
		{
			if( mov==WHITE )
			{
				switch( type )
				{
				case 0:
					{
						board[x2][y2] = wQueen;
						extraWhiteQueens = true;
						break;
					}
				case 1:
					{
						board[x2][y2] = wBishop; 
						
						
						extraWhiteBishops = true;
						break;
					}
				case 2:
					{
						board[x2][y2] = wKnight; 
						extraWhiteKnights = true;
						break;
					}
				case 3:
					{
						board[x2][y2] = wRook; 
						extraWhiteRooks = true;
						break;
					}

				}
			}
			else
			{
				switch( type )
				{
				case 0:
					{
						board[x2][y2] = bQueen;
						extraBlackQueens = true;
						break;
					}
				case 1:
					{
						board[x2][y2] = bBishop; 
						//pc->bdark_squared = IsSquareDark(x2,y2);
						extraBlackBishops = true;
						break;
					}
				case 2:
					{
						board[x2][y2] = bKnight; 
						extraBlackKnights = true;
						break;
					}
				case 3:
					{
						board[x2][y2] = bRook; 
						extraBlackRooks = true;
						break;
					}

				}
			}
		}
		else
		{
			board[x2][y2] = board[x1][y1];

		}
		// = pc->type;
		
		board[x1][y1]= 0;
		//board[x2][y2] = temp2;
	}

	int PieceColor(int x, int y)
	{
		switch(board[x][y])
		{
		case wPawn:
		case wKnight:
		case wBishop:
		case wRook:
		case wQueen:
		case wKing:
			return WHITE;
		case bPawn:
		case bKnight:
		case bBishop:
		case bRook:
		case bQueen:
		case bKing:
			return BLACK;	
		}

		return 3;
	}

	bool IsThisMyMove(int col)
	{
		if( col == 0 || col == 2 )
		{
			if( mov == 0 || mov == 2 )
				return true;
			else return false;
		}
		else if( col == 1 )
		{
			if( mov == 1 )
				return true;
			else return false;
		}
		return false;
	}

	void SetBoardDisplaySize(float s)
	{
		board_display_size = s;
	}

	MyChessSquare findBishop(int x, int y, int col)
	{
		if( col == WHITE )
		{
			return findWhiteBishop( x, y);
		}
		else
		{
			return findBlackBishop( x, y);
		}
	}

	MyChessSquare findBlackBishop(int x, int y)
	{
		MyChessSquare sq;		
		int mod1 = (y + x) % 2;
		bool bdark = false;
		if( mod1 == 0) bdark = true;

		MyChessPiece* bshp1 = blackPcs[2];
		MyChessPiece* bshp2 = blackPcs[5];

		if( bdark )
		{
			if( !bshp2->bdark_squared )
			{
				sq.file = bshp2->x;
				sq.rank = bshp2->y;
			}
		}
		else
		{
			sq.file = bshp1->x;
			sq.rank = bshp1->y;
		}
		return sq;
	}

	MyChessSquare findWhiteBishop(int x, int y)
	{
		MyChessSquare sq;		
		int mod1 = (y + x) % 2;
		bool bdark = false;
		if( mod1 == 0) bdark = true;

		MyChessPiece* bshp1 = whitePcs[2];
		MyChessPiece* bshp2 = whitePcs[5];

		if( bdark )
		{
			if( !bshp1->bdark_squared )
			{
				sq.file = bshp1->x;
				sq.rank = bshp1->y;
			}
		}
		else
		{
			sq.file = bshp2->x;
			sq.rank = bshp2->y;
		}
		return sq;
	}

	

	MyChessSquare findWhiteKnight(int x, int y)
	{
		MyChessSquare sq;
		if( whitePcs[1]->bCaptured == false && !this->isPiecePinned(whitePcs[1]))
		{
			if( whitePcs[1]->type == Knight )
			{
				if( x == whitePcs[1]->x + 1 ||
					x == whitePcs[1]->x - 1)
				{
					if( y == whitePcs[1]->y + 2 ||
						y == whitePcs[1]->y - 2 )
					{
					sq.file = whitePcs[1]->x;
					sq.rank = whitePcs[1]->y;
					}
				}
				else if ( x == whitePcs[1]->x + 2 ||
						  x == whitePcs[1]->x - 2)
				{
					if( y == whitePcs[1]->y + 1 ||
						y == whitePcs[1]->y - 1 )
					{
					sq.file = whitePcs[1]->x;
					sq.rank = whitePcs[1]->y;
					}
				}
			}
		}
		if( whitePcs[6]->bCaptured == false &&!this->isPiecePinned(whitePcs[6]) )
		{
			if( whitePcs[6]->type == Knight )
			{
				if( x == whitePcs[6]->x + 1 ||
					x == whitePcs[6]->x - 1)
				{
					if( y == whitePcs[6]->y + 2 ||
						y == whitePcs[6]->y - 2 )
					{
					sq.file = whitePcs[6]->x;
					sq.rank = whitePcs[6]->y;
					}
				}
				else if ( x == whitePcs[6]->x + 2 ||
						  x == whitePcs[6]->x - 2)
				{
					if( y == whitePcs[6]->y + 1 ||
						y == whitePcs[6]->y - 1 )
					{
					sq.file = whitePcs[6]->x;
					sq.rank = whitePcs[6]->y;
					}
				}
			}
		}
		return sq;
	}

	MyChessSquare findBlackKnight(int x, int y)
	{
		MyChessSquare sq;
		if( blackPcs[1]->bCaptured == false && !this->isPiecePinned(blackPcs[1]))
		{
			if( blackPcs[1]->type == Knight  )
			{
				if( x == blackPcs[1]->x + 1 ||
					x == blackPcs[1]->x - 1)
				{
					if( y == blackPcs[1]->y + 2 ||
						y == blackPcs[1]->y - 2 )
					{
					sq.file = blackPcs[1]->x;
					sq.rank = blackPcs[1]->y;
					}
				}
				else if ( x == blackPcs[1]->x + 2 ||
						  x == blackPcs[1]->x - 2)
				{
					if( y == blackPcs[1]->y + 1 ||
						y == blackPcs[1]->y - 1 )
					{
					sq.file = blackPcs[1]->x;
					sq.rank = blackPcs[1]->y;
					}
				}
			}
		}
		if( blackPcs[6]->bCaptured == false && !this->isPiecePinned(blackPcs[6]))
		{
			if( blackPcs[6]->type == Knight )
			{
				if( x == blackPcs[6]->x + 1 ||
					x == blackPcs[6]->x - 1)
				{
					if( y == blackPcs[6]->y + 2 ||
						y == blackPcs[6]->y - 2 )
					{
					sq.file = blackPcs[6]->x;
					sq.rank = blackPcs[6]->y;
					}
				}
				else if ( x == blackPcs[6]->x + 2 ||
						  x == blackPcs[6]->x - 2)
				{
					if( y == blackPcs[6]->y + 1 ||
						y == blackPcs[6]->y - 1 )
					{
					sq.file = blackPcs[6]->x;
					sq.rank = blackPcs[6]->y;
					}
				}
			}
		}
		return sq;
	}



	bool IsDiagonalSpanClear( int x1, int y1, int x2, int y2 )
	{
		if((x1 < x2) && (y1 < y2))
		{
			for( int x=x1+1, y=y1+1; (y<y2)&&(x<x2); x++, y++)
			{
				if(board[x][y]!=0)
					return false;
			}
		}
		if( (x1 < x2) && (y1 > y2) )
		{
			for( int x=x1+1, y = y1-1; (y>y2)&&(x<x2); x++, y--)
			{
				if(board[x][y]!=0)
					return false;
			}
		}
		if((x1 > x2) && (y1 < y2))
		{
			for( int y=y1+1, x = x1-1; (x>x2)&&(y<y2); y++, x--)
			{
				if(board[x][y]!=0)
					return false;
			}
		}
		if((x1 > x2) && (y1 > y2))
		{
			for( int x=x2+1, y=y2+1; (y<y1)&&(x<x1); x++, y++)
			{
				if(board[x][y]!=0)
					return false;
			}
		}
		return true;
	}	

	bool IsDiagonalSpanClearIgnoringKing( int x1, int y1, int x2, int y2 )
	{
		if((x1 < x2) && (y1 < y2))
		{
			for( int x=x1+1, y=y1+1; (y<y2)&&(x<x2); x++, y++)
			{
				if(board[x][y]!=0 && (board[x][y]!=wKing) && (board[x][y]!=bKing))
					return false;
			}
		}
		if( (x1 < x2) && (y1 > y2) )
		{
			for( int x=x1+1, y = y1-1; (y>y2)&&(x<x2); x++, y--)
			{
				if(board[x][y]!=0 && (board[x][y]!=wKing) && (board[x][y]!=bKing))
					return false;
			}
		}
		if((x1 > x2) && (y1 < y2))
		{
			for( int y=y1+1, x = x1-1; (x>x2)&&(y<y2); y++, x--)
			{
				if(board[x][y]!=0 && (board[x][y]!=wKing) && (board[x][y]!=bKing))
					return false;
			}
		}
		if((x1 > x2) && (y1 > y2))
		{
			for( int x=x2+1, y=y2+1; (y<y1)&&(x<x1); x++, y++)
			{
				if(board[x][y]!=0 && board[x][y]!=wKing && board[x][y]!=bKing)
					return false;
			}
		}
		return true;
	}	

	bool IsDiagonalSpanClearIgnoringKing( int color, int x1, int y1, int x2, int y2 )
	{
		int kingId;
		if( color == WHITE )
			kingId = wKing;
		else 
			kingId = bKing;

		if((x1 < x2) && (y1 < y2))
		{
			for( int x=x1+1, y=y1+1; (y<y2)&&(x<x2); x++, y++)
			{
				if(board[x][y]!=0 && (board[x][y]!=kingId))
					return false;
			}
		}
		if( (x1 < x2) && (y1 > y2) )
		{
			for( int x=x1+1, y = y1-1; (y>y2)&&(x<x2); x++, y--)
			{
				if(board[x][y]!=0 && (board[x][y]!=kingId))
					return false;
			}
		}
		if((x1 > x2) && (y1 < y2))
		{
			for( int y=y1+1, x = x1-1; (x>x2)&&(y<y2); y++, x--)
			{
				if(board[x][y]!=0 && (board[x][y]!=kingId))
					return false;
			}
		}
		if((x1 > x2) && (y1 > y2))
		{
			for( int x=x2+1, y=y2+1; (y<y1)&&(x<x1); x++, y++)
			{
				if(board[x][y]!=0 && (board[x][y]!=kingId))
					return false;
			}
		}
		return true;
	}	

	bool isRankSpanBlocked(int x1, int x2, int y )
	{
		int X1 = x1;
		int X2 = x2;
		if( x2 < x1 )
		{
			X1 = x2;
			X2 = x1;
		}
		//if( (X1+1) == X2) return false;
		for( int x=X1+1; x<X2; x++ )
		{
			if( board[x][y]!=0 )
			{
				return true;
			}
		}
		return false;
	}

	bool isFileSpanBlocked(int x, int y1, int y2 )
	{
		
		int Y1 = y1;
		int Y2 = y2;
		if( y2 < y1 )
		{
			Y1 = y2;
			Y2 = y1;
		}
		//if( (Y1+1) == Y2) return false;
		for( int y=Y1+1; y<Y2; y++ )
		{
			if( board[x][y]!=0 )
			{
				return true;
			}
		}
		return false;
	}

	bool isFileSpanBlockedIgnoringKing(int x, int y1, int y2 )
	{
		
		int Y1 = y1;
		int Y2 = y2;
		if( y2 < y1 )
		{
			Y1 = y2;
			Y2 = y1;
		}
		//if( (Y1+1) == Y2) return false;
		for( int y=Y1+1; y<Y2; y++ )
		{
			if( board[x][y]!=0 && board[x][y]!=wKing &&board[x][y]!=bKing )
			{
				return true;
			}
		}
		return false;
	}

	bool isFileSpanBlockedIgnoringKing(int color, int x, int y1, int y2 )
	{
		
		int Y1 = y1;
		int Y2 = y2;
		if( y2 < y1 )
		{
			Y1 = y2;
			Y2 = y1;
		}

		int kingId;
		if( color == WHITE )
			kingId = wKing;
		else
			kingId = bKing;
		//if( (Y1+1) == Y2) return false;
		for( int y=Y1+1; y<Y2; y++ )
		{
			if( board[x][y]!=0 && board[x][y]!=kingId )
			{
				return true;
			}
		}
		return false;
	}

	bool isRankSpanBlockedIgnoringKing(int x1, int x2, int y )
	{
		int X1 = x1;
		int X2 = x2;
		if( x2 < x1 )
		{
			X1 = x2;
			X2 = x1;
		}


		//if( (X1+1) == X2) return false;
		for( int x=X1+1; x<X2; x++ )
		{
			if( board[x][y]!=0 && board[x][y]!=wKing && board[x][y]!=bKing )
			{
				return true;
			}
		}
		return false;
	}

	bool isRankSpanBlockedIgnoringKing(int color, int x1, int x2, int y )
	{
		int X1 = x1;
		int X2 = x2;
		if( x2 < x1 )
		{
			X1 = x2;
			X2 = x1;
		}
		//if( (X1+1) == X2) return false;

		int kingId;
		if( color == WHITE )
			kingId = wKing;
		else
			kingId = bKing;

		for( int x=X1+1; x<X2; x++ )
		{
			if( board[x][y]!=0 && board[x][y]!=kingId )
			{
				return true;
			}
		}
		return false;
	}

	

	MyChessSquare findBlackQueen(int x, int y)
	{
		MyChessSquare sq;

		sq.file = blackPcs[3]->x;
		sq.rank = blackPcs[3]->y;

		return sq;
	}

	MyChessSquare findWhiteQueen(int x, int y)
	{
		MyChessSquare sq;

		sq.file = whitePcs[3]->x;
		sq.rank = whitePcs[3]->y;

		return sq;
	}

	MyChessSquare findBlackKing(int x, int y)
	{
		MyChessSquare sq;

		sq.file = blackPcs[4]->x;
		sq.rank = blackPcs[4]->y;

		return sq;
	}

	MyChessSquare findWhiteKing(int x, int y)
	{
		MyChessSquare sq;

		sq.file = whitePcs[4]->x;
		sq.rank = whitePcs[4]->y;

		return sq;
	}




	bool isPiecePinned(MyChessPiece* pc)
	{
		// is the king on the same rank or file
		if( pc->color == WHITE )
		{
			MyChessPiece* king = whitePcs[4];
			int abs1 = abs(king->x - pc->x);
			int abs2 = abs(king->y - pc->y);

			if( abs1 == abs2 )
			{
				if( IsDiagonalSpanClear(king->x, king->y, pc->x, pc->y))
				{
					// find diagonal enemy
					if((king->x < pc->x) && (king->y < pc->y))
					{
						for(int x=pc->x+1, y=pc->y+1; (x<8)&&(y<8); x++, y++)
						{
							if((board[x][y] == bBishop)||(board[x][y] == bQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else if((king->x < pc->x) && (king->y > pc->y))
					{
						for(int x=pc->x+1, y=pc->y-1; (x<8)&&(y>-1); x++, y--)
						{
							if((board[x][y] == bBishop)||(board[x][y] == bQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else if((king->x > pc->x) && (king->y < pc->y))
					{
						for(int x=pc->x-1, y=pc->y+1; (x>-1)&&(y<8); x--, y++)
						{
							if((board[x][y] == bBishop)||(board[x][y] == bQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else if((king->x > pc->x) && (king->y > pc->y))
					{
						for(int x=pc->x-1, y=pc->y-1; (x>-1)&&(y>-1); x--, y--)
						{
							if((board[x][y] == bBishop)||(board[x][y] == bQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
				}
			}
			else if( king->x == pc->x )
			{
				if( !isFileSpanBlocked(king->x, king->y, pc->y ))
				{
					// find horizontal enemy
					if(king->y < pc->y)
					{
						int x=king->x;
						for(int y=pc->y+1; y<8; y++)
						{
							if((board[x][y]==bRook)||(board[x][y]==bQueen))
							{
								if( !isFileSpanBlocked(pc->x, pc->y, y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else
					{
						int x=king->x;
						for(int y=pc->y-1; y>-1; y--)
						{
							if((board[x][y]==bRook)||(board[x][y]==bQueen))
							{
								if( !isFileSpanBlocked(pc->x, pc->y, y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
				}
			}
			else if( king->y == pc->y )
			{
				if( !isRankSpanBlocked(king->x, pc->x, pc->y ))
				{
					// find vertical enemy
					if(king->x < pc->x)
					{
						int y=king->y;
						for(int x=pc->x+1; x<8; x++)
						{
							if((board[x][y]==bRook)||(board[x][y]==bQueen))
							{
								if( !isRankSpanBlocked(pc->x, x, pc->y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else
					{
						int y=king->y;
						for(int x=pc->x-1; x>-1; x--)
						{
							if((board[x][y]==bRook)||(board[x][y]==bQueen))
							{
								if( !isRankSpanBlocked(pc->x, x, pc->y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			MyChessPiece* king = blackPcs[4];
			int abs1 = abs(king->x - pc->x);
			int abs2 = abs(king->y - pc->y);

			if( abs1 == abs2 )
			{
				if( IsDiagonalSpanClear(king->x, king->y, pc->x, pc->y))
				{
					if((king->x < pc->x) && (king->y < pc->y))
					{
						for(int x=pc->x+1, y=pc->y+1; (x<8)&&(y<8); x++, y++)
						{
							if((board[x][y] == wBishop)||(board[x][y] == wQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					if((king->x < pc->x) && (king->y > pc->y))
					{
						for(int x=pc->x+1, y=pc->y-1; (x<8)&&(y>-1); x++, y--)
						{
							if((board[x][y] == wBishop)||(board[x][y] == wQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else if((king->x > pc->x) && (king->y < pc->y))
					{
						for(int x=pc->x-1, y=pc->y+1; (x>-1)&&(y<8); x--, y++)
						{
							if((board[x][y] == wBishop)||(board[x][y] == wQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else if((king->x > pc->x) && (king->y > pc->y))
					{
						for(int x=pc->x-1, y=pc->y-1; (x>-1)&&(y>-1); x--, y--)
						{
							if((board[x][y] == wBishop)||(board[x][y] == wQueen))
							{
								if(IsDiagonalSpanClear(pc->x, pc->y, x, y))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
				}
			}
			else if( king->x == pc->x )
			{
				if( !isFileSpanBlocked(king->x, king->y, pc->y ))
				{
					// find horizontal enemy
					if(king->y < pc->y)
					{
						int x=king->x;
						for(int y=pc->y+1; y<8; y++)
						{
							if((board[x][y]==wRook)||(board[x][y]==wQueen))
							{
								if( !isFileSpanBlocked(pc->x, pc->y, y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else
					{
						int x=king->x;
						for(int y=pc->y-1; y>-1; y--)
						{
							if((board[x][y]==wRook)||(board[x][y]==wQueen))
							{
								if( !isFileSpanBlocked(pc->x, pc->y, y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
				}
			}
			else if( king->y == pc->y )
			{
				if( !isRankSpanBlocked(king->x, pc->x, pc->y ))
				{
					// find vertical enemy
					if(king->x < pc->x)
					{
						int y=king->y;
						for(int x=pc->x+1; x<8; x++)
						{
							if((board[x][y]==wRook)||(board[x][y]==wQueen))
							{
								if( !isRankSpanBlocked(pc->x, x, pc->y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
					else
					{
						int y=king->y;
						for(int x=pc->x-1; x>-1; x--)
						{
							if((board[x][y]==wRook)||(board[x][y]==wQueen))
							{
								if( !isRankSpanBlocked(pc->x, x, pc->y ))
								{
									pc->PinFromX = x;
									pc->PinFromY = y;
									return true;
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

	

	int mov;

	void parseMove(string& strInpt)
	{
	
	
	if(mov==2)
		mov = 0;
	
	//cout<<"*****************************"<<endl<< "parsing move: "<<mov++<<endl<<"*****************************"<<endl;
	int len = strInpt.length();

	if( len == 0 )
	{
		if( postReset == false )
		{
			postReset = true;
		}
		mov++;
		return;
	}

	int white_wins = -1;
	white_wins = strInpt.find("1-0");
	int black_wins = -1;
	black_wins = strInpt.find("0-1");
	int drawn_game = -1;
	black_wins = strInpt.find("1/2");

	canEmpasse = false;



	if( strInpt =="1-0")
	{
		// white wins
		postReset = true;
		mov++;
		return;
	}
	
	if( strInpt == "0-1" || strInpt == "0-1 ")
	{
		postReset = true;
		mov++; 
		return;
		// black wins
	}
	
	if ( strInpt == "1/2-1/2")
	{
		postReset = true;
		mov++;
		return;
		//drawn game
	}

	if ( strInpt == "*")
	{
		postReset = true;
		mov++;
		return;
		//drawn game
	}

	if( strInpt[len-1] == '+')
	{
//		cout<< "CHECK"<<endl;
		len--;
	}

	if( strInpt[len-1] == '#')
	{
//		cout<< "CHECK"<<endl;
		len--;
		cout<< "Checkmate"<<endl;
		postReset = true;
		//return;
	}


	if( len == 0 )
	{
		if( postReset == false )
		{
			postReset = true;
		}
		mov++;
		return;
	}

/*	if( str.find("??")!=0)
	{
//		cout<< "CHECK"<<endl;
		len -= 2;
	}*/

	//int end = strInpt.find('!');
	//if( end > 0 )
	//	len = end;

	/*if( strInpt[len-1] == '!')
	{
		if( strInpt[len-2] == '?')
		{
			len--;
		}
		else
		if( strInpt[len-2] == '!')
		{
			len--;
		}
		len--;
	}*/
	
	if( strInpt[len-1] == '!')
	{
//		cout<< "CHECK"<<endl;
		len--;

		if( strInpt[len-1] == '?')
		{ 
			len--;
			if( strInpt[len-1] == '?')
				len--;
			if( strInpt[len-1] == '!')
				len--;
			if( strInpt[len-1] == '+')
				len--;
			if( strInpt[len-1] == '#')
				len--;
		}
		if( strInpt[len-1] == '!')
		{
			len--;
			if( strInpt[len-1] == '?')
				len--;
			if( strInpt[len-1] == '!')
				len--;
			if( strInpt[len-1] == '+')
				len--;
			if( strInpt[len-1] == '#')
				len--;
		}
		if( strInpt[len-1] == '+')
		{
			len--;
		}
		if( strInpt[len-1] == '#')
		{
			len--;
		}
	}

	if( strInpt[len-1] == '?')
	{
//		cout<< "CHECK"<<endl;
		/*if( strInpt[len-2] == '?')
		{
			len--;
		}
		else
		if( strInpt[len-2] == '!')
		{
			len--;
		}*/
		len--;
		if( strInpt[len-1] == '?')
		{
			len--;
			if( strInpt[len-1] == '?')
				len--;
			if( strInpt[len-1] == '!')
				len--;
			if( strInpt[len-1] == '+')
				len--;
			if( strInpt[len-1] == '#')
				len--;
		}
		if( strInpt[len-1] == '!')
		{
			len--;
			if( strInpt[len-1] == '?')
				len--;
			if( strInpt[len-1] == '!')
				len--;
			if( strInpt[len-1] == '+')
				len--;
			if( strInpt[len-1] == '#')
				len--;
		}
		if( strInpt[len-1] == '+')
		{
			len--;
		}
		if( strInpt[len-1] == '#')
		{
			len--;
		}
	}



	string str = "";
	if( !isalpha(strInpt[0]) )
	{
		int found = -1;
		found = strInpt.find("19...");
		if( found != -1 )
		{
			char buffer[10];
			for( int i = 0; i < len-5; i++ )
				buffer[i] = strInpt[i+5];
			//strInpt.assign(str, 5, 10 );
			len -=5;
			str = buffer;
		}
		return;
		
	}
	else
	{
		if( (strInpt[len-1] == 'Q' && strInpt[len-2]!= '=') ||  
			(strInpt[len-1] == 'R' && strInpt[len-2]!= '=') ||
			(strInpt[len-1] == 'B' && strInpt[len-2]!= '=') ||
			(strInpt[len-1] == 'N' && strInpt[len-2]!= '='))
		{
			for( int i = 0; i < len-1; i++ )
				str+= strInpt[i];

			str += "=";
			str += strInpt[len-1];
			len++;
		}
		else
		{
			str = strInpt;
		}
	}


	

	
	// pawn moves
	if( len == 2 )
	{
	//	cout<< "pawn push: "<< str<<endl;
		int y1=0;
		if( mov == WHITE )
		{
			int file = (str[0]-'a');
			int rank = (str[1]-'1');
			for( int y = 0; y<rank; y++)
			{
				if( board[file][y]!=0)
				{
					if( board[file][y] == wPawn )
					{
						y1 = y;
						//break;
					}
				}
			}
			if( (rank-y1) == 2 )
			{
				canEmpasse = true;
				empassantTarget.sq.file = file;
				empassantTarget.sq.rank = rank-1;
			}
			
				
			makemove(file,y1, file, rank, false, 0);
		}
		else
		{
			int file = (str[0]-'a');
			int rank = (str[1]-'1');

			for( int y = str[1]-'1'; y<'8'; y++)
			{
				if( board[file][y]!=0)
				{
					if( board[file][y] == bPawn )
					{
						y1 = y;
						break;
					}
				}
			}
			if( (y1-rank) == 2)
			{
				canEmpasse = true;
				empassantTarget.sq.file = file;
				empassantTarget.sq.rank = rank+1;
			}
			//if( y1 <= 6)
				
			
			makemove(str[0]-'a',y1, str[0]-'a', str[1] - '1', false, 0);
		}
		mov++;
		return;
	}

	// castling short or Piece Moves
	if( len == 3 )
	{
		if( str[0] == '0' || str[0] == 'O')
		{
			if( mov == WHITE )
			{
				//makemove(e, 0, g, 0); 
				//makemove(h, 0, f, 0);

				makemove(7, 0, 5, 0, false, 0);
				makemove(4, 0, 6, 0, false, 0,  true); 
				
				this->whiteCastled = true;
				mov++;
				return;
			}
			else
			{
				makemove(7, 7, 5, 7, false, 0);
				makemove(4, 7, 6, 7, false, 0, true); 
				
				this->blackCastled = true;
				mov++;
				return;
			}
		}
		else
		{
			if( str[0] == 'B' )
			{
			//	cout<< "bishop move: "<< str << ", file= "<< str[1]-'a' <<", and this is: "<< str[1]<<", in algebraic notation. And rank: "<< str[2]-'1' <<endl;
				// note need to calculate which bishop.
				int fil = (int)(str[1]-'a');
				int rank = (int)(str[2] - '1');
				if( mov == WHITE)
				{
					if( extraWhiteBishops )
					{
						vector<MyChessPiece*> bishops;
						for( int i = 8; i < 16; i++ )
						{
							if(whitePcs[i]->type == Bishop)
							{
								if(!whitePcs[i]->bCaptured)
									bishops.push_back(whitePcs[i]);
							}
						}

						if(!whitePcs[2]->bCaptured)
							bishops.push_back(whitePcs[2]);

						if(!whitePcs[5]->bCaptured)
							bishops.push_back(whitePcs[5]);
						
						for( int i = 0; i < bishops.size(); i++ )
						{
							if( IsSquareDark(fil,rank))
							{
								if( bishops[i]->bdark_squared && !isPiecePinned(bishops[i]))
								{
									int abs1 = abs(fil-bishops[i]->x);
									int abs2 = abs(rank-bishops[i]->y);
									// is square accessable
									if( abs1 == abs2)
									{
										// square is accessable
										// check for blockades
										if( (abs1 == 1) )// && (board[fil][rank]==0))
										{
											makemove(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank,
																false,
																0);
											mov++;
											return;
										}
										else
										if( IsDiagonalSpanClear(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank))
										{
											makemove(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank,
																false,
																0);
											mov++;
											return;
										}	
									}
								}
							}
							else
							{
								if( !bishops[i]->bdark_squared && !isPiecePinned(bishops[i]))
								{
									// check for blockades
									int abs1 = abs(fil-bishops[i]->x);
									int abs2 = abs(rank-bishops[i]->y);
									// is square accessable
									if( abs1 == abs2)
									{
										// square is accessable
										// check for blockades
									// square is accessable
										// check for blockades
										if( (abs1 == 1)  )//&& (board[fil][rank]==0))
										{
											makemove(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank,
																false,
																0);
											mov++;
											return;
										}
										else
										if( IsDiagonalSpanClear(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank))
										{
											makemove(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank,
																false,
																0);
											mov++;
											return;
										}
									}
								}
							}
						}
					}
					else
					{
						MyChessSquare sq = this->findWhiteBishop(fil,rank);
						makemove(sq.file, sq.rank,fil,rank, false, 0);
						mov++;
					}
					return;
				}
				else
				{
					if( extraBlackBishops )
					{
						vector<MyChessPiece*> bishops;
						for( int i = 8; i < 16; i++ )
						{
							if(blackPcs[i]->type == Bishop)
							{
								if(!blackPcs[i]->bCaptured)
									bishops.push_back(blackPcs[i]);
							}
						}

						if(!blackPcs[2]->bCaptured)
							bishops.push_back(blackPcs[2]);

						if(!blackPcs[5]->bCaptured)
							bishops.push_back(blackPcs[5]);

						for( int i = 0; i < bishops.size(); i++ )
						{
							if( IsSquareDark(fil,rank))
							{
								if( bishops[i]->bdark_squared && !isPiecePinned(bishops[i]) )
								{
									int abs1 = abs(fil-bishops[i]->x);
									int abs2 = abs(rank-bishops[i]->y);
									// is square accessable
									if( abs1 == abs2)
									{
										// square is accessable
										// check for blockades
										if( (abs1 == 1) )//&& (board[fil][rank]==0))
										{
											makemove(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank,
																false,
																0);
											mov++;
											return;
										}
										else
										if( IsDiagonalSpanClear(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank))
										{
											makemove(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank,
																false,
																0);
											mov++;
											return;
										}	
									}
								}
							}
							else
							{
								if( !bishops[i]->bdark_squared && !isPiecePinned(bishops[i]))
								{
									// check for blockades
									int abs1 = abs(fil-bishops[i]->x);
									int abs2 = abs(rank-bishops[i]->y);
									// is square accessable
									if( abs1 == abs2)
									{
										// square is accessable
										// check for blockades
										if( IsDiagonalSpanClear(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank))
										{
											makemove(bishops[i]->x,
																bishops[i]->y,
																fil,
																rank,
																false,
																0);
											mov++;
											return;
										}			
									}
								}
							}
						}
					}
					else
					{
						MyChessSquare sq = this->findBlackBishop(fil,rank);
						makemove(sq.file, sq.rank,fil,rank, false, 0);
						mov++;
					}
					return;
				}
			}
			if( str[0] == 'N' )
			{
				int fil = (int)(str[1]-'a');
				int rank = (int)(str[2] - '1');
			//	cout<< "knight move: "<< str << ", file= "<< str[1]-'a' <<", and this is: "<< str[1]<<", in algebraic notation. And rank: "<< str[2]-'1' <<endl;
				
				if( mov == WHITE )
				{
					if( extraWhiteKnights )
					{
						vector<MyChessPiece*> knights;
						for( int i = 8; i < 16; i++ )
						{
							if(whitePcs[i]->type == Knight)
							{
								if(!whitePcs[i]->bCaptured)
									knights.push_back(whitePcs[i]);
							}
						}

						if(!whitePcs[1]->bCaptured)
							knights.push_back(whitePcs[1]);

						if(!whitePcs[6]->bCaptured)
							knights.push_back(whitePcs[6]);

						for( int i = 0; i < knights.size(); i++ )
						{
							if( rank == knights[i]->y + 1 ||
								rank == knights[i]->y - 1)
							{
								if( fil == knights[i]->x + 2 ||
									fil == knights[i]->x - 2 )
								{
									if( !isPiecePinned(knights[i]))
									{
										makemove(knights[i]->x, knights[i]->y,
											fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
							else if ( rank == knights[i]->y + 2 ||
									  rank == knights[i]->y - 2)
							{
								if( fil == knights[i]->x + 1 ||
									fil == knights[i]->x - 1 )
								{
									if(!isPiecePinned(knights[i]))
									{
										makemove(knights[i]->x, knights[i]->y,
										fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}							
						}
					}
					else
					{
						MyChessSquare sq = findWhiteKnight(fil,rank);
						makemove(sq.file, sq.rank,fil,rank, false, 0);
						mov++;
					}
					return;
				}
				else
				{
					if( extraBlackKnights )
					{
						vector<MyChessPiece*> knights;
						for( int i = 8; i < 16; i++ )
						{
							if(blackPcs[i]->type == Knight)
							{
								if(!blackPcs[i]->bCaptured)
									knights.push_back(blackPcs[i]);
							}
						}

						if(!blackPcs[1]->bCaptured)
							knights.push_back(blackPcs[1]);

						if(!blackPcs[6]->bCaptured)
							knights.push_back(blackPcs[6]);

						for( int i = 0; i < knights.size(); i++ )
						{
							if( rank == knights[i]->y + 1 ||
								rank == knights[i]->y - 1)
							{
								if( fil == knights[i]->x + 2 ||
									fil == knights[i]->x - 2 )
								{
									//sq.file = knights[i]->x;
									//sq.rank = knights[i]->y;
									if(!isPiecePinned(knights[i]))
									{
										makemove(knights[i]->x, knights[i]->y,
											fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
							else if ( rank == knights[i]->y + 2 ||
									  rank == knights[i]->y - 2)
							{
								if( fil == knights[i]->x + 1 ||
									fil == knights[i]->x - 1 )
								{
									if(!isPiecePinned(knights[i]))
									{
										makemove(knights[i]->x, knights[i]->y,
											fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}							
						}
					}
					else
					{
						MyChessSquare sq = findBlackKnight(fil,rank);
						makemove(sq.file, sq.rank,fil,rank, false, 0);
						mov++;
					}
					return;
				}
			}
			if( str[0] == 'R' )
			{
				int fil = (int)(str[1]-'a');
				int rank = (int)(str[2] - '1');
				int indx1;
				int indx2;

				MyChessPiece *r1;
				MyChessPiece *r2;

		//MyChessSquare sq;

		//MyChessPiece *rook1;
		//MyChessPiece *rook2;

				bool bMultiRook = false;
				int numExtraRooks = 0;

				if( mov == WHITE)
				{
					for( int i = 8; i < 16; i++)
					{
						if( whitePcs[i]->type == Rook )
						{
							bMultiRook = true;
							numExtraRooks++;
						}
					}
				}
				else
				{
					for( int i = 8; i < 16; i++)
					{
						if( blackPcs[i]->type == Rook )
						{
							bMultiRook = true;
								
							numExtraRooks++;
						}
					}
				}

						if( extraWhiteRooks || extraBlackRooks )
						{
						// build a vector of rooks, now attempt to find the correct rook
						// by assuming that if it has access to the square and is not blocked
						// then it must be the correct rook
							vector<MyChessPiece*> rooks;
							if( mov == WHITE )
							{
								for( int i = 8; i < 16; i++)
								{
									if( whitePcs[i]->type == Rook )
									{
										if( !whitePcs[i]->bCaptured )
											rooks.push_back(whitePcs[i]);
									}
								}
								if( !whitePcs[0]->bCaptured )
									rooks.push_back(whitePcs[0]);

								if( !whitePcs[7]->bCaptured )
									rooks.push_back(whitePcs[7]);
							}
							else
							{
								for( int i = 8; i < 16; i++)
								{
									if( blackPcs[i]->type == Rook )
									{
										if( !blackPcs[i]->bCaptured )
											rooks.push_back(blackPcs[i]);
									}
								}
								if( !blackPcs[0]->bCaptured )
									rooks.push_back(blackPcs[0]);

								if( !blackPcs[7]->bCaptured )
									rooks.push_back(blackPcs[7]);
							}

							for( int i = 0; i < rooks.size(); i++ )
							{
								if( rooks[i]->x == fil )
								{
									
									if( (((rooks[i]->y-1)==rank)||  
										 ((rooks[i]->y+1)==rank)) && 
										 (board[fil][rank]==0) && !isPiecePinned(rooks[i]))
									{
										makemove(rooks[i]->x, rooks[i]->y, str[1]-'a',str[2]-'1', false,0);
										mov++;
										return;
									}
									else if( !isFileSpanBlocked(str[1]-'a', rooks[i]->y, str[2]-'1' ))
								//	continue;
									//else
									{
										//sq.file = rooks[i]->x;
										//sq.rank = rooks[i]->y;
										//Rook Found,
										if(!isPiecePinned(rooks[i]))
										{
											makemove(rooks[i]->x, rooks[i]->y, str[1]-'a',str[2]-'1', false,0);
											mov++;
											return;
										}
									}
								}
								if( rooks[i]->y == rank )
								{
									if( !isRankSpanBlocked(rooks[i]->x, fil, rank ))
									//continue;
								//else
									{
									// rook found
									//sq.file = rooks[i]->x;
									//sq.rank = rooks[i]->y;
										if(!isPiecePinned(rooks[i]))
										{
											makemove(rooks[i]->x, rooks[i]->y, str[1]-'a',str[2]-'1', false,0);
											mov++;
											return;
										}
									}
								}
						
							}
						}


				if( mov == WHITE )
				{
					indx1 = 0;
					indx2 = 7;
					r1 = whitePcs[0];
					r2 = whitePcs[7];
				}
				else
				{
					r1 = blackPcs[0];
					r2 = blackPcs[7];
				}
					//MyChessSquare sq = findWhiteRook(fil,rank);


					if( r1->bCaptured == true )
					{
						//if(!isPiecePinned(r2))
						{
							makemove(r2->x, r2->y, fil, rank , false, 0);
							mov++;
							return;
						}
					}
					if( r2->bCaptured == true )
					{
						//if(!isPiecePinned(r1))
						{
							makemove(r1->x, r1->y, fil, rank , false, 0);
							mov++;
							return;
						}
					}
					
					if( r1->x != fil && r1->y != rank )
					{
						//if(!isPiecePinned(r2))
						{
							makemove(r2->x, r2->y, fil, rank , false, 0);
							mov++;
							return;
						}
						// move r2
					}
					if( r2->x != fil && r2->y != rank )
					{
						//if(!isPiecePinned(r1))
						{
							makemove(r1->x, r1->y, fil, rank , false, 0);
							mov++;
							return;
						}
						// move r1
					}

					bool movr1 = true;
					bool movr2 = true;

					if( r1->x == fil )
					{
						if( r1->y < rank )
						{
							//bool blocked = false;
							for( int i = r1->y+1; i < rank; i++ )
								if( board[fil][i] != 0 )
									movr1 = false;
						}
						else 
						{
							//bool blocked = false;
							for( int i = rank; i < r1->y; i++ )
								if( board[fil][i] != 0 )
									movr1 = false;
						}
					}
					if( r2->x == fil )
					{
						if( r2->y < rank )
						{
							//bool blocked = false;
							for( int i = r2->y+1; i < rank; i++ )
								if( board[fil][i] != 0 )
									movr2 = false;
						}
						else 
						{
							//bool blocked = false;
							for( int i = rank; i < r2->y; i++ )
								if( board[fil][i] != 0 )
									movr2 = false;
						}
					}
					if( r1->y == rank )
					{
						if( r1->x < fil )
						{
							//bool blocked = false;
							for( int i = r1->x+1; i < fil; i++ )
								if( board[i][rank] != 0 )
									movr1 = false;
						}
						else 
						{
							//bool blocked = false;
							for( int i = fil; i < r1->x; i++ )
								if( board[i][rank] != 0 )
									movr1 = false;
						}
					}
					if( r2->y == rank )
					{
						if( r2->x < fil )
						{
							//bool blocked = false;
							for( int i = r2->x+1; i < fil; i++ )
								if( board[i][rank] != 0 )
									movr2 = false;
						}
						else 
						{
						//	bool blocked = false;
							for( int i = fil; i < r2->x; i++ )
								if( board[i][rank] != 0 )
									movr2 = false;
						}
					}

					if(this->isPiecePinned(r1) )
					{
	
						if( mov == WHITE )
						{
							if( ((whitePcs[4]->x == r1->PinFromX) &&
								(fil != whitePcs[4]->x )))
								if( fil != r1->PinFromX &&
									rank != r1->PinFromY )
									movr1 = false;

							if( ((whitePcs[4]->y == r1->PinFromY) &&
								(rank != whitePcs[4]->y)))
								if( fil != r1->PinFromX &&
									rank != r1->PinFromY )
									movr1 = false;

						}
						else
						{
							if( ((blackPcs[4]->x == r1->PinFromX) &&
								(fil != blackPcs[4]->x )))
								if( fil != r1->PinFromX &&
									rank != r1->PinFromY )
									movr1 = false;

							if( ((blackPcs[4]->y != r1->PinFromY) &&
								(rank == blackPcs[4]->y)))
								if( fil != r1->PinFromX &&
									rank != r1->PinFromY )
									movr1 = false;
						}
								
							
					}
					if( isPiecePinned(r2))
					{
						if( mov == WHITE )
						{
							if( ((whitePcs[4]->x == r2->PinFromX) &&
								(fil != whitePcs[4]->x )))
								if( fil != r2->PinFromX &&
									rank != r2->PinFromY )
									movr2 = false;
								

							if( ((whitePcs[4]->y == r2->PinFromY) &&
								(rank != whitePcs[4]->y)))
								if( fil != r2->PinFromX &&
									rank != r2->PinFromY )
									movr2 = false;

						}
						else
						{
							if( ((blackPcs[4]->x == r2->PinFromX) &&
								(fil != blackPcs[4]->x )))
								if( fil != r2->PinFromX &&
									rank != r2->PinFromY )
									movr2 = false;

							if( ((blackPcs[4]->y == r2->PinFromY) &&
								(rank != blackPcs[4]->y)))
								if( fil != r2->PinFromX &&
									rank != r2->PinFromY )
									movr2 = false;
						}
					}
					

					if( movr1 )
					{
						//if(!isPiecePinned(r1))
						{
							makemove( r1->x, r1->y, fil, rank , false, 0);
						}
					}

					if( movr2 )
					{
						//if(!isPiecePinned(r2))
						{
							makemove(r2->x, r2->y, fil, rank , false, 0);
						}
					}

					mov++;
					return;
			//	cout<< "rook move: "<< ", file= "<< str[1]-'a' <<", and this is: "<< str[1]<<", in algebraic notation. And rank: "<< str[2]-'1' <<endl;
			}
			if( str[0] == 'Q' )
			{
				int fil = (int)(str[1]-'a');
				int rank = (int)(str[2] - '1');
				if( mov == WHITE )
				{
					if( extraWhiteQueens )
					{
						vector<MyChessPiece*> queens;
						for( int i = 8; i < 16; i++ )
						{
							if(whitePcs[i]->type == Queen)
							{
								if(!whitePcs[i]->bCaptured)
									queens.push_back(whitePcs[i]);
							}
						}

						if(!whitePcs[3]->bCaptured)
							queens.push_back(whitePcs[3]);

						
						for( int i = 0; i < queens.size(); i++ )
						{
							// for Qb6 we need a Queen that is on a non-blocked file
							// or diagonal ... so first we test to see that it
							// can be accessed
							int abs1 = abs(fil-queens[i]->x);
							int abs2 = abs(rank-queens[i]->y);

							if( abs1 == abs2 )
							{
								if( IsDiagonalSpanClear(queens[i]->x,
														queens[i]->y,
														fil,
														rank))
								{
									if(!isPiecePinned(queens[i]))
									{
										makemove(queens[i]->x,
												 queens[i]->y,
												 fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
							else if(queens[i]->x == fil)
							{
								if( !isFileSpanBlocked(queens[i]->x, queens[i]->y, rank ))
								{
									if(!isPiecePinned(queens[i]))
									{
										makemove(queens[i]->x,
												 queens[i]->y,
												 fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
							else if(queens[i]->y == rank)
							{
								if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
								{
									if(!isPiecePinned(queens[i]))
									{
										makemove(queens[i]->x,
												 queens[i]->y,
												 fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
						}
					}
					else
					{
						MyChessSquare sq = findWhiteQueen(fil,rank);
						makemove(sq.file, sq.rank,fil,rank, false, 0);
						mov++;
					}
					return;
				}
				else
				{
					if( extraBlackQueens )
					{
						vector<MyChessPiece*> queens;
						for( int i = 8; i < 16; i++ )
						{
							if(blackPcs[i]->type == Queen)
							{
								if(!blackPcs[i]->bCaptured)
									queens.push_back(blackPcs[i]);
							}
						}

						if(!blackPcs[3]->bCaptured)
							queens.push_back(blackPcs[3]);

						
						for( int i = 0; i < queens.size(); i++ )
						{
							// for Qb6 we need a Queen that is on a non-blocked file
							// or diagonal ... so first we test to see that it
							// can be accessed
							int abs1 = abs(fil-queens[i]->x);
							int abs2 = abs(rank-queens[i]->y);

							if( abs1 == abs2 )
							{
								if( IsDiagonalSpanClear(queens[i]->x,
														queens[i]->y,
														fil,
														rank))
								{
									if(!isPiecePinned(queens[i]))
									{
										makemove(queens[i]->x,
												 queens[i]->y,
												 fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
							else if(queens[i]->x == fil)
							{
								if( !isFileSpanBlocked(queens[i]->x, queens[i]->y, rank ))
								{
									if(!isPiecePinned(queens[i]))
									{
										makemove(queens[i]->x,
												 queens[i]->y,
												 fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
							else if(queens[i]->y == rank)
							{
								if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
								{
									if(!isPiecePinned(queens[i]))
									{
										makemove(queens[i]->x,
												 queens[i]->y,
												 fil, rank, false, 0);
										mov++;
										return;
									}
								}
							}
						}
					}
					else
					{
						MyChessSquare sq = findBlackQueen(fil,rank);
						makemove(sq.file, sq.rank,fil,rank, false, 0);
						mov++;
					}
					return;
				}
			//	cout<< "queen move: "<< ", file= "<< str[1]-'a' <<", and this is: "<< str[1]<<", in algebraic notation. And rank: "<< str[2]-'1' <<endl;
			}
			if( str[0] == 'K')
			{
				int fil = (int)(str[1]-'a');
				int rank = (int)(str[2] - '1');
				if( mov == WHITE )
				{
					MyChessSquare sq = findWhiteKing(fil,rank);
					makemove(sq.file, sq.rank,fil,rank, false, 0);
					mov++;
					return;
				}
				else
				{
					MyChessSquare sq = findBlackKing(fil,rank);
					makemove(sq.file, sq.rank,fil,rank, false, 0);
					mov++;
					return;
				}
			//	cout<< "king move: "<< ", file= "<< str[1]-'a' <<", and this is: "<< str[1]<<", in algebraic notation. And rank: "<< str[2]-'1' <<endl;
			}
		}
	}

	// pawn captures
	// piece captures
	// piece moves with more than one possibility
	// pawn promotions
	if( len == 4 )
	{
		int type = -1;
		bool piece_move = false;
		switch( str[0] )
		{
			case 'B':
				{
					type=  Bishop;
					//cout<< "Bishop Move: "<<str;
					piece_move = true;
					break;
				}
			case 'N':
				{
					type=  Knight;
					//cout<< "Knight Move: "<<str;
					piece_move = true;
					break;
				}
			case 'R':
				{
					type=  Rook;
					//cout<< "Rook Move: "<<str;
					piece_move = true;
					break;
				}
			case 'Q':
				{
					type=  Queen;
					//cout<< "Queen Move: "<<str;
					piece_move = true;
					break;
				}
			case 'K':
				{
					type=  King;
					//cout<< "King Move: "<<str;
					piece_move = true;
					break;
				}
			default: break;
		}
		if( str[0] >= 'a' && str[0] <= 'h' && !piece_move )
		{
			//cout<< "Pawn Move, File = " << str[0]-'a' << ", and this is: "<< str[0] <<", in algebraic notation."<< endl;
			if( str[1] >= '1' && str[1] <= '8' )
			{
				// probably promotion
				if( str[2] == '=' )
				{
					switch(str[3])
					{
					case 'Q': 
						{
							if( mov == WHITE )
							{
								makemove( str[0]-'a', 6, str[0]-'a',7, true, 0);
							}
							else
							{
								makemove( str[0]-'a', 1, str[0]-'a',0, true, 0);
							}
							//cout<< "promote to Queen"<<endl; 
							mov++;
							return;
						}
					case 'B':
						{
							if( mov == WHITE )
							{
								makemove( (int)(str[0]-'a'), 6, (int)(str[0]-'a'),7, true, 1);
							}							
							else
							{
								makemove( (int)(str[0]-'a'), 1, (int)(str[0]-'a'),0, true, 1);
						
							}
							//cout<< "promote to bishop"<<endl; 
							mov++;
							return;
						}
					case 'N':
						{
							if( mov == WHITE )
							{
								makemove( str[0]-'a', 6, str[0]-'a',7, true, 2);
							}						
							else
							{
								makemove( str[0]-'a', 1, str[0]-'a',0, true, 2);
							}
							//cout<< "promote to knight"<<endl; 
							mov++;
							return;
						}
					case 'R':
						{
							if( mov == WHITE )
							{
								makemove( str[0]-'a', 6, str[0]-'a',7, true, 3);
								
							}							
							else
							{
								makemove( str[0]-'a', 1, str[0]-'a',0, true, 3);
							}
							//cout<< "promote to rook"<<endl; 
							mov++;
							return;
						}
					}
				}
			}
			else if( str[1] == 'x' )
			{
				if( str[2] >= 'a' && str[2] <= 'h' )
				{
					if( str[3] >='1' && str[3] <= '8' )
					{
						int start_file = str[0]-'a';
						int val_ah = str[2]-'a';
						int val_18 = str[3]-'1';

						//case of direct capture
						if( board[val_ah][val_18] != 0 )
						{
							if( mov == WHITE )
							{
							// 
								makemove( start_file, 
									val_18-1, 
									val_ah,
									val_18, 
									false, 
									0);
							}
							else
							{
								makemove( start_file, 
									val_18+1, 
									val_ah,
									val_18,
									false,
									0);		
							}
						}
						else
						{
							if( mov == WHITE )
							{
								//case of empasse
								makemove( start_file, 
									val_18-1, 
									val_ah,
									val_18, false,
									0);

								board[val_ah][ val_18-1]=0;
							}
							else
							{
								makemove( start_file, 
									val_18+1, 
									val_ah,
									val_18, false, 
									0);

								board[val_ah][ val_18+1]=0;
							}
						}

						// to find capture canidates we need to find all pawns on
						// the same file ... so here we go ...


						// TODO
						mov++;
						return;
						//cout<< "capture on file: "<<str[2]-'a'<<", and this is: "<<str[2]<<", in algebraic notation. And rank = " <<str[3]-'1'<<endl;
					}
				}
			}
		}

		else if( piece_move == true )
		{
			int file = str[2]-'a';
			int rank = str[3]-'1';
			if( str[1] >='a' && str[1] <= 'h' )
			{
				//cout<< "piece type: "<< str[0]<<" on file: "<<str[1] <<", moveing to file: "<< str[2] <<", and rank: "<<str[3]<<endl;
				switch( type )
				{
				case Bishop: 
					{
						//int fil = (int)(str[2]-'a');
						//int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Bishop)
									{
										if(!whitePcs[i]->bCaptured)
											bishops.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[2]->bCaptured)
									bishops.push_back(whitePcs[2]);

								if(!whitePcs[5]->bCaptured)
									bishops.push_back(whitePcs[5]);

								for( int i = 0; i < bishops.size(); i++ )
								{
									
									if( (bishops[i]->x == file) && IsSquareDark(file,rank))
									{
										if( bishops[i]->bdark_squared &&!isPiecePinned(bishops[i]) )
										{
											int abs1 = abs(file-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )//&& (board[file][rank]==0))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}
											}
										}
									}
									else
									{
										if( !bishops[i]->bdark_squared && !isPiecePinned(bishops[i]))
										{
											// check for blockades
											int abs1 = abs(file-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )//&& (board[file][rank]==0))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}			
											}
										}
									}
								}
							}
							else
							{
								MyChessPiece *pc1 = blackPcs[2];
								MyChessPiece *pc2 = blackPcs[5];

								if( this->IsSquareDark(file, rank) )
								{
									if( pc1->bdark_squared && !pc1->bCaptured)
									{
										makemove( pc1->x, pc1->y, file, rank, false,0);
										mov++;
										return;
									}
									else if(!pc2->bCaptured)
									{
										makemove( pc2->x, pc2->y, file, rank, false,0);
										mov++;
										return;
									}
								}
								else
								{
									if( pc1->bdark_squared &&!pc2->bCaptured)
									{
										makemove( pc2->x, pc2->y, file, rank, false,0);
										mov++;
										return;
									}
									else if(!pc1->bCaptured)
									{
										makemove( pc1->x, pc1->y, file, rank, false,0);
										mov++;
										return;
									}
								}
							mov++;
							return;
							}
						}
						else
						{
							if( extraBlackBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Bishop)
									{
										if(!blackPcs[i]->bCaptured)
											bishops.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[2]->bCaptured)
									bishops.push_back(blackPcs[2]);

								if(!blackPcs[5]->bCaptured)
									bishops.push_back(blackPcs[5]);

								for( int i = 0; i < bishops.size(); i++ )
								{
									if( bishops[i]->x == (str[1]-'a') )
									{
										if( IsSquareDark(file,rank))
										{
											if( bishops[i]->bdark_squared &&!isPiecePinned(bishops[i]))
											{
												int abs1 = abs(file-bishops[i]->x);
												int abs2 = abs(rank-bishops[i]->y);
												// is square accessable
												if( abs1 == abs2)
												{
													// square is accessable
													// check for blockades
													if( (abs1 == 1) )// && board[file][rank]==0)
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}
													else
													if( IsDiagonalSpanClear(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank))
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}
												}
											}
										}
										else
										{
											if( !bishops[i]->bdark_squared &&!isPiecePinned(bishops[i]))
											{
												// check for blockades
												int abs1 = abs(file-bishops[i]->x);
												int abs2 = abs(rank-bishops[i]->y);
												// is square accessable
												if( abs1 == abs2)
												{
													// square is accessable
													// check for blockades
													if( IsDiagonalSpanClear(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank))
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}		
												}
											}
										}
									}
								}
							}
							else
							{
								MyChessPiece *pc1 = blackPcs[2];
								MyChessPiece *pc2 = blackPcs[5];

								if( this->IsSquareDark(file, rank) )
								{
									if( pc1->bdark_squared &&!pc1->bCaptured )
									{
										makemove( pc1->x, pc1->y, file, rank, false,0);
										mov++;
										return;
									}
									else if(!pc2->bCaptured)
									{
										makemove( pc2->x, pc2->y, file, rank, false,0);
										mov++;
										return;
									}
								}
								else
								{
									if( pc1->bdark_squared &&!pc2->bCaptured)
									{
										makemove( pc2->x, pc2->y, file, rank, false,0);
										mov++;
										return;
									}
									else if(!pc1->bCaptured)
									{
										makemove( pc1->x, pc1->y, file, rank, false,0);
										mov++;
										return;
									}
								}
								
								mov++;	
								return;
							}
						}
					}
				case Knight:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteKnights )
							{
								vector<MyChessPiece*> knights;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Knight)
									{
										if(!whitePcs[i]->bCaptured)
											knights.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[1]->bCaptured)
									knights.push_back(whitePcs[1]);

								if(!whitePcs[6]->bCaptured)
									knights.push_back(whitePcs[6]);

								for( int i = 0; i < knights.size(); i++ )
								{
									if( knights[i]->x == (str[1]-'a') )
									{
										if( rank == knights[i]->y + 1 ||
											rank == knights[i]->y - 1)
										{
											if( fil == knights[i]->x + 2 ||
												fil == knights[i]->x - 2 )
											{
												//sq.file = knights[i]->x;
												//sq.rank = knights[i]->y;
												if( !isPiecePinned(knights[i]) )
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
													return;
												}
											}
										}
										else if ( rank == knights[i]->y + 2 ||
												  rank == knights[i]->y - 2)
										{
											if( fil == knights[i]->x + 1 ||
												fil == knights[i]->x - 1 )
											{
												if( !isPiecePinned(knights[i]) )
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
												}
												return;
											}
										}
									}
								}
							}
							else
							{
								for( int y = 0; y < 8; y++ )
								{
									if( board[ str[1]-'a'][y] == wKnight )
									{
										makemove( str[1]-'a', y, fil, rank, false,0);
									}
								}
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackKnights )
							{
								vector<MyChessPiece*> knights;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Knight)
									{
										if(!blackPcs[i]->bCaptured)
											knights.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[1]->bCaptured)
									knights.push_back(blackPcs[1]);

								if(!blackPcs[6]->bCaptured)
									knights.push_back(blackPcs[6]);

								for( int i = 0; i < knights.size(); i++ )
								{
									if( knights[i]->x == (str[1]-'a') )
									{
										if( rank == knights[i]->y + 1 ||
											rank == knights[i]->y - 1)
										{
											if( fil == knights[i]->x + 2 ||
												fil == knights[i]->x - 2 )
											{
												//sq.file = knights[i]->x;
												//sq.rank = knights[i]->y;
												if(!isPiecePinned(knights[i]))
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
													return;
												}
											}
										}
										else if ( rank == knights[i]->y + 2 ||
												  rank == knights[i]->y - 2)
										{
											if( fil == knights[i]->x + 1 ||
												fil == knights[i]->x - 1 )
											{
												if(!isPiecePinned(knights[i]))
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
													return;
												}
											}
										}
									}
								}
							}
							else
							{
								for( int y = 0; y < 8; y++ )
								{
									if( board[ str[1]-'a'][y] == bKnight )
									{
										makemove(str[1]-'a',y, fil,rank, false,0);
									}
								}
								mov++;
								return;
							}
						}
					break;
					}
				case Rook:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteRooks )
							{
							// build a vector of rooks, now attempt to find the correct rook
							// by assuming that if it has access to the square and is not blocked
							// then it must be the correct rook
								vector<MyChessPiece*> rooks;
								
								for( int i = 8; i < 16; i++)
								{
									if( whitePcs[i]->type == Rook )
									{
										if( !whitePcs[i]->bCaptured )
											rooks.push_back(whitePcs[i]);
									}
								}
								if( !whitePcs[0]->bCaptured )
									rooks.push_back(whitePcs[0]);

								if( !whitePcs[7]->bCaptured )
									rooks.push_back(whitePcs[7]);

								for( int i = 0; i < rooks.size(); i++ )
								{
									if(  (rooks[i]->x == (str[1]-'a')) )
									{
										if( ((str[2]-'a')==(str[1] - 'a')))
										{
											//Rce1
											// x=c, y=4, y1=4
											if( !isFileSpanBlocked(rooks[i]->x, rooks[i]->y, str[3]-'1') )
											{
												if(!isPiecePinned(rooks[i]))
												{
													makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
													mov++;
													return;
												}
											}
										}
										else if( (rooks[i]->y == (str[3]-'1')) )
										{
											if( !isRankSpanBlocked(rooks[i]->x, str[2]-'a', str[3]-'1') )
											{
												if(!isPiecePinned(rooks[i]))
												{
													makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
													mov++;
													return;
												}
											}									
										}
									}
								}
							}
							else
							{
								MyChessPiece *r1 = whitePcs[0];
								MyChessPiece *r2 = whitePcs[7];
							
								if( r1->x == str[1] - 'a' )
									makemove(r1->x, r1->y,fil,rank,false,0);
								else 
									makemove(r2->x, r2->y,fil,rank,false,0);
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackRooks )
							{
								vector<MyChessPiece*> rooks;
								
								for( int i = 8; i < 16; i++)
								{
									if( blackPcs[i]->type == Rook )
									{
										if( !blackPcs[i]->bCaptured )
											rooks.push_back(blackPcs[i]);
									}
								}
								if( !blackPcs[0]->bCaptured )
									rooks.push_back(blackPcs[0]);

								if( !blackPcs[7]->bCaptured )
									rooks.push_back(blackPcs[7]);

							// build a vector of rooks, now attempt to find the correct rook
							// by assuming that if it has access to the square and is not blocked
							// then it must be the correct rook
								for( int i = 0; i < rooks.size(); i++ )
								{
									if(  rooks[i]->x == str[1]-'a' )
									{
										//if( (str[2]-'a')==(str[1] - 'a') )
										//{
										//	if( !isFileSpanBlocked(rooks[i]->x, rooks[i]->y, str[3]-'1') )
											if(!isPiecePinned(rooks[i]))
											{
												makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
												mov++;
												return;
											}
										//}
										/*else
										{
											if( !isRankSpanBlocked(rooks[i]->x, str[2]-'1', str[3]-'1') )
											{
												makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
												mov++;
												return;
											}									
										}*/
									}
								}
							}
							else
							{
								MyChessPiece *r1 = blackPcs[0];
								MyChessPiece *r2 = blackPcs[7];
								if( r1->x == str[1] - 'a' )
									makemove(r1->x, r1->y,fil,rank,false,0);
								else 
									makemove(r2->x, r2->y,fil,rank,false,0);
								mov++;
								return;
							}
						}
						break;
					}
				case Queen:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Queen)
									{
										if(!whitePcs[i]->bCaptured)
											queens.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[3]->bCaptured)
									queens.push_back(whitePcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									// for Qb6 we need a Queen that is on a non-blocked file
									// or diagonal ... so first we test to see that it
									// can be accessed
									int abs1 = abs(fil-queens[i]->x);
									int abs2 = abs(rank-queens[i]->y);

									if( queens[i]->x != (str[1]-'a'))
										continue;

									if( abs1 == abs2 )
									{
										if( IsDiagonalSpanClear(queens[i]->x,
																queens[i]->y,
																fil,
																rank))
										{
											if(!isPiecePinned(queens[i]))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->x == fil)
									{
										if( !isFileSpanBlocked(fil, queens[i]->y, rank ))
										{
											if(!isPiecePinned(queens[i]))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->y == rank)
									{
										if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
										{
											if(!isPiecePinned(queens[i]))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
								}
							}
							else
							{
								MyChessSquare sq = findWhiteQueen(fil,rank);
								makemove(sq.file, sq.rank,fil,rank,false,0);
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Queen)
									{
										if(!blackPcs[i]->bCaptured)
											queens.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[3]->bCaptured)
									queens.push_back(blackPcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									// for Qb6 we need a Queen that is on a non-blocked file
									// or diagonal ... so first we test to see that it
									// can be accessed
									int abs1 = abs(fil-queens[i]->x);
									int abs2 = abs(rank-queens[i]->y);

									if( queens[i]->x != (str[1]-'a'))
										continue;

									if( abs1 == abs2 )
									{
										if( IsDiagonalSpanClear(queens[i]->x,
																queens[i]->y,
																fil,
																rank))
										{
											if(!isPiecePinned(queens[i]))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->x == fil)
									{
										if( !isFileSpanBlocked(fil, queens[i]->y, rank ))
										{
											if(!isPiecePinned(queens[i]))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->y == rank)
									{
										if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
										{
											if(!isPiecePinned(queens[i]))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
								}
							}
							else
							{
								MyChessSquare sq = findBlackQueen(fil,rank);
								makemove(sq.file, sq.rank,fil,rank,false,0);
								mov++;
								return;
							}
						}
						break;
					}
				case King:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							MyChessSquare sq = findWhiteKing(fil,rank);
							makemove(fil,rank,sq.file, sq.rank,false,0);
							mov++;
							return;
						}
						else
						{
							MyChessSquare sq = findBlackKing(fil,rank);
							makemove(fil,rank,sq.file, sq.rank,false,0);
							mov++;
							return;
						}
						break;
					}
				default: break;
				}
			}
			else if( str[1] >= '1' && str[1] <= '8' )
			{
				//cout<< "piece type: "<< str[0]<<" on file: "<<str[1] <<", moveing to file: "<< str[2] <<", and rank: "<<str[3]<<endl;
				switch( type )
				{
				case Bishop: 
					{
						if( mov == WHITE )
						{
							if( extraWhiteBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Bishop)
									{
										if(!whitePcs[i]->bCaptured)
											bishops.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[2]->bCaptured)
									bishops.push_back(whitePcs[2]);

								if(!whitePcs[5]->bCaptured)
									bishops.push_back(whitePcs[5]);

								for( int i = 0; i < bishops.size(); i++ )
								{
									
									if( IsSquareDark(file,rank))
									{
										if( bishops[i]->bdark_squared && 
											(bishops[i]->y == (str[1]-'1'))&&
											!isPiecePinned(bishops[i]))
										{
											int abs1 = abs(file-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )//&& (board[file][rank]==0))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}
											}
										}
									}
									else
									{
										if( bishops[i]->bdark_squared && 
											(bishops[i]->y == (str[1]-'1'))&&
											!isPiecePinned(bishops[i]))
										{
											// check for blockades
											int abs1 = abs(file-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )//&& (board[file][rank]==0))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank))
												{
													makemove(bishops[i]->x,
																		bishops[i]->y,
																		file,
																		rank,
																		false,
																		0);
													mov++;
													return;
												}			
											}
										}
									}
								}
							}
							else
							{
								for( int y = 0; y < 8; y++ )
								{
									if( board[ str[1]-'a'][y] == wBishop )
									{
										makemove( str[1]-'a', y, file, rank, false,0);
									}
								}
							}
							mov++;
							return;
						}
						else
						{
							if( extraBlackBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Bishop)
									{
										if(!blackPcs[i]->bCaptured)
											bishops.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[2]->bCaptured)
									bishops.push_back(blackPcs[2]);

								if(!blackPcs[5]->bCaptured)
									bishops.push_back(blackPcs[5]);

								for( int i = 0; i < bishops.size(); i++ )
								{
									if( bishops[i]->y == (str[1]-'1') )
									{
										if( IsSquareDark(file,rank))
										{
											if( bishops[i]->bdark_squared &&!isPiecePinned(bishops[i]))
											{
												int abs1 = abs(file-bishops[i]->x);
												int abs2 = abs(rank-bishops[i]->y);
												// is square accessable
												if( abs1 == abs2)
												{
													// square is accessable
													// check for blockades
													if( (abs1 == 1) )//&& board[file][rank]==0)
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}
													else
													if( IsDiagonalSpanClear(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank))
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}
												}
											}
										}
										else
										{
											if( !bishops[i]->bdark_squared &&!isPiecePinned(bishops[i])  )
											{
												// check for blockades
												int abs1 = abs(file-bishops[i]->x);
												int abs2 = abs(rank-bishops[i]->y);
												// is square accessable
												if( abs1 == abs2)
												{
													// square is accessable
													// check for blockades
													if( IsDiagonalSpanClear(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank))
													{

														makemove(bishops[i]->x,
																			bishops[i]->y,
																			file,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}				
												}
											}
										}
									}
								}
							}
							else
							{
								for( int y = 0; y < 8; y++ )
								{
									if( board[ str[1]-'a'][y] == bBishop )
									{
										makemove( str[1]-'a', y, file, rank, false,0);
									}
								}
								mov++;
							
								return;
							}
						}
					}
				case Knight:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteKnights )
							{
								vector<MyChessPiece*> knights;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Knight)
									{
										if(!whitePcs[i]->bCaptured)
											knights.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[1]->bCaptured)
									knights.push_back(whitePcs[1]);

								if(!whitePcs[6]->bCaptured)
									knights.push_back(whitePcs[6]);

								for( int i = 0; i < knights.size(); i++ )
								{
									if( knights[i]->y == (str[1]-'1') )
									{
										if( rank == knights[i]->y + 1 ||
											rank == knights[i]->y - 1)
										{
											if( fil == knights[i]->x + 2 ||
												fil == knights[i]->x - 2 )
											{
												//sq.file = knights[i]->x;
												//sq.rank = knights[i]->y;
												if( !isPiecePinned(knights[i]) )
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
													return;
												}
											}
										}
										else if ( rank == knights[i]->y + 2 ||
												  rank == knights[i]->y - 2)
										{
											if( fil == knights[i]->x + 1 ||
												fil == knights[i]->x - 1 )
											{
												if( !isPiecePinned(knights[i]) )
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
													return;
												}
											}
										}
									}
								}
							}
							else
							{
								for( int x = 0; x < 8; x++ )
								{
									if( board[x][ str[1]-'1'] == wKnight )
									{
										makemove(x, str[1]-'1',fil,rank,false,0);
									}
								}
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackKnights )
							{
								vector<MyChessPiece*> knights;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Knight)
									{
										if(!blackPcs[i]->bCaptured)
											knights.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[1]->bCaptured)
									knights.push_back(blackPcs[1]);

								if(!blackPcs[6]->bCaptured)
									knights.push_back(blackPcs[6]);

								for( int i = 0; i < knights.size(); i++ )
								{
									if( knights[i]->y == (str[1]-'1') )
									{
										if( rank == knights[i]->y + 1 ||
											rank == knights[i]->y - 1)
										{
											if( fil == knights[i]->x + 2 ||
												fil == knights[i]->x - 2 )
											{
												//sq.file = knights[i]->x;
												//sq.rank = knights[i]->y;
												if( !isPiecePinned(knights[i]) )
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
													return;
												}
											}
										}
										else if ( rank == knights[i]->y + 2 ||
												  rank == knights[i]->y - 2)
										{
											if( fil == knights[i]->x + 1 ||
												fil == knights[i]->x - 1 )
											{
												if( !isPiecePinned(knights[i]) )
												{
													makemove(knights[i]->x, knights[i]->y,
														fil, rank, false, 0);
													mov++;
													return;
												}
											}
										}
									}
								}
							}
							else
							{
								for( int x = 0; x < 8; x++ )
								{
									if( board[x][ str[1]-'1'] == bKnight )
									{
										makemove(x, str[1]-'1',fil,rank,false,0);
									}
								}
								mov++;
								return;
							}
						}
					break;
					}
				case Rook:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteRooks )
							{
								vector<MyChessPiece*> rooks;
								
								for( int i = 8; i < 16; i++)
								{
									if( whitePcs[i]->type == Rook )
									{
										if( !whitePcs[i]->bCaptured )
											rooks.push_back(whitePcs[i]);
									}
								}
								if( !whitePcs[0]->bCaptured )
									rooks.push_back(whitePcs[0]);

								if( !whitePcs[7]->bCaptured )
									rooks.push_back(whitePcs[7]);

							// build a vector of rooks, now attempt to find the correct rook
							// by assuming that if it has access to the square and is not blocked
							// then it must be the correct rook
								for( int i = 0; i < rooks.size(); i++ )
								{
									if(  rooks[i]->y == str[1]-'1' )
									{
										if( (str[3]-'1')==(str[1] - '1') )
										{
											if( !isRankSpanBlocked(rooks[i]->x, fil, rank) )
											{
												if( !isPiecePinned(rooks[i]) )
												{
													makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
													mov++;
													return;
												}
											}	
										}
										else
										{
											if( !isFileSpanBlocked(rooks[i]->x, rooks[i]->y, str[3]-'1') )
											{
												if( !isPiecePinned(rooks[i]) )
												{
													makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
													mov++;
													return;
												}
											}								
										}
									}
								}
							}
							else
							{
								MyChessPiece *r1 = whitePcs[0];
								MyChessPiece *r2 = whitePcs[7];
								if( r1->y == str[1] - '1' )
									makemove(r1->x, r1->y,fil,rank,false,0);
								else 
									makemove(r2->x, r2->y,fil,rank,false,0);
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackRooks )
							{
								vector<MyChessPiece*> rooks;
								
								for( int i = 8; i < 16; i++)
								{
									if( blackPcs[i]->type == Rook )
									{
										if( !blackPcs[i]->bCaptured )
											rooks.push_back(blackPcs[i]);
									}
								}
								if( !blackPcs[0]->bCaptured )
									rooks.push_back(blackPcs[0]);

								if( !blackPcs[7]->bCaptured )
									rooks.push_back(blackPcs[7]);

							// build a vector of rooks, now attempt to find the correct rook
							// by assuming that if it has access to the square and is not blocked
							// then it must be the correct rook
								for( int i = 0; i < rooks.size(); i++ )
								{
									if(  rooks[i]->y == str[1]-'1' )
									{
										if( (str[3]-'1')==(str[1] - '1') )
										{
											if( !isRankSpanBlocked(rooks[i]->x, fil, rank) )
											{
												if( !isPiecePinned(rooks[i]) )
												{
													makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
													mov++;
													return;
												}
											}	
										}
										else
										{
											if( !isFileSpanBlocked(rooks[i]->x, rooks[i]->y, str[3]-'1') )
											{
												if( !isPiecePinned(rooks[i]) )
												{
													makemove(rooks[i]->x, rooks[i]->y,fil,rank,false,0);
													mov++;
													return;
												}
											}								
										}
									}
								}
							}
							else
							{
								MyChessPiece *r1 = blackPcs[0];
								MyChessPiece *r2 = blackPcs[7];
								if( r1->y == str[1] - '1' )
									makemove(r1->x, r1->y,fil,rank,false,0);
								else 
									makemove(r2->x, r2->y,fil,rank,false,0);
								mov++;
								return;
							}
						}
						break;
					}
				case Queen:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Queen)
									{
										if(!whitePcs[i]->bCaptured)
											queens.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[3]->bCaptured)
									queens.push_back(whitePcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									// for Qb6 we need a Queen that is on a non-blocked file
									// or diagonal ... so first we test to see that it
									// can be accessed
									int abs1 = abs(fil-queens[i]->x);
									int abs2 = abs(rank-queens[i]->y);

									if( queens[i]->y != (str[1]-'1'))
										continue;

									if( abs1 == abs2 )
									{
										if( IsDiagonalSpanClear(queens[i]->x,
																queens[i]->y,
																fil,
																rank))
										{
											if( !isPiecePinned(queens[i]) )
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->x == fil)
									{
										if( !isFileSpanBlocked(fil, queens[i]->y, rank ))
										{
											if( !isPiecePinned(queens[i]) )
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->y == rank)
									{
										if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
										{
											if( !isPiecePinned(queens[i]) )
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
								}
							}
							else
							{
								for( int x = 0; x < 8; x++ )
								{
									if( board[x][ str[1]-'1'] == wQueen )
									{
										makemove(x, str[1]-'1',fil,rank,false,0);
									}
								}
							
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Queen)
									{
										if(!blackPcs[i]->bCaptured)
											queens.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[3]->bCaptured)
									queens.push_back(blackPcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									// for Qb6 we need a Queen that is on a non-blocked file
									// or diagonal ... so first we test to see that it
									// can be accessed
									int abs1 = abs(fil-queens[i]->x);
									int abs2 = abs(rank-queens[i]->y);

									if( queens[i]->y != (str[1]-'1'))
										continue;

									if( abs1 == abs2 )
									{
										if( IsDiagonalSpanClear(queens[i]->x,
																queens[i]->y,
																fil,
																rank))
										{
											if( !isPiecePinned(queens[i]) )
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->x == fil)
									{
										if( !isFileSpanBlocked(fil, queens[i]->y, rank ))
										{
											if( !isPiecePinned(queens[i]) )
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if(queens[i]->y == rank)
									{
										if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
										{
											if( !isPiecePinned(queens[i]) )
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
								}
							}
							else
							{
								for( int x = 0; x < 8; x++ )
								{
									if( board[x][ str[1]-'1'] == bQueen )
									{
										makemove(x, str[1]-'1',fil,rank,false,0);
									}
								}
								mov++;
								return;
							}
						}
						break;
					}
				case King:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							MyChessSquare sq = findWhiteKing(fil,rank);
							makemove(fil,rank,sq.file, sq.rank,false,0);
							mov++;
							return;
						}
						else
						{
							MyChessSquare sq = findBlackKing(fil,rank);
							makemove(fil,rank,sq.file, sq.rank,false,0);
							mov++;
							return;
						}
						break;
					}
				default: break;
				}
				//cout<< "piece type: "<< str[0]<<" on rank: "<<str[1] <<", moving to file: "<< str[2] <<", and rank: "<<str[3]<<endl;
			}
			else if( str[1] == 'x' )
			{
				switch( type )
				{
				case Bishop: 
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Bishop)
									{
										if(!whitePcs[i]->bCaptured)
											bishops.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[2]->bCaptured)
									bishops.push_back(whitePcs[2]);

								if(!whitePcs[5]->bCaptured)
									bishops.push_back(whitePcs[5]);

								for( int i = 0; i < bishops.size(); i++ )
								{
									if( IsSquareDark(fil,rank))
									{
										if( bishops[i]->bdark_squared )
										{
											int abs1 = abs(fil-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )// && (board[fil][rank]==0))
												{
													if( !isPiecePinned(bishops[i]) )
													{
														makemove(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank,
																		false,
																		0);
														mov++;
														return;
													}
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank))
												{
													if( !isPiecePinned(bishops[i]) )
													{
															makemove(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank,
																		false,
																		0);
														mov++;
														return;
													}
												}
											}
										}
									}
									else
									{
										if( !bishops[i]->bdark_squared )
										{
											// check for blockades
											int abs1 = abs(fil-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )//&& (board[fil][rank]==0))
												{
													if( !isPiecePinned(bishops[i]) )
													{
														makemove(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank,
																		false,
																		0);
														mov++;
														return;
													}
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank))
												{
													if( !isPiecePinned(bishops[i]) )
													{
														makemove(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank,
																		false,
																		0);
														mov++;
														return;
													}
												}
											}
										}
									}
								}
							}
							else
							{
								MyChessSquare sq = findWhiteBishop(fil,rank);
								makemove(sq.file, sq.rank,fil,rank,false,0);
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Bishop)
									{
										if(!blackPcs[i]->bCaptured)
											bishops.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[2]->bCaptured)
									bishops.push_back(blackPcs[2]);

								if(!blackPcs[5]->bCaptured)
									bishops.push_back(blackPcs[5]);

								for( int i = 0; i < bishops.size(); i++ )
								{
									if( IsSquareDark(fil,rank))
									{
										if( bishops[i]->bdark_squared )
										{
											int abs1 = abs(fil-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )//&& (board[fil][rank]==0))
												{
													if( !isPiecePinned(bishops[i]) )
													{
														makemove(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank,
																		false,
																		0);
														mov++;
														return;
													}
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank))
												{
													if( !isPiecePinned(bishops[i]) )
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			fil,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}
												}
											}
										}
									}
									else
									{
										if( !bishops[i]->bdark_squared )
										{
											// check for blockades
											int abs1 = abs(fil-bishops[i]->x);
											int abs2 = abs(rank-bishops[i]->y);
											// is square accessable
											if( abs1 == abs2)
											{
												// square is accessable
												// check for blockades
												if( (abs1 == 1) )//&& (board[fil][rank]==0))
												{
													if( !isPiecePinned(bishops[i]) )
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			fil,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}
												}
												else
												if( IsDiagonalSpanClear(bishops[i]->x,
																		bishops[i]->y,
																		fil,
																		rank))
												{
													if( !isPiecePinned(bishops[i]) )
													{
														makemove(bishops[i]->x,
																			bishops[i]->y,
																			fil,
																			rank,
																			false,
																			0);
														mov++;
														return;
													}
												}	
											}
										}
									}
								}
							}
							else
							{
								MyChessSquare sq = findBlackBishop(fil,rank);
								makemove(sq.file, sq.rank,fil,rank,false,0);
								mov++;
								return;
							}
						}
						break;
						
					}
				case Knight:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteKnights )
							{
								vector<MyChessPiece*> knights;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Knight)
									{
										if(!whitePcs[i]->bCaptured)
											knights.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[1]->bCaptured)
									knights.push_back(whitePcs[1]);

								if(!whitePcs[6]->bCaptured)
									knights.push_back(whitePcs[6]);

								for( int i = 0; i < knights.size(); i++ )
								{
									if( rank == knights[i]->y + 1 ||
										rank == knights[i]->y - 1)
									{
										if( fil == knights[i]->x + 2 ||
											fil == knights[i]->x - 2 )
										{
											//sq.file = knights[i]->x;
											//sq.rank = knights[i]->y;
											if( !isPiecePinned(knights[i]) )
											{
												makemove(knights[i]->x, knights[i]->y,
													fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if ( rank == knights[i]->y + 2 ||
											  rank == knights[i]->y - 2)
									{
										if( fil == knights[i]->x + 1 ||
											fil == knights[i]->x - 1 )
										{
											if( !isPiecePinned(knights[i]) )
											{
												makemove(knights[i]->x, knights[i]->y,
													fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}							
								}
							}
							else
							{
								MyChessSquare sq = findWhiteKnight(fil,rank);

								makemove(sq.file, sq.rank,fil,rank,false,0);
								mov++;
								return;
							}
						}
						else
						{
							if( extraBlackKnights )
							{
								vector<MyChessPiece*> knights;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Knight)
									{
										if(!blackPcs[i]->bCaptured)
											knights.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[1]->bCaptured)
									knights.push_back(blackPcs[1]);

								if(!blackPcs[6]->bCaptured)
									knights.push_back(blackPcs[6]);

								for( int i = 0; i < knights.size(); i++ )
								{
									if( rank == knights[i]->y + 1 ||
										rank == knights[i]->y - 1)
									{
										if( fil == knights[i]->x + 2 ||
											fil == knights[i]->x - 2 )
										{
											//sq.file = knights[i]->x;
											//sq.rank = knights[i]->y;
											if( !isPiecePinned(knights[i]) )
											{
												makemove(knights[i]->x, knights[i]->y,
													fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}
									else if ( rank == knights[i]->y + 2 ||
											  rank == knights[i]->y - 2)
									{
										if( fil == knights[i]->x + 1 ||
											fil == knights[i]->x - 1 )
										{
											if( !isPiecePinned(knights[i]) )
											{
												makemove(knights[i]->x, knights[i]->y,
													fil, rank, false, 0);
												mov++;
												return;
											}
										}
									}							
								}
							}
							else
							{
								MyChessSquare sq = findBlackKnight(fil,rank);
								makemove(sq.file, sq.rank,fil,rank,false,0);
								mov++;
								return;
							}
						}
						break;
					}
				case Rook:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						int indx1;
						int indx2;
						MyChessPiece *r1;// = &pieces[indx1];
						MyChessPiece *r2;// = &pieces[indx2];

						if( mov == WHITE )
						{
							if( extraWhiteRooks )
							{
								vector<MyChessPiece*> rooks;
								
								
								for( int i = 8; i < 16; i++)
								{
									if( whitePcs[i]->type == Rook )
									{
										if( !whitePcs[i]->bCaptured )
											rooks.push_back(whitePcs[i]);
									}
								}
								if( !whitePcs[0]->bCaptured )
									rooks.push_back(whitePcs[0]);

								if( !whitePcs[7]->bCaptured )
									rooks.push_back(whitePcs[7]);
								

								for( int i = 0; i < rooks.size(); i++ )
								{
									if( rooks[i]->x == str[2]-'a' )
									{
										if( !isFileSpanBlocked(str[2]-'a', rooks[i]->y, str[3]-'1' ))
									//	continue;
										//else
										{
											//sq.file = rooks[i]->x;
											//sq.rank = rooks[i]->y;
											//Rook Found,
											if( !isPiecePinned(rooks[i]) )
											{
												makemove(rooks[i]->x, rooks[i]->y, str[2]-'a',str[3]-'1', false,0);
												mov++;
												return;
											}
										}
									}
									if( rooks[i]->y == str[3]-'1' )
									{
										if( !isRankSpanBlocked(rooks[i]->x, str[2]-'a', str[3]-'1' ))
										//continue;
									//else
										{
										// rook found
										//sq.file = rooks[i]->x;
										//sq.rank = rooks[i]->y;
											if( !isPiecePinned(rooks[i]) )
											{
												makemove(rooks[i]->x, rooks[i]->y, str[2]-'a',str[3]-'1', false,0);
												mov++;
												return;
											}
										}
									}
								}
							}
						}
						else
						{
							if( extraBlackRooks )
							{
								vector<MyChessPiece*> rooks;
								
								for( int i = 8; i < 16; i++)
								{
									if( blackPcs[i]->type == Rook )
									{
										if( !blackPcs[i]->bCaptured )
											rooks.push_back(blackPcs[i]);
									}
								}
								if( !blackPcs[0]->bCaptured )
									rooks.push_back(blackPcs[0]);

								if( !blackPcs[7]->bCaptured )
									rooks.push_back(blackPcs[7]);
								

								for( int i = 0; i < rooks.size(); i++ )
								{
									if( rooks[i]->x == str[2]-'a' )
									{
										if( !isFileSpanBlocked(str[2]-'a', rooks[i]->y, str[3]-'1' ))
									//	continue;
										//else
										{
											//sq.file = rooks[i]->x;
											//sq.rank = rooks[i]->y;
											//Rook Found,
											if( !isPiecePinned(rooks[i]) )
											{
												makemove(rooks[i]->x, rooks[i]->y, str[2]-'a',str[3]-'1', false,0);
												mov++;
												return;
											}
										}
									}
									if( rooks[i]->y == str[3]-'1' )
									{
										if( !isRankSpanBlocked(rooks[i]->x, str[2]-'a', str[3]-'1' ))
										//continue;
									//else
										{
										// rook found
										//sq.file = rooks[i]->x;
										//sq.rank = rooks[i]->y;
											if( !isPiecePinned(rooks[i]) )
											{
												makemove(rooks[i]->x, rooks[i]->y, str[2]-'a',str[3]-'1', false,0);
												mov++;
												return;
											}
										}
									}
								}
							}
						}

						if( mov == WHITE )
						{
							r1 = whitePcs[0];
							r2 = whitePcs[7];
						}
						else
						{
							r1 = blackPcs[0];
							r2 = blackPcs[7];
						}

						if( r1->bCaptured == true )
						{
							makemove(r2->x, r2->y, fil, rank,false,0);
							mov++;
							return;
						}
						if( r2->bCaptured == true )
						{
							makemove(r1->x, r1->y, fil, rank,false,0);
							mov++;
							return;
						}
					
						if( r1->x != fil && r1->y != rank )
						{
							makemove(r2->x, r2->y, fil, rank,false,0);
							mov++;
							return;
							// move r2
						}
						if( r2->x != fil && r2->y != rank )
						{
							makemove(r1->x, r1->y, fil, rank,false,0);
							mov++;
							return;
							// move r1
						}

						bool movr1 = true;
						bool movr2 = true;

						if( r1->x == fil )
						{
							if( r1->y < rank )
							{
								for( int i = r1->y+1; i < rank; i++ )
									if( board[fil][i] != 0 )
										movr1 = false;
							}
							else 
							{
								for( int i = rank+1; i < r1->y; i++ )
									if( board[fil][i] != 0 )
										movr1 = false;
							}
						}
						if( r2->x == fil )
						{
							if( r2->y < rank )
							{
								for( int i = r2->y+1; i < rank; i++ )
									if( board[fil][i] != 0 )
										movr2 = false;
							}
							else 
							{
								for( int i = rank+1; i < r2->y; i++ )
									if( board[fil][i] != 0 )
										movr2 = false;
							}
						}
						if( r1->y == rank )
						{
							if( r1->x < fil )
							{
								for( int i = r1->x+1; i < fil; i++ )
									if( board[i][rank] != 0 )
										movr1 = false;
							}
							else 
							{
								for( int i = fil+1; i < r1->x; i++ )
									if( board[i][rank] != 0 )
										movr1 = false;
							}
						}
						if( r2->y == rank )
						{
							if( r2->x < fil )
							{
								for( int i = r2->x+1; i < fil; i++ )
									if( board[i][rank] != 0 )
										movr2 = false;
							}
							else 
							{
								for( int i = fil+1; i < r2->x; i++ )
									if( board[i][rank] != 0 )
										movr2 = false;
							}
						}
						//ROOK BUG
						if(this->isPiecePinned(r1) )
						{
	
							if( mov == WHITE )
							{
								if( ((whitePcs[4]->x == r1->PinFromX) &&
									(fil != whitePcs[4]->x )))
									if( fil != r1->PinFromX &&
										rank != r1->PinFromY )
										movr1 = false;

								if( ((whitePcs[4]->y == r1->PinFromY) &&
									(rank != whitePcs[4]->y)))
									if( fil != r1->PinFromX &&
										rank != r1->PinFromY )
										movr1 = false;

							}
							else
							{
								if( ((blackPcs[4]->x == r1->PinFromX) &&
									(fil != blackPcs[4]->x )))
									if( fil != r1->PinFromX &&
										rank != r1->PinFromY )
										movr1 = false;

								if( ((blackPcs[4]->y != r1->PinFromY) &&
									(rank == blackPcs[4]->y)))
									if( fil != r1->PinFromX &&
										rank != r1->PinFromY )
										movr1 = false;
							}
								
							
						}
						if( isPiecePinned(r2))
						{
							if( mov == WHITE )
							{
								if( ((whitePcs[4]->x == r2->PinFromX) &&
									(fil != whitePcs[4]->x )))
									if( fil != r2->PinFromX &&
										rank != r2->PinFromY )
										movr2 = false;
								

								if( ((whitePcs[4]->y == r2->PinFromY) &&
									(rank != whitePcs[4]->y)))
									if( fil != r2->PinFromX &&
										rank != r2->PinFromY )
										movr2 = false;

							}
							else
							{
								if( ((blackPcs[4]->x == r2->PinFromX) &&
									(fil != blackPcs[4]->x )))
									if( fil != r2->PinFromX &&
										rank != r2->PinFromY )
										movr2 = false;

								if( ((blackPcs[4]->y == r2->PinFromY) &&
									(rank != blackPcs[4]->y)))
									if( fil != r2->PinFromX &&
										rank != r2->PinFromY )
										movr2 = false;
							}
						}

						if( movr1 )
						{
							makemove( r1->x, r1->y, fil, rank,false,0);
						}

						if( movr2 )
						{
							makemove(r2->x, r2->y, fil, rank,false,0);
						}

						mov++;
						return;
					}
				case Queen:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							if( extraWhiteQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Queen)
									{
										if(!whitePcs[i]->bCaptured)
											queens.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[3]->bCaptured)
									queens.push_back(whitePcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									// for Qb6 we need a Queen that is on a non-blocked file
									// or diagonal ... so first we test to see that it
									// can be accessed
									int abs1 = abs(fil-queens[i]->x);
									int abs2 = abs(rank-queens[i]->y);

									//if( queens[i]->y != (str[1]-'1'))
									//	continue;

									if( abs1 == abs2 )
									{
										if( IsDiagonalSpanClear(queens[i]->x,
																queens[i]->y,
																fil,
																rank))
										{
											makemove(queens[i]->x,
													 queens[i]->y,
													 fil, rank, false, 0);
											mov++;
											return;
										}
									}
									else if(queens[i]->x == fil)
									{
										if( !isFileSpanBlocked(fil, queens[i]->y, rank ))
										{
											makemove(queens[i]->x,
													 queens[i]->y,
													 fil, rank, false, 0);
											mov++;
											return;
										}
									}
									else if(queens[i]->y == rank)
									{
										if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
										{
											makemove(queens[i]->x,
													 queens[i]->y,
													 fil, rank, false, 0);
											mov++;
											return;
										}
									}
								}
							}
							else
							{
							MyChessSquare sq = findWhiteQueen(fil,rank);
							makemove(sq.file, sq.rank,fil,rank,false,0);
							mov++;
							return;
							}
						}
						else
						{
							if( extraBlackQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Queen)
									{
										if(!blackPcs[i]->bCaptured)
											queens.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[3]->bCaptured)
									queens.push_back(blackPcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									// for Qb6 we need a Queen that is on a non-blocked file
									// or diagonal ... so first we test to see that it
									// can be accessed
									int abs1 = abs(fil-queens[i]->x);
									int abs2 = abs(rank-queens[i]->y);

								//	if( queens[i]->y != (str[1]-'1'))
								//		continue;

									if( abs1 == abs2 )
									{
										if( IsDiagonalSpanClear(queens[i]->x,
																queens[i]->y,
																fil,
																rank))
										{
											makemove(queens[i]->x,
													 queens[i]->y,
													 fil, rank, false, 0);
											mov++;
											return;
										}
									}
									else if(queens[i]->x == fil)
									{
										if( !isFileSpanBlocked(fil, queens[i]->y, rank ))
										{
											makemove(queens[i]->x,
													 queens[i]->y,
													 fil, rank, false, 0);
											mov++;
											return;
										}
									}
									else if(queens[i]->y == rank)
									{
										if( !isRankSpanBlocked(queens[i]->x, fil, rank ))
										{
											makemove(queens[i]->x,
													 queens[i]->y,
													 fil, rank, false, 0);
											mov++;
											return;
										}
									}
								}
							}
							else
							{
							MyChessSquare sq = findBlackQueen(fil,rank);
							makemove(sq.file, sq.rank,fil,rank,false,0);
							mov++;
							return;
							}
						}
						
						break;
					}
				case King:
					{
						int fil = (int)(str[2]-'a');
						int rank = (int)(str[3] - '1');
						if( mov == WHITE )
						{
							MyChessSquare sq = findWhiteKing(fil,rank);
							makemove(sq.file, sq.rank, fil,rank,false,0);
							mov++;
							return;
						}
						else
						{
							MyChessSquare sq = findBlackKing(fil,rank);
							makemove(sq.file, sq.rank,fil,rank,false,0);
							mov++;
							return;
						}
						break;
					}
				default: break;
				}
				}
		}
		else if( str[1] != 'x' )
		{
			//cout << "Error, returning" <<endl;
			return;
		}
	}

	// castle long
	if( len == 5 )
	{
		int type = -1;
		bool piece_move = false;
		if( str[0] == '0' || str[0] == 'O')
		{
			//cout << "Castling long: "<< str << endl;
			if( mov == WHITE )
			{
				makemove(a,0,d,0,false,0);
				makemove(e,0,c,0,false,0, true);
				
				this->whiteCastled = true;
				mov++;
				return;
			}
			else
			{
				makemove(a,7,d,7,false,0);
				makemove(e,7,c,7,false,0, true);
				
				this->blackCastled = true;
				mov++;
				return;
			}
			
		}
		else
		{
			switch( str[0] )
			{
				
				case 'B':
				{
					type = Bishop;
					//cout<< "Bishop Move: "<<str;
					piece_move = true;
					break;
				}
				case 'N':
					{
						type = Knight;
						//cout<< "Knight Move: "<<str;
						piece_move = true;
						break;
					}
				case 'R':
					{
						type = Rook;
						//cout<< "Rook Move: "<<str;
						piece_move = true;
						break;
					}
				case 'Q':
					{
						type = Queen;
						//cout<< "Queen Move: "<<str;
						piece_move = true;
						break;
					}
				case 'K':
					{
						type=  King;
						//cout<< "King Move: "<<str;
						piece_move = true;
						break;
					}
				default: break;
			}
		}
		
		if( piece_move == true )
		{
			int file = str[3]-'a';
			int rank = str[4]-'1';
			if( str[1] >='a' && str[1] <= 'h' )
			{
				//cout<< "piece type: "<< str[0]<<" on file: "<<str[1] <<endl;
				if( str[2] == 'x' )
				{
					switch( type )
					{
					case Knight:
						{
							if( mov == WHITE )
							{
								if( extraWhiteKnights )
								{
									vector<MyChessPiece*> knights;
									for( int i = 8; i < 16; i++ )
									{
										if(whitePcs[i]->type == Knight)
										{
											if(!whitePcs[i]->bCaptured)
												knights.push_back(whitePcs[i]);
										}
									}

									if(!whitePcs[1]->bCaptured)
										knights.push_back(whitePcs[1]);

									if(!whitePcs[6]->bCaptured)
										knights.push_back(whitePcs[6]);

									for( int i = 0; i < knights.size(); i++ )
									{
										if( knights[i]->x == (str[1]-'a') )
										{
											if( rank == knights[i]->y + 1 ||
												rank == knights[i]->y - 1)
											{
												if( file == knights[i]->x + 2 ||
													file == knights[i]->x - 2 )
												{
													//sq.file = knights[i]->x;
													//sq.rank = knights[i]->y;
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
											else if ( rank == knights[i]->y + 2 ||
													  rank == knights[i]->y - 2)
											{
												if( file == knights[i]->x + 1 ||
													file == knights[i]->x - 1 )
												{
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
										}
									}
								}
								else
								{
									MyChessPiece *n1 = whitePcs[1];
									MyChessPiece *n2 = whitePcs[6];
									if( n1->x == (int)(str[1]-'a'))
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->x == str[1]-'a' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							else
							{
								if( extraBlackKnights )
								{
									vector<MyChessPiece*> knights;
									for( int i = 8; i < 16; i++ )
									{
										if(blackPcs[i]->type == Knight)
										{
											if(!blackPcs[i]->bCaptured)
												knights.push_back(blackPcs[i]);
										}
									}

									if(!blackPcs[1]->bCaptured)
										knights.push_back(blackPcs[1]);

									if(!blackPcs[6]->bCaptured)
										knights.push_back(blackPcs[6]);

									for( int i = 0; i < knights.size(); i++ )
									{
										if( knights[i]->x == (str[1]-'a') )
										{
											if( rank == knights[i]->y + 1 ||
												rank == knights[i]->y - 1)
											{
												if( file == knights[i]->x + 2 ||
													file == knights[i]->x - 2 )
												{
													//sq.file = knights[i]->x;
													//sq.rank = knights[i]->y;
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
											else if ( rank == knights[i]->y + 2 ||
													  rank == knights[i]->y - 2)
											{
												if( file == knights[i]->x + 1 ||
													file == knights[i]->x - 1 )
												{
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
										}
									}
								}
								else
								{
									MyChessPiece *n1 = blackPcs[1];
									MyChessPiece *n2 = blackPcs[6];
									if( n1->x == str[1]-'a')
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->x == str[1]-'a' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							break;
						}
					case Rook:
						{
							if( mov == WHITE )
							{
								if( extraWhiteRooks )
								{
								
						// build a vector of rooks, now attempt to find the correct rook
						// by assuming that if it has access to the square and is not blocked
						// then it must be the correct rook
									vector<MyChessPiece*> rooks;
									
									for( int i = 8; i < 16; i++)
									{
										if( whitePcs[i]->type == Rook )
										{
											if( !whitePcs[i]->bCaptured )
												rooks.push_back(whitePcs[i]);
										}
									}
									if( !whitePcs[0]->bCaptured )
										rooks.push_back(whitePcs[0]);

									if( !whitePcs[7]->bCaptured )
										rooks.push_back(whitePcs[7]);
									

									for( int i = 0; i < rooks.size(); i++ )
									{
										if( rooks[i]->x == str[1]-'a' )
										{
											if( file == str[1] - 'a' )
											{
												if( !isFileSpanBlocked(str[1]-'a', rooks[i]->y, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}
											}
											else
											{
												if( !isRankSpanBlocked(str[1]-'a', file, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}
											}	
										}
									}
								}
								else
								{
									MyChessPiece *n1 = whitePcs[0];
									MyChessPiece *n2 = whitePcs[7];
									if( n1->x == str[1]-'a')
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->x == str[1]-'a' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							else
							{
								if( extraBlackRooks )
								{
									vector<MyChessPiece*> rooks;
									
									for( int i = 8; i < 16; i++)
									{
										if( blackPcs[i]->type == Rook )
										{
											if( !blackPcs[i]->bCaptured )
												rooks.push_back(blackPcs[i]);
										}
									}
									if( !blackPcs[0]->bCaptured )
										rooks.push_back(blackPcs[0]);

									if( !blackPcs[7]->bCaptured )
										rooks.push_back(blackPcs[7]);
									

									for( int i = 0; i < rooks.size(); i++ )
									{
										if( rooks[i]->x == str[1]-'a' )
										{
											if( file == str[1] - 'a' )
											{
												if( !isFileSpanBlocked(str[1]-'a', rooks[i]->y, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}
											}
											else
											{
												if( !isRankSpanBlocked(str[1]-'a', file, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}
											}	
										}
									}
								}
								else
								{
									MyChessPiece *n1 = blackPcs[0];
									MyChessPiece *n2 = blackPcs[7];
									if( n1->x == str[1]-'a')
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->x == str[1]-'a' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							break;
						}
					case Queen:
						{
							if( mov == WHITE )
							{
								if( extraWhiteQueens )
								{
									vector<MyChessPiece*> queens;
									for( int i = 8; i < 16; i++ )
									{
										if(whitePcs[i]->type == Queen)
										{
											if(!whitePcs[i]->bCaptured)
												queens.push_back(whitePcs[i]);
										}
									}

									if(!whitePcs[3]->bCaptured)
										queens.push_back(whitePcs[3]);

						
									for( int i = 0; i < queens.size(); i++ )
									{
										// for Qb6 we need a Queen that is on a non-blocked file
										// or diagonal ... so first we test to see that it
										// can be accessed
										int abs1 = abs(file-queens[i]->x);
										int abs2 = abs(rank-queens[i]->y);

										if( queens[i]->x != (str[1]-'a'))
											continue;

										if( abs1 == abs2 )
										{
											if( IsDiagonalSpanClear(queens[i]->x,
																	queens[i]->y,
																	file,
																	rank))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->x == file)
										{
											if( !isFileSpanBlocked(file, queens[i]->y, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->y == rank)
										{
											if( !isRankSpanBlocked(queens[i]->x, file, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
									}
								}
							}
							else
							{
								if( extraBlackQueens )
								{
									vector<MyChessPiece*> queens;
									for( int i = 8; i < 16; i++ )
									{
										if(blackPcs[i]->type == Queen)
										{
											if(!blackPcs[i]->bCaptured)
												queens.push_back(blackPcs[i]);
										}
									}

									if(!blackPcs[3]->bCaptured)
										queens.push_back(blackPcs[3]);

						
									for( int i = 0; i < queens.size(); i++ )
									{
										// for Qb6 we need a Queen that is on a non-blocked file
										// or diagonal ... so first we test to see that it
										// can be accessed
										int abs1 = abs(file-queens[i]->x);
										int abs2 = abs(rank-queens[i]->y);

										if( queens[i]->x != (str[1]-'a'))
											continue;

										if( abs1 == abs2 )
										{
											if( IsDiagonalSpanClear(queens[i]->x,
																	queens[i]->y,
																	file,
																	rank))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->x == file)
										{
											if( !isFileSpanBlocked(file, queens[i]->y, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->y == rank)
										{
											if( !isRankSpanBlocked(queens[i]->x, file, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}

										}
									}

								}
							}

							break; // TODO
						}
					}
					//cout << "captures ...:"<<endl;
					//cout << "piece on file: "<< str[3]<<", and rank: "<<str[4]<<endl;
				}
				//
				
				else
				{
					// Case of Qg1g3	//cout<< "error in str.length() == 4"<<endl;
					if( str[0] == 'Q' )
					{
						// game of many Queens ...
						if( mov == WHITE )
						{
							if( extraWhiteQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Queen)
									{
										if(!whitePcs[i]->bCaptured)
											queens.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[3]->bCaptured)
									queens.push_back(whitePcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									if( (queens[i]->x == (str[1]-'a')) &&
										(queens[i]->y == (str[2]-'1')))
									{
										makemove(queens[i]->x, queens[i]->y,
											(str[3]-'a'), (str[4]-'1'), false, 0);
										mov++;
										return;

									}
								}
							}
						}
						else
						{
							if( extraBlackQueens )
							{
								vector<MyChessPiece*> queens;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Queen)
									{
										if(!blackPcs[i]->bCaptured)
											queens.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[3]->bCaptured)
									queens.push_back(blackPcs[3]);

						
								for( int i = 0; i < queens.size(); i++ )
								{
									if( (queens[i]->x == (str[1]-'a')) &&
										(queens[i]->y == (str[2]-'1')))
									{
										makemove(queens[i]->x, queens[i]->y,
											(str[3]-'a'), (str[4]-'1'), false, 0);
										mov++;
										return;

									}
								}
							}
						}
					}

					if( str[0] == 'B' )
					{
						// game of many Queens ...
						if( mov == WHITE )
						{
							if( extraWhiteBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(whitePcs[i]->type == Bishop)
									{
										if(!whitePcs[i]->bCaptured)
											bishops.push_back(whitePcs[i]);
									}
								}

								if(!whitePcs[2]->bCaptured)
									bishops.push_back(whitePcs[2]);

								if(!whitePcs[5]->bCaptured)
									bishops.push_back(whitePcs[5]);

						
								for( int i = 0; i < bishops.size(); i++ )
								{
									if( (bishops[i]->x == (str[1]-'a')) &&
										(bishops[i]->y == (str[2]-'1')))
									{
										makemove(bishops[i]->x, bishops[i]->y,
											(str[3]-'a'), (str[4]-'1'), false, 0);
										mov++;
										return;

									}
								}
							}
						}
						else
						{
							if( extraBlackBishops )
							{
								vector<MyChessPiece*> bishops;
								for( int i = 8; i < 16; i++ )
								{
									if(blackPcs[i]->type == Bishop)
									{
										if(!blackPcs[i]->bCaptured)
											bishops.push_back(blackPcs[i]);
									}
								}

								if(!blackPcs[2]->bCaptured)
									bishops.push_back(blackPcs[2]);

								if(!blackPcs[5]->bCaptured)
									bishops.push_back(blackPcs[5]);
						
								for( int i = 0; i < bishops.size(); i++ )
								{
									if( (bishops[i]->x == (str[1]-'a')) &&
										(bishops[i]->y == (str[2]-'1')))
									{
										makemove(bishops[i]->x, bishops[i]->y,
											(str[3]-'a'), (str[4]-'1'), false, 0);
										mov++;
										return;

									}
								}
							}
						}
					}
		
				
		
				}



			}
			else if( str[1] >= '1' && str[1] <= '8' )
			{
				//cout<< "piece type: "<< str[0]<<" on rank: "<<str[1] ;
				if( str[2] == 'x' )
				{
					switch( type )
					{
					case Queen:
						{
							if( mov == WHITE )
							{
								if( extraWhiteQueens )
								{
									vector<MyChessPiece*> queens;
									for( int i = 8; i < 16; i++ )
									{
										if(whitePcs[i]->type == Queen)
										{
											if(!whitePcs[i]->bCaptured)
												queens.push_back(whitePcs[i]);
										}
									}

									if(!whitePcs[3]->bCaptured)
										queens.push_back(whitePcs[3]);

						
									for( int i = 0; i < queens.size(); i++ )
									{
										// for Qb6 we need a Queen that is on a non-blocked file
										// or diagonal ... so first we test to see that it
										// can be accessed
										int abs1 = abs(file-queens[i]->x);
										int abs2 = abs(rank-queens[i]->y);

										if( queens[i]->y != (str[1]-'1'))
											continue;

										if( abs1 == abs2 )
										{
											if( IsDiagonalSpanClear(queens[i]->x,
																	queens[i]->y,
																	file,
																	rank))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->x == file)
										{
											if( !isFileSpanBlocked(file, queens[i]->y, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->y == rank)
										{
											if( !isRankSpanBlocked(queens[i]->x, file, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}

										}
									}

								}
							}
							else
							{
								if( extraBlackQueens )
								{
									vector<MyChessPiece*> queens;
									for( int i = 8; i < 16; i++ )
									{
										if(blackPcs[i]->type == Queen)
										{
											if(!blackPcs[i]->bCaptured)
												queens.push_back(blackPcs[i]);
										}
									}

									if(!blackPcs[3]->bCaptured)
										queens.push_back(blackPcs[3]);

						
									for( int i = 0; i < queens.size(); i++ )
									{
										// for Qb6 we need a Queen that is on a non-blocked file
										// or diagonal ... so first we test to see that it
										// can be accessed
										int abs1 = abs(file-queens[i]->x);
										int abs2 = abs(rank-queens[i]->y);

										if( queens[i]->y != (str[1]-'1'))
											continue;

										if( abs1 == abs2 )
										{
											if( IsDiagonalSpanClear(queens[i]->x,
																	queens[i]->y,
																	file,
																	rank))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->x == file)
										{
											if( !isFileSpanBlocked(file, queens[i]->y, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}
										}
										else if(queens[i]->y == rank)
										{
											if( !isRankSpanBlocked(queens[i]->x, file, rank ))
											{
												makemove(queens[i]->x,
														 queens[i]->y,
														 file, rank, false, 0);
												mov++;
												return;
											}

										}
									}

								}
							}
							
						
						break;
						}
					case Knight:
						{
							if( mov == WHITE )
							{
								if( extraWhiteKnights )
								{
									vector<MyChessPiece*> knights;
									for( int i = 8; i < 16; i++ )
									{
										if(whitePcs[i]->type == Knight)
										{
											if(!whitePcs[i]->bCaptured)
												knights.push_back(whitePcs[i]);
										}
									}

									if(!whitePcs[1]->bCaptured)
										knights.push_back(whitePcs[1]);

									if(!whitePcs[6]->bCaptured)
										knights.push_back(whitePcs[6]);

									for( int i = 0; i < knights.size(); i++ )
									{
										if( knights[i]->y == (str[1]-'1') )
										{
											if( rank == knights[i]->y + 1 ||
												rank == knights[i]->y - 1)
											{
												if( file == knights[i]->x + 2 ||
													file == knights[i]->x - 2 )
												{
													//sq.file = knights[i]->x;
													//sq.rank = knights[i]->y;
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
											else if ( rank == knights[i]->y + 2 ||
													  rank == knights[i]->y - 2)
											{
												if( file == knights[i]->x + 1 ||
													file == knights[i]->x - 1 )
												{
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
										}
									}
								}
								else
								{
									MyChessPiece *n1 = whitePcs[1];
									MyChessPiece *n2 = whitePcs[6];
									if( n1->y == str[1]-'1')
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->y == str[1]-'1' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							else
							{
								if( extraBlackKnights )
								{
									vector<MyChessPiece*> knights;
									for( int i = 8; i < 16; i++ )
									{
										if(blackPcs[i]->type == Knight)
										{
											if(!blackPcs[i]->bCaptured)
												knights.push_back(blackPcs[i]);
										}
									}

									if(!blackPcs[1]->bCaptured)
										knights.push_back(blackPcs[1]);

									if(!blackPcs[6]->bCaptured)
										knights.push_back(blackPcs[6]);

									for( int i = 0; i < knights.size(); i++ )
									{
										if( knights[i]->y == (str[1]-'1') )
										{
											if( rank == knights[i]->y + 1 ||
												rank == knights[i]->y - 1)
											{
												if( file == knights[i]->x + 2 ||
													file == knights[i]->x - 2 )
												{
													//sq.file = knights[i]->x;
													//sq.rank = knights[i]->y;
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
											else if ( rank == knights[i]->y + 2 ||
													  rank == knights[i]->y - 2)
											{
												if( file == knights[i]->x + 1 ||
													file == knights[i]->x - 1 )
												{
													makemove(knights[i]->x, knights[i]->y,
														file, rank, false, 0);
													mov++;
													return;
												}
											}
										}
									}
								}
								else
								{
									MyChessPiece *n1 = blackPcs[1];
									MyChessPiece *n2 = blackPcs[6];
									if( n1->y == str[1]-'1')
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->y == str[1]-'1' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							break;
						}
					case Rook:
						{
							if( mov == WHITE )
							{
								if( extraWhiteRooks )
								{
									vector<MyChessPiece*> rooks;
									
									for( int i = 8; i < 16; i++)
									{
										if( whitePcs[i]->type == Rook )
										{
											if( !whitePcs[i]->bCaptured )
												rooks.push_back(whitePcs[i]);
										}
									}
									if( !whitePcs[0]->bCaptured )
										rooks.push_back(whitePcs[0]);

									if( !whitePcs[7]->bCaptured )
										rooks.push_back(whitePcs[7]);
									

									for( int i = 0; i < rooks.size(); i++ )
									{
										if( rooks[i]->y == str[1]-'1' )
										{
											if( file == str[1] - '1' )
											{
												if( !isRankSpanBlocked(rooks[i]->x, file, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}

											}
											else
											{
												if( !isFileSpanBlocked(rooks[i]->x, rooks[i]->y, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}
											}
											
										}
										
						
									}
								}
								else
								{
									MyChessPiece *n1 = whitePcs[0];
									MyChessPiece *n2 = whitePcs[7];
									if( n1->y == str[1]-'1')
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->y == str[1]-'1' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							else
							{
								if( extraBlackRooks )
								{
									vector<MyChessPiece*> rooks;
									
									for( int i = 8; i < 16; i++)
									{
										if( blackPcs[i]->type == Rook )
										{
											if( !blackPcs[i]->bCaptured )
												rooks.push_back(blackPcs[i]);
										}
									}
									if( !blackPcs[0]->bCaptured )
										rooks.push_back(blackPcs[0]);

									if( !blackPcs[7]->bCaptured )
										rooks.push_back(blackPcs[7]);
									

									for( int i = 0; i < rooks.size(); i++ )
									{
										if( rooks[i]->y == str[1]-'1' )
										{
											if( file == rooks[i]->y )
											{
												//if( !isRankSpanBlocked(rooks[i]->x, file, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}

											}
											else
											{
												if( !isFileSpanBlocked(rooks[i]->x, rooks[i]->y, rank ))
												{
													makemove(rooks[i]->x, rooks[i]->y, file, rank, false,0);
													mov++;
													return;
												}
											}
											
										}
										
						
									}
								}
								else
								{
									MyChessPiece *n1 = blackPcs[0];
									MyChessPiece *n2 = blackPcs[7];
									if( n1->y == str[1]-'1')
									{
										this->makemove(n1->x, n1->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									else if( n2->y == str[1]-'1' )
									{
										this->makemove(n2->x, n2->y, (int)(str[3]-'a'), (int)(str[4]-'1'),false,0);
									}
									mov++;
									return;
								}
							}
							break;
						}
					}
					//cout << "captures ...:"<<endl;
					//cout << "piece on file: "<< str[3]<<", and rank: "<<str[4]<<endl;
				}
				else 
				{
				//		cout<< "error in str.length() == 4"<<endl;
				}
			}
		}
		else if( str[1] != 'x' )
		{
			//cout << "Error, returning" <<endl;
			return;
		}
	}

	// promote with capture

	if( len == 6)
	{
		if( str[0] >= 'a' && str[0] <= 'h' )
		{
			//cout<< "Pawn Move, File = " << str[0]-'a' << endl;
			if( str[1] == 'x' )
			{
				int file = (str[2]-'a');
				int rank = (str[3]-'1');
				int source_file = (str[0]-'a');
				if( str[2] >= 'a' && str[2] <= 'h' )
				{

					{
						// probably promotion
						if( str[4] == '=' )
						{
							switch(str[5])
							{
								case 'Q':
									{
									//	cout<< "promote to Queen"<<endl; 
										if( mov == WHITE )
										{
											makemove( source_file, 6, file,7, true, 0);
											mov++;
											return;
										}							
										else
										{
											makemove( source_file, 1, file,0, true, 0);
											mov++;
											return;
										}
										break;
									}
								case 'R':
									{
									//	cout<< "promote to rook"<<endl; 
										if( mov == WHITE )
										{
											makemove( source_file, 6, file,7, true, 3);
											mov++;
											return;
										}							
										else
										{
											makemove( source_file, 1, file,0, true, 3);
											mov++;
											return;
										}
										break;
									}
								case 'N':
									{
									//	cout<< "promote to knight"<<endl; 
										if( mov == WHITE )
										{
											makemove( source_file, 6, file,7, true, 2);
											mov++;
											return;
										}							
										else
										{
											makemove( source_file, 1, file,0, true, 2);
											mov++;
											return;
										}
										break;
									}
								case 'B':
									{
									//	cout<< "promote to bishop"<<endl; 
										if( mov == WHITE )
										{
											makemove( source_file, 6, file,7, true, 1);
											mov++;
											return;
										}							
										else
										{
											makemove( source_file, 1, file,0, true, 1);
											mov++;
											return;
										}
										break;
									}
							}
						}
					}
				}
	//			else cout<< "Error in promote with capture."<<endl;
			}
		}
		else
		{
			if( str[0] == 'Q' )
			{
				// game of many Queens ...
				if( mov == WHITE )
				{
					if( extraWhiteQueens )
					{
						vector<MyChessPiece*> queens;
						for( int i = 8; i < 16; i++ )
						{
							if(whitePcs[i]->type == Queen)
							{
								if(!whitePcs[i]->bCaptured)
									queens.push_back(whitePcs[i]);
							}
						}

						if(!whitePcs[3]->bCaptured)
							queens.push_back(whitePcs[3]);

						
						for( int i = 0; i < queens.size(); i++ )
						{
							if( (queens[i]->x == (str[1]-'a')) &&
								(queens[i]->y == (str[2]-'1')))
							{
								makemove(queens[i]->x, queens[i]->y,
									(str[4]-'a'), (str[5]-'1'), false, 0);
								mov++;
								return;

							}
						}
					}
				}
				else
				{
					if( extraBlackQueens )
					{
						vector<MyChessPiece*> queens;
						for( int i = 8; i < 16; i++ )
						{
							if(blackPcs[i]->type == Queen)
							{
								if(!blackPcs[i]->bCaptured)
									queens.push_back(blackPcs[i]);
							}
						}

						if(!blackPcs[3]->bCaptured)
							queens.push_back(blackPcs[3]);

						
						for( int i = 0; i < queens.size(); i++ )
						{
							if( (queens[i]->x == (str[1]-'a')) &&
								(queens[i]->y == (str[2]-'1')))
							{
								makemove(queens[i]->x, queens[i]->y,
									(str[4]-'a'), (str[5]-'1'), false, 0);
								mov++;
								return;

							}
						}
					}
				}
			}
			if( str[0] == 'B' )
			{
				// game of many Queens ...
				if( mov == WHITE )
				{
					if( extraWhiteBishops )
					{
						vector<MyChessPiece*> bishops;
						for( int i = 8; i < 16; i++ )
						{
							if(whitePcs[i]->type == Bishop)
							{
								if(!whitePcs[i]->bCaptured)
									bishops.push_back(whitePcs[i]);
							}
						}

						if(!whitePcs[2]->bCaptured)
							bishops.push_back(whitePcs[2]);

						if(!whitePcs[5]->bCaptured)
							bishops.push_back(whitePcs[5]);

						
						for( int i = 0; i < bishops.size(); i++ )
						{
							if( (bishops[i]->x == (str[1]-'a')) &&
								(bishops[i]->y == (str[2]-'1')))
							{
								makemove(bishops[i]->x, bishops[i]->y,
									(str[4]-'a'), (str[5]-'1'), false, 0);
								mov++;
								return;

							}
						}
					}
				}
				else
				{
					if( extraBlackBishops )
					{
						vector<MyChessPiece*> bishops;
						for( int i = 8; i < 16; i++ )
						{
							if(blackPcs[i]->type == Bishop)
							{
								if(!blackPcs[i]->bCaptured)
									bishops.push_back(blackPcs[i]);
							}
						}

						if(!blackPcs[2]->bCaptured)
							bishops.push_back(blackPcs[2]);

						if(!blackPcs[5]->bCaptured)
							bishops.push_back(blackPcs[5]);

						
						for( int i = 0; i < bishops.size(); i++ )
						{
							if( (bishops[i]->x == (str[1]-'a')) &&
								(bishops[i]->y == (str[2]-'1')))
							{
								makemove(bishops[i]->x, bishops[i]->y,
									(str[4]-'a'), (str[5]-'1'), false, 0);
								mov++;
								return;

							}
						}
					}
				}
			}
		}
	}
	mov++;
	return;

	//cout<<"*****************************"<<endl<<endl;
	}




	
	void LoadTextures()
	{
		/*textures[0] = new Drawable(new Image(wxT("images/black pawn.png")));
		textures[1] = new Drawable(new Image(wxT("images/black knight.png")));
		textures[2] = new Drawable(new Image(wxT("images/black bishop.png")));
		textures[3] = new Drawable(new Image(wxT("images/black rook.png")));
		textures[4] = new Drawable(new Image(wxT("images/black Queen.png")));
		textures[5] = new Drawable(new Image(wxT("images/black king2.png")));

		textures[6] = new Drawable(new Image(wxT("images/white pawn.png")));
		textures[7] = new Drawable(new Image(wxT("images/white knight.png")));
		textures[8] = new Drawable(new Image(wxT("images/white bishop.png")));
		textures[9] = new Drawable(new Image(wxT("images/white rook.png")));
		textures[10] = new Drawable(new Image(wxT("images/white queen.png")));
		textures[11] = new Drawable(new Image(wxT("images/white king.png")));
		*/
		/*
textures[0] = new Drawable(new Image(wxT("images/PieceThemes/Internet/bp.png")));
		textures[1] = new Drawable(new Image(wxT("images/PieceThemes/Internet/bn.png")));
		textures[2] = new Drawable(new Image(wxT("images/PieceThemes/Internet/bb.png")));
		textures[3] = new Drawable(new Image(wxT("images/PieceThemes/Internet/br.png")));
		textures[4] = new Drawable(new Image(wxT("images/PieceThemes/Internet/bq.png")));
		textures[5] = new Drawable(new Image(wxT("images/PieceThemes/Internet/bk.png")));

		textures[6] = new Drawable(new Image(wxT("images/PieceThemes/Internet/wp.png")));
		textures[7] = new Drawable(new Image(wxT("images/PieceThemes/Internet/wn.png")));
		textures[8] = new Drawable(new Image(wxT("images/PieceThemes/Internet/wb.png")));
		textures[9] = new Drawable(new Image(wxT("images/PieceThemes/Internet/wr.png")));
		textures[10] = new Drawable(new Image(wxT("images/PieceThemes/Internet/wq.png")));
		textures[11] = new Drawable(new Image(wxT("images/PieceThemes/Internet/wk.png")));
		*/

		for( int i = 0; i < 12; i++ )
			svgTextures[i] = new wxBitmap;

		/*svgTextures[0]->LoadFile("images/PieceThemes/Internet/bp.png", wxBITMAP_TYPE_PNG);
		svgTextures[1]->LoadFile("images/PieceThemes/Internet/bn.png", wxBITMAP_TYPE_PNG);
		svgTextures[2]->LoadFile("images/PieceThemes/Internet/bb.png", wxBITMAP_TYPE_PNG);
		svgTextures[3]->LoadFile("images/PieceThemes/Internet/br.png", wxBITMAP_TYPE_PNG);
		svgTextures[4]->LoadFile("images/PieceThemes/Internet/bq.png", wxBITMAP_TYPE_PNG);
		svgTextures[5]->LoadFile("images/PieceThemes/Internet/bk.png", wxBITMAP_TYPE_PNG);
		
		svgTextures[6]->LoadFile("images/PieceThemes/Internet/wp.png", wxBITMAP_TYPE_PNG);
		svgTextures[7]->LoadFile("images/PieceThemes/Internet/wn.png", wxBITMAP_TYPE_PNG);
		svgTextures[8]->LoadFile("images/PieceThemes/Internet/wb.png", wxBITMAP_TYPE_PNG);
		svgTextures[9]->LoadFile("images/PieceThemes/Internet/wr.png", wxBITMAP_TYPE_PNG);
		svgTextures[10]->LoadFile("images/PieceThemes/Internet/wq.png", wxBITMAP_TYPE_PNG);
		//svgTextures[10]->LoadFile("images/White_Queen_Test.png", wxBITMAP_TYPE_PNG); // TEST
		svgTextures[11]->LoadFile("images/PieceThemes/Internet/wk.png", wxBITMAP_TYPE_PNG);
		*/
	}

	void ChangeTextures(string path)
	{
		for( int i = 0; i < 12; i++ )
		{
			delete svgTextures[i];
			svgTextures[i] = NULL;
		}
		string bp = path+ "bp.png";
		string bn = path+ "bn.png";
		string bb = path+ "bb.png";
		string br = path+ "br.png";
		string bq = path+ "bq.png";
		string bk = path+ "bk.png";

		string wp = path+ "wp.png";
		string wn = path+ "wn.png";
		string wb = path+ "wb.png";
		string wr = path+ "wr.png";
		string wq = path+ "wq.png";
		string wk = path+ "wk.png";

		for( int i = 0; i < 12; i++ )
			svgTextures[i] = new wxBitmap;

		svgTextures[0]->LoadFile(bp.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[1]->LoadFile(bn.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[2]->LoadFile(bb.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[3]->LoadFile(br.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[4]->LoadFile(bq.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[5]->LoadFile(bk.c_str(), wxBITMAP_TYPE_PNG);
		
		svgTextures[6]->LoadFile(wp.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[7]->LoadFile(wn.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[8]->LoadFile(wb.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[9]->LoadFile(wr.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[10]->LoadFile(wq.c_str(), wxBITMAP_TYPE_PNG);
		svgTextures[11]->LoadFile(wk.c_str(), wxBITMAP_TYPE_PNG);


	}


	bool SquareHasPiece( int i, int j, int *id )
	{
		if( i >= 0 && 
			i < 8 && 
			j >=0 && 
			j < 8 && 
			board[i][j]!=0 )
		{
			*id = board[i][j];
			//*id = cboard[j*8+i];
			//board[i][j]=0;
			return true;
		}
		else
		{
			return false;
		}
	}

	MyChessPiece* FindPieceForSquare(int x, int y)
	{
		if( mov == 0 || mov == 2 )
		{
			for( int i = 0; i < 16; i++ )
			{
				if( whitePcs[i]->x == x )
				{
					if( whitePcs[i]->y == y )
						if( !whitePcs[i]->bCaptured )
							return whitePcs[i];
				}
			}
		}
		else
		{

			for( int i = 0; i < 16; i++ )
			{
				if( blackPcs[i]->x == x )
				{
					if( blackPcs[i]->y == y )
						if( !blackPcs[i]->bCaptured )
							return blackPcs[i];
				}
			}
		}

		return NULL;

	}



	bool IsMoveValid(MyChessMove* M, bool* promote, bool* pinned)
	{
		MyChessPiece* PC = FindPieceForSquare(M->from.file, M->from.rank);

		if( PC == NULL )
			return false;

		bool almost = false;
		switch( PC->type )
		{
		case Bishop: 
			{
				almost = CanBishopReachSquare(PC, M->to.file, M->to.rank);
				break;
			}
		case Knight: 
			{
				almost = CanKnightReachSquare(PC, M->to.file, M->to.rank);
				break;
			}
		case Rook:
			{
				almost = CanRookReachSquare(PC, M->to.file, M->to.rank);
				break;
			}
		case Queen:
			{
				almost = CanQueenReachSquare(PC, M->to.file, M->to.rank);
				break;
			}

		case King:
			{
				almost = CanKingReachSquare(PC, M->to.file, M->to.rank );
				break;
			}
		case Pawn:
			{
				almost = CanPawnReachSquare(PC, M->to.file, M->to.rank ); 
				if( M->to.rank == 7 || M->to.rank == 0 )
					*promote = true;
				break;
			}
		}

		if( isPiecePinned(PC) && (almost == true) )
		{
			if( (PC->PinFromX == M->to.file) && 
				(PC->PinFromY == M->to.rank))
			{
				almost = true;
			}
			else
			{
				if( PC->color == WHITE )
				{
					if( PC->PinFromX == whitePcs[4]->x )
					{
						if( M->to.file == PC->PinFromX )
						{
							if( whitePcs[4]->y < PC->PinFromY )
							{
								if( (whitePcs[4]->y < M->to.rank) &&
									 M->to.rank < PC->PinFromY)
								{
									almost = true; 
								}
								else almost = false;
							}
							else if( whitePcs[4]->y > PC->PinFromY )
							{
								if( (whitePcs[4]->y > M->to.rank) &&
									 M->to.rank > PC->PinFromY)
								{
									almost = true; 
								}
								else almost = false;
							}
							else almost = false;
						}
						else almost = false;
					}
					else if( PC->PinFromY == whitePcs[4]->y)
					{
						if( M->to.rank == PC->PinFromY )
						{
							if( whitePcs[4]->x < PC->PinFromX )
							{
								if( (whitePcs[4]->x < M->to.file) &&
									 M->to.file < PC->PinFromX)
								{
									almost = true; 
								}
								else almost = false;
							}
							else if( whitePcs[4]->x > PC->PinFromX )
							{
								if( (whitePcs[4]->x > M->to.file) &&
									 M->to.file > PC->PinFromX)
								{
									almost = true; 
								}
								else almost = false;
							}
							else almost = false;
						}
						else almost = false;
					}
					else if( abs(PC->PinFromX - whitePcs[4]->x) ==
						abs(PC->PinFromY - whitePcs[4]->y ))
					{
						int dirX = PC->PinFromX - whitePcs[4]->x > 0 ? 1: -1;
						int dirY = PC->PinFromY - whitePcs[4]->y > 0 ? 1: -1;

						if( abs(PC->PinFromX - M->to.file) ==
							abs(PC->PinFromY - M->to.rank))
						{
							int dirX2 = PC->PinFromX - M->to.file > 0?1:-1;
							int dirY2 = PC->PinFromY - M->to.rank > 0?1:-1;

							if( (dirX == dirX2) && (dirY == dirY2) )
							{
								almost = true;
							}
							else almost = false;
						}
						else almost = false;
					}
					else almost = false;
				}
				else
				{
					if( PC->PinFromX == blackPcs[4]->x )
					{
						if( M->to.file == PC->PinFromX )
						{
							if( blackPcs[4]->y < PC->PinFromY )
							{
								if( (blackPcs[4]->y < M->to.rank) &&
									 M->to.rank < PC->PinFromY)
								{
									almost = true; 
								}
								else almost = false;
							}
							else if( blackPcs[4]->y > PC->PinFromY )
							{
								if( (blackPcs[4]->y > M->to.rank) &&
									 M->to.rank > PC->PinFromY)
								{
									almost = true; 
								}
								else almost = false;
							}
							else almost = false;
						}
						else almost = false;
					}
					else if( PC->PinFromY == blackPcs[4]->y)
					{
						if( M->to.rank == PC->PinFromY )
						{
							if( blackPcs[4]->x < PC->PinFromX )
							{
								if( (blackPcs[4]->x < M->to.file) &&
									 M->to.file < PC->PinFromX)
								{
									almost = true; 
								}
								else almost = false;
							}
							else if( blackPcs[4]->x > PC->PinFromX )
							{
								if( (blackPcs[4]->x > M->to.file) &&
									 M->to.file > PC->PinFromX)
								{
									almost = true; 
								}
								else almost = false;
							}
							else almost = false;
						}
						else almost = false;
					}
					else if( abs(PC->PinFromX - blackPcs[4]->x) ==
							 abs(PC->PinFromY - blackPcs[4]->y ))
					{
						int dirX = PC->PinFromX - blackPcs[4]->x > 0 ? 1: -1;
						int dirY = PC->PinFromY - blackPcs[4]->y > 0 ? 1: -1;

						if( abs(PC->PinFromX - M->to.file) ==
							abs(PC->PinFromY - M->to.rank))
						{
							int dirX2 = PC->PinFromX - M->to.file > 0?1:-1;
							int dirY2 = PC->PinFromY - M->to.rank > 0?1:-1;

							if( (dirX == dirX2) && (dirY == dirY2) )
							{
								almost = true;
							}
							else almost = false;
						}
						else almost = false;
					}
					else almost = false;
				}
				*pinned = true;
			//	almost = false;
			}
		}

		// is king in check. does move intercede check.

		int fromX, fromY;
		vector<MyChessPiece*> attackers;
		if( almost == true )
		{
			if( PC->color == WHITE )
			{
				GetPiecesAttackingSquare(BLACK, whitePcs[4]->x, whitePcs[4]->y, &attackers );
				if( attackers.size() > 0 )
				{
					if( attackers.size() >= 2)
					{
						if( PC->type != King )
						{
							almost = false;
						}
					
					}
					else if( attackers.size() == 1 )
					{
						MyChessPiece *attacker = attackers[0];
						if( PC->type == King )
						{
							almost = true;
						}
						else if( attacker->x == M->to.file && 
							attacker->y == M->to.rank )
						{
							almost = true;
						}
						else if( attacker->x == whitePcs[4]->x )
						{
							if( attacker->y > whitePcs[4]->y )
							{
								if( M->to.rank > whitePcs[4]->y &&
									M->to.rank < attacker->y &&
									M->to.file == attacker->x)
								{
									almost = true;
								}
							
								else almost = false;
							}
							else
							{
								if( M->to.rank < whitePcs[4]->y &&
									M->to.rank > attacker->y &&
									M->to.file == attacker->x)
								{
									almost = true;
								}
								else almost = false;
							}
						}
						else if( attacker->y == whitePcs[4]->y)
						{
							if( attacker->x > whitePcs[4]->x )
							{
								if( M->to.file > whitePcs[4]->x &&
									M->to.file < attacker->x &&
									M->to.rank == attacker->y)
								{
									almost = true;
								}
								else almost = false;
							}
							else
							{
								if( M->to.file < whitePcs[4]->x &&
									M->to.file > attacker->x &&
									M->to.rank == attacker->y )
								{
									almost = true;
								}
								else almost = false;
							}
						}
						else if( abs(attacker->x-whitePcs[4]->x) == abs(attacker->y - whitePcs[4]->y) )
						{
							if( attacker->x > whitePcs[4]->x && 
								attacker->y > whitePcs[4]->y )
							{
								if( M->to.file < attacker->x &&
									M->to.rank < attacker->y &&
									M->to.file > whitePcs[4]->x &&
									M->to.rank > whitePcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}
							else if( attacker->x > whitePcs[4]->x &&
								attacker->y < whitePcs[4]->y )
							{
								if( M->to.file < attacker->x &&
									M->to.rank > attacker->y &&
									M->to.file > whitePcs[4]->x &&
									M->to.rank < whitePcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}
							else if( attacker->x < whitePcs[4]->x &&
								attacker->y > whitePcs[4]->y )
							{
								if( M->to.file > attacker->x &&
									M->to.rank < attacker->y &&
									M->to.file < whitePcs[4]->x &&
									M->to.rank > whitePcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}
							else if( attacker->x < whitePcs[4]->x && 
								attacker->y < whitePcs[4]->y )
							{
								if( M->to.file > attacker->x &&
									M->to.rank > attacker->y &&
									M->to.file < whitePcs[4]->x &&
									M->to.rank < whitePcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}

						}
						else
						{
							// knight
							if( PC->type != King )
							{
								if( M->to.file != attacker->x )
									almost = false;
								else if( M->to.rank != attacker->y )
									almost = false;
							}
						}
					}

				
				}
			}
			else
			{
				GetPiecesAttackingSquare(WHITE, blackPcs[4]->x, blackPcs[4]->y, &attackers );
				if( attackers.size() > 0 )
				{
					if( attackers.size() >= 2)
					{
						if( PC->type != King )
						{
							almost = false;
						}
					
					}
					else if( attackers.size() == 1 )
					{
						MyChessPiece *attacker = attackers[0];
						if( PC->type == King )
						{
							almost = true;
						}
						else if( attacker->x == M->to.file && 
							attacker->y == M->to.rank )
						{
							almost = true;
						}
						else if( attacker->x == blackPcs[4]->x )
						{
							if( attacker->y > blackPcs[4]->y )
							{
								if( M->to.rank > blackPcs[4]->y &&
									M->to.rank < attacker->y &&
									M->to.file == attacker->x)
								{
									almost = true;
								}
								else almost = false;
							}
							else
							{
								if( M->to.rank < blackPcs[4]->y &&
									M->to.rank > attacker->y &&
									M->to.file == attacker->x)
								{
									almost = true;
								}
								else almost = false;
							}
						}
						else if( attacker->y == blackPcs[4]->y)
						{
							if( attacker->x > blackPcs[4]->x )
							{
								if( M->to.file > blackPcs[4]->x &&
									M->to.file < attacker->x &&
									M->to.rank == attacker->y)
								{
									almost = true;
								}
								else almost = false;
							}
							else
							{
								if( M->to.file < blackPcs[4]->x &&
									M->to.file > attacker->x && 
									M->to.rank == attacker->y)
								{
									almost = true;
								}
								else almost = false;
							}
						}
						else if( abs(attacker->x-blackPcs[4]->x) == abs(attacker->y - blackPcs[4]->y) )
						{
							if( attacker->x > blackPcs[4]->x && 
								attacker->y > blackPcs[4]->y )
							{
								if( M->to.file < attacker->x &&
									M->to.rank < attacker->y &&
									M->to.file > blackPcs[4]->x &&
									M->to.rank > blackPcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}
							else if( attacker->x > blackPcs[4]->x &&
								attacker->y < blackPcs[4]->y )
							{
								if( M->to.file < attacker->x &&
									M->to.rank > attacker->y &&
									M->to.file > blackPcs[4]->x &&
									M->to.rank < blackPcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}
							else if( attacker->x < blackPcs[4]->x &&
								attacker->y > blackPcs[4]->y )
							{
								if( M->to.file > attacker->x &&
									M->to.rank < attacker->y &&
									M->to.file < blackPcs[4]->x &&
									M->to.rank > blackPcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}
							else if( attacker->x < blackPcs[4]->x && 
								attacker->y < blackPcs[4]->y )
							{
								if( M->to.file > attacker->x &&
									M->to.rank > attacker->y &&
									M->to.file < blackPcs[4]->x &&
									M->to.rank < blackPcs[4]->y )
								{
									if( abs(attacker->x - M->to.file) == abs( attacker->y - M->to.rank ))
									{
										almost = true;
									}
									else almost = false;
								}
								else almost= false;
							}

						}
						else
						{
							// knight
							if( PC->type != King )
							{
								if( M->to.file != attacker->x )
									almost = false;
								else if( M->to.rank != attacker->y )
									almost = false;
							}
						}
					}
		
				}
			}
		}


		if( almost )
		{
			switch (board[ M->to.file][ M->to.rank])
			{
			case 0: return true;
			case wKing:return false;
			case bKing:return false;
			case wPawn:
			case wRook:
			case wKnight:
			case wBishop:
			case wQueen:
				return PC->color == BLACK;
			case bPawn:
			case bRook:
			case bKnight:
			case bBishop:
			case bQueen:
				return PC->color == WHITE;
			}
		}

		return false;
	}

	

};




#endif