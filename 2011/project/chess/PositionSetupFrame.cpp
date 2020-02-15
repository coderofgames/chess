#include "PositionSetupFrame.h"
#include "mdi.h"
#include "BoardCanvas.h"
#include "TreeCanvas.h"


BEGIN_EVENT_TABLE(PositionSetupFrame, wxFrame)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_BUTTON(ID_CLEAR_BOARD, PositionSetupFrame::OnClearBoard)
	EVT_BUTTON(ID_START_POS, PositionSetupFrame::OnStartPosition)
	EVT_BUTTON(ID_PASTE_FEN, PositionSetupFrame::OnPasteFEN)
	EVT_BUTTON(ID_COPY_FEN, PositionSetupFrame::OnCopyFEN)
	EVT_BUTTON(wxID_OK, PositionSetupFrame::OnOk)
	EVT_BUTTON(wxID_CANCEL, PositionSetupFrame::OnCancel)
	EVT_BUTTON(ID_FLIP_BOARD, PositionSetupFrame::OnFlipBoard)
	EVT_RADIOBOX  (ID_RADIO_WHITE, PositionSetupFrame::OnRadio)
	EVT_CHECKBOX  (ID_WHITE_QCASTLE, PositionSetupFrame::OnCastleCheck)
	EVT_CHECKBOX  (ID_WHITE_KCASTLE, PositionSetupFrame::OnCastleCheck)
	EVT_CHECKBOX  (ID_BLACK_QCASTLE, PositionSetupFrame::OnCastleCheck)
	EVT_CHECKBOX  (ID_BLACK_KCASTLE, PositionSetupFrame::OnCastleCheck)
	EVT_CHECKBOX(ID_FLIP_BOARD, PositionSetupFrame::OnFlipBoard)
	EVT_CHECKBOX(ID_FIND_PAWN_STRUCTURE, PositionSetupFrame::OnFindPawnStructure)

	//	EVT_SIZE(PositionSetupFrame::OnSize)
	//EVT_RADIOBOX  (ID_RADIO_BLACK, PositionSetupFrame::OnRadio)
END_EVENT_TABLE()



PositionSetupFrame::PositionSetupFrame(MyFrame *parent, bool search_mode)
	:wxFrame((wxWindow*)parent, wxID_ANY, "Position Setup", wxPoint(150,150), wxSize(500,600))
{
	
	m_search_mode = search_mode;
	this->pParent = parent;

	

	wxPanel *panel = new wxPanel(this,wxID_ANY, wxPoint(3,3), wxSize(320,400));//,wxSUNKEN_BORDER);

	this->SetBackgroundColour(panel->GetBackgroundColour());

	m_miniBoard = new MiniBoardCanvas(parent, panel);
	m_miniBoard->SetSize(360,380);
	m_miniBoard->ResetBoard();
	m_miniBoard->GetBoard()->SetBoard(pParent->board_canvas->GetBoard()->Fen());
	m_miniBoard->Show(true);
	//m_miniBoard->CalculateSizeParameters();


	wxSizer *dialogSizer = new wxBoxSizer(wxHORIZONTAL);


	wxSizer *panelRight1 = new wxBoxSizer(wxVERTICAL);
	wxSizer *panelRightUpper = new wxBoxSizer(wxHORIZONTAL);
	wxSizer *panelRightLower = new wxBoxSizer(wxHORIZONTAL);

	

	wxButton *clearBoard = new wxButton(this, ID_CLEAR_BOARD, _("Clear Board"));
	wxButton *startPos = new wxButton(this, ID_START_POS, _("Start Position"));
	wxButton *pasteFen = new wxButton(this, ID_PASTE_FEN, _("Paste FEN"));
	wxButton *copyFen = new wxButton(this, ID_COPY_FEN, _("Copy FEN"));

	wxCheckBox *checkWhiteCanCastleQueenside = new wxCheckBox(this, ID_WHITE_QCASTLE, "White Queen Side");
	wxCheckBox *checkWhiteCanCastleKingside=new wxCheckBox(this, ID_WHITE_KCASTLE, "White King Side");
	wxCheckBox *checkBlackCanCastleQueenside=new wxCheckBox(this, ID_BLACK_QCASTLE, "Black Queen Side");
	wxCheckBox *checkBlackCanCastleKingside=new wxCheckBox(this, ID_BLACK_KCASTLE, "Black King Side");

	checkWhiteCanCastleQueenside->SetValue(true);
	checkWhiteCanCastleKingside->SetValue(true);
	checkBlackCanCastleQueenside->SetValue(true);
	checkBlackCanCastleKingside->SetValue(true);
	
	wxCheckBox *flipBoard=new wxCheckBox(this, ID_FLIP_BOARD, "Flip Board");


	const wxString choices[2] = {"White", "Black"};
	wxRadioBox *rdioBox = new wxRadioBox(this, ID_RADIO_WHITE, "Choose", wxDefaultPosition, wxDefaultSize, 2, choices);// long style = wxRA_SPECIFY_COLS, const wxValidator& validator = wxDefaultValidator, const wxString& name = "radioBox")

//	wxTextCtrl *textCtrl = new wxTextCtrl(this, wxID_ANY, "text");

	
    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
 //   sizerTop->Add(m_lbox, 200, wxGROW | wxALL, 5);

    wxSizer *sizerLeft = new wxBoxSizer(wxHORIZONTAL);

	wxButton * b = new wxButton( this, wxID_OK, _("OK") );
	
	wxButton * c = new wxButton( this, wxID_CANCEL, _("CANCEL") );

	sizerLeft->Add(b, 0, wxEXPAND, 5);
	sizerLeft->Add(c, 0, wxEXPAND, 5);

	sizerTop->Add(panel,0,wxEXPAND, 5);
	sizerTop->Add(sizerLeft,0,wxEXPAND, 5);


	//panelRightUpper->Add(rdioBox, 0, wxALL, 5);
	
	



    //wxSizer *ps_mid = new wxStaticBoxSizer( wxVERTICAL, this, "Castling Rights" );
    //wxSizer *ps_mid_wrap = new wxWrapSizer(wxHORIZONTAL);
	//ps_mid->Add( ps_mid_wrap, 100, wxEXPAND );

	/*
	ps_mid_wrap->Add( checkWhiteCanCastleQueenside,0, wxALIGN_CENTRE|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	ps_mid_wrap->Add( checkWhiteCanCastleKingside,0, wxALIGN_CENTRE|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	ps_mid_wrap->Add( checkBlackCanCastleQueenside,0, wxALIGN_CENTRE|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	ps_mid_wrap->Add( checkBlackCanCastleKingside,0, wxALIGN_CENTRE|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	*/
	//panelRightLower->Add(textCtrl,0, wxALL, 5);


	panelRight1->Add(rdioBox,0, wxALL, 5);
	panelRight1->Add(flipBoard, 0, wxALL, 5);
	panelRight1->Add(pasteFen,0,wxALL, 5);
	panelRight1->Add(copyFen,0,wxALL, 5);
	panelRight1->Add(clearBoard,0, wxALL, 5);
	panelRight1->Add(startPos,0, wxALL, 5);
	//panelRight1->Add(panelRightLower,0, wxALL, 5);
	//panelRight1->Add(ps_mid, wxALL, 5);
	panelRight1->Add( checkWhiteCanCastleQueenside,0, wxALL, 5 );
	panelRight1->Add( checkWhiteCanCastleKingside,0, wxALL, 5 );
	panelRight1->Add( checkBlackCanCastleQueenside,0, wxALL, 5 );
	panelRight1->Add( checkBlackCanCastleKingside,0, wxALL, 5 );

	pawns_only = false;
	find_similar = false;
	if( m_search_mode )
	{

		wxCheckBox *findSimilar=new wxCheckBox(this, ID_FIND_SIMILAR, "Find Similar");
		wxCheckBox *onlyShowPawns=new wxCheckBox(this, ID_FIND_PAWN_STRUCTURE, "Pawn Structure");
		panelRight1->Add( findSimilar,0, wxALL, 5 );
		panelRight1->Add( onlyShowPawns,0, wxALL, 5 );
	}

	dialogSizer->Add(sizerTop, 0, wxEXPAND, 5);
	dialogSizer->Add(panelRight1, 0, wxEXPAND, 5);
	

    this->SetSizer(dialogSizer);
    dialogSizer->Fit(this);

  //  Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(VariationSelectionFrame::OnKeyDown),
   //                     NULL, this);
   // Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(VariationSelectionFrame::OnKeyDown),
    //                    NULL, m_lbox);
}

PositionSetupFrame::~PositionSetupFrame()
{
	if( m_miniBoard )
	{
		delete m_miniBoard;
	}
}

void PositionSetupFrame::OnFindSimilar(wxCommandEvent& event)
{
	if( m_search_mode )
	{
		find_similar = ((wxCheckBox*)FindWindow(ID_FIND_SIMILAR))->GetValue();
		
	}
}

void PositionSetupFrame::OnFindPawnStructure(wxCommandEvent& event)
{
	if( m_search_mode )
	{
		pawns_only = ((wxCheckBox*)FindWindow(ID_FIND_PAWN_STRUCTURE))->GetValue();
		this->m_miniBoard->only_show_pawns = pawns_only;
		this->m_miniBoard->Refresh(true);
	}
}

void PositionSetupFrame::OnClearBoard(wxCommandEvent& WXUNUSED(event))
{
	m_miniBoard->GetBoard()->SetPosition2Kings();
	m_miniBoard->Refresh();
	this->m_miniBoard->SetFocus();
}

void PositionSetupFrame::OnStartPosition(wxCommandEvent& WXUNUSED(event))
{
	m_miniBoard->GetBoard()->SetInitialPosition();
	m_miniBoard->Refresh();
	this->m_miniBoard->SetFocus();
}


void PositionSetupFrame::OnRadio( wxCommandEvent &event )
{
	if( event.GetString() == "White" )
	{
		whoseMove = 0;
		m_miniBoard->GetBoard()->mov = whoseMove;
		wxMessageBox("White Selected", "");
	}
	else
	{
		whoseMove = 1;
		m_miniBoard->GetBoard()->mov = whoseMove;
		wxMessageBox("Black Selected", "");
	}
	this->m_miniBoard->SetFocus();
}


void PositionSetupFrame::OnPasteFEN(wxCommandEvent& WXUNUSED(event))
{
	wxTextDataObject data;
	if (wxTheClipboard->Open())
	{
		{
			wxTheClipboard->GetData( data );
		}
		wxTheClipboard->Close();
	}

	m_miniBoard->GetBoard()->SetBoard(data.GetText().c_str().AsChar());
	m_miniBoard->Refresh();
	this->m_miniBoard->SetFocus();
}

void PositionSetupFrame::OnCopyFEN(wxCommandEvent& WXUNUSED(event))
{
	if (wxTheClipboard->Open())
	{
		{
			string fen = m_miniBoard->GetBoard()->Fen();
			if( fen[fen.length()-1] == '0' )
				fen[fen.length()-1] = '1';			
			wxTextDataObject *td = new wxTextDataObject(fen);
			wxTheClipboard->SetData( td);
		}
		wxTheClipboard->Close();	
	}

	this->m_miniBoard->SetFocus();
}

#define ERROR_MESSAGE wxMessageBox

void PositionSetupFrame::OnOk(wxCommandEvent& event)
{


	int whoseMov = ((wxRadioBox*)FindWindow(ID_RADIO_WHITE))->GetSelection();
	if( whoseMov == 1 )
		m_miniBoard->GetBoard()->mov = 1;

	bool castleWK = ((wxCheckBox*)FindWindow(ID_WHITE_KCASTLE))->GetValue();
	bool castleWQ = ((wxCheckBox*)FindWindow(ID_WHITE_QCASTLE))->GetValue();

	bool castleBK = ((wxCheckBox*)FindWindow(ID_BLACK_KCASTLE))->GetValue();
	bool castleBQ = ((wxCheckBox*)FindWindow(ID_BLACK_QCASTLE))->GetValue();

	if( !castleWQ ) m_miniBoard->GetBoard()->SetupPositionSetWhiteCan_not_CastleQueenside();
	if( !castleWK ) m_miniBoard->GetBoard()->SetupPositionSetWhiteCan_not_CastleKingside();
	if( !castleBQ ) m_miniBoard->GetBoard()->SetupPositionSetBlackCan_not_CastleQueenside();
	if( !castleBK ) m_miniBoard->GetBoard()->SetupPositionSetBlackCan_not_CastleKingside(); 


	string fen = m_miniBoard->GetBoard()->Fen();
	vector<string> vecFen;
	istringstream iss(fen);
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
			back_inserter<vector<string> >(vecFen));
					


	if( fen.length() == 0 || fen.length() > 100 )
	{
		ERROR_MESSAGE( "Error, Ivalid FEN Length!");
		return;
	}

	
		


	int extraQueensCount[2]={0,0};
	int extraRooksCount[2]={0,0};
	int extraKnightsCount[2]={0,0};
	int extraBishopsCount[2]={0,0};
	int bishopsCount[2] = {0,0};
	int knightsCount[2] = {0,0};
	int rooksCount[2] = {0,0};
	int queensCount[2] = {0,0};
	int KingsCount[2]={0,0};
	int pawnsCount[2] = {0,0};

	int forwardSlashCount = 0;

	int rank = 0;
	int file = 0;
	bool badChar = false;
	for( unsigned int i = 0; i < vecFen[0].length(); i++ )
	{
		char c = vecFen[0][i];

		if( c== '/' )
		{
			rank++;
			file = 0;
		}
		else if( (c >= '1') && (c <= '8') )
		{
			file += c-'0';
		}
		else if( isalpha(c) )
		{
			if( c == 'K' )
				KingsCount[WHITE]++;
			else if( c == 'k' )
				KingsCount[BLACK]++;
			else if( c == 'Q' )
				queensCount[WHITE]++;
			else if( c == 'q' )
				queensCount[BLACK]++;
			else if( c == 'B' )
				bishopsCount[WHITE]++;
			else if( c == 'b' )
				bishopsCount[BLACK]++;
			else if( c == 'N' )
				knightsCount[WHITE]++;
			else if( c == 'n' )
				knightsCount[BLACK]++;
			else if( c == 'R' )
				rooksCount[WHITE]++;
			else if( c == 'r' )
				rooksCount[BLACK]++;
			else if( c == 'P' )
			{
				pawnsCount[WHITE]++;
				if( (rank == 0) || (rank == 7) )
				{
					ERROR_MESSAGE("Illegal Position, try again or press cancel to exit");
					return;
				}
			}
			else if( c == 'p' )
			{
				pawnsCount[BLACK]++;
				if( (rank == 0) || (rank == 7) )
				{
					ERROR_MESSAGE("Illegal Position, try again or press cancel to exit");
					return;
				}
			}
			else 
			{
				badChar = true;
				break;
			}


			file++;
				
		}
		else 
		{
			badChar = true;
			break;
		}
	}

	if( badChar )
	{
		ERROR_MESSAGE( "Error, Bad Char found in first part of FEN string, board cleared!");
		return;
	}

	if( rank > 7 )
	{
		ERROR_MESSAGE( "Error, too many ranks (/) detected in FEN string");
		return;
	}

	for( int col = 0; col < 2; col++ )
	{
		if( pawnsCount[col] > 8 )
		{
			ERROR_MESSAGE( "Error, Too many Pawns!");
			return;
		}

		if( KingsCount[col] == 0 )
		{
			ERROR_MESSAGE( "Error, Both Sides Must Have Kings!");
			return;
		}
		if( KingsCount[col] != 1)
		{
			ERROR_MESSAGE( "Error, Too many Kings!");
			return;
		}

		if( queensCount[col] > 1 )
		{
			extraQueensCount[col] = queensCount[col]-1;
		}

		if( bishopsCount[col] > 2 )
		{
			extraBishopsCount[col] = bishopsCount[col]-2;
		}
		if( knightsCount[col] > 2 )
		{
			extraKnightsCount[col] = knightsCount[col]-2;
		}

		if( rooksCount[col] > 2 )
		{
			extraRooksCount[col] = rooksCount[col]-2;
		}

		if( extraRooksCount[col] + extraKnightsCount[col] + 
			extraBishopsCount[col] + extraQueensCount[col] + pawnsCount[col] > 8 )
		{
			ERROR_MESSAGE( "Illegal Position, Number Of Queens must be legal, try checking the number of pawns!");
			return;
		}
	}

	if( vecFen.size() == 1)
	{
		ERROR_MESSAGE( "Error, Not Enough Substrings In FEN!" );
		return;
	}
		

	bool fixMovNums = false;
	if( vecFen.size() > 6 )
	{
		ERROR_MESSAGE( "Error, Too many Substrings in FEN!");
		return; 
	}
	else
	{
		if( vecFen[1].length() > 1 )
		{
			ERROR_MESSAGE( "Error, FEN Format Length Error in 2nd Substring!");
			return;
		}
			
		if( vecFen.size() == 6 )
		{
			if( vecFen[2].length() > 4 || vecFen[2].length() == 0)
			{
				ERROR_MESSAGE( "Error, FEN Format Length Error in 3rd Substring!");
				return;
			}

			if( vecFen[2][0] != '-' )
			{
				for( unsigned int v=0;v<4; v++){
					if( vecFen[2].length() > v && 
						vecFen[2][v] != 'K' &&
						vecFen[2][v] != 'Q' &&
						vecFen[2][v] != 'k' &&
						vecFen[2][v] != 'q')
					{
						ERROR_MESSAGE( "Error, FEN Format error in Castling Rights String!");
						return;
					}
				}
			}


			if( vecFen[3].length() > 2 || vecFen[3].length() == 0 )
			{
				ERROR_MESSAGE( "Error, FEN Format Length Error in empassant string!");
				return;
			}
			else
			{
				if( vecFen[3][0] != '-' )
				{
					if( vecFen[3].length() != 2 )
					{
						ERROR_MESSAGE( "Error, FEN Format Length Error in empassant string!");
						return;
					}

					if( !(vecFen[3][0] >= 'a' && 
							vecFen[3][0] <= 'h' && 
							vecFen[3][1] >= '1' && 
							vecFen[3][1] <= '8'))
					{
						ERROR_MESSAGE( "Error, FEN Format Error in empassant string!");
						return;
					}
				}
			}
				
			if( vecFen[4].length() > 0 && vecFen[4].length() <= 2)
			{
				int fiftyMov = atoi(vecFen[4].c_str());
				if( fiftyMov > 100  || fiftyMov < 0 )
				{
					ERROR_MESSAGE( "Error, bad 50 move string, setting defaults!");
					fixMovNums = true;
				}
			}
			if( vecFen[5].length() > 4 || vecFen[4].length() < 1 )
			{
				ERROR_MESSAGE( "Error, move number string was corrupted, setting defaults!");
				fixMovNums = true;
			}
		}
				
	}
	// hopefully this will fix loading on buggy fens.
	if( vecFen.size() == 4 )
	{
		vecFen.push_back("1");
		vecFen.push_back("25");
	}

	pParent->OnNewGame(event);

	pParent->currentGame->SetHasGeneratedFens();
	if( vecFen[1] != "w" )
	{
		/*pParent->currentGame->pRoot->x = 1;*/
		pParent->currentGame->pRoot->SetMoveFlag(MOVE_FLAG_X);
		pParent->currentGame->pRoot->info = "1";
	}

	//if( fen[fen.length()-1] == '0' )
	//	fen[fen.length()-1] = '1';

	

	pParent->board_canvas->GetBoard()->SetBoard(fen);
	pParent->currentGame->pBase->SetFenBlack(fen);
	pParent->currentGame->pBase->SetFenWhite(fen);

	if( fen != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" )
		pParent->currentGame->gameBeginsAtStartPos = false;
	
	PGN::HeaderElem hdr;
	hdr.data = "";
	hdr.data += "[FEN \"";
	hdr.data += fen;
	hdr.data += "\"]";
	pParent->currentGame->vec_header.push_back(hdr);

	pParent->board_canvas->MyRefresh(true);
	pParent->treeCanvas->MyRefresh();
	pParent->my_canvas->Refresh();
#ifdef USE_CANVAS_SET_FOCUS
	pParent->my_canvas->SetFocus();
#endif

	if( m_search_mode )
	{
		pParent->SetPositionSearchData(this->m_miniBoard->GetBoard(), this->pawns_only);

	}


	Close(true);

	if( m_search_mode )
	{
		pParent->DoPositionSearch( this->pawns_only, this->find_similar);
	}
}

#undef ERROR_MESSAGE

void PositionSetupFrame::OnCancel(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void PositionSetupFrame::OnCastleCheck(wxCommandEvent& event)
{
	switch( event.GetId() )
	{
	case ID_WHITE_QCASTLE: m_miniBoard->GetBoard()->SetupPositionSetWhiteCanCastleQueenside(); break;
	case ID_WHITE_KCASTLE: m_miniBoard->GetBoard()->SetupPositionSetWhiteCanCastleKingside(); break;
	case ID_BLACK_QCASTLE: m_miniBoard->GetBoard()->SetupPositionSetBlackCanCastleQueenside(); break;
	case ID_BLACK_KCASTLE: m_miniBoard->GetBoard()->SetupPositionSetBlackCanCastleKingside(); break;
	}

	this->m_miniBoard->SetFocus();
}

void PositionSetupFrame::OnFlipBoard(wxCommandEvent& WXUNUSED(event))
{
	m_miniBoard->Flip();
	m_miniBoard->Refresh();
	this->m_miniBoard->SetFocus();
}

void PositionSetupFrame::OnSize(wxSizeEvent &event)
{
	//wxSize sz(this->GetClientSize());
//		m_miniBoard->SetSize(m_miniBoard->GetParent()->GetSize().GetWidth()*320/320,m_miniBoard->GetParent()->GetSize().GetWidth()*380/400);
//	m_miniBoard->Refresh();
	this->m_miniBoard->SetFocus();
}
