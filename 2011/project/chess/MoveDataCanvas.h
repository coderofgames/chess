#ifndef MOVE_DATA_CANVAS_H
#define MOVE_DATA_CANVAS_H

#include "headers.h"


class MoveDataCanvas : public wxScrolledWindow
{
public:

	MoveDataCanvas(MyFrame *parent);
	~MoveDataCanvas();


	void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);


	void OnMouseMove(wxMouseEvent &event){}
	void OnMouseLeftClick(wxMouseEvent &event);


	MyFrame *m_frame;

	wxFont m_font;


	DECLARE_EVENT_TABLE()
};

#endif