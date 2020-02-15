#ifndef GAME_H
#define GAME_H

#include "ByteBoard.h"
#include "BoardCanvas.h"
#include "PgnLoad.h"

class Game
{
public:

	Game(){}
	~Game(){}



	void SetChessBoard(ChessBoard *pBoard){chess_board = pBoard;}
	void SetBoardCanvas(BoardCanvas* pBoardCanvas){
		m_pBoardCanvas = pBoardCanvas;
	}
	void SetPGN(PGN::Game* pCurrPGN)
	{
		m_pCurrPGN = pCurrPGN;
	}

	PGN::Game * GetPGN(){ return m_pCurrPGN;}
	ChessBoard * GetBoard(){return chess_board;}
	BoardCanvas * GetBoardCanvas() { return m_pBoardCanvas;}


	void NewGame();
	void SetMove(string epn);


private:
	BoardCanvas* m_pBoardCanvas;
	ChessBoard *chess_board;
	PGN::Game* m_pCurrPGN;
};

#endif