#include "GameClock.h"
#include "mdi.h"

#ifndef WHITE
enum {WHITE, BLACK};
#endif






BEGIN_EVENT_TABLE(GameClockPanel, wxScrolledWindow)
    EVT_PAINT  (GameClockPanel::OnPaint)
	EVT_IDLE(GameClockPanel::OnIdle)
    EVT_SIZE(GameClockPanel::OnSize)
END_EVENT_TABLE()

GameClockPanel::GameClockPanel(MyFrame *parent)
//:wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxFULL_REPAINT_ON_RESIZE  )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	SetDoubleBuffered(true);
	m_owner = parent;


	m_useContext = true;

	SetTimeControl(5,0,0,100);
	//Start();
	wxLongLong 	localTime = wxGetLocalTimeMillis ();
	lastTime = localTime;
	
	timeDelta = 10;
	increment = 0;

	whiteMoves = 0;
	blackMoves = 0;

	white_black = WHITE;

	bFideTimeControl = false;

	m_timeMode = BLITZ;
	
}


GameClockPanel::GameClockPanel(wxWindow* parent)
	       : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxFULL_REPAINT_ON_RESIZE  )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	SetDoubleBuffered(true);
	//m_owner = parent;



	m_useContext = true;

	SetTimeControl(5,0,3,100);
	//Start();
	wxLongLong 	localTime = wxGetLocalTimeMillis ();
	lastTime = localTime;
	lastRenderTime = localTime;
	
	timeDelta = 10;

	m_paused = false;

	white_black = WHITE;
}

GameClockPanel::~GameClockPanel()
{

}

void GameClockPanel::OnPaint(wxPaintEvent &event)
{
    wxPaintDC pdc(this);
	
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

    //m_owner->PrepareDC(dc);

	dc.SetFont(wxFont(14, wxMODERN, wxNORMAL, wxBOLD, false));


	char buff1[100];
	int minutesWhite;
	int secondsWhite;

	currTimeSecondsWhite = this->currTimeMilliSecondsWhite / 1000;

	if( currTimeSecondsWhite >= 60 )
	{
		minutesWhite = currTimeSecondsWhite/60;
		secondsWhite = currTimeSecondsWhite - minutesWhite*60;
	}
	else
	{
		minutesWhite = 0;
		secondsWhite=  currTimeSecondsWhite;
	}

	if( secondsWhite < 10 )
	{
		sprintf(buff1, "%d : 0%d", minutesWhite, secondsWhite );
	}
	else
	{
		sprintf(buff1, "%d : %d", minutesWhite, secondsWhite );
	}

	char buff2[100];
	int minutesBlack;
	int secondsBlack;

	currTimeSecondsBlack = this->currTimeMilliSecondsBlack / 1000;
	if( currTimeSecondsBlack >= 60 )
	{
		minutesBlack = currTimeSecondsBlack/60;
		secondsBlack = currTimeSecondsBlack - minutesBlack*60;
	}
	else
	{
		minutesBlack = 0;
		secondsBlack=  currTimeSecondsBlack;
	}
	if( secondsBlack < 10 )
	{
		sprintf(buff2, "%d : 0%d", minutesBlack,secondsBlack );
	}
	else
	{
		sprintf(buff2, "%d : %d", minutesBlack,secondsBlack );
	}

	wxSize sz1 = dc.GetTextExtent(buff1);
	wxSize sz2 = dc.GetTextExtent(buff2);

	wxSize sz = GetClientSize();

	dc.SetBrush(wxBrush(wxColour(150,  150,  255, 128)));
	
	dc.DrawRectangle(sz.GetWidth()/2 - sz1.GetWidth()- 35,
		sz.GetHeight()/2 - sz1.GetHeight()/2-5, sz1.GetWidth()+10, sz1.GetHeight()+10);
	
	dc.DrawText(buff1, wxPoint( sz.GetWidth()/2 - sz1.GetWidth()- 30,
								sz.GetHeight()/2 - sz1.GetHeight()/2 ));

	dc.DrawRectangle(sz.GetWidth()/2 + 30-5,
		sz.GetHeight()/2 - sz2.GetHeight()/2 -5, sz2.GetWidth()+10, sz2.GetHeight()+10);

	dc.DrawText(buff2, wxPoint( sz.GetWidth()/2 + 30,
								sz.GetHeight()/2 - sz2.GetHeight()/2 ));

	if( white_black == WHITE )
	{
		dc.SetBrush(wxBrush(wxColour(255,  255,  255, 255)));
		dc.DrawRectangle(sz.GetWidth()/2 - sz1.GetWidth()- 35 - sz1.GetHeight() - 10,
			sz.GetHeight()/2 - sz2.GetHeight()/2, 
			sz1.GetHeight(), sz1.GetHeight());
	}
	else
	{
		dc.SetBrush(wxBrush(wxColour(0,  0,  0, 255)));
		dc.DrawRectangle(sz.GetWidth()/2 + sz1.GetWidth()+ 35 + 10 ,
			sz.GetHeight()/2 - sz2.GetHeight()/2, sz1.GetHeight(), sz1.GetHeight());
	}


}

void GameClockPanel::OnSize(wxSizeEvent& event)
{
}

void GameClockPanel::Start()
{
}

void GameClockPanel::Update()
{
}


void GameClockPanel::SetTimeControl(int Minutes, int Seconds, int Incr_Millis, int tick_millis)
{
	//m_gameClock->SetTimeControl(Minutes, Seconds, Incr_seconds, tick_millis);
	currTimeSecondsWhite = Minutes * 60 + Seconds;
	currTimeSecondsBlack = Minutes * 60 + Seconds;

	currTimeMilliSecondsWhite = currTimeSecondsWhite * 1000;
	currTimeMilliSecondsBlack = currTimeSecondsBlack * 1000;

	SetMinutes = Minutes;
	SetSeconds = Seconds;

	increment = Incr_Millis;

	whiteMoves = 0;
	blackMoves = 0;

	if( currTimeMilliSecondsWhite <= 10000 )
		this->m_timeMode = HYPER_BULLET;
	else if( currTimeMilliSecondsWhite <= 180000 )
		this->m_timeMode = BULLET;
	else if( currTimeMilliSecondsWhite <= 600000 )
		this->m_timeMode = BLITZ;
	else if( currTimeMilliSecondsWhite <= 1800000)
		this->m_timeMode = RAPID;
	else if( currTimeMilliSecondsWhite <= 3600000)
		this->m_timeMode = STANDARD;
	else this->m_timeMode = LONG_TIME;

	plyCount = 0;

}

bool GameClockPanel::CheckTimeOutWhite()
{
	return false;
}

bool GameClockPanel::CheckTimeOutBlack()
{
	return false;
}

void GameClockPanel::PauseGame()
{
	//m_gameClock->PauseGame();
	this->m_paused = true;
}

void GameClockPanel::Stop()
{
	//ResetTime();
	this->m_paused = true;
}

void GameClockPanel::SetWhiteBlack(int wb) 
{ 
	white_black = wb;
	lastTime = wxGetLocalTimeMillis ();
	plyCount++;
	if( white_black != BLACK )
	{
		whiteMoves++;
		if( bFideTimeControl && (movesBeforeTimeControl == whiteMoves) )
		{
			currTimeMilliSecondsWhite = numMilliSecondsAfterTimeControl;
			increment = this->incrementAfterTimeControl;
		}
		else
		{
			currTimeMilliSecondsBlack +=increment;
		}
		
	}
	else
	{
		blackMoves++;
		if( bFideTimeControl && (movesBeforeTimeControl == blackMoves) )
		{
			currTimeMilliSecondsBlack = numMilliSecondsAfterTimeControl;
			increment = this->incrementAfterTimeControl;
		}
		else
		{
			currTimeMilliSecondsWhite +=increment;
		}
	}
	Refresh();
}

void GameClockPanel::ResetTime()
{
//	m_gameClock->Reset();
	currTimeSecondsWhite = SetMinutes * 60 + SetSeconds;
	currTimeSecondsBlack = SetMinutes * 60 + SetSeconds;

	currTimeMilliSecondsWhite = currTimeSecondsWhite * 1000;
	currTimeMilliSecondsBlack = currTimeSecondsBlack * 1000;
	plyCount = 0;
}

void GameClockPanel::OnIdle(wxIdleEvent &event)
{
//	OnPaint(evt);
	//event.RequestMore(false);
	TickClock();

}

void GameClockPanel::StartGame()
{
	lastTime = wxGetLocalTimeMillis ();
}


void GameClockPanel::TickClock()
{
	if( m_paused ) 
		return;

	currTime = wxGetLocalTimeMillis ();
	//lastTime = localTime;
	if( white_black != BLACK )
	{
		wxLongLong timeIncrement = currTime - lastTime;
		if( timeIncrement > timeDelta)
		{
			lastTime = currTime;

			this->currTimeMilliSecondsWhite -= timeIncrement.ToLong();
			
			// tick down in millis
			//currTimeSecondsWhite--;
		}
		//else event.RequestMore();

		
		/// calculate time to refresh
		if( currTime - lastRenderTime >= 1000 )
		{
			lastRenderTime = currTime;
			Refresh();
		}
		//else event.RequestMore();
	}
	else
	{
		wxLongLong timeIncrement = currTime - lastTime;
		if( timeIncrement > timeDelta)
		{
			lastTime = currTime;

			this->currTimeMilliSecondsBlack -= timeIncrement.ToLong();
			// tick down in millis
			//currTimeSecondsBlack--;
		}
		//else event.RequestMore();

		/// calculate time to refresh
		if( currTime - lastRenderTime >= 1000 )
		{
			lastRenderTime = currTime;
			Refresh();
		}
		//else event.RequestMore();

		
		
	}
}


