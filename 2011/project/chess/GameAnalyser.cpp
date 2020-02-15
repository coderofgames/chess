#include "GameAnalyser.h"
#include "mdi.h"

#include "MyPipePanel.h"


BEGIN_EVENT_TABLE(GameAnalyser, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (GameAnalyser::OnPaint)
	//EVT_IDLE (TreeCanvas::OnIdle)
	//EVT_LEFT_DOWN (TreeCanvas::OnMouseLeftClick)
	
END_EVENT_TABLE()

GameAnalyser::GameAnalyser(MyFrame* parent, MyPipePanel *pp)
        : wxScrolledWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE |wxWANTS_CHARS )
{
	m_parent = parent;

	m_pipePanel = pp;

	secondsPerMove = 30;
	gameAnalysis = new AnalysisInfo();
	//int maximum = m_game->GetMoveCount();

    m_Progress = new wxGauge
						(this, PROGRESS_ID,100,wxPoint(10,50));
	/*
                         wxT("Progress dialog"),
                         wxT("Wait until the thread terminates or press [Cancel]"),
                         100,
                         this,
                         wxPD_CAN_ABORT |
                         wxPD_APP_MODAL |
                         wxPD_ELAPSED_TIME |
                         wxPD_ESTIMATED_TIME |
                         wxPD_REMAINING_TIME
                        );
						*/

	
	
}


GameAnalyser::~GameAnalyser()
{
	if( gameAnalysis )
	{
		delete gameAnalysis;
	}
}


void GameAnalyser::OnPaint(wxPaintEvent &event)
{
   wxPaintDC pdc(this);

	wxSize sz(GetClientSize());
	
#ifdef wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
	 bool m_useContext = true;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

    m_parent->PrepareDC(dc);

	dc.Clear();

#ifdef wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();

	gc->PushState();

	wxString text1 = "Moves Analyzed: ";
	double w, h;
	gc->GetTextExtent(text1,&w,&h);
	gc->SetBrush(wxColour(150,  150,  255, 255));
	gc->DrawRectangle(0,0,w+5,h);
	gc->DrawText(text1, 5,0,0);
	
	
	gc->DrawText("Progress", 0, h, 0);
	gc->PopState();
#endif


}

void GameAnalyser::OnIdle(wxIdleEvent &event)
{
}
