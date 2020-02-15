#include "PromotionSelectionDialog.h"
#include "ByteBoard.h"

BEGIN_EVENT_TABLE(PromotionSelectionDialaog, wxDialog)
	EVT_BUTTON(QUEEN, PromotionSelectionDialaog::OnQueen)
	EVT_BUTTON(BISHOP, PromotionSelectionDialaog::OnBishop)
	EVT_BUTTON(KNIGHT, PromotionSelectionDialaog::OnKnight)
	EVT_BUTTON(ROOK, PromotionSelectionDialaog::OnRook)
	
END_EVENT_TABLE()

PromotionSelectionDialaog::PromotionSelectionDialaog(wxWindow * parent, ChessBoard *pBoard, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{
	m_pBoard = pBoard;
//	m_displayPanel = new wxPanel(this, PANEL);

	//wxBitmap queenBitmap;
	
	//stopBitmap.LoadFile("icons/dev icons sekkumu/Stop_small.png", wxBITMAP_TYPE_PNG);
//	stopBitmapSelected.LoadFile("icons/dev icons sekkumu/Gear.png", wxBITMAP_TYPE_PNG);

	int txIdxQueen, txIdxRook, txIdxBishop, txIdxKnight;
	if(pBoard->mov == 0 || pBoard->mov == 2 )
	{
		//nbrq	
		//78910
		txIdxQueen = 10;
		txIdxRook = 9;
		txIdxBishop = 8;
		txIdxKnight = 7;
	}
	else
	{
		//1234
		txIdxQueen = 4;
		txIdxRook = 3;
		txIdxBishop = 2;
		txIdxKnight = 1;
	}

	wxBitmap copyQueen(*m_pBoard->svgTextures[txIdxQueen]);
	wxBitmap copyBishop(*m_pBoard->svgTextures[txIdxBishop]);
	wxBitmap copyKnight(*m_pBoard->svgTextures[txIdxKnight]);
	wxBitmap copyRook(*m_pBoard->svgTextures[txIdxRook]);

	wxBitmapButton *QueenBtn = new wxBitmapButton(this,QUEEN, copyQueen, wxDefaultPosition, wxSize(88,88),wxBU_AUTODRAW);//,wxT("&Analyze"));
	wxBitmapButton *BishopBtn = new wxBitmapButton(this,BISHOP, copyBishop, wxDefaultPosition, wxSize(88,88),wxBU_AUTODRAW);//,wxT("&Analyze"));
	wxBitmapButton *KnightBtn = new wxBitmapButton(this,KNIGHT, copyKnight, wxDefaultPosition, wxSize(88,88),wxBU_AUTODRAW);//,wxT("&Analyze"));
	wxBitmapButton *RookBtn = new wxBitmapButton(this,ROOK, copyRook, wxDefaultPosition, wxSize(88,88),wxBU_AUTODRAW);//,wxT("&Analyze"));

	//QueenBtn->SetStyle
	wxSizer *sizerBtns = new wxBoxSizer(wxHORIZONTAL);
	sizerBtns->
        Add(QueenBtn, 0, wxALL, 5);
	sizerBtns->
        Add(BishopBtn, 0, wxALL, 5);
	sizerBtns->
        Add(KnightBtn, 0, wxALL, 5);
	sizerBtns->
        Add(RookBtn, 0, wxALL, 5);

	    this->SetSizer(sizerBtns);
    sizerBtns->Fit(this);
	
}

PromotionSelectionDialaog::~PromotionSelectionDialaog()
{
}

void PromotionSelectionDialaog::OnQueen(wxCommandEvent& evt)
{
	*promoteID = 0;
	this->Close(true);
}

void PromotionSelectionDialaog::OnKnight(wxCommandEvent& evt)
{
	*promoteID = 2;
	this->Close(true);
}

void PromotionSelectionDialaog::OnRook(wxCommandEvent& evt)
{
	*promoteID = 3;
	this->Close(true);
}

void PromotionSelectionDialaog::OnBishop(wxCommandEvent& evt)
{
	*promoteID = 1;
	this->Close(true);
}