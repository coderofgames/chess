#include "EngineAnalysisCanvas.h"
#include "mdi.h"
#include "BoardCanvas.h"
#include "MyPipePanel.h"
#include "VariationBoardFrame.h"
#include "VariationBoardCanvas.h"




BEGIN_EVENT_TABLE(EngineAnalysisCanvas, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (EngineAnalysisCanvas::OnPaint)
	//EVT_IDLE (TreeCanvas::OnIdle)
	EVT_SCROLL(EngineAnalysisCanvas::OnScroll)
	EVT_SIZE(EngineAnalysisCanvas::OnSize)
	EVT_LEFT_DOWN (EngineAnalysisCanvas::OnMouseLeftClick)
	
END_EVENT_TABLE()

EngineAnalysisCanvas::EngineAnalysisCanvas(MyPipePanel* parent, MyFrame *myframe)
        : wxScrolledWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxHSCROLL |wxFULL_REPAINT_ON_RESIZE// )
							 |wxSUNKEN_BORDER )
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

	whitetoMove = false;
	variationClicked = false;
	pSel = NULL;
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
	MoveListMPVArray* pVecArrayFA = pp->GetFADataArray();


	if( pp->GetMode() == 0 )
	{
	//	if( !pVecArray->HasPVChangedThisFrame() && !needsRefresh)
	//		return;
	}
	else if( pp->GetMode() == 1 )
	{
	//	if( !pp->GetGameModeDepthPV()->HasPVChangedThisFrame() && !needsRefresh)
	//	{
	//		return;
	//	}
	}
	else if( pp->GetMode() == 2 )
	{
		pVecArray = pVecArrayFA->GetCurrent();
	}

	if( pVecArray->HasPVChangedThisFrame() && variationClicked)
	{
		/*if( whitetoMove )
		{
			//m_frame->board_canvas->GetBoard()->SetBoard(m_frame->currentGame->pCurrMove->m_fenBlack);
			m_frame->m_variationBoardFrame->m_variationBoard->GetBoard()->SetBoard(m_frame->currentGame->pCurrMove->m_fenBlack);
					m_frame->m_variationBoardFrame->m_variationBoard->m_moves.clear();
			//m_frame->board_canvas->MyRefresh(false);
		}
		else
		{
		//	m_frame->board_canvas->GetBoard()->SetBoard(m_frame->currentGame->pCurrMove->m_fenWhite);
		//	m_frame->board_canvas->MyRefresh(false);
			m_frame->m_variationBoardFrame->m_variationBoard->GetBoard()->SetBoard(m_frame->currentGame->pCurrMove->m_fenWhite);
					m_frame->m_variationBoardFrame->m_variationBoard->m_moves.clear();
		}*/

		variationClicked = false;
		
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


	if( needsRefresh )
		needsRefresh = false;


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
		// if drawing bar at the top
		//gc->SetBrush( wxBrush( wxColour( 100, 100, 255, 128),wxSOLID));
		//gc->DrawRectangle(localXX,0,sz.GetWidth(), 16);
	gc->PopState();


	if( pp->GetMode() == 0 )
	{

		wxString PVHeader = "PV";
		double pvhW=0.0f;
		double pvhH=0.0f;
		gc->GetTextExtent(PVHeader, &pvhW, &pvhH);

		wxString DepthHeader = "Depth";
		double dhW=0.0f;
		double dhH=0.0f;
		gc->GetTextExtent(DepthHeader, &dhW, &dhH);

		wxString ScoreHeader = "Score";
		double shW=0.0f;
		double shH=0.0f;
		gc->GetTextExtent(ScoreHeader, &shW, &shH);

		wxString MovesHeader = "Moves";
		double mhW=0.0f;
		double mhH=0.0f;
		gc->GetTextExtent(MovesHeader, &mhW, &mhH);

		//gc->DrawText(PVHeader, 5.0f, 0.0f, 0.0f);
		double offset1 = 5.0f + pvhW + pvhW;
		//gc->DrawText("D", offset1, 0.0f, 0.0f);
		double offset2 = offset1 + dhW *0.75 ;
		//gc->DrawText(ScoreHeader, offset2, 0.0f, 0.0f);
		double offset3 = offset2 + shW + dhW ;
		//gc->DrawText(MovesHeader, offset3, 0.0f, 0.0f);

		//gc->DrawText("PV  Depth   Score    Moves",0, 0,0);
	

		for( int i = 0; i < pp->GetNumKbtzLines(); i++ )
		{
			string str1 = " ";
			char buf[3];
			itoa(i+1, buf, 10);
		
			wxString numStr = buf; 
			// if drawing bar at the top (i+1)*2*fontSize
			gc->DrawText(numStr, 5.0f, (i)*2*fontSize, 0.0f);
			str1 += buf;
			str1 += ":  ";


			str1 += pVecArray->mpvs[i].depthStr;
			str1+= "    ";
			int depthLen = pVecArray->mpvs[i].depthStr.length();
			if( depthLen < 2 )
			{
				str1+= " ";
			}

			gc->SetFont(fontRich,*wxBLUE);
			// if drawing bar at the top (i+1)*2*fontSize
			gc->DrawText(pVecArray->mpvs[i].depthStr, offset1, (i)*2*fontSize, 0.0f);

			string spaceAfterScore = "";

			string scoreStr = "";

	


			scoreStr = pVecArray->mpvs[i].score_str;
			int scoreLen = scoreStr.length();
			string scoreStrToDraw = "(" + scoreStr + ")";
			// if drawing bar at the top (i+1)*2*fontSize
			gc->DrawText(scoreStrToDraw, offset2, (i)*2*fontSize, 0.0f);
			str1 += "(";
			str1 += scoreStr;
			str1 += ") ";
			//char buf2[10];
			//sprintf(buf2, "%d  ", pVecArray->mpvs[i].depth );
			for( int sp=0; sp< 7-scoreLen; sp++ )
			{
				str1 += " ";
			}

			string str3 = "";

			gc->SetFont(fontRich,*wxBLACK);

			double move_offset = offset3;

			for( int j = 0; j < pVecArray->mpvs[i].moves.size(); j++ )
			{
				string toRender = "";//pVecArray->mpvs[i].moves[j].info;
				//toRender += "...";
				
				if( j== 0 )
				{
					if( !pVecArray->mpvs[i].moves[j].whitesMove )
					{
						toRender += pVecArray->mpvs[i].moves[j].info;
						toRender += "...";

						str3 += pVecArray->mpvs[i].moves[j].info;
						str3 += "...";
					}
					else
					{
						toRender += pVecArray->mpvs[i].moves[j].info;
						toRender += ".";
						
						str3 += pVecArray->mpvs[i].moves[j].info;
						str3 += ".";
					}
				}
				else
				{
					if( pVecArray->mpvs[i].moves[j].whitesMove )
					{
						toRender += pVecArray->mpvs[i].moves[j].info;
						toRender += ".";

						str3 += pVecArray->mpvs[i].moves[j].info;
						str3 += ".";
					}
					else
					{
						toRender += " ";
						str3 += " ";
					}
				}
				toRender += pVecArray->mpvs[i].moves[j].pgnMove;
				str3 += pVecArray->mpvs[i].moves[j].pgnMove;


				if( !pVecArray->mpvs[i].moves[j].whitesMove )
				{
					toRender += " ";
					str3 += " ";
				}

				double rctW =0;
				double rctH =0;
				gc->GetTextExtent(toRender, &rctW, &rctH);

				wxRect rctToRender;
				rctToRender.SetWidth(rctW);
				rctToRender.SetHeight(rctH);

				// if drawing bar at the top (i+1)*2*fontSize
				rctToRender.SetPosition(wxPoint(move_offset, (i)*2*fontSize));

				move_offset += rctW;

				if( pVecArray->mpvs[i].moves[j].selected )
				{
				
					
					gc->SetBrush(*wxTRANSPARENT_BRUSH);
					gc->SetPen(wxPen(*wxBLACK, 1));
					gc->DrawRectangle(0, rctToRender.y, sz.GetWidth(), rctToRender.height);
					gc->SetPen(wxPen(*wxBLACK, 1));
					gc->SetBrush(*wxBLACK);
				}

				gc->DrawText( toRender, rctToRender.x, rctToRender.y );
				

				pVecArray->mpvs[i].moves[j].startX = rctToRender.x;
				pVecArray->mpvs[i].moves[j].startY = rctToRender.y;
				pVecArray->mpvs[i].moves[j].width = rctToRender.width;
				pVecArray->mpvs[i].moves[j].height = rctToRender.height;

			}
			//gc->DrawText(str3,offset3, (i+1)*2*fontSize,0);
		}

	}
	else if( pp->GetMode()==1 )
	{

		gc->DrawText("PV  Score  Depth  Moves",0, 0,0);
	
		int drawMode = 0;
		if( drawMode )
		{
			string str1 = " ";
		//	char buf[3];
		//	itoa(i+1, buf, 10);
		
		//	str1 += buf;
			str1 += "pv:  ";

			str1 += pp->GetGameModePV()->depthStr;
			str1+= "   ";
			int depthLen = pp->GetGameModePV()->depthStr.length();
			if( depthLen < 2 )
			{
				str1+= " ";
			}

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

			for( int j = 0; j < pp->GetGameModePV()->moves.size(); j++ )
			{
				if( j== 0 )
				{
					if( !pp->GetGameModePV()->moves[j].whitesMove )
					{
						str1 += pp->GetGameModePV()->moves[j].info;
						str1 += "...";
					}
					else
					{
						str1 += pp->GetGameModePV()->moves[j].info;
						str1 += ".";
					}
				}
				else
				{
					if( pp->GetGameModePV()->moves[j].whitesMove )
					{
						str1 += pp->GetGameModePV()->moves[j].info;
						str1 += ".";
					}
					else
					{
						str1 += " ";
					}
				}
				str1 += pp->GetGameModePV()->moves[j].pgnMove;

				if( !pp->GetGameModePV()->moves[j].whitesMove )
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

				str1 += pp->GetGameModeDepthPV()->m_moves[i].depthStr;
				str1+= "   ";
				int depthLen = pp->GetGameModeDepthPV()->m_moves[i].depthStr.length();
				if( depthLen < 2 )
				{
					str1+= " ";
				}

				int scoreLen = pp->GetGameModeDepthPV()->m_moves[i].score_str.length();
				if( scoreLen < 5 )
				{
					for( int slen = 0; slen < 5-scoreLen; slen++ )
						str1+= " ";
				}

				str1 += "(";
				str1 += pp->GetGameModeDepthPV()->m_moves[i].score_str;
				str1 += ") ";
				//char buf2[10];
				//sprintf(buf2, "%d  ", pVecArray->mpvs[i].depth );

				for( int j = 0; j < pp->GetGameModeDepthPV()->m_moves[i].moves.size(); j++ )
				{
					if( j== 0 )
					{
						if( !pp->GetGameModeDepthPV()->m_moves[i].moves[j].whitesMove )
						{
							str1 += pp->GetGameModeDepthPV()->m_moves[i].moves[j].info;
							str1 += "...";
						}
						else
						{
							str1 += pp->GetGameModeDepthPV()->m_moves[i].moves[j].info;
							str1 += ".";
						}
					}
					else
					{
						if( pp->GetGameModeDepthPV()->m_moves[i].moves[j].whitesMove )
						{
							str1 += pp->GetGameModeDepthPV()->m_moves[i].moves[j].info;
							str1 += ".";
						}
						else
						{
							str1 += " ";
						}
					}
					str1 += pp->GetGameModeDepthPV()->m_moves[i].moves[j].pgnMove;

					if( !pp->GetGameModeDepthPV()->m_moves[i].moves[j].whitesMove )
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
	else if( pp->GetMode() == 2 )
	{
		gc->DrawText("  PV  Score  Depth  Moves",0, 0,0);
	

		for( int i = 0; i < pp->GetNumKbtzLines(); i++ )
		{
			string str1 = " ";
			char buf[3];
			itoa(i+1, buf, 10);
		
			str1 += buf;
			str1 += ":  ";


			str1 += pVecArray->mpvs[i].depthStr;
			str1+= "   ";
			
			int depthLen = pVecArray->mpvs[i].depthStr.length();
			if( depthLen < 2 )
			{
				str1+= " ";
			}

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

			for( int j = 0; j < pVecArray->mpvs[i].moves.size(); j++ )
			{
				if( j== 0 )
				{
					if( !pVecArray->mpvs[i].moves[j].whitesMove )
					{
						str1 += pVecArray->mpvs[i].moves[j].info;
						str1 += "...";
					}
					else
					{
						str1 += pVecArray->mpvs[i].moves[j].info;
						str1 += ".";
					}
				}
				else
				{
					if( pVecArray->mpvs[i].moves[j].whitesMove )
					{
						str1 += pVecArray->mpvs[i].moves[j].info;
						str1 += ".";
					}
					else
					{
						str1 += " ";
					}
				}
				str1 += pVecArray->mpvs[i].moves[j].pgnMove;

				if( !pVecArray->mpvs[i].moves[j].whitesMove )
				{
					str1 += " ";
				}

			}
			gc->DrawText(str1.c_str(),0, (i+1)*2*fontSize,0);
		}
	}

#endif
}


void EngineAnalysisCanvas::OnIdle(wxIdleEvent &event)
{
	event.Skip();
}


void EngineAnalysisCanvas::OnMouseLeftClick(wxMouseEvent &event)
{
#if wxUSE_STATUSBAR
    wxClientDC dc(this);
    PrepareDC(dc);
    m_frame->PrepareDC(dc);

	if( m_frame->enginesMatch )
		return;

    wxString str;
   // str.Printf( wxT("Current mouse position: %d,%d"), (int)x, (int)y );
   // m_owner->SetStatusText( str );
#else
    wxUnusedVar(event);
#endif // wxUSE_STATUSBAR
    wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );

	MyPipePanel* pp = m_parent;//m_frame->m_pipePanel[0];

	if( pp->isReading || pp->GetMode() != 0 )
		return;

	MPVMoveNodeVectorArray* pVecArray = pp->GetMPVNodeVectorArray();

		for( int i = 0; i < pp->GetNumKbtzLines(); i++ )
		{
			for( int j = 0; j < pVecArray->mpvs[i].moves.size(); j++ )
			{
				
				if( (x > pVecArray->mpvs[i].moves[j].startX) &&
					(x < pVecArray->mpvs[i].moves[j].startX + pVecArray->mpvs[i].moves[j].width) &&
					(y > pVecArray->mpvs[i].moves[j].startY) && 
					(y < pVecArray->mpvs[i].moves[j].startY + pVecArray->mpvs[i].moves[j].height))
				{
					pVecArray->mpvs[i].moves[j].selected = true;

					if( m_frame->board_canvas->GetBoard()->mov != BLACK )
					{
						whitetoMove = true;
					}
					else
					{
						whitetoMove = false;
					}

					if( pSel )
					{
						pSel->selected = false;
					}

					m_frame->m_variationBoardFrame->m_variationBoard->GetBoard()->SetBoard(pVecArray->mpvs[i].moves[j].fen);
					m_frame->m_variationBoardFrame->m_variationBoard->m_moves.clear();
					for( int v = 0; v < pVecArray->mpvs[i].moves.size(); v++ )
					{
						m_frame->m_variationBoardFrame->m_variationBoard->m_moves.push_back( pVecArray->mpvs[i].moves[v] );
					}
					m_frame->m_variationBoardFrame->m_variationBoard->selectedMove = j;
					m_frame->m_variationBoardFrame->Refresh(false);
					
					//m_frame->board_canvas->GetBoard()->SetBoard(pVecArray->mpvs[i].moves[j].fen);
					variationClicked = true;
					pSel = &pVecArray->mpvs[i].moves[j];
					Refresh(true);
					return;
				}


			}
			//gc->DrawText(str3,offset3, (i+1)*2*fontSize,0);
		}
}