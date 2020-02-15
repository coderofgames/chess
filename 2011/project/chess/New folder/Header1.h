#ifndef PGN_LOAD_H
#define PGN_LOAD_H

#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "ChessSquare.h"
#include "PGKey.h"
//#include <ofstream>



#include <string>
using namespace std;


enum FORMAT_ERROR_CODES
{
	INCORRECT_LENGTH,
	FORMAT_ERROR,
	PASSED_TEST
};

#ifndef WHITE
#define WHITE 0
#define BLACK 1
#endif


class Line; // from notation canvas


class VarMoveNode
{
public:

	VarMoveNode()
	{
		whitesMove = false;
		fen = "";
		pgnMove = "";
	}

	VarMoveNode(string &moveStr, MyChessMove& mv, string& Fen, int white_black, string movNum )
	{
		if( white_black == 1 )
			whitesMove = true;
		else whitesMove = false;
		//whitesMove = false;
		fen = Fen;
		pgnMove = moveStr;
		move = mv;
		info = movNum;
	}

	~VarMoveNode()
	{

	}


	string info;
	string pgnMove;
	MyChessMove move;
	string fen;
	bool whitesMove;

};


namespace PGN {

template<typename Operator>
void tokenize(Operator& op, const char* input, const char* delimiters) {
  const char* s = input;
  const char* e = s;
  while (*e != 0) {
    e = s;
    while (*e != 0 && strchr(delimiters, *e) == 0) ++e;
    if (e - s > 0) {
      op(s, e - s);
    }
    s = e + 1;
  }
}


template<typename Operator>
void tokenize2(Operator& op, const char* input, const char* delim)
{
  const char* s = input;
  const char* e = s;
  while (*e != 0) {
    e = s;
    while (*e != 0 && strchr(delim, *e) == 0) 
	{
		++e;
	}
	if( *e == '(' || *e == ')' || *e == '{' || *e == '}')
	{
		if( e-s > 0)
		{
			op(s, e - s);
			op(e, 1);
			
		}
		else
			op(e, 1);

		s = e + 1;
	}
	else
	{
		if (e - s > 0) {
			op(s, e - s);
		}
		s = e + 1;
	}
  }
}


class Appender : public std::vector<std::string> {
public:
  void operator() (const char* s, unsigned length) { 
    this->push_back(std::string(s,length));
  }
};

class HeaderElem
{
public:
	string data;

};

class Move;

class Variation
{
public:
	Variation(){
		after_whites_move = false;

	}

	~Variation()
	{
		vecMoves.clear();
	}
	bool after_whites_move;
	vector<Move> vecMoves;
};

class Comment
{
public:
	Comment()
	{
		after_whites_move = false;
	}
	~Comment()
	{
		text.clear();
	}

	void operator=(Comment &rhs)
	{
		after_whites_move = rhs.after_whites_move;
		for( int i = 0; i < rhs.text.size(); i++ )
			text.push_back(rhs.text[i]);
	}
	bool after_whites_move;
	//string text;
	vector<string> text;
};
//ofstream of;


class MoveInserter
{
public:

	MoveInserter( string pgn_string, int white_black, string fen, MyChessMove* epn=0,
		string epn_string="")
		
	{
		m_pgn_string = pgn_string;
		if(( white_black == 0 ) || (white_black == 2))
		{
			m_white_black = 0;
		}
		else 
		{
			m_white_black = 1;
		}
		m_fen = fen;
		if( epn )
			m_epn = *epn;

		vector<string> vec_fen;

		istringstream iss(fen);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(vec_fen));

		//int movNum = atoi(vec_fen[5].c_str());
		//movNum--;
		//char buff[4];
		//itoa(movNum, buff, 10);
		
		//	moveNum = buff;
			moveNum = vec_fen[5];

	}

	string m_fen;
	MyChessMove m_epn;
	string m_epn_string;
	string m_pgn_string;
	string moveNum;
	int m_white_black;
};


struct RenderMoveColor
{
	int r;
	int g;
	int b;
};


class Move
{
public:

	Move()
	{
		variation_count_white = 0;
		variation_count_black = 0;
		x = y = 0;
		pNext = NULL;
		pPrev = NULL;
		hasFenWhite = false;
		hasFenBlack = false;
		white ="";
		black ="";
		pVarWhite = NULL;
		pVarBlack = NULL;
		pUp = NULL;
		for( int i = 0; i < 64; i++ )
		{
			pVariationBlack[i] = NULL;
			pVariationWhite[i] = NULL;
		}

		atEndWhite = false;
		atEndBlack = false;

		displayVariations = true;
		SetRenderColor(0,0,0);
		SetRenderColorWhite(0,0,0);
		SetRenderColorBlack(0,0,0);
		visited = false;
		move_cp_white = 0;
		move_cp_black = 0;
		depthWhite = "";
		depthBlack = "";
	}

	~Move()
	{
		for( int i = 0; i < 64; i++ )
		{
			if( pVariationWhite[i] )
			{
				delete pVariationWhite[i];
				pVariationWhite[i] = NULL;
			}
			if( pVariationBlack[i] )
			{
				delete pVariationBlack[i];
				pVariationBlack[i] = NULL;
			}
		}
		
		if( pNext )
		{
			delete pNext;
			pNext = NULL;
		}

		if( pVarWhite )
		{			
			delete pVarWhite;
			pVarWhite = NULL;
		}

		if( pVarBlack )
		{
			delete pVarBlack;
			pVarBlack = NULL;
		}
	}

	void SetLine( Line* pLine )
	{
		m_line = pLine;
	}


	Move* GetVariationRoot()
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}
		return temp->pUp;
	}


	bool IsThisMainLine()
	{
		return (this->GetVariationRoot()==NULL);
	}

	string GetCurrentVariationAsString()
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			temp = temp->pPrev;
		}

		ostringstream of;
		temp->SaveToStream(of);

		return of.str();

	}

	int GetMoveCount(int &num)
	{
		Move* temp = this;

		while( temp )
		{
			if( temp->pVarWhite )
			{
				Move* temp2 = temp->pVarWhite;
				while( temp2 )
				{
					num = temp2->GetMoveCount(num);
					temp2 = temp2->pVarWhite;
				}
			}
			if( temp->pVarBlack )
			{
				Move* temp2 = temp->pVarBlack;
				while( temp2 )
				{
					num = temp2->GetMoveCount(num);
					temp2 = temp2->pVarBlack;
				}
			}
			num++;
			temp = temp->pNext;
		}

		return num;
	}

	/*int GetVariationMoveCount(int &inpt)
	{
		if( 
	}*/

	// gets the move strings from the start in inverse order
	string GetMoveStringsFromStart(int black_white)
	{
		string retVal = "";
		Move* temp = this;
		
		if( black_white != 1 )
		{
			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal += movBlack;   // = " e7e5\n";


			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );
					
					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();
				retVal += movWhite;

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( temp->pPrev == 0 )
			return retVal;

		while( temp->pPrev != NULL)
		{

			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal.insert(0, movBlack );

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}

		return retVal;
		

	}


// gets the move strings from the start in inverse order
	string GetMoveStringsFromFenSetupPosition(int black_white)
	{
		string retVal = "";
		Move* temp = this;
		
		if( black_white != 1 )
		{
			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal += movBlack;   // = " e7e5\n";


			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return retVal;
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );
					
					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();
				retVal += movWhite;

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( temp->pPrev == 0 )
			return retVal;

		while( temp->pPrev != NULL)
		{

			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal.insert(0, movBlack );

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return retVal;
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}

		return retVal;
		

	}



	int GetVariationRootColor()
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}
		
		if( temp->pUp )
		{
			Move* tpUp = temp->pUp;
			if( tpUp->pVarWhite )
			{
				while( tpUp && tpUp != temp )
				{
					tpUp = tpUp->pVarWhite;
				}
				if( tpUp )
				{
					return 0;
				}
			}
			else return 1;
		}

		return 2; // unreachable
	}


	bool MoveIsVariationRoot(Move* pMoveInVar)
	{
		Move* pVarRoot = GetVariationRoot();
		if( pMoveInVar == pVarRoot )
			return true;
		else return false;
	}



	//	1. e4 e5
	//  insert variation of e4 ... move d4
	//  x=1 y=1
	//  d4!=e4 add move
	// insert variation of black into d4 ... move d5
	//	x=1 y=0
	//  d4 d5
	// insert variation into d5 ... move c4
	// x=1 y=1
	// 
	Move* InsertMove(MoveInserter *movPgn)
	{
		if( movPgn->m_white_black == 0 )
		{
			if( x!= 0 )
			{
				if( y!= 0 )
				{
					if( pNext == NULL  )
					{
						Move* pMov = new Move;
						pMov->pPrev = this;
						
						if( this->atEndBlack )
							this->atEndBlack = false;
						pMov->atEndWhite = true;

						this->pNext = pMov;
						pMov->white = movPgn->m_pgn_string;
						pMov->x = 1;
						//if(  != "" )
						char buf[10];
						itoa(atoi(info.c_str())+1, buf, 10);
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
		
						pMov->whiteMove = movPgn->m_epn;
						return pMov;
					}
					else if( this->pNext->white == "" )
					{
						delete this->pNext;
						Move* pMov = new Move;
						pMov->pPrev = this;
						
						if( this->atEndBlack )
							this->atEndBlack = false;
						pMov->atEndWhite = true;

						this->pNext = pMov;
						pMov->white = movPgn->m_pgn_string;
						pMov->x = 1;
						//if(  != "" )
						char buf[10];
						itoa(atoi(info.c_str())+1, buf, 10);
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						pMov->whiteMove = movPgn->m_epn;
						return pMov;
					}
					else if( movPgn->m_pgn_string != this->pNext->white )
					{
						Move* pMov = pNext->getWhiteVariationForParse();
						pMov->white = movPgn->m_pgn_string;
						
						if( this->atEndBlack )
							this->atEndBlack = false;
						pMov->atEndWhite = true;
						
						pMov->x = 1;
						char buf[10];
						itoa(atoi(info.c_str()), buf, 10);
							
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
						return pMov;
					}
					else if( movPgn->m_pgn_string == this->pNext->white )
					{
						return this->pNext;
					}
					//pMov->info = string((atoi(this->info.c_str())+1));
				}
				
				else if( movPgn->m_pgn_string != this->white )
				{
					Move* pMov = getWhiteVariationForParse();
					pMov->white = movPgn->m_pgn_string;
					if( this->atEndBlack )
						this->atEndBlack = false;
					pMov->atEndWhite = true;
					
					pMov->x = 1;
					pMov->info = this->info;
					pMov->m_fenWhite = movPgn->m_fen;
					pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
					pMov->whiteMove = movPgn->m_epn;
					return pMov;
				}
				else return this;
			}
			else
			{
				x = 1;
				white = movPgn->m_pgn_string;
				this->atEndWhite = true;
				//if( this->pPrev && this->pPrev->pPrev == NULL )
				//	info = "1";
				info = movPgn->moveNum;
				m_fenWhite = movPgn->m_fen;
				zhashKeyWhite = hash_from_fen((char*)m_fenWhite.c_str());
				whiteMove = movPgn->m_epn;
				//pMove->info = this->info;
			}
			return this;
		}
		else
		{
			if( y!= 0 )
			{
				if( movPgn->m_pgn_string != this->black )
				{
					
					Move* pMov = getBlackVariationForParse();
					pMov->black = movPgn->m_pgn_string;
					if( this->atEndWhite )
						this->atEndWhite = false;
					pMov->atEndBlack = true;
					pMov->x = 1;
					pMov->y = 1;
					char buf[10];
					itoa(atoi(info.c_str()), buf, 10);
					pMov->info = buf;//this->info;//movPgn->moveNum;
					//info = 
					pMov->m_fenBlack = movPgn->m_fen;
					pMov->zhashKeyBlack = hash_from_fen((char*)pMov->m_fenBlack.c_str());
					pMov->blackMove = movPgn->m_epn;
					return pMov;
				}
			}
			else
			{
				black = movPgn->m_pgn_string;
				if( this->atEndWhite )
					this->atEndWhite = false;
				this->atEndBlack = true;
				x = 1;
				y = 1;
				m_fenBlack = movPgn->m_fen;
				zhashKeyBlack = hash_from_fen((char*)m_fenBlack.c_str());
				blackMove = movPgn->m_epn;
			}
			return this;
		}
		return this;
	}

	//	1. e4 e5
	//  insert variation of e4 ... move d4
	//  x=1 y=1
	//  d4!=e4 add move
	// insert variation of black into d4 ... move d5
	//	x=1 y=0
	//  d4 d5
	// insert variation into d5 ... move c4
	// x=1 y=1
	// 
	Move* InsertMoveToVariation(MoveInserter *movPgn)
	{
		if( movPgn->m_white_black == 0 )
		{
			if( x!= 0 )
			{
				if( y!= 0 )
				{
					if( pNext == NULL  )
					{
						Move* pMov = new Move;
						pMov->pPrev = this;
						
						if( this->atEndBlack )
							this->atEndBlack = false;
						pMov->atEndWhite = true;

						this->pNext = pMov;
						pMov->white = movPgn->m_pgn_string;
						pMov->x = 1;
						//if(  != "" )
						char buf[10];
						itoa(atoi(info.c_str())+1, buf, 10);
						pMov->info = buf;//movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
		
						pMov->whiteMove = movPgn->m_epn;
						return pMov;
					}
					else if( this->pNext->white == "" )
					{
						delete this->pNext;
						Move* pMov = new Move;
						pMov->pPrev = this;
						
						if( this->atEndBlack )
							this->atEndBlack = false;
						pMov->atEndWhite = true;

						this->pNext = pMov;
						pMov->white = movPgn->m_pgn_string;
						pMov->x = 1;
						//if(  != "" )
						char buf[10];
						itoa(atoi(info.c_str())+1, buf, 10);
						pMov->info = buf;//movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						pMov->whiteMove = movPgn->m_epn;
						return pMov;
					}
					else if( movPgn->m_pgn_string != this->pNext->white )
					{
						Move* pMov = pNext->getWhiteVariationForParse();
						pMov->white = movPgn->m_pgn_string;
						
						if( this->atEndBlack )
							this->atEndBlack = false;
						pMov->atEndWhite = true;
						
						pMov->x = 1;
						char buf[10];
						itoa(atoi(info.c_str())+1, buf, 10);
							
						pMov->info = buf;//movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
						return pMov;
					}
					else if( movPgn->m_pgn_string == this->pNext->white )
					{
						return this->pNext;
					}
					//pMov->info = string((atoi(this->info.c_str())+1));
				}
				
				else if( movPgn->m_pgn_string != this->white )
				{
					Move* pMov = getWhiteVariationForParse();
					pMov->white = movPgn->m_pgn_string;
					if( this->atEndBlack )
						this->atEndBlack = false;
					pMov->atEndWhite = true;
					
					pMov->x = 1;
					pMov->info = this->info;
					pMov->m_fenWhite = movPgn->m_fen;
					pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
					pMov->whiteMove = movPgn->m_epn;
					return pMov;
				}
				else return this;
			}
			else
			{
				x = 1;
				white = movPgn->m_pgn_string;
				this->atEndWhite = true;
				char buf[10];
				itoa(atoi(pPrev->info.c_str())+1, buf, 10);
				//if( this->pPrev && this->pPrev->pPrev == NULL )
				//	info = "1";
				info = buf;// movPgn->moveNum;
				m_fenWhite = movPgn->m_fen;
				zhashKeyWhite = hash_from_fen((char*)m_fenWhite.c_str());
				whiteMove = movPgn->m_epn;
				//pMove->info = this->info;
			}
			return this;
		}
		else
		{
			if( y!= 0 )
			{
				if( movPgn->m_pgn_string != this->black )
				{
					
					Move* pMov = getBlackVariationForParse();
					pMov->black = movPgn->m_pgn_string;
					if( this->atEndWhite )
						this->atEndWhite = false;
					pMov->atEndBlack = true;
					pMov->x = 1;
					pMov->y = 1;
					pMov->info = movPgn->moveNum;
					//info = 
					pMov->m_fenBlack = movPgn->m_fen;
					pMov->zhashKeyBlack = hash_from_fen((char*)pMov->m_fenBlack.c_str());
					pMov->blackMove = movPgn->m_epn;
					return pMov;
				}
			}
			else
			{
				black = movPgn->m_pgn_string;
				if( this->atEndWhite )
					this->atEndWhite = false;
				this->atEndBlack = true;
				x = 1;
				y = 1;
				m_fenBlack = movPgn->m_fen;
				zhashKeyBlack = hash_from_fen((char*)m_fenBlack.c_str());
				blackMove = movPgn->m_epn;
			}
			return this;
		}
		return this;
	}

	Move* InsertVariation(MoveInserter *movPgn)
	{
		if( movPgn->m_white_black == 0 )
		{
			if( x!= 0 && pNext != NULL)
			{
				Move* pMov = pNext->getWhiteVariationForParse();
				pMov->white = movPgn->m_pgn_string;
						
				if( this->atEndBlack )
					this->atEndBlack = false;
				pMov->atEndWhite = true;
						
				pMov->x = 1;
				char buf[10];
				itoa(atoi(info.c_str())+1, buf, 10);
							
				pMov->info = pNext->info;//movPgn->moveNum;
				pMov->m_fenWhite = movPgn->m_fen;
				pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
				pMov->whiteMove = movPgn->m_epn;
			//his-> = pMov;
				
				return pMov;
			}
			else if( pNext == NULL && x!=0)
			{
				pNext = new Move();
				Move* pMov = pNext;
				pMov->pPrev = this;
				pMov->white = movPgn->m_pgn_string;
						
				if( this->atEndBlack )
					this->atEndBlack = false;
				pMov->atEndWhite = true;
						
				pMov->x = 1;
				char buf[10];
				itoa(atoi(info.c_str())+1, buf, 10);
							
				pMov->info = movPgn->moveNum;
				pMov->m_fenWhite = movPgn->m_fen;
				pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
				pMov->whiteMove = movPgn->m_epn;
			//his-> = pMov;
				
				return pMov;
			}
			else if( x==0)
			{
				x = 1;
				white = movPgn->m_pgn_string;
				this->atEndWhite = true;
				//if( this->pPrev && this->pPrev->pPrev == NULL )
				//	info = "1";
				//info = movPgn->moveNum;
				m_fenWhite = movPgn->m_fen;
				zhashKeyWhite = hash_from_fen((char*)m_fenWhite.c_str());
				whiteMove = movPgn->m_epn;
				return this;
			}

		}
		else
		{
			if( y!= 0 )
			{	
				Move* pMov = getBlackVariationForParse();
				pMov->black = movPgn->m_pgn_string;
				if( this->atEndWhite )
					this->atEndWhite = false;
				pMov->atEndBlack = true;
				pMov->x = 1;
				pMov->y = 1;
				pMov->info = this->info;
				pMov->m_fenBlack = movPgn->m_fen;
				pMov->blackMove = movPgn->m_epn;
				return pMov;
			}
			else
			{
				black = movPgn->m_pgn_string;
				if( this->atEndWhite )
					this->atEndWhite = false;
				this->atEndBlack = true;
				x = 1;
				y = 1;
				m_fenBlack = movPgn->m_fen;
				blackMove = movPgn->m_epn;
				return this;
			}
		}
		return this;
	}

	Move* RemoveNode()
	{
		Move* prev = pPrev;
		pPrev->pNext = NULL;
		return pPrev;
	}

	Move* PromoteVariation(Move* sel)
	{       
		Move *temp = sel;
		if( temp->pUp == NULL )
			while( temp->pUp == NULL && temp->pPrev)
				temp = temp->pPrev;

		if( temp->pUp == NULL )
			return temp;
		
		Move* copy = temp;
		Move* Up = copy->pUp;
		Move* UpPrev = Up->pPrev;
		Move* UpNext = Up->pNext;
		Move* UpDown;

		if( temp->white != "" ) {
			UpDown = Up->pVarWhite;
			Move* tempDown= temp->pVarWhite;
			Move* insert = UpDown;
			Move* uppUp = Up->pUp;
			
			if( insert == temp )
			{
				//insert->pVarWhite = Up;
				temp->pVarWhite = Up;
			}
			else
			{
				temp->pVarWhite = UpDown;
				while(  insert->pVarWhite && insert->pVarWhite != temp )
					insert = insert->pVarWhite;

				insert->pVarWhite = Up;
			}


			/*
			Move* UppVarBlack = Up->pVarBlack;
			Move* temppVarBlack = temp->pVarBlack;

				Up->pVarBlack = temppVarBlack;
			temp->pVarBlack = UppVarBlack;
			*/
			// swap
			Up->pUp = temp;
			temp->pUp = uppUp;
			
			Up->pVarWhite = tempDown;
			temp->pPrev = UpPrev;
			if( uppUp == NULL )
				UpPrev->pNext = temp;
			Up->pPrev = temp;

			insert = temp->pVarWhite;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarWhite;
			}

			/*insert = temp->pVarBlack;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarBlack;
			}*/

		} else {
			UpDown = Up->pVarBlack;
			Move* tempDown= temp->pVarBlack;
			Move* insert = UpDown;
			Move* uppUp = Up->pUp;

			if( insert == temp )
			{
				//insert->pVarWhite = Up;
				temp->pVarBlack = Up;
			//	tempDown = temp;
			}
			else
			{
				temp->pVarBlack = UpDown;
				while(  insert->pVarBlack && insert->pVarBlack != temp )
					insert = insert->pVarBlack;

				insert->pVarBlack = Up;
			}

			temp->white = Up->white;
		//	MyChessMove mvTemp = temp->whiteMove;
			temp->whiteMove = Up->whiteMove;
			Up->white = "";

			Move* UppVarWhite = Up->pVarWhite;
			Move* temppVarWhite = temp->pVarWhite;

			string fen_up = Up->m_fenWhite;
			string fen_temp = temp->m_fenWhite;
			uint64 zhashUpWhite = Up->zhashKeyWhite;
			uint64 zhashTempWhite = temp->zhashKeyWhite;

			
			Up->m_fenWhite = fen_temp;
			Up->zhashKeyWhite = zhashTempWhite;
			temp->m_fenWhite = fen_up;
			temp->zhashKeyWhite = zhashUpWhite;
			Up->pVarWhite = temppVarWhite;
			//Up->whiteMove = mvTemp;
			temp->pVarWhite = UppVarWhite;

			// swap
			Up->pUp = temp;
			temp->pUp = uppUp;
			if( uppUp != NULL )
			{
				Move* findUpUppVarWhite = uppUp;
				while( findUpUppVarWhite->pVarWhite != Up )
					findUpUppVarWhite = findUpUppVarWhite->pVarWhite;

				findUpUppVarWhite->pVarWhite = temp;
			}
			Up->pVarBlack = tempDown;
			temp->pPrev = UpPrev;
			if( uppUp == NULL )
				UpPrev->pNext = temp;
			Up->pPrev = temp;

			insert = temp->pVarBlack;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarBlack;
			}
			insert = temp->pVarWhite;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarWhite;
			}
		}
		return Up;
	}

	bool IsThisFirstMoveInVariationWithoutNext(int white_black)
	{
		if( this->pUp != NULL && this->pNext == NULL )
		{
			if( white_black == 1 && this->white == "" && pNext == NULL)
			{
				return true;
			}
			else if( !(white_black == 1) && this->y == 0 && pNext == NULL )
			{
				return true;
			}
		}

		return false;
	}

	Move* DeleteRemainingMoves(int black_white)
	{
		if( black_white == 1 )
		{
			this->y = 0;
			this->m_fenBlack = "";
			this->black = "";
			if( pVarBlack )
			{
				delete pVarBlack;
				pVarBlack = NULL;
			}
			this->atEndWhite = true;
		}
		if( this->pNext )
		{
			delete pNext;
			pNext = NULL;

		}

		return this;
	}

	int Count(int &numMoves)
	{
		numMoves++;
		Move* temp = pVarWhite;
		while( temp )
		{
			temp->Count(numMoves);
			temp = temp->pVarWhite;
		}

		temp = pVarBlack;
		while( temp )
		{
			temp->Count(numMoves);
			temp = temp->pVarBlack;
		}

		if( pNext )
			pNext->Count(numMoves);

		return numMoves;
	}

	Move* getWhiteVariationForParse()
	{
		/*pVariationWhite[variation_count_white] = new Move();
		pVariationWhite[variation_count_white]->pPrev = this;

		return pVariationWhite[variation_count_white++];*/
		variation_count_white++;
		if( pVarWhite == NULL )
		{
			pVarWhite = new Move;
			pVarWhite->pPrev = this;
			pVarWhite->pUp = this;
			return pVarWhite;
		}
		else
		{
			Move* temp = pVarWhite;
			while( temp->pVarWhite )
			{
				temp = temp->pVarWhite;
			}
			temp->pVarWhite = new Move;
			temp->pVarWhite->pPrev = this;
			temp->pVarWhite->pUp = this;
			temp = temp->pVarWhite;
			return temp;
		}
	}

	Move* getBlackVariationForParse()
	{
		variation_count_black++;
		if( pVarBlack == NULL )
		{
			pVarBlack = new Move;
			pVarBlack->pPrev = this;
			pVarBlack->pUp = this;
		//	pVarBlack->variation_count_black = variation_count_black;
			return pVarBlack;
		}
		else
		{
			Move* temp = pVarBlack;
			while( temp->pVarBlack )
			{
				temp = temp->pVarBlack;
			}
			temp->pVarBlack = new Move;
			temp->pVarBlack->pPrev = this;
			temp->pVarBlack->pUp = this;
		//	temp->pVarBlack->variation_count_black = variation_count_black;
			temp = temp->pVarBlack;
			return temp;
		}
	}

	void Print()
	{
		/*if( annotation.after_whites_move == false )
		{
			cout<< "printing"<<endl;
			cout<< info << " w: "
				<< white <<" b: "
				<< black <<" c: ";

				for( int aa = 0; aa < annotation.text.size(); aa++)
					cout<< annotation.text[aa];
				cout << endl;
		}
		else
		{
			cout<< info << " w: "
				<< white <<" c: ";
			for( int aa = 0; aa < annotation.text.size(); aa++)
				cout<< annotation.text[aa];
					

			cout <<" b: " << black << endl;
		}*/


		cout<< info << " w: "
			<< white <<" ";
	//	of<< info << " w: "
	//		<< white <<" ";
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
				cout<< annotation_white.text[aa];
		//		of <<annotation_white.text[aa];
		
		}
		cout << " b: "	<< black << " ";
		//of << " b: "	<< black << " ";
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			cout<< annotation_black.text[aa];
			//of <<annotation_black.text[aa];
		}

		cout<<endl;
		//of <<endl;

		if( pVarWhite )
		{
				cout<< "    [variation white]"<<endl;
			//	of<< "    [variation white]"<<endl;
				pVarWhite->Print();
			//	pVarWhite = pVarWhite->pVarWhite;
				cout<<"    [end variation white]"<<endl;
			//	of<<"    [end variation white]"<<endl;
			
		}
		if( pVarBlack )
		{
				cout<< "    [variation black]"<<endl;
		//		of<< "    [variation black]"<<endl;
				pVarBlack->Print();
				cout<<"    [end variation black]"<<endl;
		//		of<<"    [end variation black]"<<endl;
			//	pVarWhite = pVarWhite->pVarWhite;
			
		}

		/*
		if( variation_count_white != 0 )
		{
			for( int i = 0; i < variation_count_white; i++ )
			{
				cout<<"		[[variation white]]"<< endl;
				of<<"		[[variation white]]"<< endl;
				while( pVariationWhite[i]->pPrev != this)
					pVariationWhite[i] = pVariationWhite[i]->pPrev;

				pVariationWhite[i]->Print();

				cout<<"		[[end variation]]"<< endl;
				of<<"		[[end variation]]"<< endl;
				
			}
		}
		if( variation_count_black != 0 )
		{
			for( int i = 0; i < variation_count_black; i++ )
			{
				cout<<"		[[variation black]]"<< endl;
				of<<"		[[variation black]]"<< endl;
				while( pVariationBlack[i]->pPrev != this)
					pVariationBlack[i] = pVariationBlack[i]->pPrev;

				//cout<< "     ";
				//of<< "     ";
				pVariationBlack[i]->Print();

				cout<<"		[[end variation]]"<< endl;
				of<<"		[[end variation]]"<< endl;

				
			}
		}*/

		if( pNext )
			pNext->Print();
	}

	void SaveNode(ofstream &of)
	{
		if( white == "" )
		{
			of<< info <<"... ";
		}
		else
		{
			of<< info <<". "<< white <<" ";
		}

		if( annotation_white.text.size() > 0 )
			of << "{ ";
		
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
			of<< annotation_white.text[aa] <<" ";
		}
		
		if( annotation_white.text.size() > 0 )
			of << " }" <<endl;;
		


		of << black << " ";
		
		if( annotation_black.text.size() > 0 )
			of << "{ "; 
		
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			of<< annotation_black.text[aa] << " ";
		}
		
		if( annotation_black.text.size() > 0 )
			of << " }" <<endl;
	}


	void SaveToFile(ofstream &of)
	{
		if( info == "" )
		{
			if( pPrev->result != "" )
				of << pPrev->result;
			return;
		}

		if( white == "" )
		{
			of<< info <<"... ";
		}
		else
		{
			of<< info <<". "<< white <<" ";
		}

		
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
			of<< annotation_white.text[aa] <<" ";
		}
		
		if( pVarWhite )
		{
			
			Move* tempVar = pVarWhite;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}


					//if( tempNext != pVarWhite )
					{
						if( tempNext->white == "" )
						{
							of<< tempNext->info <<"... ";
						}
						else
						{
							of<< tempNext->info <<". "<< tempNext->white <<" ";
						}
		
						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if((tempNext!=tempVar) && (tempNext->pVarWhite))
					{
						of << "( ";
						tempNext->pVarWhite->SaveToFile(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext!=tempVar) && (tempNext->pVarWhite))
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";
				
		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToFile(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarWhite;
				of << ") ";	
			}
			
		}

		if( black == "" )
		{
			if( result != "" )
				of << " " <<result;
			return;
		}



		if( pVarWhite )
			of << info << "... " << black <<" ";
		else 
			of << black << " ";
		
		
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			of<< annotation_black.text[aa] << " ";
		}
		

		if( pVarBlack )
		{
			
			Move* tempVar = pVarBlack;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}

					if( tempNext->white != "" )
					{
						of<< tempNext->info <<". "<< tempNext->white <<" ";
					}

					//if( tempNext != tempVar )
					{

						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if( (tempNext->white == "") )
					{
						of<< tempNext->info <<"... ";
					}

					if(tempNext!=tempVar && tempNext->pVarWhite)
					{
						of << "( ";
						tempNext->pVarWhite->SaveToFile(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext != tempVar) && (tempNext->pVarWhite) )
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";

		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext!=tempVar && tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToFile(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarBlack;
				of << ") ";
			}
				
		}

		if( pNext )
			pNext->SaveToFile(of);
	}

	void SaveToStream(ostringstream &of)
	{
		if( info == "" )
		{
			if( pPrev->result != "" )
				of << pPrev->result;
			return;
		}

		if( white == "" )
		{
			of<< info <<"... ";
		}
		else
		{
			of<< info <<". "<< white <<" ";
		}

		
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
			of<< annotation_white.text[aa] <<" ";
		}
		
		if( pVarWhite )
		{
			
			Move* tempVar = pVarWhite;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}


					//if( tempNext != pVarWhite )
					{
						if( tempNext->white == "" )
						{
							of<< tempNext->info <<"... ";
						}
						else
						{
							of<< tempNext->info <<". "<< tempNext->white <<" ";
						}
		
						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if((tempNext!=tempVar) && (tempNext->pVarWhite))
					{
						of << "( ";
						tempNext->pVarWhite->SaveToStream(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext!=tempVar) && (tempNext->pVarWhite))
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";
				
		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToStream(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarWhite;
				of << ") ";	
			}
			
		}

		if( black == "" )
		{
			if( result != "" )
				of << " " <<result;
			return;
		}



		if( pVarWhite )
			of << info << "... " << black <<" ";
		else 
			of << black << " ";
		
		
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			of<< annotation_black.text[aa] << " ";
		}
		

		if( pVarBlack )
		{
			
			Move* tempVar = pVarBlack;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}

					if( tempNext->white != "" )
					{
						of<< tempNext->info <<". "<< tempNext->white <<" ";
					}

					//if( tempNext != tempVar )
					{

						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if( (tempNext->white == "") )
					{
						of<< tempNext->info <<"... ";
					}

					if(tempNext!=tempVar && tempNext->pVarWhite)
					{
						of << "( ";
						tempNext->pVarWhite->SaveToStream(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext != tempVar) && (tempNext->pVarWhite) )
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";

		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext!=tempVar && tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToStream(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarBlack;
				of << ") ";
			}
				
		}

		if( pNext )
			pNext->SaveToStream(of);
	}

	void SetFenWhite( string fen )
	{
		m_fenWhite = fen;
		zhashKeyWhite = hash_from_fen((char*)fen.c_str());
		hasFenWhite = true;
	}

	string getFenWhite()
	{
		return m_fenWhite;
	}

	void SetFenBlack( string fen )
	{
		m_fenBlack = fen;
		zhashKeyBlack = hash_from_fen((char*)fen.c_str());
		hasFenBlack = true;
	}

	string getFenBlack()
	{
		return m_fenBlack;
	}

	bool IsThisTheEnd(){
		return (atEndWhite||atEndBlack); 
	}

	bool EndWhite()
	{
		return atEndWhite;
	}

	bool EndBlack()
	{
		return atEndBlack;
	}

	void SetWhiteMoveSquares(MyChessMove &src)
	{
		//whiteMove.from.file = src.from.file;
		//whiteMove.from.rank = src.from.rank;
		//whiteMove.to.file = src.to.file;
		//whiteMove.to.rank = src.to.rank;
		whiteMove = src;
	}

	void SetBlackMoveSquares(MyChessMove &src)
	{
		//blackMove.from.file = src.from.file;
		//blackMove.from.rank = src.from.rank;
		//blackMove.to.file = src.to.file;
		//blackMove.to.rank = src.to.rank;
		blackMove = src;
	}

	void CopyNode( Move* temp )
	{
		x = temp->x;
		y = temp->y;
		white = temp->white;
		black = temp->black;
		number = temp->number;
		mov_num = temp->mov_num;
		annotations = temp->annotations;
		annotation_white = temp->annotation_white;
		annotation_black = temp->annotation_black;
		capture = temp->capture;
		promote = temp->promote;
		info = temp->info;
		result = temp->result;
		m_fenWhite = temp->m_fenWhite;
		m_fenBlack = temp->m_fenBlack;
		hasFenWhite = temp->hasFenWhite;
		hasFenBlack = temp->hasFenBlack;
		zhashKeyWhite = temp->zhashKeyWhite;
		zhashKeyBlack = temp->zhashKeyBlack;
		atEndWhite = temp->atEndWhite;
		atEndBlack = temp->atEndBlack;
		whiteMove = temp->whiteMove;
		blackMove = temp->blackMove;
		variation_count_white = temp->variation_count_white;
		variation_count_black = temp->variation_count_black;
	}


	void PrepareAnnotations()
	{
		for( int aa = 0; aa < annotation_white.text.size(); aa++ )
		{
			int find_curly = -1;
			find_curly = annotation_white.text[aa].find("}");
			if( find_curly != -1 )
			{
				int find_smooth = -1;
				find_smooth = annotation_white.text[aa].find(")");
				if( find_smooth > find_curly )
				{
					annotation_white.text[aa].erase(find_smooth, 1);
				}
			}
			text_white += " "+annotation_white.text[aa];
		}
		for( int aa = 0; aa < annotation_black.text.size(); aa++ )
		{
			int find_curly = -1;
			find_curly = annotation_black.text[aa].find("}");
			if( find_curly != -1 )
			{
				int find_smooth = -1;
				find_smooth = annotation_black.text[aa].find(")");
				if( find_smooth > find_curly )
				{
					annotation_black.text[aa].erase(find_smooth, 1);
				}
			}
			text_black += " "+annotation_black.text[aa];
		}
	}

	// BROKEN
	void EnumerateVariations()
	{
		char varIds[15]="ABCDEFGHIJKLMN";
		char varIdsBlack[15]="abcdefghijklmn";
		if( variation_count_white > 0 )
		{
			if( variation_count_white > 1 )
			{
				Move* pVarIter = pVarWhite;
				pVarIter->variation_id += varIds[0];
				pVarIter->variation_id += "\0";
				int count = 1;
				pVarIter = pVarIter->pVarWhite;
				while( pVarIter && count < 14)
				{
					pVarIter->variation_id += varIds[count++];
					pVarIter->variation_id += "\0";
					Move* pVarNextIter = pVarIter->pNext;
					while( pVarNextIter )
					{
						pVarNextIter->EnumerateVariations();
						pVarNextIter = pVarNextIter->pNext;
					}
					pVarIter = pVarIter->pVarWhite;
				}
			}
		}
		if( variation_count_black > 0 )
		{
			if( variation_count_black > 1 )
			{
				Move* pVarIter = pVarBlack;
				pVarIter->variation_id += varIdsBlack[0];
				pVarIter->variation_id += "\0";
				int count = 1;
				pVarIter = pVarIter->pVarBlack;
				while( pVarIter && count < 14)
				{
					pVarIter->variation_id += varIdsBlack[count++];
					pVarIter->variation_id += "\0";
					Move* pVarNextIter = pVarIter->pNext;
					while( pVarNextIter )
					{
						pVarNextIter->EnumerateVariations();
						pVarNextIter = pVarNextIter->pNext;
					}
					pVarIter = pVarIter->pVarBlack;
				}
			}
		}
		if( pNext )
			pNext->EnumerateVariations();
	}

	bool IsTrueDisplayVariations()
	{
		return displayVariations;
	}

	void SetDisplayVariations(bool set)
	{
		displayVariations = set;
	}

	void SetRenderColor( int r, int g, int b )
	{
		renderMoveColor.r = r;
		renderMoveColor.g = g;
		renderMoveColor.b = b;
	}

	RenderMoveColor* GetRenderColor()
	{
		return &renderMoveColor;
	}

	void SetRenderColorWhite(int r, int g, int b)
	{
		
		renderMoveColorWhite.r = r;
		renderMoveColorWhite.g = g;
		renderMoveColorWhite.b = b;
	}

	void SetRenderColorBlack(int r, int g, int b)
	{
		renderMoveColorBlack.r = r;
		renderMoveColorBlack.g = g;
		renderMoveColorBlack.b = b;
	}

	RenderMoveColor* GetRenderColorWhite()
	{
		return &renderMoveColorWhite;
	}

	RenderMoveColor* GetRenderColorBlack()
	{
		return &renderMoveColorBlack;
	}


	void SetVariationRenderColor(int r, int g, int b)
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}
		
		while(temp)
		{
			temp->SetRenderColor(r,g,b);
			temp->SetRenderColorWhite(r,g,b);
			temp->SetRenderColorBlack(r,g,b);
			temp = temp->pNext;
		}
	}

	void Visit()
	{
		visited = true;
	}

	bool HasBeenVisited()
	{
		return visited;
	}

	void PrepareForVisitor()
	{
		visited = false;
		
		Move* temp = this->pVarWhite;
		while( temp )
		{
			temp->PrepareForVisitor();
			temp = temp->pVarWhite;
		}

		temp = this->pVarBlack;

		while( temp )
		{
			temp->PrepareForVisitor();
			temp = temp->pVarBlack;
		}

		if( pNext )
			pNext->PrepareForVisitor();
	}



#define PIECE_FLAG 0x00000001
#define FILE_FLAG 0x00000010
#define RANK_FLAG 0x00000100
#define CAPTURE_FLAG 0x00001000
#define FILE_DEST_FLAG 0x00010000
#define RANK_DEST_FLAG 0x00100000
#define PROMOTE_FLAG 0x01000000
#define GLYPHS 0x10000000

#define INVALID_1 0x01000001
#define INVALID_2 0x00111110
#define INVALID_3 0x00000001

	int patternFlag;
#define TEST_FLAG(X) (patternFlag & X)
#define SET_FLAG(X) patternFlag |= X
#define UNSET_FLAG(X) patternFlag &= ~X
#define RESET() patternFlag=0x00000000
#define PATTERN_ZERO() (patternFlag == 0x00000000)

	

	int SetMove(string &str, int color)
	{
		if( color == WHITE )
		{
			white = str;
		}
		else
		{
			black = str;

		}
		return PASSED_TEST;
		RESET();

		int length = str.length();

		if( length < 2 ) return INCORRECT_LENGTH;
		if( length > 9 ) return INCORRECT_LENGTH;

		int pos = 0;
		bool pieceMove = false;
		

		// {Piece} {file|rank} {captures} [file][rank] {pawn:promote}{glyphs&check} | castles
		if( str[pos] == 'O' )
		{
			if( str.find("O-O") != -1)
			{
				pos=3;
				if( length >= 5 && str[4]=='O')
				{
					pos = 5;
				}
				SET_FLAG(PIECE_FLAG);
				SET_FLAG(FILE_DEST_FLAG);
				SET_FLAG(RANK_DEST_FLAG);
			}	
		}

		if (pos < length && 
			(str[pos] == 'B' || str[pos] == 'N' ||
			 str[pos] == 'R' || str[pos] == 'Q' ||
			 str[pos] == 'K')) 
		{
			pieceMove = true;
			pos++;
			SET_FLAG(PIECE_FLAG);
		}		

		if( pos < length && (str[pos] >= 'a' && str[pos] <= 'h'))
		{
			pos++;
			if( length == pos) return INCORRECT_LENGTH;
			SET_FLAG(FILE_FLAG);
		}
		

		if( pos < length && (str[pos] >= '1' && str[pos] <= '8'))
		{
			if( pos == 0 ) return FORMAT_ERROR;
			pos++;
			if( (TEST_FLAG( FILE_FLAG) && !TEST_FLAG( PIECE_FLAG))|| 
				(TEST_FLAG( 0x00000111) ))
			{
				UNSET_FLAG( FILE_FLAG);
				UNSET_FLAG( RANK_FLAG);
				SET_FLAG(FILE_DEST_FLAG);
				SET_FLAG(RANK_DEST_FLAG);
			}
		}

		if( pos < length && str[pos] == 'x' )
		{
			if( pos == 0 ) return FORMAT_ERROR;
			pos++;
			if( pos == length-1) return INCORRECT_LENGTH;
			
			SET_FLAG(CAPTURE_FLAG);
		}

		if (pos < length && (str[pos] >= 'a' && str[pos] <= 'h'))
		{
			pos++;
			if( pos == length) return INCORRECT_LENGTH;
			SET_FLAG(FILE_DEST_FLAG);
			
		}

		if (pos < length && str[pos] >= '1' && str[pos] <= '8')
		{
			pos++;
			SET_FLAG(RANK_DEST_FLAG);
		}

		if( pos < length && str[pos] == '=' )
		{
			pos++;
			
			SET_FLAG(PROMOTE_FLAG);
			
			if(TEST_FLAG(PIECE_FLAG))
				return FORMAT_ERROR;

			if( pos < length && 
				(str[pos] == 'Q' ||
				 str[pos] == 'R' ||
				 str[pos] == 'N' ||
				 str[pos] == 'B'))
			{
				pos++;
			}
			else return FORMAT_ERROR;
		}

		
		if( pos < length && (str[pos] == '+' || str[pos] == '#'))
		{
			pos++;
		}

		if( pos < length && (str[pos] == '!' || str[pos] == '?'))
		{
			pos++;
			SET_FLAG(GLYPHS);
		}
		if( pos < length && (str[pos] == '!' || str[pos] == '?'))
		{
			pos++;
			SET_FLAG(GLYPHS);
		}

		if( pos == 0 )
			return FORMAT_ERROR;

		if( PATTERN_ZERO() )
			return FORMAT_ERROR;

		
		if ( !TEST_FLAG(FILE_DEST_FLAG) || 
			 !TEST_FLAG(RANK_DEST_FLAG))
			 return FORMAT_ERROR;

		if( !TEST_FLAG(PIECE_FLAG) )
		{
			if( TEST_FLAG(FILE_FLAG) &&
				TEST_FLAG(RANK_FLAG) &&
				TEST_FLAG(CAPTURE_FLAG))
				return FORMAT_ERROR;

			if( color == WHITE )
			{
				white = str;
			}
			else
			{
				black = str;
			}
			return PASSED_TEST;
		}
		
		if( TEST_FLAG(PIECE_FLAG))
		{
			if( TEST_FLAG(PROMOTE_FLAG))
				return FORMAT_ERROR;
			
			if( color == WHITE )
			{
				white = str;
			}
			else
			{
				black = str;
			}
			return PASSED_TEST;
		}

		return FORMAT_ERROR;
	}


	//typedef enum varIds{ A, B, C, D, E, F, G, H };

	int x;
	int y;
	string white;
	string black;
	int number;
	string mov_num;
	string annotations;
	string text_white;
	string text_black;
	Comment annotation_white;
	Comment annotation_black;
	int capture;
	int promote;
	string info;
	string result;
	vector<Move> variations[64];

	Line* m_line;
	
	Variation variation[64];
	string m_fenWhite;
	string m_fenBlack;
	bool hasFenWhite;
	bool hasFenBlack;
	uint64 zhashKeyWhite;
	uint64 zhashKeyBlack;
	//int end_white_black;
	bool atEndWhite;
	bool atEndBlack;
	MyChessMove whiteMove;
	MyChessMove blackMove;

	Move* pNext;
	Move* pPrev;
	Move* pVarWhite;
	Move* pVarBlack;
	Move* pUp;
	Move* pVariationWhite[64];
	Move* pVariationBlack[64];
	int variation_count_white;
	int variation_count_black;

	string variation_id;
	bool displayVariations;
	RenderMoveColor renderMoveColor;
	RenderMoveColor renderMoveColorWhite;
	RenderMoveColor renderMoveColorBlack;

	int move_cp_white;
	int move_cp_black;
	string depthWhite;
	string depthBlack;
	string analysis_engine;
	int depth_analyzed;
	bool visited;

};

class Game
{
public:

	Move* pCurrMove;
	Move* pRoot;
	Move* pBase; // the base from which the tree groweth
	int selectedMove;
	string whiteName;
	string blackName;
	string eventName;
	string resultName;
	string dateName;
	bool bHasGeneratedFens;
	
	Game()
	{
		move_counter = -1;
		white_black = 0;
		game_curr_move = 0;
		//pCurrMove = NULL;
		pRoot = new Move();
		pBase = new Move();
		pBase->pNext = pRoot;
		pRoot->pPrev = pBase;
		
		pBase->SetFenBlack(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
		pBase->SetFenWhite(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

		pCurrMove = pRoot;
		selectedMove = 0;
		end_white_black = -1;
		bHasGeneratedFens = false;
		gameBeginsAtStartPos = true;
	}

	~Game()
	{
	//	Reset();
		// clear vectors
		if( pBase )
		{
			delete pBase; // and all the pNext's are deleted recursively
			pBase = NULL;
		}
	}

	void PrepareForVisitor()
	{
		pBase->PrepareForVisitor();
	}


	void SetHasGeneratedFens()
	{
		bHasGeneratedFens = true;
	}

	bool HasGeneratedFens()
	{
		return bHasGeneratedFens;
	}

	void SaveGame(char* filename)
	{
		ofstream of;
		of.open(filename); 
		for( int i = 0; i < vec_header.size(); i++ )
			of << vec_header[i].data.c_str() << endl;
		
		/*if( pBase->m_fenWhite != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
		{
			of << "[FEN \"";
			
			of << pBase->m_fenWhite;

			of<< "\"]";

			of << endl;
		}*/

		of<<endl;

		pRoot->SaveToFile(of);
		
		of<<endl;
	}

	ostringstream& GameToBuffer(ostringstream& of)
	{

		//of.open(filename); 
		for( int i = 0; i < vec_header.size(); i++ )
			of << vec_header[i].data.c_str() << endl;
		
		
		/*if( pBase->m_fenWhite != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
		{
			bool foundFen = false;

			if( !foundFen )
			{
				of << "[FEN \"";
			
				of << pBase->m_fenWhite;

				of<< "\"]";

				of << endl;
			}
		}*/

		of<<endl;

		pRoot->SaveToStream(of);
		
		of<<endl;

		return of;
	}


	void DeleteVariation()
	{
		// if the node has an up pointer
		if( pCurrMove->pUp == NULL )
		{
			Move* temp = pCurrMove;
			while( temp->pUp == NULL )
			{
				temp = temp->pPrev;
			}
			pCurrMove = temp;
		}
		
		if( pCurrMove->white == "" )
		{
			Move* Up = pCurrMove->pUp;
			Move* temp = pCurrMove->pUp->pVarBlack;
			if( temp == pCurrMove )
			{
				Up->pVarBlack = pCurrMove->pVarBlack;
				pCurrMove->pVarBlack = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;
			}
			else
			{
				while( temp->pVarBlack != pCurrMove )
				{
					temp = temp->pVarBlack;
				}
				temp->pVarBlack = pCurrMove->pVarBlack;
				pCurrMove->pVarBlack = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;

			}
		}
		else
		{
			Move* Up = pCurrMove->pUp;
			Move* temp = pCurrMove->pUp->pVarWhite;
			if( temp == pCurrMove )
			{					
				Up->pVarWhite = pCurrMove->pVarWhite;
				pCurrMove->pVarWhite = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;
			}
			else
			{
				while( temp->pVarWhite != pCurrMove )
				{
					temp = temp->pVarWhite;
				}
				temp->pVarWhite = pCurrMove->pVarWhite;
				pCurrMove->pVarWhite = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;
			}
		}

	}



	void CreateEmptyHeader()
	{
/*
1) Event (the name of the tournament or match event)

2) Site (the location of the event)

3) Date (the starting date of the game)

4) Round (the playing round ordinal of the game)

5) White (the player of the white pieces)

6) Black (the player of the black pieces)

7) Result (the result of the game)
*/

		HeaderElem eventElem;
		eventElem.data += "[Event \"?\"]";
		
		HeaderElem siteElem;
		siteElem.data += "[Site \"?\"]";

		HeaderElem dateElem;
		dateElem.data += "[Date \"????.??.??\"]";

		HeaderElem whiteElem;
		whiteElem.data += "[White \"?\"]";

		HeaderElem blackElem;
		blackElem.data += "[Black \"?\"]";

		HeaderElem resultElem;
		resultElem.data += "[Result \"?\"]";

		vec_header.push_back(eventElem);
		vec_header.push_back(siteElem);
		vec_header.push_back(dateElem);
		vec_header.push_back(whiteElem);
		vec_header.push_back(blackElem);
		vec_header.push_back(resultElem);
	}

	string GetHeaderElem(string name)
	{
		for( unsigned int i = 0; i < vec_header.size(); i++ )
		{
			if( vec_header[i].data.substr(1, name.length()) == name )
			{
				return GetSubstringInQuotes(vec_header[i].data);
			}
		}
		return "*";
	}

	string GetSubstringInQuotes(string &inpt)
	{
		int openQuotes = inpt.find('"');
		int closeQuotes = inpt.find_last_of('"');

		int length = inpt.length();
		int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
		return inpt.substr(openQuotes+1, size-2);
	}

	void ParseHeader()
	{
		pBase->SetFenBlack(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
		pBase->SetFenWhite(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

		if( vec_header.size() > 0 )
		{
			wxString strEvent;
			wxString strDate;
			wxString strWhite;
			wxString strBlack;
			wxString strResult;

			for( int j = 0; j < vec_header.size(); j++ )
			{
				string currString = vec_header[j].data;
						
				int findEvent = -1;
				findEvent = currString.find("Event");
				int findEventLc = -1;
				findEventLc = currString.find("event");
				if( findEvent!= -1 )
				{

					eventName =  GetSubstringInQuotes(currString);
				}
				if( findEventLc!= -1 )
				{
					eventName =  GetSubstringInQuotes(currString);
				}

				int findDate = -1;
				findDate = currString.find("Date");
				int findDateLc = -1;
				findDateLc = currString.find("date");
				if( findDate != -1 )
				{
					dateName =  GetSubstringInQuotes(currString);
				}
				if( findDateLc != -1 )
				{
					dateName =  GetSubstringInQuotes(currString);
				}

				int findWhite = -1; 
				findWhite = currString.find("White");
				int findWhiteLc = -1; 
				findWhiteLc = currString.find("white");
				int findWhiteElo = -1;
				findWhiteElo = currString.find("WhiteElo");
				int findWhiteEloLc = -1;
				findWhiteEloLc = currString.find("whiteelo");
				if( (findWhite != -1) && (findWhiteElo != findWhite))
				{
							 
					whiteName =  GetSubstringInQuotes(currString);
				}
				if( (findWhiteLc != -1) && (findWhiteEloLc != findWhiteLc))
				{
					whiteName =  GetSubstringInQuotes(currString);
				}

				int findBlack = -1; 
				findBlack = currString.find("Black");
				int findBlackLc = -1;
				findBlackLc = currString.find("black");
				int findBlackElo = -1;
				findBlackElo = currString.find("BlackElo");
				int findBlackEloLc = -1;
				findBlackEloLc = currString.find("blackelo");
				if( (findBlack != -1) && (findBlackElo != findBlack) )
				{
					blackName =  GetSubstringInQuotes(currString);
				}
				if( (findBlackLc != -1) && (findBlackEloLc != findBlackLc) )
				{
					blackName =  GetSubstringInQuotes(currString);
				}

				int findResult = -1; 
				findResult = currString.find("Result");
				int findResultLc = -1; 
				findResultLc = currString.find("result");
				if( findResult != -1 )
				{
					resultName =  GetSubstringInQuotes(currString);
				}	
				if( findResultLc != -1 )
				{
					resultName =  GetSubstringInQuotes(currString);
				}	

				int findFen = -1;
				findFen = currString.find("FEN");
				if( findFen != -1 )
				{
					if(pRoot->white == "" && pRoot->x == 1)
					{
						pRoot->m_fenWhite = GetSubstringInQuotes(currString);
						pRoot->zhashKeyWhite = hash_from_fen((char*)pRoot->m_fenWhite.c_str());
						pBase->m_fenBlack = GetSubstringInQuotes(currString);
						pBase->zhashKeyBlack = hash_from_fen((char*)pBase->m_fenBlack.c_str());
						pBase->m_fenWhite = GetSubstringInQuotes(currString);
						pBase->zhashKeyWhite = hash_from_fen((char*)pBase->m_fenWhite.c_str());
						gameBeginsAtStartPos = false;
					}
					else {
						pBase->m_fenBlack = GetSubstringInQuotes(currString);
						pBase->zhashKeyBlack = hash_from_fen((char*)pBase->m_fenBlack.c_str());
						pBase->m_fenWhite = GetSubstringInQuotes(currString);
						pBase->zhashKeyWhite = hash_from_fen((char*)pBase->m_fenWhite.c_str());
						gameBeginsAtStartPos = false;
					}
				}
			}
		}
	}


	bool MergeGame(Game* pGame)
	{
		if( pRoot->info == pGame->pRoot->info )
		{
			if( pGame->pRoot->white != "" )
			{
				if( pGame->pRoot->white != pRoot->white )
				{
					Move* pMoveInsert = pRoot->getWhiteVariationForParse();
					pMoveInsert->CopyNode(pGame->pRoot);
					
					pMoveInsert->pNext = pGame->pRoot->pNext;
					pGame->pRoot->pNext->pPrev = pMoveInsert;

					pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
					if( pMoveInsert->pVarWhite )
						pMoveInsert->variation_count_white++;
					pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
					if( pMoveInsert->pVarBlack )
						pMoveInsert->variation_count_black++;

					pGame->pRoot->pNext = NULL;
					pGame->pRoot->pVarWhite = NULL;
					pGame->pRoot->pVarBlack = NULL;

					return true;
				}
				else if ( pGame->pRoot->black != pRoot->black )
				{
					Move* pMoveInsert = pRoot->getBlackVariationForParse();
					pMoveInsert->CopyNode(pGame->pRoot);
					
					pMoveInsert->pNext = pGame->pRoot->pNext;
					pGame->pRoot->pNext->pPrev = pMoveInsert;
					
					pMoveInsert->white = "";

					pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
					if( pMoveInsert->pVarWhite )
						pMoveInsert->variation_count_white++;
					pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
					if( pMoveInsert->pVarBlack )
						pMoveInsert->variation_count_black++;

					pGame->pRoot->pNext = NULL;
					pGame->pRoot->pVarWhite = NULL;
					pGame->pRoot->pVarBlack = NULL;

					return true;
				}
				else
				{
					Move* tempIter = pRoot;
					Move* pGameIter = pGame->pRoot;
					// first check to see that game 2 does not match to the end
					while( tempIter && pGameIter)
					{
						if( (tempIter->white != pGameIter->white) && (pGameIter->white != ""))
						{
							Move* pMoveInsert = tempIter->getWhiteVariationForParse();
							pMoveInsert->CopyNode(pGameIter);

							pMoveInsert->pNext = pGameIter->pNext;
							if( pGameIter->pNext )
								pGameIter->pNext->pPrev = pMoveInsert;
							
							pMoveInsert->pVarWhite = pGameIter->pVarWhite;
							if( pMoveInsert->pVarWhite )
								pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGameIter->pVarBlack;
							if( pMoveInsert->pVarBlack )
								pMoveInsert->variation_count_black++;

							pGameIter->pNext = NULL;
							pGameIter->pVarWhite = NULL;
							pGameIter->pVarBlack = NULL;
							return true;
						}
						
						if( (tempIter->black != pGameIter->black) ||
							(pGameIter->white == ""))
						{
							Move* pMoveInsert = tempIter->getBlackVariationForParse();
							pMoveInsert->CopyNode(pGameIter);
							
							pMoveInsert->pNext = pGameIter->pNext;
							if( pGameIter->pNext )
								pGameIter->pNext->pPrev = pMoveInsert;
							
							pMoveInsert->white = "";

							pMoveInsert->pVarWhite = pGameIter->pVarWhite;
							if( pMoveInsert->pVarWhite )
								pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGameIter->pVarBlack;
							if( pMoveInsert->pVarBlack )
								pMoveInsert->variation_count_black++;

							Move* pIter = pMoveInsert->pUp->pVarBlack;
							while( pIter )
							{
								pIter->pUp = pMoveInsert->pUp;
								pIter->pPrev = pMoveInsert->pUp;
								pIter = pIter->pVarBlack;
							}

							pGameIter->pNext = NULL;
							pGameIter->pVarWhite = NULL;
							pGameIter->pVarBlack = NULL;
							return true;
						}
						
						if( pGameIter->pVarWhite )
						{
							Move* pMoveInsert = tempIter->getWhiteVariationForParse();
							pMoveInsert->CopyNode(pGameIter->pVarWhite);

							pMoveInsert->pNext = pGameIter->pVarWhite->pNext;
							if( pGameIter->pVarWhite->pNext )
								pGameIter->pVarWhite->pNext->pPrev = pMoveInsert;

							pMoveInsert->pVarWhite = pGameIter->pVarWhite->pVarWhite;
							if( pMoveInsert->pVarWhite )
								pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGameIter->pVarWhite->pVarBlack;
							if( pMoveInsert->pVarBlack )
								pMoveInsert->variation_count_black++;

							Move* pMoveInsertVarIter = pMoveInsert;
							while( pMoveInsertVarIter )
							{
								pMoveInsertVarIter->pUp = tempIter;
								pMoveInsertVarIter->pPrev = tempIter;
								pMoveInsertVarIter = pMoveInsertVarIter->pVarWhite;
							}

							//pGameIter->pVarWhite = NULL;
							pGameIter->pVarWhite->pNext = NULL;
							pGameIter->pVarWhite->pVarWhite = NULL;
							pGameIter->pVarWhite->pVarBlack = NULL;
							//return true;
						}
						
						if( pGameIter->pVarBlack )
						{
							Move* pMoveInsert = tempIter->getBlackVariationForParse();
							pMoveInsert->CopyNode(pGameIter->pVarBlack);

							pMoveInsert->pNext = pGameIter->pVarBlack->pNext;
							if( pGameIter->pVarBlack->pNext )
								pGameIter->pVarBlack->pNext->pPrev = pMoveInsert;

							//pMoveInsert->pVarWhite = pGameIter->pVarWhite->pVarWhite;
							//if( pMoveInsert->pVarWhite )
							//	pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGameIter->pVarBlack->pVarBlack;
							if( pMoveInsert->pVarBlack )
								pMoveInsert->variation_count_black++;

							Move* pMoveInsertVarIter = pMoveInsert;
							while( pMoveInsertVarIter )
							{
								pMoveInsertVarIter->pUp = tempIter;
								pMoveInsertVarIter->pPrev = tempIter;
								pMoveInsertVarIter = pMoveInsertVarIter->pVarBlack;
							}

							pGameIter->pVarBlack = NULL;
							//pGameIter->pVarWhite->pNext = NULL;
							//pGameIter->pVarWhite->pVarWhite = NULL;
							//pGameIter->pVarWhite->pVarBlack = NULL;
							//return true;
						}
						pGameIter = pGameIter->pNext;
						tempIter = tempIter->pNext;
					}

					return false;
				}

			}
			else
			{
				Move* pMoveInsert = pRoot->getBlackVariationForParse();
				pMoveInsert->CopyNode(pGame->pRoot);
					
				pMoveInsert->pNext = pGame->pRoot->pNext;
				pGame->pRoot->pNext->pPrev = pMoveInsert;
					
				pMoveInsert->white = "";

				pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
				if( pMoveInsert->pVarWhite )
					pMoveInsert->variation_count_white++;
				pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
				if( pMoveInsert->pVarBlack )
					pMoveInsert->variation_count_black++;

				pGame->pRoot->pNext = NULL;
				pGame->pRoot->pVarWhite = NULL;
				pGame->pRoot->pVarBlack = NULL;

				return true;
			}
		}
		else
		{
			// find the node whose info is equal 
			Move* tempIter = pRoot;
			while( tempIter )
			{
				if( tempIter->info == pGame->pRoot->info )
				{
					if( pGame->pRoot->white != "" )
					{
						//if( pGame->pBase->m_fenBlack == tempIter->pPrev->m_fenBlack )
						{
							Move* pMoveInsert = tempIter->getWhiteVariationForParse();
							pMoveInsert->CopyNode(pGame->pRoot);
							pMoveInsert->pNext = pGame->pRoot->pNext;
							pGame->pRoot->pNext->pPrev = pMoveInsert;
							pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
							if( pMoveInsert->pVarWhite )
								pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
							if( pMoveInsert->pVarBlack )
								pMoveInsert->variation_count_black++;

							pGame->pRoot->pNext = NULL;
							pGame->pRoot->pVarWhite = NULL;
							pGame->pRoot->pVarBlack = NULL;
							return true;
						}
						//else return false;
					}
					else
					{
						//if( pGame->pBase->m_fenBlack == tempIter->m_fenWhite )
						{
							Move* pMoveInsert = tempIter->getBlackVariationForParse();
							pMoveInsert->CopyNode(pGame->pRoot);
							pMoveInsert->pNext = pGame->pRoot->pNext;
							if( pGame->pRoot->pNext )
								pGame->pRoot->pNext->pPrev = pMoveInsert;
							
							pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
							if( pMoveInsert->pVarWhite )
								pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
							if( pMoveInsert->pVarBlack )
								pMoveInsert->variation_count_black++;

							Move* pMoveInsertVarIter = pMoveInsert;
							while( pMoveInsertVarIter )
							{
								pMoveInsertVarIter->pUp = tempIter;
								pMoveInsertVarIter->pPrev = tempIter;
								pMoveInsertVarIter = pMoveInsertVarIter->pVarBlack;
							}

							pGame->pRoot->pNext = NULL;
							pGame->pRoot->pVarWhite = NULL;
							pGame->pRoot->pVarBlack = NULL;
							return true;
						}
						//else return false;
					}
				}
				tempIter = tempIter->pNext;
			}
		}
		return false;
	}

	int GetMoveCount()
	{
		int count = 0;
		return pRoot->GetMoveCount(count);
	}

	void EnumerateVariations()
	{
		pRoot->EnumerateVariations();
	}


	int CountMoves()
	{
		int numMoves = 0;
		return pBase->Count(numMoves);
	}

	// current working stable
	bool IsInBase()
	{
		return (this->pCurrMove == this->pBase);
	}

	// deprectaed
	Move* GetSelectedMove(int sel)
	{
		if( sel >= move_counter )
			return NULL;

		Move* movOut = pRoot;
		for( int count = 0; (count < sel); count++)
		{
			movOut = movOut->pNext;
		}

		return movOut;
	}


	// deprectaed
	Move* GetNextMove()
	{
		Move* movOut = pCurrMove;
		

		if( white_black == 0 )
		{
			white_black = 1;
			return movOut;
			
		}
		else
		{
			white_black = 0;
			if( pCurrMove->pNext != NULL )
				pCurrMove = pCurrMove->pNext;
			return movOut;

		}
	}

	// deprectaed
	Move* GetPreviousMove()
	{
		Move* movOut = pCurrMove;
		
		if( movOut->pPrev == NULL )
		{
			//while(movOut->pNext)
			//	movOut = movOut->pNext;
	
			white_black = end_white_black;
		}

		
		if( white_black == 0 )
		{
			white_black = 1;
			if( pCurrMove->pPrev!= NULL )
				pCurrMove = pCurrMove->pPrev;
			return movOut;
			
		}
		else
		{
			white_black = 0;
			
			return movOut;

		}
	}

	Move* GetCurrentMove()
	{
		return pCurrMove;
	}

	//current working stable
	string CurrFenBlack()
	{
		return pCurrMove->getFenBlack();
	}

	//current working stable
	string CurrFenWhite()
	{
		return pCurrMove->getFenWhite();
	}

	//current working stable
	Move* NextMove()
	{
		if( pCurrMove->pNext != NULL )
			pCurrMove = pCurrMove->pNext;

		return pCurrMove;
	}
	// current working stable
	Move* PrevMove()
	{
		if( pCurrMove->pPrev != NULL )
			pCurrMove = pCurrMove->pPrev;

		return pCurrMove;
	}

	// deprecated for now
	void AddNode(MyChessMove *mv, int black_white, int movNum, string fen="", string pgn_move="")
	{
		/*if( black_white == WHITE )
		{
			if( pCurrMove->whiteMove == *mv )
			{
				return;
			}
			else
			{
				Move* pMove = new Move();
				pMove->whiteMove = *mv;
				if( fen != "" )
					pMove->m_fenWhite = fen;
				if( pgn_move != "" )
					pMove->white = pgn_move;

				//if( movNum > 0 )
				//	pMove->info = tostring(movNum);

				pCurrMove->pVariation[ pCurrMove->variation_count++ ] = pMove;// = new Move();
				
			}
		}
		else
		{
			if( pCurrMove->blackMove == *mv )
			{
				return;
			}
			else
			{
				Move* pMove = new Move();
				pMove->blackMove = *mv;
				if( fen != "" )
					pMove->m_fenWhite = fen;
				if( pgn_move != "" )
					pMove->white = pgn_move;

				//if( movNum > 0 )
				//	pMove->info = tostring(movNum);

				pCurrMove->pVariation[ pCurrMove->variation_count++ ] = pMove;// = new Move();
				
			}
		}*/
	}


	bool AtEnd()
	{
		return (pCurrMove->atEndWhite || pCurrMove->atEndBlack);
	}


	// deprecated
	bool AtMoveBeforeEnd()
	{
		return (pCurrMove->pNext != NULL);
	}

	//deprecated
	string GetCurrentMoveInfo()
	{
		return pCurrMove->info; 
	}


	
	//Move* Prev	




	void ResetMoveList()
	{
		pCurrMove = pBase;
		selectedMove = 0;
		white_black = 0;
	}

	void Parse()
	{
		Move *pMove = pRoot;
		for(  vec_s_iter = vec_moves.begin(); vec_s_iter!= vec_moves.end(); vec_s_iter++)
		{
			// ERROR CHECKER, DETECT ILLEGAL TOKENS BY EXCLUDING BAD CHARS e.g."\|/'tywlo^£"
			// PARSE THE $ FOR GLYPHS, ALSO (DETECT CHESSBASE META DATA)?
			int dollar = -1;
			dollar = vec_s_iter->find("$");
			if( dollar != -1 )
				continue;
			int open_curly_brace = vec_s_iter->find("{");
			if( open_curly_brace == 0 )
			{
				if( pMove->x == 0 && pMove->pPrev!= NULL)
				{
					vec_s_iter = HandleComment(vec_s_iter, pMove->pPrev, 1);
					if( vec_s_iter == vec_moves.end() )
						break;
				}
				else
				{
					vec_s_iter = HandleComment(vec_s_iter, pMove, 0 );
					if( vec_s_iter == vec_moves.end() )
						break;
				}
			}
			else 
			{
				int open_bracket = vec_s_iter->find("(");
				if( open_bracket == 0)
				{
					// special case of open bracket followed by new line
					if( open_bracket+1 == vec_s_iter->length() )
					{
						if( pMove->x == 0 && pMove->pPrev != NULL )
						{
							vec_s_iter = HandleOpenBracket2(++vec_s_iter, pMove->pPrev->getBlackVariationForParse(), true);
						}
						else
						{
							if( pMove->x == 0 )
								vec_s_iter = HandleOpenBracket2(++vec_s_iter, pMove->getBlackVariationForParse(), true);
							else 
								vec_s_iter = HandleOpenBracket2(++vec_s_iter, pMove->getWhiteVariationForParse(), true);
						}
					}
					else{
						if( pMove->x == 0 && pMove->pPrev != NULL )
						{
							vec_s_iter = HandleOpenBracket2(vec_s_iter, pMove->pPrev->getBlackVariationForParse(), false);	
						}
						else
						{
							if( pMove->x == 0 )
								vec_s_iter = HandleOpenBracket2(vec_s_iter, pMove->getBlackVariationForParse(), false);	
							else
								vec_s_iter = HandleOpenBracket2(vec_s_iter, pMove->getWhiteVariationForParse(), false);	
					
						}
					}
				}
				else 
				{
					size_t comp = vec_s_iter->compare("1-0");
					size_t comp2 = vec_s_iter->compare("0-1");
					size_t comp3 = vec_s_iter->compare("1/2-1/2");
					size_t comp4 = vec_s_iter->compare("*");
					bool the_end = false;
					if( comp == 0 || comp2 == 0 || comp3 == 0 || comp4 == 0)
					{
							cout<< "result == 0" <<endl;
							the_end = true;
							
							if( pMove->x == 1 )
							{
								int ret = pMove->SetMove(*vec_s_iter, BLACK);
								//pMove->black = *vec_s_iter;
								if( ret != PASSED_TEST )
								{
									// error
								}
								end_white_black = 1;

								pMove->atEndWhite = true;

								return;
							}
							else
							{

								Move *mv;
								mv = pMove;

								mv->result = *vec_s_iter;

								pMove->atEndBlack = true;
								move_counter++;

								end_white_black = 0;
								return;
							}
							
								//move_counter;
							// white has no move on triple dots

					}
					
					int full_stop = -1;
					full_stop = vec_s_iter->find(".");

					int triple_dots = -1;
					triple_dots = vec_s_iter->find("...");

					int close_smooth = -1;
					close_smooth = vec_s_iter->find(")");

					if( close_smooth >= 0 ) // ?? WHY THE EMPTY IF ??? SKIPS THE REST
					{
					}
					else if( full_stop > 0 && !(triple_dots>0) )
					{
						if( full_stop != vec_s_iter->length()-1 )
						{
							Move *mv;
							mv = pMove; 

							// ERROR CHECK NUMERICAL FORMAT AND NUMBER CORRECTNESS
							// ERROR CHECK MOVE STRING IS CORRECT FORMAT
							CreateWithNoSpaceAfterFullstop2(vec_s_iter, full_stop, &mv);


							//moves.push_back(mv);
							//move_counter++;
							//move_iter = moves.begin() + moves.size()-1;//move_counter;
							pMove->x = 1; // white has no move on triple dots
						}
						else
						{
							Move *mv;
							mv = pMove; 

							// ERROR CHECK NUMERICAL FORMAT AND NUMBER CORRECTNESS
							mv->info = vec_s_iter->substr(0, full_stop) ;
							//mv->info[mv->info.length()-1] = 0;
							//pMove->pNext = new Move();
							//pMove = pMove->pNext;
							//pMove->pPrev = mv;
							//move_counter++;
							
							//pMove->x = 1;
						}
					}
					else if(triple_dots>0)
					{
						if( pMove->x == 0 )
						{
							if( pMove->pPrev->y== 0 )
							{
								if( full_stop != vec_s_iter->length()-3 )
								{
									pMove->x=1;
									pMove->y = 1;
									pMove->info = vec_s_iter->substr(0, full_stop);
									//pMove->black = vec_s_iter->substr(full_stop+3, vec_s_iter->length());
									int ret = pMove->SetMove(vec_s_iter->substr(full_stop+3, vec_s_iter->length()), BLACK);
								//pMove->black = *vec_s_iter;
									if( ret != PASSED_TEST )
									{
										// error
									}
									pMove->pNext = new Move();
									Move* mv = pMove;
									pMove = pMove->pNext;
									pMove->pPrev = mv;

									move_counter++;
								}
								else
								{
									pMove->info = vec_s_iter->substr(0, full_stop);
									pMove->x = 1;
								}
							}
							else
							{
								pMove->x = 1; 
								//pMove->white = *vec_s_iter; // possible bug
								// the input string is of the form 18... or 18...Bc5
								int ret = pMove->SetMove(*vec_s_iter, WHITE);
								//pMove->black = *vec_s_iter;
								if( ret != PASSED_TEST )
								{
										// error
								}
							}
						}
						else
						{
							
							/*for( int i = triple_dots, j = 0; i < vec_s_iter->length()-3; j++, i++)
							{
								move_iter->black[j] = (*vec_s_iter)[i+3];
							}*/
							// debug strim triple dots
							if( full_stop != vec_s_iter->length()-3 )
							{
								pMove->y = 1;
								//pMove->black 
									string toset= vec_s_iter->substr(full_stop+3, vec_s_iter->length());
								int ret = pMove->SetMove(toset, BLACK);
								//pMove->black = *vec_s_iter;
								if( ret != PASSED_TEST )
								{
										// error
								}
								pMove->pNext = new Move();
								Move* mv = pMove;
								pMove = pMove->pNext;
								pMove->pPrev = mv;

								move_counter++;
							}
							else
							{
							//	pMove->info = 
							}
						}
					}
					else if( vec_s_iter->length() != 1 )
					{
						if( pMove->x == 0 )
						{
							// ERROR CHECK, MOVE HAS A NUMBER
							pMove->x = 1; 
							//pMove->white = *vec_s_iter;
							int ret = pMove->SetMove(*vec_s_iter, WHITE);
							//pMove->black = *vec_s_iter;
							if( ret != PASSED_TEST )
							{
										// error
							}
						}
						else
						{
							// ERROR CHECK, SET MOVE STRING IS VALID TYPE
							pMove->y = 1;
							//pMove->black = *vec_s_iter;
							int ret = pMove->SetMove(*vec_s_iter, BLACK);
							//pMove->black = *vec_s_iter;
							if( ret != PASSED_TEST )
							{
										// error
							}							
							pMove->pNext = new Move();
							Move* mv = pMove;
							pMove = pMove->pNext;
							pMove->pPrev = mv;

							// debug: gone to next move too early
							// the variation is of the next input for this move sometimes
							move_counter++;
						}
					}

					/*if( the_end == true )
					{
						if( move_iter->x == 0 )
						{
							move_iter->x = 1; 
							move_iter->white = *vec_s_iter;
						}
						else
						{
							move_iter->y = 1;
							move_iter->black = *vec_s_iter;
							cout<<move_iter->info<<" "<<
								  move_iter->white <<" "<<
								  move_iter->black<<" "<<endl;
						}
						return;
					}*/
				}
			}
		}

		// exit for loop
		if( pMove->white == "1-0" || 
			pMove->white == "0-1" || 
			pMove->white == "1/2-1/2" ||
			pMove->white == "*" )
		{
			pMove->pPrev->atEndBlack = true;
		}
		else if( pMove->black == "1-0" || 
			pMove->black == "0-1" || 
			pMove->black == "1/2-1/2" ||
			pMove->black == "*" )
		{
			pMove->atEndWhite = true;
		}
		else if( pMove->black == "" )
		{
			if( pMove->white == "" )
			{
				if( pMove->pPrev && pMove->pPrev->y == 0 )
				{
					if( pMove->pPrev->x == 0 || 
						pMove->pPrev->white == "1-0" || 
						pMove->pPrev->white == "0-1" || 
						pMove->pPrev->white == "1/2-1/2" ||
						pMove->pPrev->white == "*" )
					{
						if( pMove->pPrev->pPrev )
							pMove->pPrev->pPrev->atEndBlack = true;
					}
					else
					{
						pMove->pPrev->atEndWhite = true;
					}
				}
			}
			else pMove->atEndWhite = true;
				
			
		}
		else if( pMove->white == "" )
		{
			if( pMove->pPrev &&
				pMove->pPrev->black == "1-0" || 
				pMove->pPrev->black == "0-1" || 
				pMove->pPrev->black == "1/2-1/2" ||
				pMove->pPrev->black == "*" )
			{
				pMove->pPrev->atEndWhite = true;
			}
			else if( pMove->white == "1-0" || 
				pMove->white == "0-1" || 
				pMove->white == "1/2-1/2" ||
				pMove->white == "*" )
			{
				if( pMove->pPrev && pMove->pPrev->pPrev )
					pMove->pPrev->pPrev->atEndBlack = true;
			}
		}

		
	}


	vector<string>::iterator 
		HandleComment(vector<string>::iterator vinpt,
					   Move* pMove, bool white_black)
	{
		// EDITION : COMMENTS MUST CONFORM TO TEXT BEFORE MOVE 
		//cout<< "curly_brace_open at ...";
		for(; vinpt != vec_moves.end(); vinpt++) 
		{
			int close_curly_brace = -1;
			close_curly_brace = vinpt->find("}");
			if( close_curly_brace != -1 )
			{
				//cout<<*vinpt<<"curly brace closes at ... "<<endl;
				if( white_black == 0 )
					pMove->annotation_white.text.push_back(*vinpt);
				else
					pMove->annotation_black.text.push_back(*vinpt);
				//pMove->annotation.text += *vinpt;

				//if( pMove->x != 0 && pMove->y == 0)
				//	pMove->annotation.after_whites_move = true;
				
//				cout<<" "<<pMove->annotation.text;
				
				break;
			}
			else {
				if( pMove->y == 0 )
					pMove->annotation_white.text.push_back(*vinpt + " ");
				else 
					pMove->annotation_black.text.push_back(*vinpt + " ");
				//pMove->annotation.text += *vinpt + " ";
				
			}
		}
		if( vinpt == vec_moves.end() ) // ERROR MAKE SURE THE CLOSE BRACKET IS DETECTED
			return vinpt;
		else return vinpt++;
		
	}



vector<string>::iterator 
		HandleOpenBracket2(vector<string>::iterator vinpt,
						  Move* pmove, bool newline)
	{
		//cout<< "process current open bracket "<< *vinpt <<endl;
		// ERROR ... EXCLUDE BAD CHARS

		int curly_brace_1 = -1;
		curly_brace_1 = vinpt->find("{");

		if( curly_brace_1 != -1 )
		{
			vinpt = HandleComment(vinpt, pmove, 0);	
		}

		int close_curly_brace_1 = -1;
		close_curly_brace_1 = vinpt->find("}");

		if( close_curly_brace_1 != -1 )
		{
			vinpt++;	
		}

		int full_stop = -1;
		full_stop = vinpt->find(".");

		int trip_dot_1 = -1;
		trip_dot_1 = vinpt->find("...");
		
		int length = vinpt->length();
		
		if( (full_stop > 0) && !(trip_dot_1 > 0))
		{
			Move *newmove = pmove;
			//pMove = new Move();
			
			int open_bracket = -1;
			open_bracket = vinpt->find("(");
			if( newline )
			{
				if( open_bracket != -1 )
				{
					// ERROR ... IS FULLSTOP IN THE RIGHT PLACE PRECEDing A NUMBER
					// IS THE NUMBER THE CORRECT ONE?
					newmove->info = vinpt->substr(open_bracket+1,full_stop-1);

					if( (full_stop != vinpt->length()-1) )
					{
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						newmove->x =1;
					}
					// ERROR WHAT IF THIS IS SKIPPED?
				}
				else
				{
					if( (full_stop != vinpt->length()-1) )
					{
						newmove->info = vinpt->substr(0,full_stop);
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						newmove->x =1;
					}
					else
					{
						newmove->info = vinpt->substr(0,full_stop);
						//if( newmove->info[newmove->info.length()-1] == '.')
						//	newmove->info[vinpt->length()-1] = 0;
					}
					// create with no space after full stop?
				}
				//pMove->info="(" + *vinpt;
			}
			else
			{
				if( open_bracket != -1 )
				{
					newmove->info = vinpt->substr(open_bracket+1,full_stop-1);
					if( (full_stop != vinpt->length()-1) )
					{
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						newmove->x =1;
					}
				}
				else
				{
					if( (full_stop != vinpt->length()-1) )
					{
						newmove->info = vinpt->substr(0,full_stop);
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						newmove->x =1;
					}
					else
					{
						newmove->info = vinpt->substr(0,full_stop);
						//if( newmove->info[newmove->info.length()-1] == '.')
						//	newmove->info[vinpt->length()-1] = 0;
					}
				}
				//pMove->info = *vinpt;
			}
			
			//newmove.info = *vinpt;
			//pMove->x = 0;
		//	newmove->x = 1;
			//vmoves->vecMoves.push_back(newmove);
		}
		else if( trip_dot_1 != -1 )
		{
			Move *newmove = pmove;// = new Move();
			int open_bracket = -1;
			open_bracket = vinpt->find("(");
			if( newline )
			{

				if( (full_stop+3 != length) )
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						pmove->info = tempstr;
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						//pmove->black = tempstr2;
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}						
						pmove->x =1;
						newmove->y = 1;
						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
						pmove->info = tempstr;
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						pmove->x =1;
						newmove->y = 1;
						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
				}
				else
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						pmove->info = tempstr;
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						pmove->x =1;
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
						pmove->info = tempstr;
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						pmove->x =1;
					}
				}
				//newmove->info = "(" + *vinpt;
				//pMove->info = "(" + *vinpt;
			}
			else
			{
				if( (full_stop+3 != length)) 
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						pmove->info = tempstr;
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						pmove->x =1;
						//string tempstr = vinpt->substr(0, full_stop);
						//pmove->info = tempstr;
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						pmove->x =1;
						newmove->y = 1;
						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
						pmove->info = tempstr;
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						//pmove->x =1;
						//string tempstr = vinpt->substr(0, full_stop);
						//pmove->info = tempstr;
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						pmove->x =1;
						newmove->y = 1;
						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
				}
				else
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						pmove->info = tempstr;
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						
						pmove->x =1;
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
						pmove->info = tempstr;
						pmove->x =1;
					}
				}
			}
			
			//pMove->x = 1;
			//vmoves->vecMoves.push_back(newmove);	
		}

		for(vector<string>::iterator vlocal = vinpt+1; vlocal != vec_moves.end(); vlocal++) 
		{

			/*int dollar = -1;
			dollar = vlocal->find("$");
			if( dollar != -1 )
				continue;*/

			int open_curly_brace = vlocal->find("{");
			if( open_curly_brace == 0 )
			{
				if( pmove->x == 0 && pmove->pPrev!=NULL)
				{
					vlocal = HandleComment(vlocal, pmove->pPrev, 1);
				}
				else
				{
					vlocal = HandleComment(vlocal, pmove, 0);	
				}
				//continue;
			}

			int open_bracket = vlocal->find("(");
			if( open_bracket == 0)
			{
				if( open_bracket+1 == vlocal->length() ) 
				{	// rare case of '(' followed by newline
						if( pmove->x == 0 && pmove->pPrev != NULL )
						{
							if( (pmove->pPrev->pUp != NULL) && (pmove->pPrev->white == ""))
								vlocal = HandleOpenBracket2(++vlocal, pmove->pPrev->pUp->getBlackVariationForParse(), true);
							else
								vlocal = HandleOpenBracket2(++vlocal, pmove->pPrev->getBlackVariationForParse(), true);
	
						}
						else
						{
							if( pmove->x == 0 )
							{
								vlocal = HandleOpenBracket2(++vlocal, pmove->getBlackVariationForParse(), true);
							}
							/*else if( pmove->pPrev != NULL )
							{
							}*/
							else 
							{
								if( pmove->pUp != NULL )
									vlocal = HandleOpenBracket2(++vlocal, pmove->pUp->getWhiteVariationForParse(), true);
								else
									vlocal = HandleOpenBracket2(++vlocal, pmove->getWhiteVariationForParse(), true);
							}
						}
				}
				else
				{
						if( pmove->x == 0 && pmove->pPrev != NULL  )
						{
							if( (pmove->pPrev->pUp != NULL) && (pmove->pPrev->white == ""))
								vlocal = HandleOpenBracket2(vlocal, pmove->pPrev->pUp->getBlackVariationForParse(), false);	
							else
								vlocal = HandleOpenBracket2(vlocal, pmove->pPrev->getBlackVariationForParse(), false);	
						}
						else
						{
							if( pmove->x == 0 )
							{
								vlocal = HandleOpenBracket2(vlocal, pmove->getBlackVariationForParse(), false);
							}
							else
							{
								if( pmove->pUp != NULL )
									vlocal = HandleOpenBracket2(vlocal, pmove->pUp->getWhiteVariationForParse(), false);	
								else
								{
									vlocal = HandleOpenBracket2(vlocal, pmove->getWhiteVariationForParse(), false);	
								}

							}
					
						}
				}
			}

			int full_stop = -1;
			full_stop = vlocal->find(".");

			int triple_dots = -1;
			triple_dots = vlocal->find("...");

			int close_curly = -1;
			close_curly = vlocal->find("}");

			int close_smooth = -1;
			close_smooth = vlocal->find(")");

			int dollar = -1;
			dollar = vlocal->find("$");


			//vector<Move> *pMoves = &vmoves->vecMoves;
			if( dollar != -1 )
			{
			}
			else if( close_curly>0 && close_smooth >0)
			{
			}
			else if( close_curly == 0 )
			{
			}
			else if( close_smooth == 0 )
			{
				if( pmove->white != "" )
				{
					if( pmove->black != "" )
					{
						pmove->atEndBlack = true;
					}
					else pmove->atEndWhite = true;
				}
				else
				{
					// try to set the previous one ...
					// ERROR I CAN ASSUME THIS IS HANDLED BELOW
				}
			}// ERROR ARE THE FULLSTOPS PRECEDED BY NUMERICAL CHARS AND IS THE NUMBER CORRECT?
			else if( full_stop > 0 && !(triple_dots>0) && !(close_curly>0)
				/*&&(!close_curly>0)*/)
			{
				int res = -1;
				res = vlocal->find("}");
				// shamefully enough we need to check that we aren't in 
				// a closed bracket node. I can't return ++vinput from the
				// above function to handle comments.

				// is there a space after the full stop?
				if( full_stop != vlocal->length()-1 && res < 0  )
				{
					Move *newmove = pmove;// = 
					// ERROR: CHECK THE MOVE FORMAT, {piece char}{from file|rank} destination {pawns:promotion}{glyphs||checks||mates etc}
					CreateWithNoSpaceAfterFullstop2(vlocal, full_stop, &pmove);

					newmove->x = 1; // white has no move on triple dots
					//pMoves->push_back(newmove);
				}
				else
				{
					Move *newmove = pmove;
					if( !(res<0) )
					{
						if( newmove->pPrev )
						{
							// ERROR WHY AM I STUFFING IN THE END OF AN ANNOTATION?
							string annotation = *vlocal;
							if( pmove->y == 0 )
								pmove->annotation_white.text.push_back(*vinpt + " ");
							else 
								pmove->annotation_black.text.push_back(*vinpt + " ");
						}
					}
					else
					{
						// ERROR: SET INFO
						newmove->info = vlocal->substr(0,full_stop);
						//newmove->info[vlocal->length()-1] = 0;
					}
					//pmove->pNext = new Move();
					//pmove = pmove->pNext;
					//pmove->pPrev = newmove;
					//pMoves->push_back(newmove);
				}
			}
			else if( triple_dots>0 )
			{
				// ERROR: CHECK FULL STOPS ARE IN CORRECT PLACE
				// AND ARE PRECEDED BY NUMERICAL VALUES
				// AND THE NUMBER IS CORRECT
				if( pmove->x == 0 )
				{
					if( full_stop+3 != vlocal->length() /*&& res > 0*/)
					{
						Move *newmove = pmove;
				//pmove->info = *vlocal;
						string tempstr = vlocal->substr(0, full_stop);
						pmove->info = tempstr;
						string tempstr2 = vlocal->substr(full_stop+3, vlocal->length());
						int ret = pmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
						pmove->y =1;
							
						pmove->pNext = new Move();
						pmove = pmove->pNext;
						pmove->pPrev = newmove;
					}
					else
					{
						string tempstr = vlocal->substr(0, full_stop);
						pmove->info = tempstr;
						pmove->x = 1;
					}
				}
				else
				{
					int res = -1;
					res = vlocal->find("}");
				// shamefully enough we need to check that we aren't in 
				// a closed bracket node. I can't return ++vinput from the
				// above function to handle comments.

					// is there a space after the full stop?
					if( full_stop+3 != vlocal->length() /*&& res > 0*/)
					{
						Move *newmove = pmove;// = 
						//newmove->black 
							string tempstr2= vlocal->substr(full_stop+3, vlocal->length());
						int ret = pmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}	
						newmove->y = 1; // white has no move on triple dots
						pmove->pNext = new Move();
						pmove = pmove->pNext;
						pmove->pPrev = newmove;
					//pMoves->push_back(newmove);
					}
					else
					{
						pmove->x = 1;
					}
				}
				//pMoves->push_back(newmove);
			}
			else if( pmove->x == 0 && !(close_curly>0) && !(close_smooth==0) && !(vlocal->length()==0))
			{
				pmove->x = 1;
				//pmove->white = *vlocal;
				int ret = pmove->SetMove(*vlocal, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
			}
			else if( pmove->y == 0 
					&& (pmove->x != 0) 
					&& !(close_curly>0) 
					&& !(close_smooth==0) && (vlocal->length() != 0))
			{
				int res = -1;
				res = vlocal->find("}");
				// shamefully enough we need to check that we aren't in 
				// a closed bracket node. I can't return ++vinput from the
				// above function to handle comments.
				if( res < 0  )
				{
					Move *newmove = pmove;
					pmove->y = 1;
					//pmove->black = *vlocal;
					int ret = pmove->SetMove(*vlocal, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
						}
					pmove->pNext = new Move();
					pmove = pmove->pNext;
					pmove->pPrev = newmove;
					cout<<pmove->info<<" "<<
						pmove->white <<" "<<
						pmove->black<<" "<<endl;
				}
			}

			int close_bracket = -1;
			close_bracket = vlocal->find(")");
			
			if( close_bracket != -1 )
			{
				if( pmove->white != "" )
				{
					if( pmove->black != "" && 
						pmove->black != " " &&
						!isResultString(pmove->black) &&
						pmove->y != 0)
					{
						pmove->atEndWhite = false;
						pmove->atEndBlack = true;
					}
					else if( !isResultString( pmove->white ) && pmove->x != 0 )
					{
						pmove->atEndWhite = true;
						pmove->atEndBlack = false;
					}
					else
					{
						if( pmove->pPrev )
						{
							pmove->pPrev->atEndWhite = false;
							pmove->pPrev->atEndBlack = true;
						}
					}
				}
				else
				{
					if( pmove->pPrev )
					{
						if( pmove->pPrev->black != "" &&
							!isResultString(pmove->pPrev->black))
						{
							if( pmove->pPrev->y != 0 )
							{
								pmove->pPrev->atEndWhite = false;
								pmove->pPrev->atEndBlack = true;
							}
							else if( pmove->pPrev->white != "" )
							{
								if( pmove->pPrev->x != 0 )
								{
									pmove->pPrev->atEndWhite = true;
									pmove->pPrev->atEndBlack = false;
								}
							}
						}
						else if( pmove->pPrev->white != "")
						{
							if( pmove->pPrev->x != 0 && 
								!isResultString(pmove->pPrev->white))
							{
								pmove->pPrev->atEndWhite = true;
								pmove->pPrev->atEndBlack = false;
							}
							else if( pmove->pPrev->pPrev )
							{
								if( pmove->pPrev->pPrev->black != "" )
								{
									pmove->pPrev->pPrev->atEndWhite = false;
									pmove->pPrev->pPrev->atEndBlack = true;
									
								}
								else 
								{
									pmove->pPrev->pPrev->atEndWhite = true;
									pmove->pPrev->pPrev->atEndBlack = false;
								}
							}
							
						}
						else if(pmove->pPrev->pPrev)
						{
							if( pmove->pPrev->pPrev->black != "" )
							{
								pmove->pPrev->pPrev->atEndWhite = false;
								pmove->pPrev->pPrev->atEndBlack = true;
							}
							else 
							{
								pmove->pPrev->pPrev->atEndWhite = true;
								pmove->pPrev->pPrev->atEndBlack = false;
							}
						}
					}
					// try to set the previous one ...
				}
				//cout <<"bracket closed"<<endl;
				int erase = -1;
				erase = vlocal->find(")"); // quick hack to handle multiple brackets
				vlocal->erase(erase,1);	// stored in the same string (we didn't split brackets)
										// more convenient than writing a recursive descent interpreter
				return vlocal;
			}
		}
		return vinpt;
	}


	bool isResultString( string str )
	{
		return ((str == "1-0") || (str == "0-1") || (str== "1/2-1/2") || (str == "*"));
	}


	Move* CreateWithNoSpaceAfterFullstop2(vector<string>::iterator it_s,  
										int fullstop, Move **newmove)
	{		
		char fname[255];

		string tempstr = it_s->substr(0, fullstop);
					

		(*newmove)->info = tempstr;


		string tempstr2 = it_s->substr(fullstop+1);

		//(*newmove)->white = tempstr2;
		int ret = (*newmove)->SetMove(tempstr2, WHITE);
							//pMove->black = *vec_s_iter;
		if( ret != PASSED_TEST )
		{
										// error
		}

		return *newmove;
	}




		// note: some moves are of the format 8. e4 and 8...e5 with a variation in
	// between so these moves are duplicated and so here we have a large
	// watered down function to simply search through all the strings looking
	// for ugly ... text.
	void FixMoves()
	{
		vector<Move>::iterator it = moves.begin();
		vector<Move>::iterator it2 = moves.begin();
		int counter1 = 0;
		int counter2 = 0;
		for(;it!= moves.end(); it++,counter1++)
		{
			string str1 = it->info;
			int has_triple_dots = str1.find("...");
			string strNum;
			strNum.assign(str1, 0, has_triple_dots);
			
			if( has_triple_dots != -1 )
			{
				cout<< strNum << endl;
				for(;it2 != moves.end(); it2++,counter2++)
				{
				
					if( counter2 != counter1 )
					{
						string str2 = it2->info;
						int point_of_dot1 = str1.find_first_of(".",0);
						string strNum2;
						strNum2.assign(it2->info, 0, point_of_dot1);
						if( strNum == strNum2 )
						{
							cout<<"found 8"<<endl;
							cout<<"here is the info for it2: "<<
								it2->info << " and the move for white: "<<
								it2->white << " and the move for black: "<<
								it2->black << endl;


							if( it->black.length() == 0 )
							{
								// merge black
								if( it2->white.length() != 0 )
								{
									cout<< "it white : "<<it->white << endl;
									it->black = it->white;
									it->white = it2->white;

									/*if( it2->variation_count > 0 )
									{
										for (int p = 0; p < it2->variation_count; p++)
										{
											it->getVariationForParse2()->vecMoves.swap(it2->variation[p].vecMoves);
										}
									}*/
									it->info.erase(has_triple_dots+1, 2);
									moves.erase(it2);
								}
								return;
							}
						}
					}
				}
				
			}
		}
	}


	
	vector<string>::iterator vec_s_iter;
	vector<string> vec_moves;
	vector<Move>::iterator move_iter;
	int move_counter;
	vector<HeaderElem> vec_header;
	vector<Move> moves;
	int game_curr_move;
	int white_black;
	int end_white_black;

	bool gameBeginsAtStartPos;
};


class PgnLoad
{
public:
	vector<string> vec_strings;
	vector<HeaderElem> vec_header;
	vector<string> vec_moves;
	vector<string>::iterator vec_s_iter;
	vector<Move> moves;
	vector<Move>::iterator move_iter;
	int move_counter;

	vector<Game*> games;

	PgnLoad(){move_counter = -1;}
	PgnLoad(string filename){}
	~PgnLoad()
	{
		Reset();
		
	}

	void Reset()
	{
		vec_strings.clear();
		vec_header.clear();
		vec_moves.clear();
		moves.clear();
		for( int i = 0; i < games.size(); i++)
		{
			if( games[i] != NULL )
			{
				games[i]->ResetMoveList();
				delete games[i];
				games[i] = NULL;
			}
		}
		games.clear();
	}

	void Startup()
	{

		Game *game = new Game();
		game->pCurrMove = game->pRoot;
		games.push_back(game);
		game->CreateEmptyHeader();
		
	}

	Game* AddGame()
	{
		vec_strings.clear();
		vec_header.clear();
		vec_moves.clear();
		moves.clear();

		Game* game = new Game();
		game->pCurrMove = game->pRoot;
		games.push_back(game);
		game->CreateEmptyHeader();
		return game;
	}

	Game* NewGame()
	{
		vec_strings.clear();
		vec_header.clear();
		vec_moves.clear();
		moves.clear();
		for( int i = 0; i < games.size(); i++)
		{
			if( games[i] != NULL )
			{
				games[i]->ResetMoveList();
				delete games[i];
				games[i] = NULL;
			}
		}
		games.clear();
		Game* game = new Game();
		game->pCurrMove = game->pRoot;
		games.push_back(game);
		game->CreateEmptyHeader();
		return game;

	}

	char GetLastMeaningfulChar(string line)
	{
		for( int i = line.length()-1; i >-1; i--)
		{
			if( (line[i] != ' ') && 
				(line[i] != '\r') &&
				(line[i] != '\t'))
				return line[i];
		}
		return 0;
	}

	Game* LoadPgnFromString(const char* data) // maybe a bit unsafe
	{
		//Appender v;
		//ifstream myfile (filename);
		istringstream myfile(data);
		int currentGame = games.size()-1;
		bool in_header = false;
		
		try
		{
			while ( myfile.good() )
			{
				string line;
				getline (myfile,line);
				int n = line.find(']');
				int n2 = line.find('[');

				if(line.length() == 0 )
					continue;
				
				if( line[0] == '[' && (GetLastMeaningfulChar(line) == ']'))
				//if( (n>0) && (n2==0) && ((n==line.size()-1)||(n==line.size()-2)))
				{
					if( in_header == false )
					{
						Game *game = new Game();
						games.push_back(game);
						currentGame++;
						in_header = true;
					}
					HeaderElem h;
					h.data.assign(line, 0, line.size());
					games[currentGame]->vec_header.push_back(h);
					//vec_header.push_back(h);
					//in_header = true;
				}
				else
				{
					in_header = false;
				}

				if( !in_header ) // then just split the rest by white space
				{ 
					/*istringstream iss(line);
					copy(istream_iterator<string>(iss),
						istream_iterator<string>(),
						back_inserter<vector<string> >(games[currentGame]->vec_moves));
						*/
					Appender v;
										
					tokenize2(v, line.c_str(), " \r\n\t{}()");
					for( int i = 0; i < v.size(); i++ )
					{
						//if( v[i].length() != 0 && v[i] != "\n" && v[i] != "\t" && v[i] != " ")
						games[currentGame]->vec_moves.push_back(v[i]);
					//	cout<<v[i]<<endl;
					}

					
				}
				//cout << line << endl;
			}

			return games[currentGame];
		}
		catch(...)
		{

			return NULL;
		//else cout << "Unable to open file"; 
		}
		return NULL;
	}


	void LoadPgn(string filename)
	{
		//Appender v;
		ifstream myfile (filename);
		int currentGame = games.size()-1;
		bool in_header = false;
		if (myfile.is_open())
		{
			while ( myfile.good() )
			{
				string line;
				getline (myfile,line);
				int n = line.find(']');
				int n2 = line.find('[');

				if(line.length() == 0 )
					continue;
				
				if( line[0] == '[' && ((line[line.length()-1] == ']')||(line[line.length()-2] == ']')))
				{
					if( in_header == false )
					{
						if( games.size() == 1 )
						{
							if( games[0]->vec_moves.size() == 0 )
							{
								delete games[0];
								games[0] = NULL;
								
								games.clear();
							}
							
						}
						Game *game = new Game();
						games.push_back(game);
						currentGame = games.size()-1;
						in_header = true;
					}
					HeaderElem h;
					h.data.assign(line, 0, n+1);
					games[currentGame]->vec_header.push_back(h);
					//vec_header.push_back(h);
					//in_header = true;
				}
				else
				{
					in_header = false;
				}

				if( !in_header ) // then just split the rest by white space
				{ 
					/*istringstream iss(line);
					copy(istream_iterator<string>(iss),
						istream_iterator<string>(),
						back_inserter<vector<string> >(vec_moves));
						*/
					/*istringstream iss(line);
					copy(istream_iterator<string>(iss),
						istream_iterator<string>(),
						back_inserter<vector<string> >(games[currentGame]->vec_moves));*/
					Appender v;
										
					tokenize2(v, line.c_str(), " \n\t{}()");
					for( int i = 0; i < v.size(); i++ )
					{
						games[currentGame]->vec_moves.push_back(v[i]);
						cout<<v[i]<<endl;
					}

				}
				//cout << line << endl;
			}
			myfile.close();
		}
		else cout << "Unable to open file"; 
	}

	void ParseGames()
	{
		for( int i = 0; i < games.size(); i++ )
		{
			games[i]->Parse();
		}
	}

	void PrintGames()
	{
		for( int i = 0; i < games.size(); i++ )
		{
		//	games[i]->Print();
		}
	}


	




	// note: some moves are of the format 8. e4 and 8...e5 with a variation in
	// between so these moves are duplicated and so here we have a large
	// watered down function to simply search through all the strings looking
	// for ugly ... text.
	void FixMoves()
	{
		vector<Move>::iterator it = moves.begin();
		vector<Move>::iterator it2 = moves.begin();
		int counter1 = 0;
		int counter2 = 0;
		for(;it!= moves.end(); it++,counter1++)
		{
			string str1 = it->info;
			int has_triple_dots = str1.find("...");
			string strNum;
			strNum.assign(str1, 0, has_triple_dots);
			
			if( has_triple_dots != -1 )
			{
				cout<< strNum << endl;
				for(;it2 != moves.end(); it2++,counter2++)
				{
				
					if( counter2 != counter1 )
					{
						string str2 = it2->info;
						int point_of_dot1 = str1.find_first_of(".",0);
						string strNum2;
						strNum2.assign(it2->info, 0, point_of_dot1);
						if( strNum == strNum2 )
						{
							cout<<"found 8"<<endl;
							cout<<"here is the info for it2: "<<
								it2->info << " and the move for white: "<<
								it2->white << " and the move for black: "<<
								it2->black << endl;


							if( it->black.length() == 0 )
							{
								// merge black
								if( it2->white.length() != 0 )
								{
									cout<< "it white : "<<it->white << endl;
									it->black = it->white;
									it->white = it2->white;

								/*	if( it2->variation_count_white > 0 )
									{
										for (int p = 0; p < it2->variation_count_white; p++)
										{
											it->getWhiteVariationForParse()->vecMoves.swap(it2->variation[p].vecMoves);
										}
									}*/
									it->info.erase(has_triple_dots+1, 2);
									moves.erase(it2);
								}
								return;
							}
						}
					}
				}
				
			}
		}
	}



	int FileSize(const char* sFileName)
	{
	  std::ifstream f;
	  f.open(sFileName, std::ios_base::binary | std::ios_base::in);
	  if (!f.good() || f.eof() || !f.is_open()) { return 0; }
	  f.seekg(0, std::ios_base::beg);
	  std::ifstream::pos_type begin_pos = f.tellg();
	  f.seekg(0, std::ios_base::end);
	  return static_cast<int>(f.tellg() - begin_pos);
	}

}; // end class pgnload

}; // end namespace PGN

#endif