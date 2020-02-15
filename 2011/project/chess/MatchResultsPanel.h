#ifndef MATCH_RESULTS_PANEL_H
#define MATCH_RESULTS_PANEL_H

#include "Headers.h"

class MatchResultsCanvas;

class MatchResultsPanel : public wxPanel
{
public:
	MatchResultsPanel(MyFrame *parent);


	void DoSize();
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent &event);





	wxPanel *m_panel;


	wxSizer *sizer;
	wxFont m_font;


	MyFrame *m_parent;

	MatchResultsCanvas *m_canvas;

	DECLARE_EVENT_TABLE()
};

#endif