#ifndef FEN_TREE_H
#define FEN_TREE_H

#include "headers.h"

class FenTreeNode
{
public:
	FenTreeNode(){}
	~FenTreeNode(){}

	FenTreeNode *prev;
	FenTreeNode *branches;
	string fen;
};

class FenTree
{
public:

	void PushNodeIntoTree(FenTreeNode* newNode){}
	FenTreeNode *root;
};

#endif