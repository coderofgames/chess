#ifndef PROMOTION_SELECTION_DIALOG_H
#define PROMOTION_SELECTION_DIALOG_H

#include "headers.h"

enum
{
	QUEEN = wxID_HIGHEST + 100,
	KNIGHT,
	BISHOP,
	ROOK,
	PANEL,
};

class ChessBoard;

class PromotionSelectionDialaog : public wxDialog
{
public:
	PromotionSelectionDialaog(wxWindow * parent, ChessBoard *pBoard, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(450,200),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~PromotionSelectionDialaog();

	//void OnPaint(wxPaintEvent &event);
	void SetPromoteID( int *id ){ promoteID = id;}
	void OnQueen(wxCommandEvent& evt);
	void OnKnight(wxCommandEvent& evt);
	void OnRook(wxCommandEvent& evt);
	void OnBishop(wxCommandEvent& evt);

	int *promoteID;
	ChessBoard *m_pBoard;
	wxPanel *m_displayPanel;

	DECLARE_EVENT_TABLE()
};


#endif