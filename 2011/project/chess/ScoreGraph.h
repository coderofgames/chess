#ifndef SCORE_GRAPH_H
#define SCORE_GRAPH_H

#include "headers.h"

class ScorePanel;

class ScoreElem
{
public:

	wxRect rct;
	string san;
	double score;
};

class ScoreGraph : public wxScrolledWindow
{
public:

	ScoreGraph(wxWindow *parent, MyFrame* frame);
	~ScoreGraph();

	void SetScorePanel( ScorePanel* scorePanel );
	void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);


	void OnMouseMove(wxMouseEvent &event){}
	void OnMouseLeftClick(wxMouseEvent &event);

	void OnScroll(wxScrollEvent &event)
	{
		scrollControl = true;
	}

	void mouseReleased(wxMouseEvent& event)
	{
		scrollControl = false;
	}


	MyFrame *m_frame;
	double global_offset;
	double curr_draw_pos;

	ScorePanel *m_scorePanel;

	vector< ScoreElem > scoreElems;

	bool scrollControl;

	DECLARE_EVENT_TABLE()
};

#endif