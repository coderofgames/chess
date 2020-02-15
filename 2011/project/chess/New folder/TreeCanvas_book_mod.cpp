#include "TreeCanvas.h"
#include "mdi.h"
#include "BoardCanvas.h"
#include "MyPipePanel.h"


BEGIN_EVENT_TABLE(TreeCanvas, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (TreeCanvas::OnPaint)
	EVT_IDLE (TreeCanvas::OnIdle)
	EVT_LEFT_DOWN (TreeCanvas::OnMouseLeftClick)
	
END_EVENT_TABLE()

TreeCanvas::TreeCanvas(MyFrame *parent)
        : wxScrolledWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE |wxWANTS_CHARS )
{
	m_frame = parent;
	m_useContext = true;
	selection = 0;
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

#ifdef WINDOWS_7
	SetDoubleBuffered(true);
#endif
	showVariationRoot = true;
	numSelectionsThisMove = 0;

	visitTreeCount = 0;
	SetScrollbars( 0, 10, 0, 1024 );
}

TreeCanvas::~TreeCanvas()
{
}

void TreeCanvas::IncrementSelection()
{
	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;



	if( whiteMove )
	{
		PGN::Move *temp = m_frame->currentGame->pCurrMove->pNext;
		PGN::Move* pRoot = NULL;
		if( temp != NULL )
			pRoot = temp->GetVariationRoot();

		if( pRoot != NULL )
		{
			int count = 0;
			vector<int> skip;
			while( temp )
			{
				for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
				{
					if( m_frame->entries[bk].san == temp->white )
					{
						skip.push_back(bk);
						//dc.DrawRectangle(r2.x+r2.width-6, r2.y +3, 2,2); 
					}
				}
				count++;
				temp = temp->pVarWhite;
			}

			for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
			{
				bool bFound = false;
				for( int j = 0; j < skip.size(); j++ )
				{
					if( skip[j] == bk )
					{
						bFound = true;
					}
				}
				if( !bFound )
				{
					count++;
				}
			}

			count += 1;

			if( selection + 1 < count )
			{
				selection++;
			}
			else
			{
				selection = 0;
			}
		}
		else
		{
			int count = 0;
			vector<int> skip;
			while( temp )
			{
				for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
				{
					if( m_frame->entries[bk].san == temp->white )
					{
						skip.push_back(bk);
						//continue;
						//dc.DrawRectangle(r2.x+r2.width-6, r2.y +3, 2,2); 
					}
				}
				count++;
				temp = temp->pVarWhite;
			}

			for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
			{
				bool bFound = false;
				for( int j = 0; j < skip.size(); j++ )
				{
					if( skip[j] == bk )
					{
						bFound = true;
					}
				}
				if( !bFound )
				{
					count++;
				}
			}

			if( selection + 1 < count )
			{
				selection++;
			}
			else
			{
				selection = 0;
			}
		}
	}
	else
	{
		PGN::Move *temp = m_frame->currentGame->pCurrMove;
		PGN::Move* pRoot = NULL;
		if( temp != NULL )
			pRoot = temp->GetVariationRoot();

		if( pRoot != NULL )
		{
			int count = 0;

			vector<int> skip;
			while( temp )
			{
				for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
				{
					if( m_frame->entries[bk].san == temp->black )
					{
						skip.push_back(bk);
						//continue;
						//dc.DrawRectangle(r2.x+r2.width-6, r2.y +3, 2,2); 
					}
				}
				count++;
				temp = temp->pVarBlack;
			}

			count += 1;

			for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
			{
				bool bFound = false;
				for( int j = 0; j < skip.size(); j++ )
				{
					if( skip[j] == bk )
					{
						bFound = true;
					}
				}
				if( !bFound )
				{
					count++;
				}
			}

			if( selection + 1 < count )
			{
				selection++;
			}
			else 
			{
				selection = 0;
			}
		}
		else
		{
			int count = 0;
			vector<int> skip;
			while( temp )
			{
				for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
				{
					if( m_frame->entries[bk].san == temp->black )
					{
						skip.push_back(bk);
						//continue;
						//dc.DrawRectangle(r2.x+r2.width-6, r2.y +3, 2,2); 
					}
				}
				count++;
				temp = temp->pVarBlack;
			}

			for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
			{
				bool bFound = false;
				for( int j = 0; j < skip.size(); j++ )
				{
					if( skip[j] == bk )
					{
						bFound = true;
					}
				}
				if( !bFound )
				{
					count++;
				}
			}

			if( selection + 1 < count )
			{
				selection++;
			}
			else 
			{
				selection = 0;
			}
		}
	}

	Refresh(true);
}

void TreeCanvas::DecrementSelection()
{
	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	if( whiteMove )
	{
		PGN::Move *temp = m_frame->currentGame->pCurrMove->pNext;
		PGN::Move* pRoot = NULL;
		if( temp != NULL && showVariationRoot)
			pRoot = temp->GetVariationRoot();

		if( pRoot != NULL )
		{
			int count = 0;
			while( temp )
			{
				count++;
				temp = temp->pVarWhite;
			}

			count+=1;

			if( selection - 1 >= 0 )
			{
				selection--;
			}
			else
			{
				selection = count-1;
			}
		}
		else
		{
			int count = 0;
			while( temp )
			{
				count++;
				temp = temp->pVarWhite;
			}

			if( selection - 1 >= 0 )
			{
				selection--;
			}
			else
			{
				selection = count-1;
			}
		}
	}
	else
	{
		PGN::Move *temp = m_frame->currentGame->pCurrMove;
		PGN::Move* pRoot = NULL;
		if( temp != NULL && showVariationRoot)
			pRoot = temp->GetVariationRoot();

		if( pRoot != NULL )
		{
			int count = 0;
			while( temp )
			{
				count++;
				temp = temp->pVarBlack;
			}

			count += 1;

			if( selection - 1 >= 0)
			{
				selection--;
			}
			else 
			{
				selection = count-1;
			}
		}
		else
		{
			int count = 0;
			while( temp )
			{
				count++;
				temp = temp->pVarBlack;
			}

			if( selection - 1 >= 0)
			{
				selection--;
			}
			else 
			{
				selection = count-1;
			}
		}
	}
	Refresh(true);
}


void TreeCanvas::OnPaint(wxPaintEvent &event)
{
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

    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);

    gc->SetFont(font,*wxBLACK);

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

	numSelectionsThisMove = 0;

	int descent = 0;
    //dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;

	if( whiteMove )
	{
		//if(  m_frame->currentGame->pCurrMove->pVarWhite != NULL )
		{
			PGN::Move *temp = m_frame->currentGame->pCurrMove->pNext;
			lastMovePtr = temp;
			PGN::Move* pRoot = NULL;
			if( temp != NULL && showVariationRoot )
				pRoot = temp->GetVariationRoot();

			if( pRoot != NULL )
			{
				int count = 1;

				vector<int> skip;
				while( temp )
				{
					wxRect r2(margin+10,margin+20*count,width-margin-5,20) ;
					// draw rounded rectangle
					dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
					dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 128),wxSOLID));
					dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
					
					if( count-1 == selection )
					{
						dc.SetPen( *wxTRANSPARENT_PEN ) ;
						dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
						dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
					}
					
					dc.SetTextForeground( wxColour(0,0,0,255) );
					
					dc.SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
					
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	

					
					
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );

					for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
					{
						if( m_frame->entries[bk].san == temp->white )
						{
							skip.push_back(bk);
							dc.DrawRectangle(r2.x+r2.width-6, r2.y +3, 2,2); 
						}
					}

					count++;
					numSelectionsThisMove++;
					temp = temp->pVarWhite;
				}

				descent = count*20+margin;

				for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
				{
					bool bFound = false;
					for( int s = 0; s < skip.size(); s++ )
					{
						if( skip[s] == bk )
						{
							bFound = true;
						}
					}
					if( !bFound )
					{
						wxRect r2(margin+10,margin+20*bk+descent,width-margin-5,20) ;
						// draw rounded rectangle
						dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
						dc.SetBrush( wxBrush( wxColour( 0, 255, 0, 128),wxSOLID));
						dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
						count++;
						numSelectionsThisMove++;
					}
				}
				
				wxRect r2(margin+5,margin,width-margin-5,20) ;
				// draw rounded rectangle
				dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
				dc.SetBrush( wxBrush( wxColour( 255, 0, 0, 128),wxSOLID));
				dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
				
				if( count-1 == selection )
				{
					dc.SetPen( *wxTRANSPARENT_PEN ) ;
					dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
					dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
				}

				dc.SetTextForeground( wxColour(0,0,0,255) );
				
				dc.SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
				// draw text
				string textStr = pRoot->info;
				int move_color = lastMovePtr->GetVariationRootColor();
				if( move_color == 0 )
				{
					textStr+=".";
					textStr+=pRoot->white;	
				}
				else
				{
					textStr+="...";
					textStr+=pRoot->black;	
				}
				dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
			}
			else
			{
				int count = 0;
				vector<int> skip;
				while( temp )
				{
					wxRect r2(margin+5,margin+20*count,width-margin-5,20) ;
					// draw rounded rectangle
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
					
					dc.SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
					//draw text
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );

					for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
					{
						if( m_frame->entries[bk].san == temp->white )
						{
							skip.push_back(bk);
							dc.DrawRectangle(r2.x+r2.width-6, r2.y +3, 2,2); 
						}
					}

					numSelectionsThisMove++;
					count++;
					temp = temp->pVarWhite;
				}
				int num = count;//--;
				num--;
				if( num < 0 ) num = 0;
					descent = num*20;

				for( int bk = 0; bk < m_frame->bookLookupCount; bk++ )
				{
					bool bFound = false;
					for( int s = 0; s < skip.size(); s++ )
					{
						if( skip[s] == bk )
						{
							bFound = true;
						}
					}
					if( !bFound )
					{
						
						wxRect r2(margin+5,margin+20*bk+descent,width-margin-5,20) ;
						// draw rounded rectangle
						dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
						dc.SetBrush( wxBrush( wxColour( 0, 255, 0, 128),wxSOLID));
						dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;

						if( count == selection )
						{
							dc.SetPen( *wxTRANSPARENT_PEN ) ;
							dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
							dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
						}
						//string textStr = lastMovePtr->info;
						//textStr+=".";
						string textStr=m_frame->entries[bk].san;	
						dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
						numSelectionsThisMove++;
						count++;
					}
				}
			}
		}
	}
	else
	{
		//if(  m_frame->currentGame->pCurrMove->pVarBlack != NULL )
		{
			PGN::Move *temp = m_frame->currentGame->pCurrMove;
			lastMovePtr = temp;
			PGN::Move* pRoot = NULL;
			if( temp != NULL && showVariationRoot)
				pRoot = temp->GetVariationRoot();

			if( pRoot != NULL )
			{
				int count = 1;
				while( temp )
				{
					wxRect r2(margin+10,margin+20*count,width-margin-5,20) ;
					// draw rounded rectangle
					dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
					dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 128),wxSOLID));	
					dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
					
					if( count-1 == selection )
					{
						dc.SetPen( *wxTRANSPARENT_PEN ) ;
						dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
						dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
					}
				
					dc.SetTextForeground( wxColour(0,0,0,255) );
					dc.SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
					// draw text
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
				
					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;

				wxRect r2(margin+5,margin,width-margin-5,20) ;
				// draw rounded rectangle
				dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
				dc.SetBrush( wxBrush( wxColour( 255, 0, 0, 128),wxSOLID));	
				dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
				
				if( count-1 == selection )
				{
					dc.SetPen( *wxTRANSPARENT_PEN ) ;
					dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
					dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
				}
				
				dc.SetTextForeground( wxColour(0,0,0,255) );
				// draw text
				dc.SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
				string textStr = pRoot->info;
				int move_color = lastMovePtr->GetVariationRootColor();
				if( move_color == 0 )
				{
					textStr+=".";
					textStr+=pRoot->white;	
				}
				else
				{
					textStr+="...";
					textStr+=pRoot->black;	
				}
				dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
			}
			else
			{
				int count = 0;
				while( temp )
				{
					wxRect r2(margin+5,margin+20*count,width-margin-5,20) ;
					// draw rounded rectangle
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
					//draw text
					dc.SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
				
					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;
			}
		}
	}


/*	descent+= 25;
	wxFont fontBold = wxFont(10, wxMODERN, wxNORMAL, wxBOLD, false);

	wxRect r4(margin+5,descent,width-margin-5,25) ;

	dc.SetFont(fontBold);
	dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 128),wxSOLID));
	dc.DrawRectangle(r4);
	dc.DrawText("Move Data", wxPoint(r4.x+5, r4.y));
	dc.SetTextForeground(*wxBLUE);
	dc.DrawText("Current Move: ", wxPoint(r4.x+5, r4.y + 25));
	if( whiteMove )
		dc.DrawText(this->m_frame->currentGame->pCurrMove->black, wxPoint(r4.x+25, r4.y + 25));
	else
	{
		dc.DrawText(this->m_frame->currentGame->pCurrMove->white, wxPoint(r4.x+25, r4.y + 25));
	}
	dc.DrawText("Score: ", wxPoint(r4.x+5, r4.y + 35));
	dc.DrawText("Depth: ", wxPoint(r4.x+5, r4.y + 45));*/
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


void TreeCanvas::OnIdle(wxIdleEvent &event)
{
	event.Skip();
}


void TreeCanvas::OnMouseLeftClick(wxMouseEvent &event)
{
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
		PGN::Move* pRoot = NULL;
		if( temp != NULL )
			pRoot = temp->GetVariationRoot();
		
		int count = 0;
		while( temp )
		{
			wxRect r2(margin+5,margin+25*count,width-margin-5,25) ;

			if( (x > r2.x) && (x < r2.x + r2.GetWidth()) )
			{
				if( (y > r2.y) && (y < r2.y + r2.GetHeight()))
				{
						

					/*if( selection == count )
					{
						m_frame->SelectMove(selection);


						//m_frame->board_canvas->Refresh(false);
						//m_frame->my_canvas->Refresh(false);
						selection = 0;
						Refresh();
						return;
					}*/
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
}