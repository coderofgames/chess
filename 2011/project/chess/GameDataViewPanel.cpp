#include "GameDataViewPanel.h"
#include "mdi.h"
#include "PgnLoad.h"
#include "TreeCanvas.h"
#include "NotationFrame.h"

#define ID_MUSIC_CTRL wxID_HIGHEST + 10001

BEGIN_EVENT_TABLE(GameDataViewPanel, wxPanel)
	//EVT_PAINT  (NotationFrame::OnPaint)
    //EVT_SIZE(NotationFrame::OnSize)
	 //EVT_DATAVIEW_SELECTION_CHANGED(ID_MUSIC_CTRL, GameDataViewPanel::OnSelectionChanged)
	 EVT_LEFT_DOWN (GameDataViewPanel::OnMouseLeftClick)
END_EVENT_TABLE()

GameDataViewPanel *g_dataView = 0;
GameDataViewPanel::GameDataViewPanel(MyFrame *parent) : wxPanel(parent, wxID_ANY)
{
	m_frame = parent;
    unsigned long style = 0;
    /*if (GetMenuBar()->FindItem(ID_SINGLE)->IsChecked())
        style |= wxDV_SINGLE;*/
    //if (GetMenuBar()->FindItem(ID_MULTIPLE)->IsChecked())
        style |= wxDV_MULTIPLE;
    //if (GetMenuBar()->FindItem(ID_ROW_LINES)->IsChecked())
        style |= wxDV_ROW_LINES;
   // if (GetMenuBar()->FindItem(ID_HORIZ_RULES)->IsChecked())
        style |= wxDV_HORIZ_RULES;
   // if (GetMenuBar()->FindItem(ID_VERT_RULES)->IsChecked())
        style |= wxDV_VERT_RULES;
     lc = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, style );



	g_dataView=  this;
	lc->AppendToggleColumn( "Analyze" );
    lc->AppendTextColumn( "White" );
	lc->AppendTextColumn( "Black" );
	lc->AppendTextColumn( "Result" );
	lc->AppendTextColumn( "Event" );
	lc->AppendTextColumn( "Date" );
    lc->AppendProgressColumn( "Progress" );

	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(lc, 5, wxEXPAND);

	
	    Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(GameDataViewPanel::OnSelectionChanged),
                        NULL, lc);


	SetSizer(m_sizer);
	m_sizer->Fit(this);
}

GameDataViewPanel::~GameDataViewPanel()
{
}


void GameDataViewPanel::AppendGameList(PGN::PgnLoad *gameList)
{
	//lc->ClearColumns();
	lc->GetStore()->DeleteAllItems();
	wxVector<wxVariant> data;
	for( unsigned int i = 0; i < gameList->games.size(); i++ )
	{
		data.clear();
		data.push_back(false);
		data.push_back(gameList->games[i]->GetHeaderElem("White "));
		data.push_back(gameList->games[i]->GetHeaderElem("Black "));
		data.push_back(gameList->games[i]->GetHeaderElem("Result "));
		data.push_back(gameList->games[i]->GetHeaderElem("Event "));
		data.push_back(gameList->games[i]->GetHeaderElem("Date "));
		data.push_back( long(0) );
		lc->AppendItem(data);
	}
	Refresh(true);
}


void GameDataViewPanel::SelectItem(int idx)
{
	lc->Unselect(lc->GetStore()->GetItem(this->selectedRow));
	selectedRow = idx;
	lc->Select(lc->GetStore()->GetItem(idx));
}

void GameDataViewPanel::SetSelectedItemProgress(int p)
{
	lc->GetStore()->SetValueByRow(p,selectedRow,6);
}

void GameDataViewPanel::CheckAllCheckBoxes()
{
	for( int i = 0; i < m_frame->pgnLoad.games.size(); i++ )
	{
		
		lc->GetStore()->SetValueByRow(true,i,0);
	}
	lc->Update();
	this->Refresh();
	//m_frame->m_mgr.Update();
}

void GameDataViewPanel::Unselect(int item)
{
	lc->GetStore()->SetValueByRow(false,item,0);
}

bool GameDataViewPanel::IsSelected(int item)
{
	wxVariant val;
	lc->GetStore()->GetValueByRow(val,item,0);

	return val.GetBool();
}

void GameDataViewPanel::UnCheckAllCheckBoxes()
{
	for( int i = 0; i < m_frame->pgnLoad.games.size(); i++ )
	{
		lc->GetStore()->SetValueByRow(false,i,0);
	}
	lc->Update();
	this->Refresh();
}

void GameDataViewPanel::OnSelectionChanged( wxDataViewEvent &event )
{
    //if(!m_log)
     //   return;

	//if( lc )
	
		
		
		//wxDataViewItem item = lc->GetSelection(); 
		//event.GetI
	if( g_dataView->lc->GetStore()->GetRow(event.GetItem()) < g_dataView->m_frame->pgnLoad.games.size() )
	{
		g_dataView->selectedRow = g_dataView->lc->GetStore()->GetRow(event.GetItem());

		g_dataView->m_frame->currentGame = g_dataView->m_frame->pgnLoad.games[g_dataView->lc->GetStore()->GetRow(event.GetItem())];

		if( !g_dataView->m_frame->currentGame->HasGeneratedFens() )
		{
			if( !g_dataView->m_frame->currentGame->Parse() )
			{
				wxMessageBox(wxString::Format("Parse Error for Game %s",g_dataView->m_frame->currentGame->GetParseError()));
				g_dataView->m_frame->pgnLoad.EraseGameAtIndex(g_dataView->lc->GetStore()->GetRow(event.GetItem()));
				g_dataView->m_frame->currentGame = g_dataView->m_frame->pgnLoad.games[0];
				g_dataView->AppendGameList(&g_dataView->m_frame->pgnLoad);
				return;
			}

			g_dataView->m_frame->board_canvas->GetBoard()->SetInitialPosition();

			PGN::Move * pMove = g_dataView->m_frame->currentGame->pRoot;
			while(pMove)
			{
				if( pMove->white != "" )
				{

					g_dataView->m_frame->board_canvas->GetBoard()->parseMove(pMove->white);

					
					pMove->SetFenWhite(g_dataView->m_frame->board_canvas->GetBoard()->Fen());
					
					
					pMove->SetWhiteMoveSquares(g_dataView->m_frame->board_canvas->GetBoard()->currMove);
					g_dataView->m_frame->board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
				}
				else g_dataView->m_frame->board_canvas->GetBoard()->mov = 1;

				//time_01 = sw2.Time();
				g_dataView->m_frame->board_canvas->GetBoard()->parseMove(pMove->black);
				//time_02 = sw2.Time();
				//total_time_in_variation += time_02 - time_01;
				//time_01 = sw2.Time();
				pMove->SetFenBlack(g_dataView->m_frame->board_canvas->GetBoard()->Fen());
				//time_02 = sw2.Time();
				//total_time_in_variation += time_02 - time_01;
				pMove->SetBlackMoveSquares(g_dataView->m_frame->board_canvas->GetBoard()->currMove);
				g_dataView->m_frame->board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				
				//currentGame->NextMove();
				pMove->PrepareAnnotations();
				g_dataView->m_frame->CreateFenForVariation(pMove);
				g_dataView->m_frame->board_canvas->GetBoard()->SetBoard( pMove->getFenBlack());
				pMove = pMove->pNext;
			}

			g_dataView->m_frame->currentGame->SetHasGeneratedFens();
		

		}

		g_dataView->m_frame->currentGame->ParseHeader();
		g_dataView->m_frame->currentGame->pCurrMove = g_dataView->m_frame->currentGame->pBase;
		g_dataView->m_frame->currentGame->ResetMoveList();
		g_dataView->m_frame->my_canvas->needsRefresh = true;
		g_dataView->m_frame->my_canvas->move_count = 0;
		g_dataView->m_frame->my_canvas->white_black_move = WHITE;
		g_dataView->m_frame->my_canvas->ResetBoard();
		g_dataView->m_frame->my_canvas->Refresh(true);
		g_dataView->m_frame->treeCanvas->Refresh(true);
		g_dataView->m_frame->noteFrame->Refresh(true);
		g_dataView->m_frame->board_canvas->GetBoard()->SetBoard(g_dataView->m_frame->currentGame->pBase->m_fenBlack);
		g_dataView->m_frame->board_canvas->MyRefresh(false);
		g_dataView->m_frame->m_mgr.Update();
	}
	

}

void GameDataViewPanel::OnMouseLeftClick(wxMouseEvent &event)
{
/*	wxDataViewItem item = lc->GetSelection();
	if( lc->GetStore()->GetRow(item) < m_frame->pgnLoad.games.size() )
		m_frame->currentGame = m_frame->pgnLoad.games[lc->GetStore()->GetRow(item)];
	else 
		m_frame->currentGame = m_frame->pgnLoad.games[0];*/
}