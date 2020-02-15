#include "MatchResultsCanvas.h"

//
#include "mdi.h"
#include "GameClock.h"
//#include "BoardCanvas.h"
//#include "MyPipePanel.h"
//#include "PGLookup.h"
//#include "PGKey.h"
//#include "TreeCanvas.h"



BEGIN_EVENT_TABLE(MatchResultsCanvas, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (MatchResultsCanvas::OnPaint)
	EVT_IDLE (MatchResultsCanvas::OnIdle)
	EVT_LEFT_DOWN (MatchResultsCanvas::OnMouseLeftClick)
	
END_EVENT_TABLE()

MatchResultsCanvas::MatchResultsCanvas(MyFrame *parent, wxWindow *winParent)
        : wxScrolledWindow((wxWindow*)winParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
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
	m_font3 = wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD );
	//showVariationRoot = true;
	//numSelectionsThisMove = 0;
	//m_bmp.LoadFile("book.png", wxBITMAP_TYPE_PNG);
	//visitTreeCount = 0;
	//SetScrollbars( 10, 10, 50, 1024 );
	SetBackgroundColour(*wxWHITE);

	

	SetScrollbars( 10, 10, 1024, 1024 );

#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif
}

MatchResultsCanvas::~MatchResultsCanvas()
{
}



void MatchResultsCanvas::OnPaint(wxPaintEvent &event)
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

#ifdef wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();
	gc->SetFont(m_font3,*wxBLACK);
	double currentHeight = 0;

	double topMargin = 40.0f;
	double leftMargin = 10.0f;

	wxString matchResults = "Match Results";
	wxString eventName = "Event Name: ";
	wxString timeControls = "Time Controls";
	wxString numGames = "Number Of Games";
	wxString swapSidesBetweenGames = "Swap Sides Between Games: ";

	// draw match results
	gc->DrawText(matchResults, leftMargin, currentHeight, 0);

	double header1w=0.0f;
	double header1h=0.0f;
	gc->GetTextExtent(matchResults, &header1w, &header1h);

	currentHeight+= header1h + 20;

	// draw event name
	gc->SetFont(m_font2,*wxBLACK);
	gc->DrawText(eventName, leftMargin, currentHeight);

	double eventNameW=0.0f;
	double eventNameH=0.0f;
	gc->GetTextExtent(eventName, &eventNameW, &eventNameH);

	

	gc->SetFont(m_font, *wxBLACK);
	gc->DrawText(m_frame->m_engineVsEngine.eventName.c_str(), leftMargin+eventNameW + 10, currentHeight);

	currentHeight+= eventNameH *1.5;


	// draw Time Controls
	gc->SetFont(m_font2,*wxBLACK);
	gc->DrawText(timeControls, leftMargin, currentHeight, 0);

	double timeControlW=0.0f;
	double timeControlH=0.0f;
	gc->GetTextExtent(timeControls, &timeControlW, &timeControlH);

	gc->SetFont(m_font, *wxBLACK);
	gc->DrawText(wxString::Format("%d Minutes, %d Seconds, %d Increment (ms)", m_frame->gameClock->SetMinutes, m_frame->gameClock->SetSeconds, m_frame->gameClock->increment ), leftMargin+timeControlW + 10, currentHeight);

	currentHeight+= timeControlH *1.5;


	// draw number of games
	gc->SetFont(m_font2,*wxBLACK);
	gc->DrawText(numGames, leftMargin, currentHeight, 0);

	double numGamesW=0.0f;
	double numGamesH=0.0f;
	gc->GetTextExtent(numGames, &numGamesW, &numGamesH);

	gc->SetFont(m_font, *wxBLACK);
	gc->DrawText(wxString::Format("%d", m_frame->m_engineVsEngine.numGames), leftMargin+numGamesW + 10, currentHeight);

	currentHeight+= numGamesH *1.5;


	// draw swap sides between games
	gc->SetFont(m_font2,*wxBLACK);
	gc->DrawText(swapSidesBetweenGames, leftMargin, currentHeight, 0);

	double swapSidesW=0.0f;
	double swapSidesH=0.0f;
	gc->GetTextExtent(swapSidesBetweenGames, &swapSidesW, &swapSidesH);

	gc->SetFont(m_font, *wxBLACK);
	gc->DrawText((m_frame->m_engineVsEngine.swapSidesBetweenGames ? "true": "false"), leftMargin+swapSidesW + 10, currentHeight);

	currentHeight+= swapSidesH *1.5;

	currentHeight += swapSidesH;

	// draw table header



	double textHeight = 0.0f;
	double tableNumberWidth =0.0f;
	double tableEngine1Width = 0.0f;
	double tableEngine2Width = 0.0f;
	double drawWidth = 0.0f;

	gc->SetFont(m_font2,*wxBLACK);

	wxString numberText = "Number";
	wxString e1 = m_frame->m_engineVsEngine.engine1Name;
	wxString e2 = m_frame->m_engineVsEngine.engine2Name;

	gc->GetTextExtent(numberText, &tableNumberWidth, &textHeight);
	gc->GetTextExtent(e1, &tableEngine1Width, &textHeight);
	gc->GetTextExtent(e2, &tableEngine2Width, &textHeight);


	gc->DrawText(numberText, leftMargin, currentHeight);
	gc->DrawText(e1, leftMargin, currentHeight + textHeight);
	gc->DrawText(e2, leftMargin, currentHeight + 2*textHeight);

	//currentHeight += textHeight;



	double innerMargin = (tableEngine1Width > tableEngine2Width ? tableEngine1Width : tableEngine2Width);
	gc->SetFont(m_font,*wxBLACK);
	gc->GetTextExtent("1/2", &drawWidth, &textHeight);
	drawWidth*=2;

	gc->SetBrush(*wxWHITE_BRUSH);
	gc->DrawRectangle(leftMargin-2,currentHeight-2, 
		leftMargin + drawWidth*(m_frame->m_engineVsEngine.numGames+1) + innerMargin+4,
		3*(textHeight+4));

	gc->SetBrush(*wxTRANSPARENT_BRUSH);
	gc->DrawRectangle(leftMargin-2,currentHeight-2, 
		leftMargin + drawWidth*(m_frame->m_engineVsEngine.numGames+1) + innerMargin+4,
		textHeight+4);

	gc->DrawRectangle(leftMargin-2,currentHeight-2, 
		innerMargin,
		3*(textHeight+4));

	gc->SetBrush(*wxBLACK_BRUSH);

	textHeight+=4;
	gc->DrawText(numberText, leftMargin, currentHeight);
	gc->DrawText(e1, leftMargin, currentHeight + textHeight);
	gc->DrawText(e2, leftMargin, currentHeight + 2*textHeight);

	innerMargin+=2;

	

	// results
	for( int i = 0; i < m_frame->m_engineVsEngine.list_engine_games.size(); i++ )
	{
		gc->DrawText( wxString::Format("%d",i+1), leftMargin + innerMargin+(i)*drawWidth, currentHeight);

		if( m_frame->m_engineVsEngine.list_engine_games[i]->whiteName == e1 )
		{
			if( m_frame->m_engineVsEngine.list_engine_games[i]->result == "1/2-1/2")
			{
				gc->DrawText( "1/2", leftMargin + innerMargin+ (i)*drawWidth, currentHeight + textHeight);
				gc->DrawText( "1/2", leftMargin + innerMargin+(i)*drawWidth, currentHeight+2*textHeight);
			}
			else if ( m_frame->m_engineVsEngine.list_engine_games[i]->result == "1-0")
			{
				gc->DrawText( "1", leftMargin + innerMargin+(i)*drawWidth, currentHeight+textHeight);
				gc->DrawText( "0", leftMargin + innerMargin+(i)*drawWidth, currentHeight+2*textHeight);
			}
			else if ( m_frame->m_engineVsEngine.list_engine_games[i]->result == "0-1")
			{
				gc->DrawText( "0", leftMargin + innerMargin+(i)*drawWidth, currentHeight+textHeight);
				gc->DrawText( "1", leftMargin + innerMargin+(i)*drawWidth, currentHeight+2*textHeight);
			}
		}
		else
		{
			if( m_frame->m_engineVsEngine.list_engine_games[i]->result == "1/2-1/2")
			{
				gc->DrawText( "1/2", leftMargin + innerMargin+(i)*drawWidth, currentHeight+textHeight);
				gc->DrawText( "1/2", leftMargin + innerMargin+(i)*drawWidth, currentHeight+2*textHeight);
			}
			else if ( m_frame->m_engineVsEngine.list_engine_games[i]->result == "1-0")
			{
				gc->DrawText( "0", leftMargin + innerMargin+(i)*drawWidth, currentHeight+textHeight);
				gc->DrawText( "1", leftMargin + innerMargin+(i)*drawWidth, currentHeight+2*textHeight);
			}
			else if ( m_frame->m_engineVsEngine.list_engine_games[i]->result == "0-1")
			{
				gc->DrawText( "1", leftMargin + innerMargin+(i)*drawWidth, currentHeight+textHeight);
				gc->DrawText( "0", leftMargin + innerMargin+(i)*drawWidth, currentHeight+2*textHeight);
			}
		}
	}
#endif
}





void MatchResultsCanvas::OnIdle(wxIdleEvent &event)
{
	event.Skip();
}


void MatchResultsCanvas::OnMouseLeftClick(wxMouseEvent &event)
{

}