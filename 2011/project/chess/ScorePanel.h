#ifndef SCORE_PANEL_H
#define SCORE_PANEL_H

#include "headers.h"

class MyFrame;
class ScoreGraph;

class ScorePanel : public wxPanel
{
public:

	ScorePanel(MyFrame *parent);


	void DoSize();
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent &event);




	wxPanel *m_panel;

	wxSizer *sizer;
	wxFont m_font;


	ScoreGraph *m_scoreGraph;

	MyFrame *m_parent;

	wxString txt1;
	wxString txt2;
	wxString txt3;

	DECLARE_EVENT_TABLE()
};


#endif