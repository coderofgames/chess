#ifndef TREE_CANVAS_H
#define TREE_CANVAS_H

#include "headers.h"

class TreeCanvasElem
{
public:
	TreeCanvasElem()
	{
		bSelected = false;
		bRoot = false;
		pNext = NULL;
		pPrev = NULL;
		isBookMove = false;
		sanMove = "";
		bookMove = "";
		pMove = NULL;
		pBookEntry = NULL;
		white_black = false;
		rect = wxRect(0,0,1,1);
	}

	~TreeCanvasElem()
	{
		if( pNext )
		{
		//	delete pNext;
		}
	}

	TreeCanvasElem* pNext;
	TreeCanvasElem* pPrev;

	bool active;

	PGN::Move* pMove;
	bool white_black; 
	
	wxString score;
	wxString sanMove;
	string bookMove;
	bool isBookMove;
	bool bRoot;
	bool bSelected;

	void* pBookEntry;
	wxRect rect;
	wxRect scoreRect;
};

class TreeCanvas : public wxScrolledWindow
{
public:

	TreeCanvas(MyFrame *parent);
	~TreeCanvas();

	void MyRefresh(bool clearScreen=true);
	void OnPaint(wxPaintEvent &event);
	void OnPaint2(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);
	void SetSelection(int sel)
	{
		selectedElem = tree_elems[0];
		selection = sel;
	}

	int GetSelection()
	{
		if( selectedElem == tree_elems[0] )
			selection = 0;

		return selection;
	}

	void SetupTreeElems();
	void SetupTreeElemsNonStandardStartPos();

	void OnMouseMove(wxMouseEvent &event){}
	void OnMouseLeftClick(wxMouseEvent &event);

	void IncrementSelection();
	void DecrementSelection();

	MyFrame *m_frame;
	bool m_useContext;
	int selection;
	int numSelectionsThisMove;
	
	TreeCanvasElem *tree_elems[255];
	int numElems;
	TreeCanvasElem *root_elem;
	TreeCanvasElem* selectedElem;

	int visitTreeCount;

	PGN::Move* lastMovePtr;


	bool showVariationRoot;

	wxFont m_font;
	wxPen normalPen;
	wxBrush normalRectBrush;
	wxBrush selectedRectBrush;
	wxBrush rootBrush;
	wxBrush bookRectBrush;


	DECLARE_EVENT_TABLE()
};

#endif