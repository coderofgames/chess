#include "EngineAnalysisCanvas.h"
#include "mdi.h"
#include "BoardCanvas.h"
#include "MyPipePanel.h"


BEGIN_EVENT_TABLE(EngineAnalysisCanvas, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (EngineAnalysisCanvas::OnPaint)
	//EVT_IDLE (TreeCanvas::OnIdle)
	EVT_LEFT_DOWN (EngineAnalysisCanvas::OnMouseLeftClick)
	
END_EVENT_TABLE()

EngineAnalysisCanvas::EngineAnalysisCanvas(MyPipePanel* parent, MyFrame *myframe)
        : wxScrolledWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxHSCROLL |wxFULL_REPAINT_ON_RESIZE )
{
	//wxSize sz(GetClientSize());
	m_parent = parent;
	m_frame = myframe;
	m_useContext = true;
	selection = 0;
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	SetDoubleBuffered(true);
	//SetVirtualSize(sz.GetWidth()*20, sz.GetHeight());
	SetScrollbars( 5, 5, 1024, 100 );	
	
	line_pos_game_mode=0;
	clearFlag = false;
	this->SetBackgroundColour(*wxWHITE);

	fontSize = 8;
    fontRich= wxFont(fontSize, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD);
}

EngineAnalysisCanvas::~EngineAnalysisCanvas()
{
}



void EngineAnalysisCanvas::OnPaint(wxPaintEvent &event)
{
	MyPipePanel* pp = m_parent;//m_frame->m_pipePanel[0];

	if( pp->isReading )
		return;

	MPVMoveNodeVectorArray* pVecArray = pp->GetMPVNodeVectorArray();

	if( pp->GetMode() == 0 )
	{
		if( !pVecArray->HasPVChangedThisFrame())
			return;
	}
	else if( pp->GetMode() == 1 )
	{
		if( !pp->GetGameModeDepthPV()->HasPVChangedThisFrame() )
		{
			return;
		}
	}
    
	wxPaintDC pdc(this);

	wxSize sz(GetClientSize());
	
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

    m_frame->PrepareDC(dc);





	dc.Clear();

#if wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();
//	DrawGraphics();



    
	gc->SetFont(fontRich,*wxBLACK);




/*	gc->PushState();
		//gc->SetBrush(*wxBLUE);
		gc->SetBrush( wxBrush( *wxWHITE,wxSOLID));
		gc->DrawRectangle(0,16,26, sz.GetHeight());
	//	gc->SetBrush( wxBrush( *wxWHITE,wxSOLID));
		gc->DrawRectangle(26,16,48, sz.GetHeight());
	//	gc->SetBrush( wxBrush( *wxWHITE,wxSOLID));
		gc->DrawRectangle(26+48,16,sz.GetWidth(), sz.GetHeight());
	gc->PopState();*/
	//if((pp->GetMode() == 0) || clearFlag)
	{
		gc->PushState();
		//gc->SetBrush(*wxBLUE);
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush( wxBrush( wxColour( 255, 255, 255, 255),wxSOLID));
			gc->DrawRectangle(0,0,sz.GetWidth(), sz.GetHeight());
			gc->SetPen(*wxBLACK);
		gc->PopState();
		clearFlag = false;
		//Scroll(0, 0);
	}
//	gc->DrawText(" PV  Score  Moves",0, 0,0);
	
	gc->PushState();
		//gc->SetBrush(*wxBLUE);
		int localXX, localYY;
		CalcScrolledPosition(0,0,&localXX, &localYY);
		gc->SetBrush( wxBrush( wxColour( 100, 100, 255, 128),wxSOLID));
		gc->DrawRectangle(localXX,0,sz.GetWidth(), 16);
	gc->PopState();


	if( pp->GetMode() == 0 )
	{

		gc->DrawText("  PV  Score  Depth  Moves",0, 0,0);
	

		for( int i = 0; i < pp->GetNumKbtzLines(); i++ )
		{
			string str1 = " ";
			char buf[3];
			itoa(i+1, buf, 10);
		
			str1 += buf;
			str1 += ":  ";
			str1 += "(";
			str1 += pVecArray->mpvs[i].score_str;
			str1 += ") ";

			int scoreLen = pVecArray->mpvs[i].score_str.length();
			if( scoreLen < 5 )
			{
				for( int slen = 0; slen < 5-scoreLen; slen++ )
					str1+= " ";
			}
			//char buf2[10];
			//sprintf(buf2, "%d  ", pVecArray->mpvs[i].depth );
			str1 += pVecArray->mpvs[i].depthStr;
			str1+= "   ";
			int depthLen = pVecArray->mpvs[i].depthStr.length();
			if( depthLen < 2 )
			{
				str1+= " ";
			}
			for( int j = 0; j < pVecArray->mpvs[i].moves->size(); j++ )
			{
				if( j== 0 )
				{
					if( !(*pVecArray->mpvs[i].moves)[j].whitesMove )
					{
						str1 += (*pVecArray->mpvs[i].moves)[j].info;
						str1 += "...";
					}
					else
					{
						str1 += (*pVecArray->mpvs[i].moves)[j].info;
						str1 += ".";
					}
				}
				else
				{
					if( (*pVecArray->mpvs[i].moves)[j].whitesMove )
					{
						str1 += (*pVecArray->mpvs[i].moves)[j].info;
						str1 += ".";
					}
					else
					{
						str1 += " ";
					}
				}
				str1 += (*pVecArray->mpvs[i].moves)[j].pgnMove;

				if( !(*pVecArray->mpvs[i].moves)[j].whitesMove )
				{
					str1 += " ";
				}

			}
			gc->DrawText(str1.c_str(),0, (i+1)*2*fontSize,0);
		}

	}
	else if( pp->GetMode()==1 )
	{

		gc->DrawText("  PV  Score  Depth  Moves",0, 0,0);
	
		int drawMode = 0;
		if( drawMode )
		{
			string str1 = " ";
		//	char buf[3];
		//	itoa(i+1, buf, 10);
		
		//	str1 += buf;
			str1 += "pv:  ";
			str1 += "(";
			str1 += pp->GetGameModePV()->score_str;
			str1 += ") ";

			int scoreLen = pp->GetGameModePV()->score_str.length();
			if( scoreLen < 5 )
			{
				for( int slen = 0; slen < 5-scoreLen; slen++ )
					str1+= " ";
			}
			//char buf2[10];
			//sprintf(buf2, "%d  ", pVecArray->mpvs[i].depth );
			str1 += pp->GetGameModePV()->depthStr;
			str1+= "   ";
			int depthLen = pp->GetGameModePV()->depthStr.length();
			if( depthLen < 2 )
			{
				str1+= " ";
			}
			for( int j = 0; j < pp->GetGameModePV()->moves->size(); j++ )
			{
				if( j== 0 )
				{
					if( !(*pp->GetGameModePV()->moves)[j].whitesMove )
					{
						str1 += (*pp->GetGameModePV()->moves)[j].info;
						str1 += "...";
					}
					else
					{
						str1 += (*pp->GetGameModePV()->moves)[j].info;
						str1 += ".";
					}
				}
				else
				{
					if( (*pp->GetGameModePV()->moves)[j].whitesMove )
					{
						str1 += (*pp->GetGameModePV()->moves)[j].info;
						str1 += ".";
					}
					else
					{
						str1 += " ";
					}
				}
				str1 += (*pp->GetGameModePV()->moves)[j].pgnMove;

				if( !(*pp->GetGameModePV()->moves)[j].whitesMove )
				{
					str1 += " ";
				}

			}
			gc->DrawText(str1.c_str(),0, (++line_pos_game_mode)*2*fontSize,0);
			int pixPerUnitX;
			int pixPerUnitY;
			GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);
			Scroll(0, line_pos_game_mode / pixPerUnitY);
		}
		else
		{
			for( int i = pp->GetGameModeDepthPV()->GetMaxDepth()-1; i >=0 ; i-- )
			{
				//if( pp->GetGameModeDepthPV()->m_moves[i].moves.size() == 0 )
				//	continue;

				string str1 = " PV";

				str1 += ":  ";
				str1 += "(";
				str1 += pp->GetGameModeDepthPV()->m_moves[i].score_str;
				str1 += ") ";

				int scoreLen = pp->GetGameModeDepthPV()->m_moves[i].score_str.length();
				if( scoreLen < 5 )
				{
					for( int slen = 0; slen < 5-scoreLen; slen++ )
						str1+= " ";
				}
				//char buf2[10];
				//sprintf(buf2, "%d  ", pVecArray->mpvs[i].depth );
				str1 += pp->GetGameModeDepthPV()->m_moves[i].depthStr;
				str1+= "   ";
				int depthLen = pp->GetGameModeDepthPV()->m_moves[i].depthStr.length();
				if( depthLen < 2 )
				{
					str1+= " ";
				}
				for( int j = 0; j < pp->GetGameModeDepthPV()->m_moves[i].moves->size(); j++ )
				{
					if( j== 0 )
					{
						if( !(*pp->GetGameModeDepthPV()->m_moves[i].moves)[j].whitesMove )
						{
							str1 += (*pp->GetGameModeDepthPV()->m_moves[i].moves)[j].info;
							str1 += "...";
						}
						else
						{
							str1 += (*pp->GetGameModeDepthPV()->m_moves[i].moves)[j].info;
							str1 += ".";
						}
					}
					else
					{
						if( (*pp->GetGameModeDepthPV()->m_moves[i].moves)[j].whitesMove )
						{
							str1 += (*pp->GetGameModeDepthPV()->m_moves[i].moves)[j].info;
							str1 += ".";
						}
						else
						{
							str1 += " ";
						}
					}
					str1 += (*pp->GetGameModeDepthPV()->m_moves[i].moves)[j].pgnMove;

					if( !(*pp->GetGameModeDepthPV()->m_moves[i].moves)[j].whitesMove )
					{
						str1 += " ";
					}

				}
				if( i == pp->GetGameModeDepthPV()->GetMaxDepth()-1 )
					gc->SetFont(fontRich,*wxRED);

				gc->DrawText(str1.c_str(),0, (pp->GetGameModeDepthPV()->GetMaxDepth()-i)*2*fontSize,0);

				gc->SetFont(fontRich,*wxBLACK);
				/*int pixPerUnitX;
				int pixPerUnitY;
				GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);
				
				Scroll(0,0);
				Scroll(0, (i+1)*2*fontSize / pixPerUnitY);*/
			}
		}
	}
/*
	//wxGraphicsPath pth = gc->CreatePath();

	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;
	


    wxDouble margin = 10 ;
	wxDouble width = sz.GetWidth() * 0.75 ;
    wxDouble radius = 10 ;

    dc.SetPen( wxPen( wxColour( 128, 0, 0, 255 ),4, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 255, 0, 0, 255),wxSOLID));


	wxRect r(margin,margin,width,sz.GetHeight()*0.75) ;

    //dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;

	if( whiteMove )
	{
		//if(  m_frame->currentGame->pCurrMove->pVarWhite != NULL )
		{
			PGN::Move *temp = m_frame->currentGame->pCurrMove->pNext;
			int count = 0;
			while( temp )
			{
				wxRect r2(margin+5,margin+25*count,width-margin-5,25) ;
				dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
				dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 128),wxSOLID));
				dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
				if( count == selection )
				{
					dc.SetPen( *wxTRANSPARENT_PEN ) ;
					dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
					dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
				}
				dc.SetTextForeground( wxColour(0,0,0,255) );
				dc.SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
				string textStr = temp->info;
				textStr+=".";
				textStr+=temp->white;	
				dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );

				count++;
				temp = temp->pVarWhite;
			}
		}
	}
	else
	{
		//if(  m_frame->currentGame->pCurrMove->pVarBlack != NULL )
		{
			PGN::Move *temp = m_frame->currentGame->pCurrMove;
			int count = 0;
			while( temp )
			{
				wxRect r2(margin+5,margin+25*count,width-margin-5,25) ;
				dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
				dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 128),wxSOLID));	
				dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
				if( count == selection )
				{
					dc.SetPen( *wxTRANSPARENT_PEN ) ;
					dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
					dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
				}
				
				dc.SetTextForeground( wxColour(0,0,0,255) );
				dc.SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
				string textStr = temp->info;
				textStr+="...";
				textStr+=temp->black;
				dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
				

				count++;
				temp = temp->pVarBlack;
			}
		}
	}

	
   /* dc.SetPen( wxPen( wxColour( 0, 0, 128, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 255),wxSOLID));

    

    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;

    dc.SetPen( wxPen( wxColour( 128, 128, 0, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 192, 192, 0, 255),wxSOLID));

    r.Offset( width * 0.8 , width *0.5 ) ;

    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;

    dc.SetPen( *wxTRANSPARENT_PEN ) ;
    dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
    dc.DrawRoundedRectangle( 0 , margin + width / 2 , width * 3 , 100 , radius) ;

    dc.SetTextForeground( wxColour(255,255,0,128) );
    dc.SetFont( wxFont( 40, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
    dc.DrawText( wxT("Hello!"), 120, 80 );
	*/
#endif
}


void EngineAnalysisCanvas::OnIdle(wxIdleEvent &event)
{
	event.Skip();
}


void EngineAnalysisCanvas::OnMouseLeftClick(wxMouseEvent &event)
{
	/*
#if wxUSE_STATUSBAR
    wxClientDC dc(this);
    PrepareDC(dc);
   // m_owner->PrepareDC(dc);


    wxString str;
   // str.Printf( wxT("Current mouse position: %d,%d"), (int)x, (int)y );
   // m_owner->SetStatusText( str );
#else
    wxUnusedVar(event);
#endif // wxUSE_STATUSBAR
    wxPoint pos = event.GetPosition();
    long x = pos.x;//dc.DeviceToLogicalX( pos.x );
    long y = pos.y;//dc.DeviceToLogicalY( pos.y );


	wxSize sz(GetClientSize());
    wxDouble margin = 10 ;
	wxDouble width = sz.GetWidth() * 0.75 ;
    wxDouble radius = 10 ;

bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	if( whiteMove )
	{

		if( m_frame->currentGame->pCurrMove->pNext == NULL )
			return;

		PGN::Move *temp = m_frame->currentGame->pCurrMove->pNext;
		int count = 0;
		while( temp )
		{
			wxRect r2(margin+5,margin+25*count,width-margin-5,25) ;

			if( (x > r2.x) && (x < r2.x + r2.GetWidth()) )
			{
				if( (y > r2.y) && (y < r2.y + r2.GetHeight()))
				{
						


					selection = count;
					//m_frame->SelectMove(count);
					//selection = 0;
					Refresh();

					return;
				}
			}

			count++;
			temp = temp->pVarWhite;
		}
		
	}
	else
	{
		//if(  m_frame->currentGame->pCurrMove->pVarBlack != NULL )
		{
			PGN::Move *temp = m_frame->currentGame->pCurrMove;
			int count = 0;
			while( temp )
			{
				wxRect r2(margin+5,margin+25*count,width-margin-5,25) ;
				if( (x > r2.x) && (x < r2.x + r2.GetWidth()) )
				{
					if( (y > r2.y) && (y < r2.y + r2.GetHeight()))
					{
						
						//selection = 0;
					selection = count;
					//m_frame->SelectMove(count);
					//selection = 0;
					Refresh();					
						//m_frame->currentGame->pCurrMove = temp;
					//	m_frame->my_canvas->white_black_move = BLACK;
						
					//	m_frame->my_canvas->Refresh();
					//	m_frame->board_canvas->Refresh();
						return;
					}
				}
				
				count++;
				temp = temp->pVarBlack;
			}
		}
	}
	*/
}