#ifndef VARIATION_BOARD_FRAME_H
#define VARIATION_BOARD_FRAME_H

#include "headers.h"

class VariationBoardCanvas;
class MyFrame;
class BaseVariationBoard;


class VariationBoardFrame : public wxPanel
{
public:
	VariationBoardFrame(MyFrame *parent);


	void DoSize();
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent &event);





	wxPanel *m_panel;


	wxSizer *sizer;
	wxFont m_font;


	BaseVariationBoard *m_variationBoard;
	MyFrame *m_parent;

	DECLARE_EVENT_TABLE()
};
#endif