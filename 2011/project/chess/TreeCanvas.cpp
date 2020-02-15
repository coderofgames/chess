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


	m_font = wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL );

	normalPen = wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID);
	normalRectBrush = wxBrush( wxColour( 0, 0, 255, 128),wxSOLID);

	selectedRectBrush = wxBrush( wxColour(255,255,128,128) );
	rootBrush = wxBrush( wxColour( 255, 0, 0, 128),wxSOLID);
	bookRectBrush = wxBrush( wxColour( 150, 150, 255, 128),wxSOLID);

	for( int i = 0; i < 254; i++ )
	{
		tree_elems[i] = new TreeCanvasElem();
		tree_elems[i+1] = new TreeCanvasElem();
		tree_elems[i]->pNext = tree_elems[i+1];
		tree_elems[i]->bRoot = false;
		tree_elems[i+1]->pPrev = tree_elems[i];
		tree_elems[i+1]->bRoot = false;
		tree_elems[i]->bSelected =false;
		tree_elems[i]->isBookMove = false;
		tree_elems[i+1]->bSelected =false;
		tree_elems[i+1]->isBookMove = false;
	}

	root_elem = new TreeCanvasElem();

	tree_elems[0]->pPrev = tree_elems[254];
	
	tree_elems[254]->pNext = tree_elems[0];

	root_elem->pPrev = tree_elems[254];
	root_elem->pNext = tree_elems[0];
	this->selectedElem = tree_elems[0];

	root_elem->bRoot = true;
	numElems = 0;
	visitTreeCount = 0;
	SetScrollbars( 0, 10, 0, 1024 );
#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif
}

TreeCanvas::~TreeCanvas()
{
	for( int i = 0; i < 255; i++ )
	{
		delete tree_elems[i];
	}
	delete root_elem;

}

void TreeCanvas::IncrementSelection()
{
	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	if( numElems == 0 ) return;

	selectedElem = selectedElem->pNext;

/*	if( whiteMove )
	{
		PGN::Move *temp = m_frame->currentGame->pCurrMove->pNext;
		PGN::Move* pRoot = NULL;
		if( temp != NULL )
			pRoot = temp->GetVariationRoot();

		if( pRoot != NULL )
		{
			int count = 0;
			while( temp )
			{
				count++;
				temp = temp->pVarWhite;
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
			while( temp )
			{
				count++;
				temp = temp->pVarWhite;
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
			while( temp )
			{
				count++;
				temp = temp->pVarBlack;
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
			while( temp )
			{
				count++;
				temp = temp->pVarBlack;
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
	*/
	Refresh(true);
}

void TreeCanvas::DecrementSelection()
{
	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	if( numElems == 0 ) return;

	selectedElem = selectedElem->pPrev;

	/*if( whiteMove )
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
	}*/
	Refresh(true);
}


void TreeCanvas::OnPaint(wxPaintEvent &event)
{
    wxPaintDC pdc(this);

	wxSize sz(GetClientSize());
	
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = true? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif


    PrepareDC(dc);

    m_frame->PrepareDC(dc);

	dc.Clear();

    wxGraphicsContext *gc = gdc.GetGraphicsContext();
	gc->SetAntialiasMode(wxANTIALIAS_NONE);
//	DrawGraphics();

    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);

    dc.SetFont(font);//,*wxBLACK);

	//wxGraphicsPath pth = gc->CreatePath();

	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;
	

	//MyRefresh(false);

	if( numElems == 0 ) return;

	if( root_elem->active )
	{
		dc.SetPen( normalPen );
		dc.SetBrush( this->rootBrush );
		dc.DrawRoundedRectangle( root_elem->rect.x,
								 root_elem->rect.y, 
								 root_elem->rect.width, 
								 root_elem->rect.height, 2 ) ;
				
	/*	if( root_elem.bSelected )
		{
			dc.SetPen( *wxTRANSPARENT_PEN ) ;
			dc.SetBrush( selectedRectBrush );
			dc.DrawRoundedRectangle( root_elem.rect.x,root_elem.rect.y, root_elem.rect.width, root_elem.rect.height, 2 ) ;
		}*/

		dc.SetTextForeground( wxColour(0,0,0,255) );
				
		dc.SetFont( m_font );
		if( root_elem != selectedElem )
			dc.DrawText(root_elem->sanMove, root_elem->rect.x+2, root_elem->rect.y+2);
	}

	for( int i = 0; i < numElems; i++ )
	{
		if( tree_elems[i] == selectedElem )


		dc.SetPen( normalPen );
		if( tree_elems[i]->pMove == NULL && tree_elems[i]->isBookMove )
		{
			dc.SetBrush( this->bookRectBrush );
		}
		else
		{
			dc.SetBrush( this->normalRectBrush );
		}
		dc.DrawRoundedRectangle( tree_elems[i]->rect.x,
								 tree_elems[i]->rect.y, 
								 tree_elems[i]->rect.width,
								 tree_elems[i]->rect.height, 2 ) ;
				
		/*if( tree_elems[i].bSelected )
		{
			dc.SetPen( *wxTRANSPARENT_PEN ) ;
			dc.SetBrush( selectedRectBrush );
			dc.DrawRoundedRectangle( tree_elems[i].rect.x + 2,
									 tree_elems[i].rect.y + 2, 
									 tree_elems[i].rect.width - 4, 
									 tree_elems[i].rect.height - 4, 
									 2 ) ;
									 
		}*/

		dc.SetTextForeground( wxColour(0,0,0,255) );
				
		dc.SetFont( m_font );
		if( tree_elems[i] != selectedElem )
			dc.DrawText(tree_elems[i]->sanMove, tree_elems[i]->rect.x+2, tree_elems[i]->rect.y+2);
		//dc.DrawText(tree_elems[i].score, tree_elems[i].scoreRect.x, tree_elems[i].scoreRect.y);
	}

	
	dc.SetPen( *wxTRANSPARENT_PEN ) ;
	dc.SetBrush( selectedRectBrush );
	dc.DrawRoundedRectangle( selectedElem->rect.x + 2,
		selectedElem->rect.y + 2, 
		selectedElem->rect.width - 4, 
		selectedElem->rect.height - 4, 
			2 ) ;

		dc.SetTextForeground( wxColour(0,0,0,255) );
				
		dc.SetFont( m_font );
		dc.DrawText(selectedElem->sanMove, selectedElem->rect.x+2, selectedElem->rect.y+2);
	

	return;


 /*   wxDouble margin = 10 ;
	wxDouble width = sz.GetWidth() * 0.85 ;
    wxDouble radius = 10 ;

    dc.SetPen( wxPen( wxColour( 128, 0, 0, 255 ),4, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 255, 0, 0, 255),wxSOLID));

	wxSize szTxtGlobal = dc.GetTextExtent("000.00");

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

				while( temp )
				{
					wxRect r2(margin+10,margin+20*count,width-margin-5,20) ;
					// draw rounded rectangle
					dc.SetPen( normalPen );
					dc.SetBrush( normalRectBrush );
					dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
					
					if( count-1 == selection )
					{
						dc.SetPen( *wxTRANSPARENT_PEN ) ;
						dc.SetBrush( selectedRectBrush );
						dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
					}
					
					dc.SetTextForeground( wxColour(0,0,0,255) );
					
					dc.SetFont( m_font );
					
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	
					
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );

					
					wxSize szTxt = dc.GetTextExtent(textStr.c_str());
					//if( (r2.GetWidth() - szTxt.GetWidth()) > szTxt.GetWidth()*2 )
					{
						dc.DrawText( wxString::Format("%3.2f",(float)(temp->move_cp_white )/100.0f), r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2 );
					}
					count++;
					numSelectionsThisMove++;
					temp = temp->pVarWhite;
				}

				descent = count*20+margin;
				
				wxRect r2(margin+5,margin,width-margin-5,20) ;
				// draw rounded rectangle
				dc.SetPen( normalPen );
				dc.SetBrush( this->rootBrush );
				dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
				
				if( count-1 == selection )
				{
					dc.SetPen( *wxTRANSPARENT_PEN ) ;
					dc.SetBrush( selectedRectBrush );
					dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
				}

				dc.SetTextForeground( wxColour(0,0,0,255) );
				
				dc.SetFont( m_font );
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
				//wxSize szTxt = dc.GetTextExtent(textStr.c_str());
				//if( (r2.GetWidth() - szTxt.GetWidth()) > szTxt.GetWidth()*2 )
				//{
				//	dc.DrawText( wxString::Format("%3.2f",(float)(temp->move_cp_white )/100.0f), r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2 );
				//}
			}
			else
			{
				int count = 0;
				while( temp )
				{
					wxRect r2(margin+5,margin+20*count,width-margin-5,20) ;
					// draw rounded rectangle
					dc.SetPen( normalPen );
					dc.SetBrush( normalRectBrush );
					dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
					
					if( count == selection )
					{
						dc.SetPen( *wxTRANSPARENT_PEN ) ;
						dc.SetBrush( selectedRectBrush );
						dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
					}
					
					dc.SetTextForeground( wxColour(0,0,0,255) );
					
					dc.SetFont( m_font );
					//draw text
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
					wxSize szTxt = dc.GetTextExtent(textStr.c_str());
					//if( (r2.GetWidth() - szTxt.GetWidth()) > szTxt.GetWidth()*2 )
					{
						dc.DrawText( wxString::Format("%3.2f",(float)(temp->move_cp_white )/100.0f), r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2 );
					}

					numSelectionsThisMove++;
					count++;
					temp = temp->pVarWhite;
				}
				descent = count*20+margin;
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
					dc.SetPen( normalPen );
					dc.SetBrush( normalRectBrush );	
					dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
					
					if( count-1 == selection )
					{
						dc.SetPen( *wxTRANSPARENT_PEN ) ;
						dc.SetBrush( selectedRectBrush );
						dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
					}
				
					dc.SetTextForeground( wxColour(0,0,0,255) );
					dc.SetFont( m_font );
					// draw text
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );

					wxSize szTxt = dc.GetTextExtent(textStr.c_str());
//					if( (r2.GetWidth() - szTxt.GetWidth()) > szTxt.GetWidth()*2 )
					{
						dc.DrawText( wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f), r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2 );
					}

					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;

				wxRect r2(margin+5,margin,width-margin-5,20) ;
				// draw rounded rectangle
				dc.SetPen( normalPen );
				dc.SetBrush( this->rootBrush );	
				dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
				
				if( count-1 == selection )
				{
					dc.SetPen( *wxTRANSPARENT_PEN ) ;
					dc.SetBrush( this->selectedRectBrush );
					dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
				}
				
				dc.SetTextForeground( wxColour(0,0,0,255) );
				// draw text
				dc.SetFont( m_font );

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
				
				//wxSize szTxt = dc.GetTextExtent(textStr.c_str());
				//if( (r2.GetWidth() - szTxt.GetWidth()) > szTxt.GetWidth()*2 )
				//{
				//	dc.DrawText( wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f), r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2 );
				//}
			}
			else
			{
				int count = 0;
				while( temp )
				{
					wxRect r2(margin+5,margin+20*count,width-margin-5,20) ;
					// draw rounded rectangle
					dc.SetPen( normalPen );
					dc.SetBrush( normalRectBrush);	
					dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
					
					if( count == selection )
					{
						dc.SetPen( *wxTRANSPARENT_PEN ) ;
						dc.SetBrush( selectedRectBrush  );
						dc.DrawRoundedRectangle( r2.x+2, r2.y+2, r2.width-4, r2.height-4, 2 ) ;
					}
				
					dc.SetTextForeground( wxColour(0,0,0,255) );
					//draw text
					dc.SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					dc.DrawText( textStr.c_str(), r2.x+2, r2.y+2 );
					
					wxSize szTxt = dc.GetTextExtent(textStr.c_str());
					//if( (r2.GetWidth() - szTxt.GetWidth()) > szTxt.GetWidth()*2 )
					{
						dc.DrawText( wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f), r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2 );
					}				
					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;
			}
		}
	}*/


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
//#endif
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

void TreeCanvas::MyRefresh(bool repaint)
{
	//if( selection == 0 )
	
	
	
	if( (m_frame->currentGame->gameBeginsAtStartPos == false) &&
		(m_frame->currentGame->pCurrMove == m_frame->currentGame->pBase))
	{
		SetupTreeElemsNonStandardStartPos();
	}
	else
	{
		SetupTreeElems();
	}
	

	Refresh(repaint);

}



void TreeCanvas::SetupTreeElems()
{
	if( (numElems < 255) && (numElems > 0))
	{
		tree_elems[numElems-1]->pNext = tree_elems[numElems];
		tree_elems[0]->pPrev = tree_elems[254];
	}
	else
	{
		for( int i = 0; i < 254; i++ )
		{
			tree_elems[i]->pNext = tree_elems[i+1];
			tree_elems[i+1]->pPrev = tree_elems[i];
		}

		tree_elems[0]->pPrev = tree_elems[254];
		tree_elems[254]->pNext = tree_elems[0];
	}

	numElems = 0;
    wxClientDC dc(this);
	PrepareDC(dc);
	
	wxSize sz(GetClientSize());

	root_elem->active = false;

	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

    wxDouble margin = 10 ;
	wxDouble width = sz.GetWidth() * 0.85 ;
    wxDouble radius = 10 ;

	double descent;


	wxSize szTxtGlobal = dc.GetTextExtent("000.0g");

	wxRect r(margin,margin,width,sz.GetHeight()*0.75) ;

	numSelectionsThisMove = 0;
	int count =0;
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
				count = 1;

				while( temp )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+10, margin+cH*count, width-margin-5, cH) ;
					// draw rounded rectangle
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->isBookMove = false;
					tree_elems[numElems]->pBookEntry = NULL;

					
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	

					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->white;
					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = true;
					
					tree_elems[numElems]->score = wxString::Format("%3.2f",(float)(temp->move_cp_white )/100.0f);

					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );
					numElems++;
					count++;
					numSelectionsThisMove++;
					temp = temp->pVarWhite;
				}

				descent = count*20+margin;
				
				int cH = szTxtGlobal.GetHeight()+4;
				wxRect r2(margin+5,margin,width-margin-5,cH) ;
				
				root_elem->active = true;
				root_elem->rect = r2;
				root_elem->pBookEntry = NULL;
				// draw rounded rectangle

				if( count-1 == selection )
				{
					root_elem->bSelected = true;
					selectedElem = root_elem;
				}
				else root_elem->bSelected = false;

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
				
				root_elem->white_black = false;
				root_elem->bookMove = pRoot->black;

				if( pRoot == m_frame->currentGame->pRoot )
				{
					root_elem->sanMove = "Start";
					root_elem->pMove = pRoot;
				}
				else
				{
					root_elem->sanMove = textStr;
					root_elem->pMove = pRoot;
				}

			}
			else
			{
				count = 0;
				while( temp )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+5,margin+cH*count,width-margin-5,cH) ;
					// draw rounded rectangle
						
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->isBookMove = false;
					tree_elems[numElems]->pBookEntry = NULL;

					
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	

					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->white;
					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = true;
					
					tree_elems[numElems]->score = wxString::Format("%3.2f",(float)(temp->move_cp_white )/100.0f);
					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );

					numElems++;

					numSelectionsThisMove++;
					count++;
					temp = temp->pVarWhite;
				}
				descent = count*20+margin;
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
				count = 1;
				while( temp && temp->black != "")
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+10,margin+cH*count,width-margin-5,cH) ;
					// draw rounded rectangle
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->isBookMove = false;
					tree_elems[numElems]->pBookEntry = NULL;
				
				
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					
					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->black;
					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = false;

					wxSize szTxt = dc.GetTextExtent(textStr.c_str());
					tree_elems[numElems]->score = wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f);
					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );

					numElems++;
					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;
				int cH = szTxtGlobal.GetHeight()+4;
				wxRect r2(margin+5,margin,width-margin-5,cH) ;
				// draw rounded rectangle
				root_elem->active = true;
				root_elem->rect = r2;
				root_elem->isBookMove = false;
				root_elem->pBookEntry = NULL;
				

				
				string textStr = pRoot->info;
				int move_color = lastMovePtr->GetVariationRootColor();
				if( move_color == 0 )
				{
					textStr+=".";
					textStr+=pRoot->white;	
					if( pRoot == m_frame->currentGame->pRoot )
					{
						root_elem->sanMove = "Start";
						root_elem->pMove = pRoot;
					}
					else
					{
						root_elem->sanMove = textStr;
						root_elem->pMove = pRoot;
					}
				}
				else
				{
					textStr+="...";
					textStr+=pRoot->black;
					if( pRoot == m_frame->currentGame->pRoot )
					{
						root_elem->sanMove = "Start";
						root_elem->pMove = pRoot;
					}
					else
					{
						root_elem->sanMove = textStr;
						root_elem->pMove = pRoot;
					}
				}



				//root_elem.pMove = pRoot;
				root_elem->white_black = false;
				//root_elem.sanMove = textStr;
				root_elem->bookMove = pRoot->black;
			}
			else
			{
				count = 0;
				while( temp && temp->black != "")
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+5,margin+cH*count,width-margin-5,cH) ;
					// draw rounded rectangle
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->pBookEntry = NULL;

				
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->black;
					tree_elems[numElems]->score  = wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f);
	
					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );

					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = false;
					tree_elems[numElems]->isBookMove = false;

					numElems++;
					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;
			}
		}
	}

	if( (numElems == 0)&& (m_frame->currentGame->pCurrMove->pNext == NULL) &&
		(m_frame->m_mgr.GetPane("moveData").IsShown()))
	{
		PGN::Move *pRoot = m_frame->currentGame->pCurrMove->GetVariationRoot();
		lastMovePtr = m_frame->currentGame->pCurrMove;
		if( pRoot )
		{
			if( whiteMove )
			{


			
				int cH = szTxtGlobal.GetHeight()+4;
				wxRect r2(margin+5,margin,width-margin-5,cH) ;
				
				root_elem->active = true;
				root_elem->rect = r2;
				root_elem->pBookEntry = NULL;
				// draw rounded rectangle

				if( count-1 == selection )
				{
					root_elem->bSelected = true;
					selectedElem = root_elem;
				}
				else root_elem->bSelected = false;

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
				
				root_elem->white_black = false;
				root_elem->bookMove = pRoot->black;

				if( pRoot == m_frame->currentGame->pRoot )
				{
					root_elem->sanMove = "Start";
					root_elem->pMove = pRoot;
				}
				else
				{
					root_elem->sanMove = textStr;
					root_elem->pMove = pRoot;
				}
			}
			else
			{
				int cH = szTxtGlobal.GetHeight()+4;
				wxRect r2(margin+5,margin,width-margin-5,cH) ;
				// draw rounded rectangle
				root_elem->active = true;
				root_elem->rect = r2;
				root_elem->isBookMove = false;
				root_elem->pBookEntry = NULL;
				

				
				string textStr = pRoot->info;
				int move_color = lastMovePtr->GetVariationRootColor();
				if( move_color == 0 )
				{
					textStr+=".";
					textStr+=pRoot->white;	
					if( pRoot == m_frame->currentGame->pRoot )
					{
						root_elem->sanMove = "Start";
						root_elem->pMove = pRoot;
					}
					else
					{
						root_elem->sanMove = textStr;
						root_elem->pMove = pRoot;
					}
				}
				else
				{
					textStr+="...";
					textStr+=pRoot->black;
					if( pRoot == m_frame->currentGame->pRoot )
					{
						root_elem->sanMove = "Start";
						root_elem->pMove = pRoot;
					}
					else
					{
						root_elem->sanMove = textStr;
						root_elem->pMove = pRoot;
					}
				}



				//root_elem.pMove = pRoot;
				root_elem->white_black = false;
				//root_elem.sanMove = textStr;
				root_elem->bookMove = pRoot->black;
			}
		}
		
	}

	//m_frame->DoLookupFromBook();

	for( int i = 0; i < m_frame->bookLookupCount; i++ )
	{
		bool bFoundBookMove = false;
		for( int j = 0; j < numElems; j++ )
		{
			if( tree_elems[j]->bookMove.find(m_frame->entries[i].san)!=-1 )
			{
				bFoundBookMove = true;
				tree_elems[j]->isBookMove = true;
				tree_elems[j]->pBookEntry = &m_frame->entries[i];
			}
		}

		if( !bFoundBookMove )
		{
			wxRect r2;

			if( numElems > 0 )
			{
				if( root_elem->active )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+10,tree_elems[numElems-1]->rect.y + cH,width-margin-5,cH) ;
				}
				else
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+5,tree_elems[numElems-1]->rect.y + cH,width-margin-5,cH) ;
				}
			}
			else
			{

				if( root_elem->active )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+10,margin + cH,width-margin-5,cH) ;
				}
				else
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+5,margin,width-margin-5,cH) ;
				}
				
			}
			


					// draw rounded rectangle
			tree_elems[numElems]->active = true;
			
			tree_elems[numElems]->rect = r2;
			tree_elems[numElems]->isBookMove = true;
			tree_elems[numElems]->pBookEntry = &m_frame->entries[i];	

			
			string textStr = "";
			if( whiteMove )
			{
				char buff[10];
				sprintf(buff, "%d", atoi(m_frame->currentGame->pCurrMove->info.c_str())+1);
				textStr += buff;//m_frame->currentGame->pCurrMove->info;
				textStr+=".";
				textStr+=m_frame->entries[i].san;
				tree_elems[numElems]->white_black = true;
				//textStr+=m_frame->entries[i].san;
			}
			else
			{
				textStr = m_frame->currentGame->pCurrMove->info;
				textStr+="...";
				textStr+=m_frame->entries[i].san;
				tree_elems[numElems]->white_black = false;
			}
					
			tree_elems[numElems]->sanMove = textStr;
			tree_elems[numElems]->bookMove = m_frame->entries[i].san;
			tree_elems[numElems]->pMove = NULL;
			

			wxSize szTxt = dc.GetTextExtent(textStr.c_str());
			tree_elems[numElems]->score = "0.00";//wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f);
					
			count++;
			numElems++;
			numSelectionsThisMove++;
		}
	}


	if( root_elem->active == true )
	{
		if( !(numElems == 0) )
		{
			tree_elems[numElems-1]->pNext = root_elem;
			root_elem->pNext = tree_elems[0];
			tree_elems[0]->pPrev = root_elem;
			root_elem->pPrev = tree_elems[numElems-1];
		}
	}
	else
	{
		if( !(numElems == 0) )
		{
			tree_elems[numElems-1]->pNext = tree_elems[0];
			tree_elems[0]->pPrev = tree_elems[numElems-1];
		}
	}
}

void TreeCanvas::SetupTreeElemsNonStandardStartPos()
{
	if( (numElems < 255) && (numElems > 0))
	{
		tree_elems[numElems-1]->pNext = tree_elems[numElems];
		tree_elems[0]->pPrev = tree_elems[254];
	}
	else
	{
		for( int i = 0; i < 254; i++ )
		{
			tree_elems[i]->pNext = tree_elems[i+1];
			tree_elems[i+1]->pPrev = tree_elems[i];
		}

		tree_elems[0]->pPrev = tree_elems[254];
		tree_elems[254]->pNext = tree_elems[0];
	}

	numElems = 0;
    wxClientDC dc(this);
    PrepareDC(dc);
	
	wxSize sz(GetClientSize());

	root_elem->active = false;

	bool whiteMove;
	int mov = m_frame->board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

    wxDouble margin = 10 ;
	wxDouble width = sz.GetWidth() * 0.85 ;
    wxDouble radius = 10 ;

	double descent;


	wxSize szTxtGlobal = dc.GetTextExtent("000.0g");

	wxRect r(margin,margin,width,sz.GetHeight()*0.75) ;

	numSelectionsThisMove = 0;
	int count =0;
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
				count = 1;

				while( temp )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+10, margin+cH*count, width-margin-5, cH) ;
					// draw rounded rectangle
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->isBookMove = false;
					tree_elems[numElems]->pBookEntry = NULL;

					
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	

					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->white;
					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = true;
					
					tree_elems[numElems]->score = wxString::Format("%3.2f",(float)(temp->move_cp_white )/100.0f);

					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );
					numElems++;
					count++;
					numSelectionsThisMove++;
					temp = temp->pVarWhite;
				}

				descent = count*20+margin;
				
				int cH = szTxtGlobal.GetHeight()+4;
				wxRect r2(margin+5,margin,width-margin-5,cH) ;
				
				root_elem->active = true;
				root_elem->rect = r2;
				root_elem->pBookEntry = NULL;
				// draw rounded rectangle

				if( count-1 == selection )
				{
					root_elem->bSelected = true;
					selectedElem = root_elem;
				}
				else root_elem->bSelected = false;

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
				
				root_elem->white_black = false;
				root_elem->bookMove = pRoot->black;

				if( pRoot == m_frame->currentGame->pRoot )
				{
					root_elem->sanMove = "Start";
					root_elem->pMove = pRoot;
				}
				else
				{
					root_elem->sanMove = textStr;
					root_elem->pMove = pRoot;
				}

			}
			else
			{
				count = 0;
				while( temp )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+5,margin+cH*count,width-margin-5,cH) ;
					// draw rounded rectangle
						
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->isBookMove = false;
					tree_elems[numElems]->pBookEntry = NULL;

					
					string textStr = temp->info;
					textStr+=".";
					textStr+=temp->white;	

					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->white;
					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = true;
					
					tree_elems[numElems]->score = wxString::Format("%3.2f",(float)(temp->move_cp_white )/100.0f);
					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );

					numElems++;

					numSelectionsThisMove++;
					count++;
					temp = temp->pVarWhite;
				}
				descent = count*20+margin;
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
				count = 1;
				while( temp && temp->black != "")
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+10,margin+cH*count,width-margin-5,cH) ;
					// draw rounded rectangle
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->isBookMove = false;
					tree_elems[numElems]->pBookEntry = NULL;
				
				
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					
					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->black;
					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = false;

					wxSize szTxt = dc.GetTextExtent(textStr.c_str());
					tree_elems[numElems]->score = wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f);
					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );

					numElems++;
					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;

				int cH = szTxtGlobal.GetHeight()+4;
				wxRect r2(margin+5,margin,width-margin-5,cH) ;
				// draw rounded rectangle
				root_elem->active = true;
				root_elem->rect = r2;
				root_elem->isBookMove = false;
				root_elem->pBookEntry = NULL;
				

				
				string textStr = pRoot->info;
				int move_color = lastMovePtr->GetVariationRootColor();
				if( move_color == 0 )
				{
					textStr+=".";
					textStr+=pRoot->white;	
					if( pRoot == m_frame->currentGame->pRoot )
					{
						root_elem->sanMove = "Start";
						root_elem->pMove = pRoot;
					}
					else
					{
						root_elem->sanMove = textStr;
						root_elem->pMove = pRoot;
					}
				}
				else
				{
					textStr+="...";
					textStr+=pRoot->black;
					if( pRoot == m_frame->currentGame->pRoot )
					{
						root_elem->sanMove = "Start";
						root_elem->pMove = pRoot;
					}
					else
					{
						root_elem->sanMove = textStr;
						root_elem->pMove = pRoot;
					}
				}



				//root_elem.pMove = pRoot;
				root_elem->white_black = false;
				//root_elem.sanMove = textStr;
				root_elem->bookMove = pRoot->black;
			}
			else
			{
				count = 0;
				while( temp && temp->black != "")
				{
					int cH = szTxtGlobal.GetHeight()+4;
					wxRect r2(margin+5,margin+cH*count,width-margin-5,cH) ;
					// draw rounded rectangle
					tree_elems[numElems]->active = true;
					tree_elems[numElems]->rect = r2;
					tree_elems[numElems]->pBookEntry = NULL;

				
					string textStr = temp->info;
					textStr+="...";
					textStr+=temp->black;
					tree_elems[numElems]->sanMove = textStr;
					tree_elems[numElems]->bookMove = temp->black;
					tree_elems[numElems]->score  = wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f);
	
					tree_elems[numElems]->scoreRect = wxRect( r2.x+2 +r2.GetWidth() -szTxtGlobal.GetWidth()-2, r2.y+2, r2.width - 4, r2.height-4 );

					tree_elems[numElems]->pMove = temp;
					tree_elems[numElems]->white_black = false;
					tree_elems[numElems]->isBookMove = false;

					numElems++;
					numSelectionsThisMove++;
					count++;
					temp = temp->pVarBlack;
				}
				descent = count*20+margin;
			}
		}
	}

//	m_frame->DoLookupFromBook();

	for( int i = 0; i < m_frame->bookLookupCount; i++ )
	{
		bool bFoundBookMove = false;
		for( int j = 0; j < numElems; j++ )
		{
			if( tree_elems[j]->bookMove.find(m_frame->entries[i].san)!=-1 )
			{
				bFoundBookMove = true;
				tree_elems[j]->isBookMove = true;
				tree_elems[j]->pBookEntry = &m_frame->entries[i];
			}
		}

		if( !bFoundBookMove )
		{
			wxRect r2;

			if( numElems > 0 )
			{
				if( root_elem->active )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+10,tree_elems[numElems-1]->rect.y + cH,width-margin-5,cH) ;
				}
				else
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+5,tree_elems[numElems-1]->rect.y + cH,width-margin-5,cH) ;
				}
			}
			else
			{

				if( root_elem->active )
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+10,margin,width-margin-5,cH) ;
				}
				else
				{
					int cH = szTxtGlobal.GetHeight()+4;
					r2 = wxRect(margin+5,margin,width-margin-5,cH) ;
				}
				
			}
			


					// draw rounded rectangle
			tree_elems[numElems]->active = true;
			
			tree_elems[numElems]->rect = r2;
			tree_elems[numElems]->isBookMove = true;
			tree_elems[numElems]->pBookEntry = &m_frame->entries[i];	

			
			string textStr = "";
			if( whiteMove )
			{
				char buff[10];
				sprintf(buff, "%d", atoi(m_frame->currentGame->pCurrMove->info.c_str())+1);
				textStr += buff;//m_frame->currentGame->pCurrMove->info;
				textStr+=".";
				textStr+=m_frame->entries[i].san;
				tree_elems[numElems]->white_black = true;
				//textStr+=m_frame->entries[i].san;
			}
			else
			{
				textStr = m_frame->currentGame->pCurrMove->info;
				textStr+="...";
				textStr+=m_frame->entries[i].san;
				tree_elems[numElems]->white_black = false;
			}
					
			tree_elems[numElems]->sanMove = textStr;
			tree_elems[numElems]->bookMove = m_frame->entries[i].san;
			tree_elems[numElems]->pMove = NULL;
			

			wxSize szTxt = dc.GetTextExtent(textStr.c_str());
			tree_elems[numElems]->score = "0.00";//wxString::Format("%3.2f",(float)(temp->move_cp_black )/100.0f);
					
			count++;
			numElems++;
			numSelectionsThisMove++;
		}
	}


	if( root_elem->active == true )
	{
		if( !(numElems == 0) )
		{
			tree_elems[numElems-1]->pNext = root_elem;
			root_elem->pNext = tree_elems[0];
			tree_elems[0]->pPrev = root_elem;
			root_elem->pPrev = tree_elems[numElems-1];
		}

	}
	else
	{
		if( !(numElems == 0) )
		{
			tree_elems[numElems-1]->pNext = tree_elems[0];
			tree_elems[0]->pPrev = tree_elems[numElems-1];
		}

	}
}

void TreeCanvas::OnPaint2(wxPaintEvent &event)
{
}