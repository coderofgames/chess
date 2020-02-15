#include "MoveDataCanvas.h"

//
#include "mdi.h"
#include "BoardCanvas.h"
#include "MyPipePanel.h"
#include "PGLookup.h"
#include "PGKey.h"
#include "TreeCanvas.h"

BEGIN_EVENT_TABLE(MoveDataCanvas, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (MoveDataCanvas::OnPaint)
	EVT_IDLE (MoveDataCanvas::OnIdle)
	EVT_LEFT_DOWN (MoveDataCanvas::OnMouseLeftClick)
	
END_EVENT_TABLE()

MoveDataCanvas::MoveDataCanvas(MyFrame *parent)
        : wxScrolledWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE |wxWANTS_CHARS )
{
	m_frame = parent;
	//m_useContext = true;
	//selection = 0;
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

#ifdef WINDOWS_7
	SetDoubleBuffered(true);
#endif

	//m_font = wxFont(8, wxMODERN, wxNORMAL, wxBOLD, false);
	m_font = wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL );
	//showVariationRoot = true;
	//numSelectionsThisMove = 0;

	//visitTreeCount = 0;
	//SetScrollbars( 10, 10, 50, 1024 );
#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif
}

MoveDataCanvas::~MoveDataCanvas()
{
}



void MoveDataCanvas::OnPaint(wxPaintEvent &event)
{
	bool m_useContext = true;
    wxPaintDC pdc(this);
	
	m_frame->ClassifyECO();

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

	
	
	gc->SetFont(m_font,*wxBLACK);

	int margin = 15;
	double textWidth,textHeight;
	
	//gc->SetFont(m_font, *wxBLACK);
	gc->GetTextExtent("AnyText", &textWidth, &textHeight);
	
	
	//wxRect r4(margin+5,margin,sz.GetWidth()-margin-10,textHeight) ;
	wxRect r4(margin,0,sz.GetWidth(),textHeight) ;

	wxFont m_font2 = wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD );

	char move_s[6];
	/*gc->SetBrush( wxBrush( *wxWHITE));
	gc->DrawRectangle(0,0, r4.width, textHeight);
	gc->SetBrush( wxBrush( wxColour( 150, 150, 255, 128),wxSOLID));
	gc->DrawRectangle(0,0, r4.width, textHeight);
	gc->DrawRectangle(0,textHeight, r4.width, textHeight);
	gc->DrawRectangle(0,2*textHeight, r4.width, textHeight);
	gc->DrawRectangle(0,3*textHeight, r4.width, textHeight);
	*/
	//gc->DrawText("Move",   
	//				double(r4.x+5), 0);

	//gc->DrawText("White Wins /Draw /Black Wins",double(r4.x+5) + textWidth, 0);
	int ECO_START = m_frame->currentECO.Find("ECO");
	int OPENING_START = m_frame->currentECO.Find("Opening");
	wxString eco_string = "ECO: ";
	wxString opening_string = "Opening: ";
	wxString variation_string = "Variation: ";
	wxString variation_string_line1= "Variation: ";
	wxString variation_string_line2 = "";
	wxString variation_string_line3 = "";

	wxString ECO_CODE = "";
	wxString OPENING_CODE = "";
	wxString VARIATION_CODE = "";

	

	if( OPENING_START != -1 )
	{
		ECO_CODE += m_frame->currentECO.substr(4, OPENING_START-4);

		int VARIATION_START = m_frame->currentECO.Find("Variation");
		if( VARIATION_START != -1 )
		{
			OPENING_CODE += m_frame->currentECO.substr(OPENING_START+8, VARIATION_START-OPENING_START-8);
			VARIATION_CODE += m_frame->currentECO.substr(VARIATION_START+10, m_frame->currentECO.Length()-VARIATION_START-10);
			/*int VARIATION_STRING_COMMA = variation_string.Find(",");
			if( VARIATION_STRING_COMMA != -1 )
			{
				variation_string_line1 += variation_string.substr(0, VARIATION_STRING_COMMA);
				wxString variation_string_line21 = variation_string.substr(VARIATION_STRING_COMMA+1, variation_string.Length() - VARIATION_STRING_COMMA-1);
				int VARIATION_LINE2_COMMA = variation_string_line21.find(",");
				if( VARIATION_LINE2_COMMA != -1 )
				{
					variation_string_line2 = variation_string_line21.substr(0, VARIATION_LINE2_COMMA);
					variation_string_line3 = variation_string_line21.substr(VARIATION_LINE2_COMMA+1, variation_string_line21.Length() - VARIATION_LINE2_COMMA-1);
				}
				else
				{
					variation_string_line2 = variation_string_line21;
				}
			}
			else
			{
				variation_string_line1 += variation_string;
			}*/
		}
		else
		{
			OPENING_CODE += m_frame->currentECO.substr(OPENING_START+8, m_frame->currentECO.Length()-OPENING_START-8);
		}
	}
	else
	{
		ECO_CODE += m_frame->currentECO.substr(4, m_frame->currentECO.Length()-4);
	}

	 
	gc->SetFont(m_font2,*wxBLACK);
		
	gc->DrawText(eco_string, 5,0);
	gc->DrawText(opening_string, 5,textHeight);
	gc->DrawText(variation_string, 5,2*textHeight);

	double eco_code_w = 0.0f;
	double eco_code_h = 0.0f;
	gc->GetTextExtent(eco_string, &eco_code_w, &eco_code_h);

	double opening_code_w = 0.0f;
	double opening_code_h = 0.0f;
	gc->GetTextExtent(opening_string, &opening_code_w, &opening_code_h);

	double var_code_w = 0.0f;
	double var_code_h = 0.0f;
	gc->GetTextExtent(variation_string, &var_code_w, &var_code_h);

	double space_size = 0.0f;
	gc->GetTextExtent("  ", &space_size, 0);

	//gc->DrawText(m_frame->currentECO, double(r4.x+5),0);
	gc->SetFont(m_font,*wxBLACK);
	gc->DrawText(ECO_CODE, eco_code_w + space_size, 0);
	gc->DrawText(OPENING_CODE, opening_code_w + space_size, textHeight);
	gc->DrawText(VARIATION_CODE, var_code_w + space_size, 2*textHeight);




	/*if( variation_string_line2 != "" )
	{
		double w, h;
		gc->GetTextExtent("Variation: ", &w, &h);
		gc->DrawText(variation_string_line2,w,3*textHeight);
		if( variation_string_line3 != "" )
		{
			gc->DrawText(variation_string_line3, w,4*textHeight);
		}
	}*/

	r4.y += 30;

	//wxFont fontBold2 = wxFont(8, wxMODERN, wxNORMAL,wxNORMAL , false);

	//wxFont fontBold3 = wxFont(6, wxMODERN, wxNORMAL,wxNORMAL , false);
	
	

	//textHeight *=1.5;
	double rH = textHeight + 5;

	if( m_frame->polyglotBook )
	{
		int total_weight=0;
		/*for(int i=0;i<m_frame->bookLookupCount;i++){
			total_weight+=m_frame->entries[i].weight;
		}*/
		//double width
		double theight = textHeight *2;

		if( (m_frame->treeCanvas->selectedElem->pBookEntry != NULL) )
		{
			entry_t* entry = (entry_t*)m_frame->treeCanvas->selectedElem->pBookEntry;

			double twidth;
			//gc->GetTextExtent(m_frame->entries[i].san.c_str(),&twidth, &theight);
			
			twidth = double(r4.x+textWidth);
			double StartP = double(r4.x+5);// + twidth;

			int drawNum = entry->n - entry->weight - entry->learn;
			double percentage = double( r4.width -twidth -r4.x-20) /((double) entry->n);

			double whiteWinsSize = ((double) entry->weight)*percentage;
			double drawSize = double(drawNum)*percentage;
			double blackWinsSize = ((double) entry->learn)*percentage;

			double totalSize = ((double)entry->n) * percentage;

			double whiteWinsPercentageTextNum = ((double)entry->weight * 100/ (double)entry->n);
			double drawPercentageTextNum = ((double)drawNum *100/ (double)entry->n);
			double blackWinsPercentageTextNum = ((double)entry->learn *100/ (double)entry->n);
			
			wxString whiteWins = wxString::Format("%3.1f%%", whiteWinsPercentageTextNum);
			wxString drawsTxt = wxString::Format("%3.1f%%", drawPercentageTextNum);
			wxString blackWins = wxString::Format("%3.1f%%", blackWinsPercentageTextNum);

			double scoreTxtSizeW, scoreTxtSizeH;

			//float wWinsPercentage = vecResults[i]->GetWhiteWinsPercentage();
			float wWinsSize = whiteWinsPercentageTextNum*2.0f;
			//float drawsPercentage = vecResults[i]->GetDrawPercentage();
			float drawsSize = drawPercentageTextNum*2.0f;
			//float bWinsPercentage = vecResults[i]->GetBlackWinsPercentage();
			float bWinsSize = blackWinsPercentageTextNum *2.0f;


				double depthOfBars = r4.y + 30;

			gc->PushState();
			if( wWinsSize > 0.0f )
			{
				gc->SetBrush(*wxWHITE_BRUSH);
				gc->DrawRectangle(StartP, depthOfBars, wWinsSize, textHeight );
		
			
				gc->GetTextExtent(whiteWins, &scoreTxtSizeW, &scoreTxtSizeH);
				if( wWinsSize > scoreTxtSizeW )
				{
					gc->SetFont(m_font,*wxBLACK);
					gc->DrawText(whiteWins, StartP + wWinsSize / 2.0f - scoreTxtSizeW / 2.0f, depthOfBars);
				}

			}
			gc->PopState();

			gc->PushState();
			if( drawsSize > 0.0f )
			{
				gc->SetBrush(*wxGREY_BRUSH);
				gc->DrawRectangle(StartP + wWinsSize, 
									   depthOfBars , drawsSize, textHeight );

				gc->GetTextExtent(drawsTxt, &scoreTxtSizeW, &scoreTxtSizeH);
				if( drawsSize > scoreTxtSizeW )
				{
					//dc.SetTextForeground(*wxWHITE);
					gc->SetFont(m_font,*wxWHITE);
					gc->DrawText(drawsTxt, StartP + wWinsSize + drawsSize / 2.0f - scoreTxtSizeW / 2.0f, depthOfBars);
				}
			}
			gc->PopState();


			gc->PushState();
			if( bWinsSize > 0.0f )
			{
				gc->SetBrush(*wxBLACK_BRUSH);
				gc->DrawRectangle(StartP + wWinsSize + drawsSize, 
										   depthOfBars, bWinsSize, textHeight );

				gc->GetTextExtent(blackWins, &scoreTxtSizeW, &scoreTxtSizeH);
				if( bWinsSize > scoreTxtSizeW )
				{
					gc->SetFont(m_font,*wxWHITE);
					gc->DrawText(blackWins, StartP + wWinsSize + drawsSize + bWinsSize/ 2.0f- scoreTxtSizeW / 2.0f , depthOfBars);

				}
			}
			gc->PopState();
/*
			float wWinsPercentage = vecResults[i]->GetWhiteWinsPercentage();
			float wWinsSize = wWinsPercentage*2.0f;
			float drawsPercentage = vecResults[i]->GetDrawPercentage();
			float drawsSize = drawsPercentage*2.0f;
			float bWinsPercentage = vecResults[i]->GetBlackWinsPercentage();
			float bWinsSize = bWinsPercentage *2.0f;

			wxString whiteWins = wxString::Format("%3.1f%%", wWinsPercentage);
			wxString drawsTxt = wxString::Format("%3.1f%%", drawsPercentage);
			wxString blackWins = wxString::Format("%3.1f%%", bWinsPercentage);
			double scoreTxtSizeW, scoreTxtSizeH;

			gc->PushState();
			if( wWinsSize > 0.0f )
			{
				gc->SetBrush(*wxWHITE_BRUSH);
				gc->DrawRectangle(rectStart, topMargin + i*rH, wWinsSize, hSan );
		
			
				gc->GetTextExtent(whiteWins, &scoreTxtSizeW, &scoreTxtSizeH);
				if( wWinsSize > scoreTxtSizeW )
					gc->DrawText(whiteWins, rectStart + wWinsSize / 2.0f - scoreTxtSizeW / 2.0f, topMargin + i*rH);

			}
			gc->PopState();

			gc->PushState();
			if( drawsSize > 0.0f )
			{
				gc->SetBrush(*wxGREY_BRUSH);
				gc->DrawRectangle(rectStart + wWinsSize, 
									   topMargin + i*rH, drawsSize, hSan );

				gc->GetTextExtent(drawsTxt, &scoreTxtSizeW, &scoreTxtSizeH);
				if( drawsSize > scoreTxtSizeW )
				{
					//dc.SetTextForeground(*wxWHITE);
					gc->SetFont(m_font,*wxWHITE);
					gc->DrawText(drawsTxt, rectStart + wWinsSize + drawsSize / 2.0f - scoreTxtSizeW / 2.0f, topMargin + i*rH);
				}
			}
			gc->PopState();


			gc->PushState();
			if( bWinsSize > 0.0f )
			{
				gc->SetBrush(*wxBLACK_BRUSH);
				gc->DrawRectangle(rectStart + wWinsSize + drawsSize, 
										   topMargin + i*rH, bWinsSize, hSan );

				gc->GetTextExtent(blackWins, &scoreTxtSizeW, &scoreTxtSizeH);
				if( bWinsSize > scoreTxtSizeW )
				{
					gc->SetFont(m_font,*wxWHITE);
					gc->DrawText(blackWins, rectStart + wWinsSize + drawsSize + bWinsSize/ 2.0f- scoreTxtSizeW / 2.0f , topMargin + i*rH);

				}
			}
			gc->PopState();
*/
	/*		double depthOfBars = r4.y + 30;

			gc->SetFont(m_font, *wxBLACK);
			gc->SetPen(*wxBLACK_PEN);

			char buffWhiteWinsText[20];
			sprintf(buffWhiteWinsText, "W: %d%%", whiteWinsPercentageTextNum);

			char bufDrawsText[20];
			sprintf(bufDrawsText, "D: %d%%", drawPercentageTextNum);

			char bufBlackWinsText[20];
			sprintf(bufBlackWinsText, "B: %d%%", blackWinsPercentageTextNum);

			//gc->DrawText(wxT("White"), StartP + totalSize + 15,depthOfBars + double(textHeight)/4);
			//gc->DrawText(wxT("Draws"), StartP + totalSize + 15,depthOfBars +textHeight+ double(textHeight)/4);		
			//gc->DrawText(wxT("Black"), StartP + totalSize + 15,depthOfBars +2*textHeight+ double(textHeight)/4);

			gc->DrawText(buffWhiteWinsText, StartP + totalSize + 15,depthOfBars + double(textHeight)/4);
			gc->DrawText(bufDrawsText, StartP + totalSize + 15,depthOfBars +textHeight+ double(textHeight)/4);		
			gc->DrawText(bufBlackWinsText, StartP + totalSize + 15,depthOfBars +2*textHeight+ double(textHeight)/4);




			

			gc->SetBrush( wxBrush( *wxWHITE));
			gc->DrawRectangle(StartP,depthOfBars, totalSize, textHeight*3);


			
			//wxPen pn(*);
			

			gc->SetBrush(gc->CreateLinearGradientBrush	( 0,
				depthOfBars,
				0,
				depthOfBars +textHeight ,
				wxColour(255,255,255,255),
			wxColour(0,235,0,255) 
				
				));

			
			gc->DrawRectangle(StartP, depthOfBars, whiteWinsSize, textHeight);


			//gc->DrawText(buffWhiteWinsText, StartP + whiteWinsSize + 12,depthOfBars + double(textHeight)/4);



			gc->SetBrush(gc->CreateLinearGradientBrush	( 0,
			depthOfBars + textHeight,
			0,
			depthOfBars + 2*textHeight,
			wxColour(255,255,255,255),
				wxColour(0,0,255,255) 
			));	
			


			gc->DrawRectangle(StartP, depthOfBars + textHeight, drawSize, textHeight);
			

			//gc->DrawText(bufDrawsText, StartP + drawSize + 12,depthOfBars+textHeight + double(textHeight)/4 );


			gc->SetBrush(gc->CreateLinearGradientBrush	( 0,
				depthOfBars + 2*textHeight,
				0,
				depthOfBars + 3*textHeight,// + textHeight,
				wxColour(255,255,255,255),
				wxColour(255,0,0,255) 
			));	
			
			gc->DrawRectangle(StartP, depthOfBars + 2*textHeight, blackWinsSize, textHeight);
		*/

			//gc->DrawText(bufBlackWinsText, StartP + blackWinsSize + 12,depthOfBars+2*textHeight+ double(textHeight)/4);

		}
		/*for(int i=0;i<m_frame->bookLookupCount;i++){
		//	move_to_string(move_s,m_frame->entries[i].move);



			gc->SetPen(*wxBLACK_PEN);
			
			double twidth;
			//gc->GetTextExtent(m_frame->entries[i].san.c_str(),&twidth, &theight);
			
			twidth = double(r4.x+textWidth);
			double max_width = double( r4.width -twidth -r4.x-20);
			double percentage = double( r4.width -twidth -r4.x-20) /((double) m_frame->entries[i].n);
			double rectanglePosY = double(r4.y) +5.0f + double(i+1)*theight;
			double black_wins_end = ((double) m_frame->entries[i].learn)*percentage;
			

			double whiteStart = double(r4.x+5) + twidth;
			double posStartDrawX = whiteStart +((double) m_frame->entries[i].weight)*percentage;
			int drawNum = m_frame->entries[i].n - (m_frame->entries[i].weight + m_frame->entries[i].learn);
			double drawEnd = double(drawNum)*percentage;// ((double) )*percentage-((double) m_frame->entries[i].weight)*percentage;
	
			int drawWidthInt = m_frame->entries[i].n - (m_frame->entries[i].weight);
			double draw_width = ((double) drawWidthInt)*percentage;

			double centerWhiteWins = (posStartDrawX - whiteStart) / 2.f;
			double centerDraw =  (drawEnd - posStartDrawX) / 2.f;
			double centerBlackWins = (black_wins_end - drawEnd)/ 2.f;
			
			//gc->SetFont(fontBold2, *wxBLACK);
			gc->DrawText(m_frame->entries[i].san.c_str(),  
					double(r4.x+5), rectanglePosY);
			//wxGraphicsBrush gb;
			//gb.
			gc->SetBrush(*wxTRANSPARENT_BRUSH);
			gc->DrawRectangle(whiteStart, rectanglePosY,max_width, textHeight);

			gc->SetPen(*wxTRANSPARENT_PEN);
			//gc->SetFont(fontBold3, *wxWHITE);

			gc->SetBrush(gc->CreateLinearGradientBrush	( 0,
			double(r4.y) +5.0f + double(i)*theight,
			0,
			double(r4.y) +5.0f + double(i+1)*theight,
			wxColour(120,255,120,255),
			wxColour(0,235,0,255) 
			));	
				//wxBrush(*wxRED));
			//gc->DrawGra
			//wxRect r
			//dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxRIGHT);
			
			
			gc->DrawRectangle(whiteStart, rectanglePosY,max_width, textHeight);
			
			


			
			//gc->DrawText(wxString::Format("%d%%",m_frame->entries[i].weight),  
		//			whiteStart+centerWhiteWins, rectanglePosY);
			//gc->SetBrush(wxBrush(*wxGREEN));
			


			gc->SetBrush(gc->CreateLinearGradientBrush	( 0,
			double(r4.y) +5.0f + double(i)*theight,
			0,
			double(r4.y) +5.0f + double(i+1)*theight,
			wxColour(100,100,255,255),
			wxColour(0,0,255,255) 
			));
			
		
			gc->DrawRectangle(posStartDrawX, rectanglePosY, draw_width, textHeight);	
			
			
			//gc->DrawText(wxString::Format("%d%%",drawNum),  
			//		posStartDrawX+centerDraw, rectanglePosY);
			
			//(double)m_frame->entries[i].learn, ((double) m_frame->entries[i].n)); 
			//gc->SetBrush(wxBrush(*wxBLUE));


			gc->SetBrush(gc->CreateLinearGradientBrush	( 0,
			double(r4.y) +5.0f + double(i)*theight,
			0,
			double(r4.y) +5.0f + double(i+1)*theight,
			wxColour(255,100,100,255),
			wxColour(255,0,0,255) 
			));	

			gc->DrawRectangle(drawEnd + posStartDrawX, rectanglePosY, black_wins_end, textHeight);				

			//gc->DrawText(wxString::Format("%d%%",m_frame->entries[i].learn),  
			//		drawEnd + posStartDrawX+centerBlackWins, rectanglePosY);
				
		}*/
	}


	//gc->SetBrush( wxBrush( wxColour( 255, 255, 255, 255),wxSOLID));
	//gc->DrawRectangle(r4.x, r4.y, r4.width, r4.height);
	//gc->SetBrush(wxBrush(wxColour(150,  150,  255, 128)));
	//gc->DrawRectangle(r4.x, r4.y, r4.width, r4.height);
	//gc->DrawText("Move Data", r4.x+5, r4.y);
	//dc.SetTextForeground(*wxBLUE);
	//entry.
	//gc->DrawText(
/*	gc->DrawText("Current Move: ", r4.x+5, r4.y +5);
	gc->GetTextExtent("Current Move: ", &textWidth, &textHeight);
	if( m_frame->board_canvas->GetBoard()->mov != BLACK)
	{
		double textMoveWidth, textMoveHeight;
		if( this->m_frame->currentGame->pCurrMove->black != "" )
		{
			gc->GetTextExtent(this->m_frame->currentGame->pCurrMove->black, &textMoveWidth, &textMoveHeight);
			int rectLeftOffset = textMoveWidth/4;
			int rectRightOffset = textMoveWidth/4;//textSize.GetWidth()/4;
			gc->PushState();
			gc->SetBrush( wxBrush( wxColour( 255, 255, 255, 255),wxSOLID));
			gc->DrawRoundedRectangle(r4.x+5 + textWidth - rectLeftOffset, r4.y + 5 , textMoveWidth + rectRightOffset, textMoveHeight,1);
			gc->SetBrush( wxBrush( wxColour( 0, 0, 255, 128),wxSOLID));
			gc->DrawRoundedRectangle(r4.x+5 + textWidth - rectLeftOffset, r4.y + 5, textMoveWidth + rectRightOffset, textMoveHeight,1);
			gc->SetFont(fontBold, *wxWHITE);
			
			gc->DrawText(this->m_frame->currentGame->pCurrMove->black, r4.x+5 + textWidth, r4.y + 5);
			gc->PopState();
		}
	}
	else
	{
		double textMoveWidth, textMoveHeight;
		if( this->m_frame->currentGame->pCurrMove->white != "" )
		{
			gc->GetTextExtent(this->m_frame->currentGame->pCurrMove->white, &textMoveWidth, &textMoveHeight);
			int rectLeftOffset = textMoveWidth/4;
			int rectRightOffset = textMoveWidth/4;//textSize.GetWidth()/4;
			gc->PushState();
				
			gc->SetBrush( wxBrush( wxColour( 255, 255, 255, 255),wxSOLID));		
			gc->DrawRoundedRectangle(r4.x+5 + textWidth - rectLeftOffset, r4.y +5, textMoveWidth + rectRightOffset, textMoveHeight,1);

			gc->SetBrush( wxBrush( wxColour( 0, 0, 255, 128),wxSOLID));
			gc->DrawRoundedRectangle(r4.x+5 + textWidth - rectLeftOffset, r4.y +5, textMoveWidth + rectRightOffset, textMoveHeight,1);
			gc->SetFont(fontBold, *wxWHITE);
			gc->DrawText(this->m_frame->currentGame->pCurrMove->white, r4.x+5 + textWidth, r4.y +5);
			gc->PopState();
		}
	}
	gc->SetFont(fontBold, *wxBLACK);
	gc->DrawText("Score: ", r4.x+5, r4.y + 1*(textHeight+5)+5);

	if( m_frame->board_canvas->GetBoard()->mov == BLACK )
	{
		char cpBuff[50];
		_itoa(m_frame->currentGame->pCurrMove->move_cp_white, cpBuff, 10);
		double extentScoreWidth, extentScoreHeight;
		
		gc->GetTextExtent("Score: ", &extentScoreWidth, &extentScoreHeight);
		gc->DrawText(cpBuff, r4.x+5 + extentScoreWidth, r4.y + 1*(textHeight+5)+5);

		gc->DrawText("Depth: ", r4.x+5, r4.y + 2*(textHeight+5)+5);
		gc->GetTextExtent("Depth: ", &extentScoreWidth, &extentScoreHeight);
		gc->DrawText(m_frame->currentGame->pCurrMove->depthWhite, r4.x+5 + extentScoreWidth, r4.y + 2*(textHeight+5)+5);

	}
	else
	{
		char cpBuff[50];
		_itoa(m_frame->currentGame->pCurrMove->move_cp_black, cpBuff, 10);
		double extentScoreWidth, extentScoreHeight;
		
		gc->GetTextExtent("Score: ", &extentScoreWidth, &extentScoreHeight);
		gc->DrawText(cpBuff, r4.x+5 + extentScoreWidth, r4.y + 1*(textHeight+5)+5);

		gc->DrawText("Depth: ", r4.x+5, r4.y + 2*(textHeight+5)+5);
		gc->GetTextExtent("Depth: ", &extentScoreWidth, &extentScoreHeight);
		gc->DrawText(m_frame->currentGame->pCurrMove->depthBlack, r4.x+5 + extentScoreWidth, r4.y + 2*(textHeight+5)+5);
	}
	*/
#endif
}


void MoveDataCanvas::OnIdle(wxIdleEvent &event)
{
	event.Skip();
}


void MoveDataCanvas::OnMouseLeftClick(wxMouseEvent &event)
{

}