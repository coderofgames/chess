#ifndef ENGINE_ANALYSIS_CANVAS_H
#define ENGINE_ANALYSIS_CANVAS_H

#include "headers.h"


class EngineAnalysisCanvas : public wxScrolledWindow
{
public:

	EngineAnalysisCanvas(MyPipePanel* parent, MyFrame *myframe);
	~EngineAnalysisCanvas();

	void OnScroll(wxScrollEvent &event)
	{
		needsRefresh = true;
	}
	void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);
	/*void SetSelection(int sel)
	{
		selection = sel;
	}

	int GetSelection()
	{
		return selection;
	}*/

	void OnMouseMove(wxMouseEvent &event){}
	void OnMouseLeftClick(wxMouseEvent &event);

	void IncrementSelection();
	void DecrementSelection();

	void RefreshGameMode()
	{
		line_pos_game_mode = 0;
		clearFlag = true;
	}

	void OnSize(wxSizeEvent& WXUNUSED(evt))
    {
		needsRefresh = true;
        //Refresh();
    }
	


	MyPipePanel* m_parent;
	MyFrame *m_frame;
	bool m_useContext;
	int selection;
	bool needsRefresh;

	int line_pos_game_mode;
	bool clearFlag;

	// options
	int fontSize;
    wxFont fontRich;

	bool whitetoMove;
	bool variationClicked;
	VarMoveNode *pSel;

	DECLARE_EVENT_TABLE()
};

#endif