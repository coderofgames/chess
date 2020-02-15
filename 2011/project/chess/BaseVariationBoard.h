#ifndef BASE_VARIATION_BOARD_H
#define BASE_VARIATION_BOARD_H

#include "ByteBoard.h"


class MyPipePanel;

class BaseVariationBoard
{
public:

	BaseVariationBoard()
	{
		selectedMove = 0;
		//pp = 0;
		pv=0;
	}

	~BaseVariationBoard()
	{
		if( m_moves.size() > 0 )
		{
			m_moves.clear();
		}
	}

	virtual void ResetBoard() =0;

	ChessBoard internalBoard;

	ChessBoard* GetBoard()
	{
		return &internalBoard;
	}

	//void SetCurrentPipePanel( MyPipePanel *pp );


	vector< VarMoveNode > m_moves;

	int selectedMove;

	//MyPipePanel *pCurrentPipePanel;
	int pv;
};

#endif