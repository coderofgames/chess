#ifndef MATCH_RESULTS_CANVAS_H
#define MATCH_RESULTS_CANVAS_H

#include "headers.h"




class MatchResultsCanvas : public wxScrolledWindow
{
public:

	MatchResultsCanvas(MyFrame *parent, wxWindow *winParent);
	~MatchResultsCanvas();


	void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);


	void OnMouseMove(wxMouseEvent &event){}
	void OnMouseLeftClick(wxMouseEvent &event);



	MyFrame *m_frame;

	wxFont m_font;
	wxFont m_font2;
	wxFont m_font3;
	wxBitmap m_bmp;



	DECLARE_EVENT_TABLE()
};
#endif