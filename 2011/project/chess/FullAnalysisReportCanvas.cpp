#include "FullAnalysisReportCanvas.h"



//
#include "mdi.h"




BEGIN_EVENT_TABLE(FullAnalysisReportCanvas, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (FullAnalysisReportCanvas::OnPaint)
	//EVT_IDLE (FullAnalysisReportCanvas::OnIdle)
	//EVT_LEFT_DOWN (FullAnalysisReportCanvas::OnMouseLeftClick)
	
END_EVENT_TABLE()

FullAnalysisReportCanvas::FullAnalysisReportCanvas(MyFrame *parent)
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
	m_font2 = wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD );
	//showVariationRoot = true;
	//numSelectionsThisMove = 0;
	//m_bmp.LoadFile("book.png", wxBITMAP_TYPE_PNG);
	//visitTreeCount = 0;
	//SetScrollbars( 10, 10, 50, 1024 );
	SetBackgroundColour(*wxWHITE);

	//m_bUsingExtraStats = false;

	SetScrollbars( 10, 10, 1024, 1024 );

#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif
}

FullAnalysisReportCanvas::~FullAnalysisReportCanvas()
{
}



void FullAnalysisReportCanvas::OnPaint(wxPaintEvent &event)
{
	bool m_useContext = true;
    wxPaintDC pdc(this);
	
//	m_frame->ClassifyECO();

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

	

	
//	gc->DrawBitmap(m_bmp, 0,0, 554, 534 ); 
/*	double wSan, hSan;
	gc->GetTextExtent("someText", &wSan, &hSan);
	double rH = hSan+5;
	double TextNumStartOffset = wSan + 20;
	double rectStart = wSan + 110;
	double topMargin = 40.0f;
	double leftMargin = 10.0f;

	gc->SetFont(m_font2,*wxBLACK);
	gc->DrawText("Move", leftMargin, 0, 0);
	gc->DrawText("Count", TextNumStartOffset, 0, 0);
	gc->DrawText("[%]", rectStart, 0, 0 );
	gc->SetFont(m_font,*wxBLACK);
	
	if( m_bUsingExtraStats == false )
	{
		for( int i = 0; i < this->vecResults.size(); i++ )
		{
			gc->DrawText(vecResults[i]->san, leftMargin, topMargin + i*rH ,0);
		
			gc->DrawText(wxString::Format("%d",vecResults[i]->GetTotal()), leftMargin+TextNumStartOffset, topMargin + i*rH ,0);

			gc->GetTextExtent(vecResults[i]->san, &wSan, &hSan);
		

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

			gc->SetFont(m_font,*wxBLACK);
		}
	}
	else
	{
		for( int i = 0; i < this->vecResultsExtraStats.size(); i++ )
		{
			gc->DrawText(vecResultsExtraStats[i]->san, leftMargin, topMargin + i*rH ,0);
		
			gc->DrawText(wxString::Format("%d",vecResultsExtraStats[i]->GetTotal()), leftMargin+TextNumStartOffset, topMargin + i*rH ,0);

			gc->GetTextExtent(vecResultsExtraStats[i]->san, &wSan, &hSan);
		

			float wWinsPercentage = vecResultsExtraStats[i]->GetWhiteWinsPercentage();
			float wWinsSize = wWinsPercentage*2.0f;
			float drawsPercentage = vecResultsExtraStats[i]->GetDrawPercentage();
			float drawsSize = drawsPercentage*2.0f;
			float bWinsPercentage = vecResultsExtraStats[i]->GetBlackWinsPercentage();
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

			gc->SetFont(m_font,*wxBLACK);
		}
	}
	*/
#endif
}







