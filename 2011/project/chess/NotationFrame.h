#ifndef NOTATION_FRAME_H
#define NOTATION_FRAME_H

#include "headers.h"

class MyCanvas;

class NotationFrame : public wxPanel
{
public:
	NotationFrame(MyFrame *parent, MyCanvas **canvas);


	void DoSize();
	void NotationFrame::OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent &event);

	void ShowAnnotationBox()
	{
		showAnnotationBox = true;
	}

	void HideAnnotationBox()
	{
		showAnnotationBox = false;
	}

	bool ToggleRemoveHeaderInfo()
	{
		removeHeaderInfo = !removeHeaderInfo;
	}

	wxPanel *m_panel;
	MyCanvas *m_canvas;
	wxTextCtrl *m_annotation;
	wxSizer *sizer;
	wxFont m_font;
	bool showAnnotationBox;

	bool removeHeaderInfo;

	MyFrame *m_parent;

	DECLARE_EVENT_TABLE()
};




#endif