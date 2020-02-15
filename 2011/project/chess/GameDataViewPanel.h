#ifndef GAME_DATA_VIEW_PANEL
#define GAME_DATA_VIEW_PANEL

#include "headers.h"

class MyFrame;

class GameDataViewPanel : public wxPanel
{
public:

	GameDataViewPanel(MyFrame *parent);
	~GameDataViewPanel();

	void AppendGameList(PGN::PgnLoad *gameList);
	void SelectItem(int idx);
	void SetSelectedItemProgress(int p);
	void OnSelectionChanged( wxDataViewEvent &event );
	void OnMouseLeftClick(wxMouseEvent &event);
	void CheckAllCheckBoxes();
	void UnCheckAllCheckBoxes();

	bool IsSelected(int item);
	void Unselect(int item);



	wxSizer *m_sizer;
	wxDataViewListCtrl* lc;
	MyFrame* m_frame;

	int selectedRow;

	DECLARE_EVENT_TABLE()
};

#endif