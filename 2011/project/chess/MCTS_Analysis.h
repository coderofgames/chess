#ifndef MCTS_ANALYSIS_H
#define MCTS_ANALYSIS_H

#include <vector>
#include <string>

#include "PgnLoad.h"

class MyFrame;


class MCTS_Node
{
public:

	MCTS_Node()
	{
		pMove = NULL;
		col = 0;
	}

	MCTS_Node(PGN::Move* pM, int Col)
	{
		pMove = pM;
		col = Col;
	}

	~MCTS_Node()
	{
		pMove = NULL;
	}

	

	void Set(PGN::Move* pM, int Col )
	{
		pMove = pM;
		col = Col;
	}

	bool IsBlackMove()
	{
		return col == 1;
	}

	PGN::Move* pMove;
	int col;
};

class MCTS_Analysis
{
public:

	MCTS_Analysis(MyFrame* frame)
	{
		m_frame=  frame;
		pStartNode = NULL;
	}

	~MCTS_Analysis()
	{
		delete pStartNode;
	}

	MCTS_Node* GetStartNode(){
		return pStartNode;
	}

	void SetStartNode(PGN::Move *pMove, int col )
	{
		pStartNode = new MCTS_Node(pMove, col);
		startColor = col;
		//pStartNode->pMove = pMove;
		//pStartNode->col = col;
	}


	int startColor;
	vector<PGN::Move*> branches;
	MCTS_Node* pStartNode;
	MyFrame *m_frame;

};



#endif