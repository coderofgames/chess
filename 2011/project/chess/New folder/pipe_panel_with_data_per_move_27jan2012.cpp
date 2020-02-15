#include "MyPipePanel.h"
#include "mdi.h"
#include "PgnLoad.h"
#include "BoardCanvas.h"
#include "TreeCanvas.h"
#include "EngineAnalysisCanvas.h"
#include "UCIEngineOptionsDialog.h"
#include "GameClock.h"
#include "MoveDataCanvas.h"
#include "OptionsFrame.h"
#include "MiscUtils.h"
#include "ScoreGraph.h"

#define USE_FEN

#if wxUSE_BUSYINFO
    #include "wx/busyinfo.h"
#endif // wxUSE_BUSYINFO

// ============================================================================
// MyPipePanel implementation
// ============================================================================




enum
{
    // timer ids
    Exec_TimerIdle = wxID_HIGHEST + 1740,
    Exec_TimerBg,

    // menu items
    Exec_Quit,// = 100,
    Exec_Kill,
    Exec_ClearLog,
    Exec_BeginBusyCursor,
    Exec_EndBusyCursor,
    Exec_SyncExec,// = 200,
    Exec_SyncNoEventsExec,
    Exec_AsyncExec,
    Exec_Shell,
    Exec_POpen,
    Exec_OpenFile,
    Exec_LaunchFile,
    Exec_OpenURL,
    Exec_DDEExec,
    Exec_DDERequest,
    Exec_Redirect,
    Exec_Pipe,
    Exec_About = wxID_ABOUT,

    // control ids
    Exec_Plus = 1000,
    Exec_Btn_SendFile,
    Exec_Minus,
    Exec_Btn_Close,
	COPY_UP,
	ENGINE_OPTIONS,
	LOCK_ENGINE
};


BEGIN_EVENT_TABLE(MyPipePanel, wxPanel)
    EVT_BUTTON(Exec_Plus, MyPipePanel::OnBtnPlus)
    EVT_TOGGLEBUTTON(Exec_Btn_SendFile, MyPipePanel::OnBtnSendFile)
    EVT_BUTTON(Exec_Minus, MyPipePanel::OnBtnMinus)
    EVT_BUTTON(Exec_Btn_Close, MyPipePanel::OnBtnClose)
	EVT_BUTTON(COPY_UP, MyPipePanel::OnBtnCopyUp)
	EVT_BUTTON(ENGINE_OPTIONS, MyPipePanel::OnBtnEngineOptions)
    EVT_TEXT_ENTER(wxID_ANY, MyPipePanel::OnTextEnter)
	EVT_TOGGLEBUTTON(LOCK_ENGINE, MyPipePanel::OnBtnLockEngine)

   // EVT_CLOSE(MyPipePanel::OnClose)

    EVT_END_PROCESS(wxID_ANY, MyPipePanel::OnProcessTerm)

	EVT_IDLE(MyPipePanel::OnIdle)
END_EVENT_TABLE()





//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
MyPipePanel::MyPipePanel(MyFrame *parent)
           : wxPanel((wxWindow*)parent, wxID_ANY,wxDefaultPosition)
{

	pParent = parent;
    

   // wxPanel *panel = new wxPanel(this, wxID_ANY);

//    m_textOut = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
//                              wxDefaultPosition, wxDefaultSize,
 //                             wxTE_PROCESS_ENTER);

#if USE_TEXT_PANEL
    m_textIn = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                               wxDefaultPosition, wxDefaultSize,
                               wxTE_PROCESS_ENTER|wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP);
	m_textIn->SetBackgroundColour(*wxWHITE);
#else
	analysisCanvas = new EngineAnalysisCanvas(this, parent);

#endif
	
	 
    m_textNodes = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                               wxDefaultPosition, wxDefaultSize,
                                 wxTE_CENTRE|wxTE_READONLY);
    m_textCurrMove = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                               wxDefaultPosition, wxDefaultSize,
                               wxTE_CENTRE|wxTE_READONLY);
    m_textTime = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                               wxDefaultPosition, wxDefaultSize,
                                wxTE_CENTRE|wxTE_READONLY);
	
	wxFont font(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD);

	wxFont fontRich(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD);

	/*
	m_textNodes->SetBackgroundColour(wxStockGDI::COLOUR_BLUE);
	m_textCurrMove->SetBackgroundColour(wxStockGDI::COLOUR_BLUE);
	m_textTime->SetBackgroundColour(wxStockGDI::COLOUR_BLUE);
	*/
    

	 // m_lbox=new wxListBox(panel, wxID_ANY,wxDefaultPosition,wxDefaultSize);
   // m_textIn->SetEditable(false);
   m_textErr = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                               wxDefaultPosition, wxDefaultSize,
                               wxTE_CENTRE| wxTE_READONLY);

	if ( font.Ok() )
	{
		m_textErr->SetFont(font);
		m_textNodes->SetFont(font);
		m_textCurrMove->SetFont(font);
		m_textTime->SetFont(font);

#if USE_TEXT_PANEL
		m_textIn->SetFont(fontRich);
#endif

	}

  //  m_textErr->SetEditable(false);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
   // sizerTop->Add(m_textOut, 0, wxGROW | wxALL, 5);

    wxSizer *sizerBtns = new wxBoxSizer(wxHORIZONTAL);

//	wxButton *sendButton = new wxButton(this, Exec_Plus, wxT("&Send"));
	// = new wxBitmap;
	analyzeBitmap.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Gear_med.png", wxBITMAP_TYPE_PNG);
//	analyzeBitmapSelected.LoadFile("icons/dev icons sekkumu/Gear.png", wxBITMAP_TYPE_PNG);
	wxBitmapToggleButton *analyzeBtn = new wxBitmapToggleButton(this,Exec_Btn_SendFile, analyzeBitmap);//,wxT("&Analyze"));
	analyzeBtn->SetToolTip(wxT("Enable/disable analysis"));
	//analyzeBtn->(analyzeBitmapSelected);
	//"icons\pixophilia\2"
	//"icons\shlyap\2\clipboard.png"
	stopBitmap.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Stop_med.png", wxBITMAP_TYPE_PNG);
//	stopBitmapSelected.LoadFile("icons/dev icons sekkumu/Gear.png", wxBITMAP_TYPE_PNG);
	wxBitmapButton *stopBtn = new wxBitmapButton(this,Exec_Btn_Close, stopBitmap);//,wxT("&Analyze"));
	stopBtn->SetToolTip(wxT("Shutdown Engine"));
	//	stopBtn->SetBitmapDisabled(stopBitmapSelected);	
	//analyzeBtn->Push
	//sendButton->SetBackgroundColour(*wxGREEN);
	 plusBitmap.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Add_small.png", wxBITMAP_TYPE_PNG);
	 
	 minusBitmap.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Remove_small.png", wxBITMAP_TYPE_PNG);

	 optionsBitmap.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Run_med.png", wxBITMAP_TYPE_PNG);
	 
	 lockEngineBitmap.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/2/key_med.png", wxBITMAP_TYPE_PNG);

	 copyAnalysisToNotationBitmap.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Write_small.png", wxBITMAP_TYPE_PNG);


	wxBitmapButton *plusBtn = new wxBitmapButton(this,Exec_Plus, plusBitmap);//,wxT("&Analyze"));
	wxBitmapButton *minusBtn = new wxBitmapButton(this,Exec_Minus, minusBitmap);
	plusBtn->SetToolTip(wxT("Increase Number of Lines"));
	minusBtn->SetToolTip(wxT("Decrease Number of Lines"));



	wxBitmapButton *copyUp = new wxBitmapButton(this,COPY_UP, copyAnalysisToNotationBitmap);//,wxT("&Analyze"));
	copyUp->SetToolTip(wxT("Copy Analysis to Notation"));

	wxBitmapButton *engineOptions = new wxBitmapButton(this,ENGINE_OPTIONS, optionsBitmap);//,wxT("&Analyze"));
	engineOptions->SetToolTip(wxT("Engine Options"));
	
	wxBitmapToggleButton *lockEngineToMove = new wxBitmapToggleButton(this,LOCK_ENGINE, lockEngineBitmap);//,wxT("&Analyze"));
	lockEngineToMove->SetToolTip(wxT("Lock Engine to Move, this allows you to browse the pgn while the engine is analysing"));
	
	//sizerBtns->
    //    Add(sendButton, 0, wxALL, 5);
    //sizerBtns->
     //   Add(analyzeBtn, 0, wxALL, 5);
 //   sizerBtns->
 //       Add(new wxButton(this, Exec_Minus, wxT("&Stop")), 0, wxALL, 5);
   // sizerBtns->
    //    Add(stopBtn, 0, wxALL, 5);

/*	wxSizer *sizerPlusMinus = new wxBoxSizer(wxVERTICAL);
	sizerPlusMinus->Add(plusBtn, 0, wxEXPAND, 2);
	sizerPlusMinus->Add(minusBtn, 0, wxEXPAND, 2);
	*/
	 
	wxSizer *sizerInfoText = new wxBoxSizer(wxHORIZONTAL);

	sizerInfoText->Add(analyzeBtn, 0, wxEXPAND, 2);
	sizerInfoText->Add(stopBtn, 0, wxEXPAND, 2);
	sizerInfoText->Add(lockEngineToMove, 0, wxEXPAND, 2);
	sizerInfoText->Add(copyUp, 0, wxEXPAND, 2);
	sizerInfoText->Add(engineOptions, 0, wxEXPAND, 2);
	//sizerInfoText->Add(plusBtn, 0, wxEXPAND, 2);
	//sizerInfoText->Add(sizerPlusMinus, 0, wxALL, 2);	
	sizerInfoText->Add(plusBtn, 0, wxEXPAND, 2);
	sizerInfoText->Add(minusBtn, 0, wxEXPAND, 2);	
	//sizerInfoText->Add(m_textErr, 0, wxEXPAND , 2);
	sizerInfoText->Add(m_textNodes, 0, wxEXPAND , 2);
	sizerInfoText->Add(m_textCurrMove, 0, wxEXPAND , 2);
	sizerInfoText->Add(m_textTime, 0, wxEXPAND , 2);
	sizerInfoText->Add(m_textErr, 0, wxEXPAND , 2);
	

//	sizerTop->Add(sizerBtns, 0,  wxTOP| wxALL, 5);
	sizerTop->Add(sizerInfoText, 0, wxTOP | wxALL, 5);
#if USE_TEXT_PANEL  
	sizerTop->Add(m_textIn, 1, wxGROW | wxALL, 5);
#else
	sizerTop->Add(analysisCanvas, 1, wxGROW | wxALL, 5);
#endif
	//sizerTop->Add

    this->SetSizer(sizerTop);
    sizerTop->Fit(this);

	engineLoaded = false;
	m_process = NULL;

	num_kibitz_lines = 4;
	
	m_readTimer = new ReadTimer(this);

	analyzing = false;

	isReading = false;

	engineColor = BLACK;

	LockEngineToMove = false;

	mode = 0;

	opponent = NULL;
	EngineVsEngineMode = false;

	timeMode = HYPER_BULLET;

	closeProblem = false;

	goMode = GO_INFINITE;

	fixedDepth = 5;
	fixedTimeMillis = 100;

	logToFile = false;

	startedAnalyzing = false;

	
	
    Show();
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
MyPipePanel::~MyPipePanel()
{
		// in case our thread is still running and for some reason we are destroyed,
// do wait for the thread to complete as it assumes that its MyImageDialog
// pointer is always valid
//	m_thread->Delete();
//	delete m_thread;
//	analyzeBitmap.FreeResource();
//	analyzeBitmapSelected.FreeResource();
	
	if( logFile.is_open() )
		logFile.close();

	if( m_readTimer )
	{
		if( m_process )
			DoShutdown();
	}
	tokenizedOptions.clear();
}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnIdle(wxIdleEvent &event)
{
	//if( engineLoaded )
	//	DoGet();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnGUIThreadEvent(wxThreadEvent& event)
{
	//DoGet();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SetPanelNameIntoCaption()
{
	wxString kbitzing_by = "Analysis Engine: ";


	kbitzing_by += panelName;


	pParent->m_mgr.GetPane(this).Name(panel_name).Caption(kbitzing_by);
	pParent->m_mgr.Update();
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::LoadEngine(const wxString& cmd,
                         wxProcess *process)
{
	closeProblem = false;

    m_process= process;
	pid = m_process->GetPid();
	m_process->SetNextHandler(this);
	engineLoaded = true;

  //  FindWindow(Exec_Plus)->Enable();
    FindWindow(Exec_Btn_SendFile)->Enable();
    FindWindow(Exec_Btn_Close)->Enable();

	//m_textErr->Clear();
	//m_textErr->AppendText(cmd);


    wxWindowDisabler disableAll;

	//m_process->



	wxString kbitzing_by = "Analysis Engine: ";
	int last_back_slash = cmd.find_last_of("\\");
	if( last_back_slash != -1 )
	{
		panelName=cmd.substr(last_back_slash+1, cmd.length()-last_back_slash+1);
		kbitzing_by += panelName;
	}
	else
	{
		panelName = cmd;
		kbitzing_by += cmd;
	}

	currentLoadedEngine = cmd;


	pParent->m_mgr.GetPane(this).Name(panel_name).Caption(kbitzing_by);
	pParent->m_mgr.Update();


    wxString s("uci");
    s += wxT('\n');
    (m_process)->GetOutputStream()->Write(s.c_str(), s.length());

	wxString loadingInfo = "Loading ";
	loadingInfo += panelName;

	wxBusyInfo info(loadingInfo, this->pParent);

    for ( int i = 0; i < 18; i++ )
    {
        wxMilliSleep(100);
        wxTheApp->Yield();
    }

	engineOptions = ""; // this has to reset;

	while ( m_process->GetInputStream()->CanRead() )
    {
		isReading = true;
        char buffer[4096];
        buffer[m_process->GetInputStream()->Read((void*)buffer, WXSIZEOF(buffer) - 1).LastRead()] = wxT('\n\0');
		
		engineOptions+= buffer;	
		//ProcessOutput(buffer, text);

    }

	tokenizedOptions.clear();
	istringstream iss(engineOptions);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
				back_inserter<vector<string> >(this->tokenizedOptions));
//	pParent->wnd10->AppendText(engineOptions);
//engineOptions

//	num_kibitz_lines
	if( mode == 0 )
	{
		wxString mpv("setoption name MultiPv value ");
		char itoabuf[5];
		itoa(num_kibitz_lines, itoabuf, 10);
		mpv += itoabuf;
		mpv += wxT('\n');
		(m_process)->GetOutputStream()->Write(mpv.c_str(), mpv.length());
	}
	else
	{
		wxString mpv("setoption name MultiPv value ");
		char itoabuf[5];
		itoa(1, itoabuf, 10);
		mpv += itoabuf;
		mpv += wxT('\n');
		(m_process)->GetOutputStream()->Write(mpv.c_str(), mpv.length());

	}


   // wxSleep(2);



	SendUciNewGame();

	wxSleep(1);


	//SendFen();
	SendMoves();

	if( m_readTimer == NULL )
	{
		m_readTimer = new ReadTimer(this);
	}

	m_readTimer->start();
  
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendStop()
{
	wxString s1("stop");
	s1 += wxT('\n');

	if( !m_process )
	{
		DoUnexpectedShutdown();
		this->isReading = false;
		return;
	}
	(m_process)->GetOutputStream()->Write(s1.c_str(), s1.length());
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendUciNewGame()
{
    wxString s2("ucinewgame");
    s2 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s2.c_str(), s2.length());

	wxString s3("isready");
	s3+= wxT('\n');
	(m_process)->GetOutputStream()->Write(s3.c_str(), s3.length());

    while( ( m_process)->GetInputStream()->CanRead() )
    {
		isReading = true;
        char buffer[20];
        buffer[( m_process)->GetInputStream()->Read((void*)buffer, WXSIZEOF(buffer) - 1).LastRead()] = wxT('\n\0');
		
		/*if( mode == 0 )
		{
			ProcessOutput(buffer, text);
		}
		else 
		{
			WaitBestMove(buffer, text);
		}*/
	// 	wxString buff(buffer);
	//	if( buff.Find("readyok"))
		//	break;
    }
}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendGoInfinite()
{
    wxString s4("go infinite");
    s4 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s4.c_str(), s4.length());
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendMoves()
{
	wxString s3 = "";

	PGN::Move* pMovePtr = pParent->currentGame->pRoot;

	// set this state somewhere
	if( pParent->currentGame->gameBeginsAtStartPos == false)
	{
		s3 += "position fen ";//moves";
		s3 += this->pParent->currentGame->pBase->getFenBlack();
		s3 += " moves";
	}
	else
	{
		s3 += "position startpos moves";
	}

	if( pMovePtr->white == "" && pMovePtr->x == 0 )
	{
		if( pParent->currentGame->gameBeginsAtStartPos == false)
		{
			SendFen();
			return;
		}
		else
		{
			s3 = "";
			s3 += "position startpos";
		}
	}
	else
	{
		if( pParent->currentGame->gameBeginsAtStartPos == true)	
		{
		
			s3 += pParent->currentGame->pCurrMove->GetMoveStringsFromStart(this->pParent->board_canvas->GetBoard()->mov);
		}
		else
		{
			s3 += pParent->currentGame->pCurrMove->GetMoveStringsFromFenSetupPosition(this->pParent->board_canvas->GetBoard()->mov);
		}
	}
	
	s3 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s3.c_str(), s3.length());	

	internalBoard.SetBoard(this->pParent->board_canvas->GetBoard()->Fen());

	int movNum;
	if( !LockEngineToMove )
	{
		movNum = atoi(pParent->currentGame->pCurrMove->info.c_str());
		storedMovNum = movNum;
	}
	else
	{
		movNum = storedMovNum;
	}

	int white_black = internalBoard.mov;

	if( white_black == 2 )
		white_black = 0;


	multiPvLines.SetCurrPly((storedMovNum-1)*2+white_black);

	multiPvLines.GetCurrent()->pMoveAnalyzed = pParent->currentGame->pCurrMove;
	

}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendFen()
{
	ChessBoard *board = pParent->board_canvas->GetBoard();

	string fen;// = board->Fen();
	if( board->mov == WHITE )
	{
		fen = pParent->currentGame->pCurrMove->m_fenBlack.c_str();
		if( fen == "" )
			fen = board->Fen();
	}
	if( board->mov == BLACK )
	{
		fen = pParent->currentGame->pCurrMove->m_fenWhite.c_str();
		if( fen == "" )
			fen = board->Fen();
	}

	internalBoard.SetBoard(fen);

	wxString s3("position fen ");

	s3 += (fen.c_str());
	s3 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s3.c_str(), s3.length());

	multiPvLines.GetCurrent()->pMoveAnalyzed = pParent->currentGame->pCurrMove;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendMoveTime()
{
	char buff[256];
	wxString s3("go ");

	//if( engineColor == WHITE )
//	{
		sprintf( buff, "%d", pParent->gameClock->GetEngineMilliSecondsWhite());
		s3+= "wtime ";
		s3+= buff;
		s3+= " ";
//	}
	//else
//	{
		sprintf( buff, "%d", pParent->gameClock->GetEngineMilliSecondsBlack());
		s3+= "btime ";
		s3+= buff;
//	}

	if( pParent->gameClock->increment != 0 )
	{
		s3+= " ";
		sprintf( buff, "%d", pParent->gameClock->increment);
		s3+= "winc ";
		s3+= buff;

		s3+=" ";
		s3+= "binc ";
		s3+= buff;
	}

	
	s3 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s3.c_str(), s3.length());
}





//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendMoveDepth()
{
	char buff[256];
	wxString s3("go ");

	//if( engineColor == WHITE )
//	{
		sprintf( buff, "%d", this->fixedDepth);
		s3+= "depth ";
		s3+= buff;
		s3+= " ";
//	}
	//else
//	{
	//	sprintf( buff, "%d", pParent->gameClock->GetEngineMilliSecondsBlack());
	//	s3+= "btime ";
	//	s3+= buff;
//	}

	
	s3 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s3.c_str(), s3.length());
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendFixedMoveTime()
{
	char buff[256];
	wxString s3("go ");

	//if( engineColor == WHITE )
//	{
		sprintf( buff, "%d", this->fixedTimeMillis);
		s3+= "movetime ";
		s3+= buff;
		s3+= " ";
//	}
	//else
//	{
	//	sprintf( buff, "%d", pParent->gameClock->GetEngineMilliSecondsBlack());
	//	s3+= "btime ";
	//	s3+= buff;
//	}

	
	s3 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s3.c_str(), s3.length());
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendOptionsString(string opts)
{
	(m_process)->GetOutputStream()->Write(opts.c_str(), opts.length());
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnBtnSendFile(wxCommandEvent& WXUNUSED(event))
{
	if( this->mode == 1 )return;

	if( !engineLoaded )
		return;

	if( !m_process )
	{
		DoUnexpectedShutdown();
		return;
	}

	if( analyzing )
	{
		startedAnalyzing = false;
		analyzing = false;
		SendStop();
		m_readTimer->Stop();
		return;
	}
	else
	{
		startedAnalyzing = true;
		analyzing = true;
	}

	wxSleep(1);

	//SendFen();
	SendMoves();

	SendGoInfinite();

	m_readTimer->start();
}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoSend()
{
    wxString s(m_textOut->GetValue());
    s += wxT('\n');
    (m_process)->GetOutputStream()->Write(s.c_str(), s.length());
    m_textOut->Clear();

    DoGet();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool MyPipePanel::CheckTime()
{
	if( engineColor == WHITE )
	{
		if( pParent->gameClock->currTimeMilliSecondsWhite > 0 )
		{
			return true;
		}
		else 
		{
			pParent->gameClock->Stop();

			if( pParent->board_canvas->GetBoard()->TimeMaterialTest(WHITE) == 0 )
			{
				pParent->currentGame->result = "1/2-1/2";
				string termination = "Draw Insufficient Material";
				if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
				{
					wxMessageBox("Draw Insufficient Material", "Game Over");
				}
				pParent->currentGame->WriteTerminationIntoHeader(termination);
				this->m_readTimer->SetTimeBetweenReads(100);
				this->m_readTimer->Stop();
				this->opponent->m_readTimer->SetTimeBetweenReads(100);
				this->opponent->m_readTimer->Stop();
				pParent->currentGame->WriteResultIntoHeader();
				pParent->RecreateGameList();
				
			}
			else
			{
				pParent->currentGame->result = "0-1";
				string termination = "Time Victory";
				if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
				{
					wxMessageBox("Black Wins On Time", "Game Over");
				}
				this->m_readTimer->SetTimeBetweenReads(100);
				this->m_readTimer->Stop();
				this->opponent->m_readTimer->SetTimeBetweenReads(100);
				this->opponent->m_readTimer->Stop();
				pParent->currentGame->WriteResultIntoHeader();
				pParent->currentGame->WriteTerminationIntoHeader(termination);
				pParent->RecreateGameList();
				
			}

			pParent->currentGame->AddResultNode(pParent->currentGame->result);
			return false;
		}
	}
	else
	{
		if( pParent->gameClock->currTimeMilliSecondsBlack > 0 )
		{
			return true;
		}
		else
		{
			pParent->gameClock->Stop();

			if( pParent->board_canvas->GetBoard()->TimeMaterialTest(BLACK) == 0 )
			{
				pParent->currentGame->result = "1/2-1/2";
				string termination = "Draw Insufficient Material";
				if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
				{
					wxMessageBox("Draw Insufficient Material", "Game Over");
				}
				pParent->currentGame->WriteTerminationIntoHeader(termination);
			}
			else
			{
				pParent->currentGame->result = "1-0";
				string termination = "Time Victory";
				if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
				{
					wxMessageBox("White Wins on Time", "Game Over");
				}
				pParent->currentGame->WriteTerminationIntoHeader(termination);
			}
			
			this->m_readTimer->SetTimeBetweenReads(100);
			this->m_readTimer->Stop();
			this->opponent->m_readTimer->SetTimeBetweenReads(100);
			this->opponent->m_readTimer->Stop();
			pParent->currentGame->WriteResultIntoHeader();
			pParent->RecreateGameList();

			pParent->currentGame->AddResultNode(pParent->currentGame->result);


			return false;
		}
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::PlayerMoves()
{
	//if( !CheckTime() )
	//	return;
	if( !m_process )//->Exists(pid) )
	{
		//this->m_readTimer->Stop();
		DoUnexpectedGameTermination();
		((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);
		closeProblem = true;
		DoClose();
		return;
	}

	ChessBoard *board = pParent->board_canvas->GetBoard();
	internalBoard.SetBoard(board->Fen());
	SendMoves();
	if( goMode == GO_TIME )
		SendMoveTime();
	else if( goMode == GO_FIXED_DEPTH )
		SendMoveDepth();
	else if( goMode == GO_FIXED_TIME )
		SendFixedMoveTime();

	analysisCanvas->RefreshGameMode();
	gameModeDepthPV.Clear();
	m_readTimer->start();
	((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(true);
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendStartGameMode()
{
	wxString mpv("setoption name MultiPv value ");
	char itoabuf[5];
	itoa(1, itoabuf, 10);
	mpv += itoabuf;
	mpv += wxT('\n');
	(m_process)->GetOutputStream()->Write(mpv.c_str(), mpv.length());


	/*wxString uciMode("setoption name UCI_AnalyseMode value false\n");
	(m_process)->GetOutputStream()->Write(uciMode.c_str(), uciMode.length());*/
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::SendStartAnalyzeMode()
{
	/*wxString uciMode("setoption name UCI_AnalyseMode value true\n");
	(m_process)->GetOutputStream()->Write(uciMode.c_str(), uciMode.length());*/

	wxString mpv("setoption name MultiPv value ");
	char itoabuf[5];
	itoa(num_kibitz_lines, itoabuf, 10);
	mpv += itoabuf;
	mpv += wxT('\n');
	(m_process)->GetOutputStream()->Write(mpv.c_str(), mpv.length());
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoGet()
{
    // we don't have any way to be notified when any input appears on the
    // stream so we have to poll it :-(
    DoGetFromStream(m_textIn, *m_process->GetInputStream());
    //DoGetFromStream(m_textErr, *m_process->GetErrorStream());
	//*m_process->GetInputStream()->
}




#include <string>
using namespace std;


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoGetFromStream(wxTextCtrl *text, wxInputStream& in)
{
	if( ((mode == 1) && (pParent->enginesMatch == true) && !CheckTime()) ||
		((mode==1) &&(pParent->board_canvas->GetBoard()->FiftyMoveTest())))
	{
		DoFinishGame();

		
		return;
	}

	if( !m_process )
	{
		this->m_readTimer->Stop();
		if( mode == 1 )
			DoUnexpectedGameTermination();
		((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);
		closeProblem = true;
		DoClose();
		return;
	}

    while ( in.CanRead() )
    {
		isReading = true;
        char buffer[4096];
        buffer[in.Read((void*)buffer, WXSIZEOF(buffer) - 1).LastRead()] = wxT('\n\0');
		
		if( mode == 0 )
		{
			ProcessOutput(buffer, text);
		}
		else 
		{
			WaitBestMove(buffer, text);
		}
    }

	isReading = false;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoUnexpectedShutdown()
{
	((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);
	if( mode == 1 )
	{
		if( this->m_readTimer && this->m_readTimer->IsRunning() )
			this->m_readTimer->Stop();
		
		DoUnexpectedGameTermination();
		closeProblem = true;
		DoClose();
		return;
	}
	else
	{
		if( this->m_readTimer && this->m_readTimer->IsRunning() )
			this->m_readTimer->Stop();
		
		closeProblem = true;
		DoClose();
		return;
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoUnexpectedGameTermination()
{
	((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);
	
	goMode = GO_TIME;
	
	if( opponent )
		opponent->SetGoMode(GO_TIME);
	
	isReading = false;

	if( m_readTimer )
	{
		this->m_readTimer->SetTimeBetweenReads(100);
	}
	if( (opponent) && opponent->m_readTimer )
	{
		this->opponent->m_readTimer->SetTimeBetweenReads(100);
	}

	pParent->gameClock->Stop();
	pParent->gameClock->Refresh();

	this->pParent->enginesMatch = false;

	

	pParent->board_canvas->MyRefresh(false);

	pParent->my_canvas->Refresh();
	pParent->currentGame->WriteResultIntoHeader();
	pParent->RecreateGameList();

	pParent->m_engineVsEngine.numGamesPlayed++;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoFinishGame()
{
	((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);

	goMode = GO_TIME;

	if( opponent )
		opponent->SetGoMode(GO_TIME);
	
	isReading = false;

	this->m_readTimer->Stop();
	if( opponent )
		this->opponent->m_readTimer->Stop();

	this->m_readTimer->SetTimeBetweenReads(100);
	if( opponent )
		this->opponent->m_readTimer->SetTimeBetweenReads(100);

	pParent->gameClock->Stop();
	pParent->gameClock->Refresh();

	this->pParent->enginesMatch = false;

	

	pParent->board_canvas->MyRefresh(false);

	pParent->my_canvas->Refresh();
	pParent->currentGame->WriteResultIntoHeader();
	pParent->RecreateGameList();

	pParent->m_engineVsEngine.numGamesPlayed++;
	pParent->NextEngineVsEngineGame();
}

/*
						
							


	
		
				
		


					
*/

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::WaitBestMove(char* buf, wxTextCtrl *text)
{

	string old_fen = internalBoard.Fen();

	int movNum;

		movNum = atoi(pParent->currentGame->pCurrMove->info.c_str());
		storedMovNum = movNum;

	//static int tick_count = 0;
	//if( tick_count ++ == 4 )
	{
		pParent->gameClock->TickClock();
		//tick_count = 0;
	}



	



	int white_black = internalBoard.mov;

	int original_white_black = white_black;

	string outputStr(buf);
	vector<string> tokens;

	istringstream iss(outputStr);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
				back_inserter<vector<string> >(tokens));

	string bestmove = "";
	int mpvNum = 0;
	bool mpvToggle = false;
	int addedLine = 0;
	//gameModePv.PvChangedThisFrame(false);
	gameModeDepthPV.PvChangedThisFrame(false);
	int currDepth = 0;
	string currTime = "";
	string currScore = "";
	for( int i = 0; i < tokens.size(); i++ )
	{
		if( tokens[i] == "bestmove" )
		{
			if( i+1 < tokens.size() )
			{
				m_textCurrMove->Clear();
				this->m_textCurrMove->AppendText(tokens[i+1].c_str());
				bestmove = tokens[i+1];
				break;
			}
		}

		if( tokens[i] == "depth" )
		{
			if( i+1 < tokens.size() )
			{
				//outBuff += tokens[i];
				//outBuff += " ";
				//outBuff += tokens[i+1];
				this->m_textNodes->Clear();
				this->m_textNodes->AppendText(tokens[i].c_str());
				this->m_textNodes->AppendText(": ");
				this->m_textNodes->AppendText(tokens[i+1].c_str());
				//char buf[10];
				//sprintf(buf, tokens[i+1].c_str());

				currDepth = atoi(tokens[i+1].c_str())-1;
				gameModeDepthPV.SetDepthString(currDepth, tokens[i+1]);
			}
			//i++;
			continue;
		}

		if( tokens[i] == "time" )
		{
			if( i+1 < tokens.size() )
			{
				this->m_textTime->Clear();
				int timeInMillis = atoi(tokens[i+1].c_str());
				int timeInSeconds = timeInMillis / 1000;
				int timeInMinutes = timeInSeconds / 60;
				int secondsTime = timeInSeconds - timeInMinutes*60;
				int timeInHours = timeInMinutes / 60;
				int minutesTime;
			//	char timeBuffer[30];
				this->m_textTime->AppendText(tokens[i].c_str());
				this->m_textTime->AppendText(" ");
				
				if( timeInMinutes > 60 )
				{
					minutesTime = timeInMinutes - timeInHours * 60;
				
					//sprintf(timeBuffer, "%d: %d:%d", timeInHours,
					//									minutesTime, secondsTime);
					currTime = 	ZeroPadNumber<int>(timeInHours) + ":" + 
												 ZeroPadNumber<int>(minutesTime) + ":" +
												 ZeroPadNumber<int>(secondsTime);
					this->m_textTime->AppendText(currTime);
														
				}
				else
				{
					minutesTime = timeInMinutes;
					//sprintf(timeBuffer, "00:%d:%d", minutesTime, secondsTime);

					currTime = "00:" + ZeroPadNumber<int>(minutesTime) + ":" +
							 ZeroPadNumber<int>(secondsTime);

					this->m_textTime->AppendText(currTime);
				}


			}
			i++;
			continue;
		}

		if( tokens[i] == "nps" )
		{
			if( i+1 < tokens.size() )
			{
				m_textErr->Clear();
				//m_textErr->AppendText("nps: ");
				int knps = atoi(tokens[i+1].c_str());
				knps /= 1000;
				//char bufNPS[30];
				//sprintf(bufNPS,"%d kN/s",knps);
				m_textErr->AppendText(to_string<int>(knps)+" kN/s");
				//i++;
			}
		}

		if( tokens[i] == "mate" )
		{

			//outBuff += " ";
			// divide by 100 and convert to string
			//char floatBuff[10];
			if( i+1 < tokens.size() )
			{
				int num = atoi(tokens[i+1].c_str());
				int sign = 1;
				if( white_black == BLACK )
				{
					sign = -1;
				}
				else
				{
					sign = 1;
				}
				float score_cp = num;
				if( num == 0 )
				{
					
			//	sprintf(floatBuff, "0.00");
				}
				else
				{
					num /= 100.0f;
					if(  (internalBoard.mov == 1))
					{
						num = -num;
						score_cp = -score_cp;
					//	sprintf(floatBuff, "%.4g", num );
					}
					else
					{
					//	sprintf(floatBuff, "%.4g", num );
					}
				}
					
				string fltBuff = "mate:" + tokens[i+1];//ZeroPadNumber<float>(num);
				//string fltBuff = floatBuff;
				/*if( logToFile )
				{
					logFile << " " <<tokens[i]<< ": "<<fltBuff;
				}*/
				int score_cpi = (int) score_cp;
				float fnum = (float)num;
				gameModeDepthPV.SetScoreValues(currDepth, score_cpi,fnum , fltBuff);
				
			}
			else break;
		}

		if( tokens[i] == "cp" )
		{
			if( i+1 < tokens.size() )
			{

				//outBuff += " ";
				// divide by 100 and convert to string
				//char floatBuff[10];
				
				float num = atof(tokens[i+1].c_str());
				float score_cp = num;
				if( num == 0 )
				{
					
				//	sprintf(floatBuff, "0.00");
				}
				else
				{
					num /= 100.0f;
					if(  (internalBoard.mov == 1))
					{
						num = -num;
						//
						
						score_cp = -score_cp;
						//sprintf(floatBuff, "%.4g", num );
					}
					else
					{
						//sprintf(floatBuff, "%.4g", num );
					}
				}
				

				string fltBuff = ZeroPadNumber<float>(num);
				
				int score_cpi = (int) score_cp;
				//gameModePv.SetScoreValues(score_cpi, num, fltBuff);
				currScore = fltBuff;
				gameModeDepthPV.SetScoreValues(currDepth,score_cpi, num, fltBuff);
			}
		}


		if( tokens[i] == "pv" && timeMode > BULLET )
		{
			int j = i+1;
			//gameModePv.PvChangedThisFrame(true);
			gameModeDepthPV.PvChangedThisFrame(true);
			// dave@ this should get rid of the bug.
			// there is only one.
			gameModeDepthPV.m_moves[ currDepth ].Clear();
			//gameModePv.Clear();

			int currMovNum = movNum;
			while(j < tokens.size())
			{
				bool promote = false;
				int tokenjl = tokens[j].length();
				if( (tokenjl == 4) || 
					((tokenjl==5) &&
					((tokens[j][tokenjl-1] == 'Q') || 
					(tokens[j][tokenjl-1] == 'R') ||
					(tokens[j][tokenjl-1] == 'N') ||
					(tokens[j][tokenjl-1] == 'B') ||
					(tokens[j][tokenjl-1] == 'q') || 
					(tokens[j][tokenjl-1] == 'r') || 
					(tokens[j][tokenjl-1] == 'n') || 
					(tokens[j][tokenjl-1] == 'b') )))
				{
					if( tokens[j]!= "info" && tokens[j] != "inf")
					{
						string out;
						string xxx = "";

						int promoteId = 3;
						if (tokenjl==5)
						{
							promote = true;
						}
					
						string inptstr = tokens[j];

						switch( inptstr[1] )
						{
						case '1': inptstr[1] = '0';break;
						case '2': inptstr[1] = '1';break;
						case '3': inptstr[1] = '2';break;
						case '4': inptstr[1] = '3';break;
						case '5': inptstr[1] = '4';break;
						case '6': inptstr[1] = '5';break;
						case '7': inptstr[1] = '6';break;
						case '8': inptstr[1] = '7';break;
						
						}

						switch( inptstr[3] )
						{
						case '1': inptstr[3] = '0';break;
						case '2': inptstr[3] = '1';break;
						case '3': inptstr[3] = '2';break;
						case '4': inptstr[3] = '3';break;
						case '5': inptstr[3] = '4';break;
						case '6': inptstr[3] = '5';break;
						case '7': inptstr[3] = '6';break;
						case '8': inptstr[3] = '7';break;
						
						}
						//tokens[j]+=" ";
						//text->AppendText(tokens[j].c_str() );
						
						string pgn = internalBoard.EPN_Move_to_PGN_Move(inptstr, promote, 3);
						string fen = internalBoard.Fen();
						MyChessMove *last_move = internalBoard.GetLastMove();
						internalBoard.SetBoard(fen);

						promote = false;

						string inf;
						if( white_black == 0 || white_black == 2)
						{
							
							//_itoa(++currMovNum, inf, 10 );
							inf = to_string<int>(++currMovNum);
							out+= inf;
							out+= ".";
							white_black = 1;
	
						}
						else
						{
							//_itoa(currMovNum, inf, 10 );
							inf = to_string<int>(currMovNum);
							white_black = 0;
						}

						out+= pgn;
						//out+=tokens[j];
						out+=" ";
						//text->AppendText(out.c_str() );
						
						//gameModePv.AddNode( VarMoveNode(pgn, *last_move, fen, white_black, string(inf)));

						gameModeDepthPV.AddNode( currDepth, VarMoveNode(pgn, *last_move, fen, white_black, inf));
					}
					else 
					{
						
						break;
					}
					
				}
				else 
				{
				
					break;
				}
				j++;
				
			}
			white_black = original_white_black;
			internalBoard.mov = original_white_black;
			internalBoard.SetBoard(old_fen);
			
			//i=j;
			continue;
		}


	}

	

	// if hyper bullet mode and no best move then checktime
	if( bestmove != "" )
	{
		int tokenjl = bestmove.length();
		if( tokenjl < 4 )
		{
			// euch ... try to get the next chars ..
			char miniBuff[40];
			miniBuff[m_process->GetInputStream()->Read((void*)miniBuff, WXSIZEOF(miniBuff) - 1).LastRead()] = wxT('\n\0');

			for( int mbi = 0; mbi < 5-tokenjl; mbi++)
			{
				if( (miniBuff[mbi] > '0' && miniBuff[mbi] < '9') ||
					(miniBuff[mbi] >='a' && miniBuff[mbi] <= 'h') ||
					(miniBuff[mbi]=='q' || miniBuff[mbi]=='b'||miniBuff[mbi]=='n'||
					miniBuff[mbi]=='r'))
					bestmove+= miniBuff[mbi];

				if( bestmove.length()<4)
					return;
			}
		}
		string out;
		string xxx = "";
		bool promote = false;
		
		int promoteId = 3;
		if (tokenjl==5)
		{
			promote = true;
		}
					
		string inptstr = bestmove;

		switch( inptstr[1] )
		{
		case '1': inptstr[1] = '0';break;
		case '2': inptstr[1] = '1';break;
		case '3': inptstr[1] = '2';break;
		case '4': inptstr[1] = '3';break;
		case '5': inptstr[1] = '4';break;
		case '6': inptstr[1] = '5';break;
		case '7': inptstr[1] = '6';break;
		case '8': inptstr[1] = '7';break;
						
		}

		switch( inptstr[3] )
		{
		case '1': inptstr[3] = '0';break;
		case '2': inptstr[3] = '1';break;
		case '3': inptstr[3] = '2';break;
		case '4': inptstr[3] = '3';break;
		case '5': inptstr[3] = '4';break;
		case '6': inptstr[3] = '5';break;
		case '7': inptstr[3] = '6';break;
		case '8': inptstr[3] = '7';break;
						
		}
		//tokens[j]+=" ";
		//text->AppendText(tokens[j].c_str() );
		int white_black = 0;
		if( pParent->board_canvas->GetBoard()->mov == 0 || pParent->board_canvas->GetBoard()->mov == 2 )
			white_black = 0;
		else white_black = 1;
				
		string pgnmove = pParent->board_canvas->GetBoard()->EPN_Move_to_PGN_Move(inptstr, promote, 3);
		int gameStatus = pParent->board_canvas->GetBoard()->GetGameStatus();
		string fen = pParent->board_canvas->GetBoard()->Fen();
		pParent->board_canvas->GetBoard()->SetBoard(fen);

		PGN::MoveInserter mv(pgnmove,white_black, fen, pParent->board_canvas->GetBoard()->GetLastMove());

		if( white_black == WHITE )
		{
			pParent->gameClock->SetWhiteBlack(BLACK);
		}
		else
		{
			pParent->gameClock->SetWhiteBlack(WHITE);
		}



		//pParent->board_canvas->Refresh(false);
		
		int repetitions = 0;
		pParent->currentGame->pCurrMove = pParent->currentGame->pCurrMove->InsertMove(&mv, &repetitions);
		
		if( white_black == WHITE )
		{
			pParent->currentGame->pCurrMove->move_cp_white = gameModeDepthPV.m_moves[currDepth].score_cp;
		}
		else
		{
			pParent->currentGame->pCurrMove->move_cp_black = gameModeDepthPV.m_moves[currDepth].score_cp;
		}

		pParent->board_canvas->needsRefresh = true;

		switch( gameStatus )
		{
			case PLAYING:
			case KINGSAFE:
				break;
			case CHECK:
				break;
			case CHECKMATE:
				{
					if( pParent->board_canvas->GetBoard()->mov == 0 || pParent->board_canvas->GetBoard()->mov == 2 )
					{
						string gameResultString = "Checkmate";
						if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "Black " + gameResultString +"'s!","Game Over");
						}
						pParent->currentGame->WriteTerminationIntoHeader(gameResultString);
						pParent->currentGame->result = "0-1";

					}
					else
					{
						string gameResultString = "Checkmate";
						if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "White "+gameResultString +"'s!","Game Over");
						}
						pParent->currentGame->WriteTerminationIntoHeader(gameResultString);
						pParent->currentGame->result = "1-0";
					}
					
					pParent->currentGame->AddResultNode(pParent->currentGame->result);
	
					DoFinishGame();
					return;

				}
				break;
			case STALEMATE:
				{
					if( pParent->board_canvas->GetBoard()->mov == 0 || pParent->board_canvas->GetBoard()->mov == 2 )
					{
						string gameResultString = "Stalemate";
						if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "White " +gameResultString+"d!","Game Over");
						}
						pParent->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					else
					{
						string gameResultString = "Stalemate";
						if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "Black "+gameResultString+"d!","Game Over");
						}
						pParent->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					pParent->currentGame->result = "1/2-1/2";
					pParent->currentGame->AddResultNode(pParent->currentGame->result);

					DoFinishGame();

					return;
				}
				break;
			case DRAW_BY_50_MOVE:
				{
					if( pParent->board_canvas->GetBoard()->mov == 0 || pParent->board_canvas->GetBoard()->mov == 2 )
					{
						string gameResultString = "Drawn, 50 moves without a pawn move or capture!";
					
						if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( gameResultString,"Game Over");
						}
						pParent->currentGame->WriteTerminationIntoHeader(gameResultString);
						pParent->currentGame->result = "1/2-1/2";
						pParent->currentGame->AddResultNode(pParent->currentGame->result);
					}
					else
					{
						string gameResultString = "Drawn, 50 moves without a pawn move or capture!";
						
						if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( gameResultString,"Game Over");
						}
						pParent->currentGame->WriteTerminationIntoHeader(gameResultString);
						pParent->currentGame->result = "1/2-1/2";
						pParent->currentGame->AddResultNode(pParent->currentGame->result);
					}

					DoFinishGame();

					return;
				}
				break;
			
		};

		bool materialDraw = pParent->board_canvas->GetBoard()->MaterialDraw();

		if( (repetitions >= 3) || (pParent->board_canvas->GetBoard()->fiftyMov >= 100) ||
			(materialDraw))
		{
			goMode = GO_TIME;
			opponent->SetGoMode(GO_TIME);
			string termination;
			if( repetitions >= 3  )
			{
				termination = "Draw By Repetition";
				gameStatus = DRAW_BY_REPETITION;
			}
			else if( materialDraw )
			{
				termination = "Draw By Insufficient Material";
				gameStatus = DRAW_BY_INSUFFICIENT_MATERIAL;
			}
			else
			{
				termination = "Draw By 50 Move Rule";
				gameStatus = DRAW_BY_50_MOVE;
			}
			pParent->currentGame->WriteTerminationIntoHeader(termination);

			if( pParent->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
			{
				
				if( gameStatus == DRAW_BY_REPETITION  )
				{
					
					wxMessageBox("Draw By Repetition","Drawn Game!");
				}
				else if( gameStatus == DRAW_BY_INSUFFICIENT_MATERIAL )
				{
					
					wxMessageBox("Draw By Insufficient Material","Drawn Game!");
				}
				else
				{
					
					wxMessageBox("Draw By 50 Move Rule","Drawn Game!");
				}
			}
			pParent->currentGame->result = "1/2-1/2";
			pParent->currentGame->AddResultNode(pParent->currentGame->result);
	
			DoFinishGame();

			return;
			
		}


			
		//if( pParent->optionsFrame->m_GameOptions.animate == true)
		//	pParent->board_canvas->StartAnimation();
		pParent->my_canvas->needsRefresh = false;
		pParent->my_canvas->ScrollToCursor();
		//
		if( timeMode > HYPER_BULLET )
		{
			if( pParent->m_mgr.GetPane("scoreGraph").IsShown() )
			{
				pParent->m_scoreGraph->Refresh();
			}
			pParent->my_canvas->needsRefresh = true;
			pParent->my_canvas->Refresh();
			pParent->treeCanvas->Refresh(false);
		}

		this->m_readTimer->Stop();
		((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);

		if( EngineVsEngineMode == false )
		{
			pParent->board_canvas->whoseMove = BoardCanvas::YOUR_MOVE;
		}
		else
		{
			if( opponent && 

				(gameStatus!= CHECKMATE) && 
				(gameStatus!=STALEMATE) &&
				(gameStatus!=DRAW_BY_50_MOVE) &&
				(gameStatus!=DRAW_BY_REPETITION))
			{

				//if( timeMode > HYPER_BULLET )
				{
					string moveDataStr;// = this->GetEngineName().c_str();
					moveDataStr += currScore;
					char DepthStrbuff[50];
					if( currDepth < 0 ) currDepth = 0;
					sprintf(DepthStrbuff,", D=%d", (currDepth+1));
					moveDataStr += DepthStrbuff;
					moveDataStr += ", t=" + currTime;
					pParent->currentGame->pCurrMove->WriteScoresToAnnotation(moveDataStr, pParent->board_canvas->GetBoard()->mov);
				}
				if( pParent->enginesMatch == true )
					opponent->PlayerMoves();
			}
		}
	}



//	
	analysisCanvas->Refresh(false);


	internalBoard.mov = original_white_black;
	internalBoard.SetBoard(old_fen);
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::ProcessOutput(char* buf, wxTextCtrl *text)
{
	//ChessBoard *board = pParent->board_canvas->GetBoard();
//	PGN::PgnLoad* pLoadPGN = pParent->pgnLoad;
	string old_fen = internalBoard.Fen();

	int movNum;
	if( !LockEngineToMove )
	{
		movNum = atoi(pParent->currentGame->pCurrMove->info.c_str());
		storedMovNum = movNum;
	}
	else
	{
		movNum = storedMovNum;
	}

	

	int white_black = internalBoard.mov;

	if( white_black == 2 )
		white_black = 0;

	int original_white_black = white_black;

	

	multiPvLines.SetCurrPly((storedMovNum-1)*2+white_black);

	
	//for( int i = 0; i < this->num_kibitz_lines; i++ )
	//	mpvStr[i] = "";

	string outputStr(buf);

/*	if (logToFile == true)
	{
		if( logFile.is_open() )
		{
			logFile << outputStr;
		}
	}*/

	vector<string> tokens;

	istringstream iss(outputStr);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
				back_inserter<vector<string> >(tokens));

	int pv_count = 0;
	string mpv = "";
	int mpvNum = 0;
	int maxDepthV = 0;
	int addedLine = 0;
	bool mpvToggle = false;
	multiPvLines.PvChangedThisFrame(false);
	
//	int numDepths = 0;
	for( int i = 0; i < tokens.size(); i++ )
	{


		if( tokens[i] == "depth" )
		{
			if( i+1 < tokens.size() )
			{
				//outBuff += tokens[i];
				//outBuff += " ";
				//outBuff += tokens[i+1];

				
				int depthV = atoi(tokens[i+1].c_str());

				if( depthV > maxDepthV )
				{
					this->m_textNodes->Clear();
					this->m_textNodes->AppendText(tokens[i].c_str());
					this->m_textNodes->AppendText(": ");
					this->m_textNodes->AppendText(tokens[i+1].c_str());
					maxDepthV = depthV;

					/*if( mpvNum < num_kibitz_lines )
					{

					}*/
				}

				if( logToFile )
				{
					logFile << " "<<tokens[i]<< ": "<<tokens[i+1];
				}
				//char buf[10];
				//sprintf(buf, tokens[i+1].c_str());
				if( white_black == BLACK )
				{
					if( mpvNum == 1 )
						multiPvLines.GetCurrent()->pMoveAnalyzed->depthWhite = tokens[i+1].c_str();
					
				}
				else
				{
					if( mpvNum == 1 )
						multiPvLines.GetCurrent()->pMoveAnalyzed->depthBlack = tokens[i+1].c_str();

				}
					
				if( mpvNum == 0 )
				{
					//mpvStr[0] += out;
					multiPvLines.GetCurrent()->mpvs[0].SetDepthString(tokens[i+1]);// = AddNode(VarMoveNode(pgn, *last_move, fen, white_black, string(inf)),0);
							
				}
				else
				{
					//mpvStr[mpvNum-1] += out;
					//multiPvLines.AddNode(VarMoveNode(pgn, *last_move, fen, white_black, string(inf)),mpvNum-1);
					multiPvLines.GetCurrent()->mpvs[mpvNum-1].SetDepthString(tokens[i+1]);
							
				}
			}
			//i++;
			continue;
		}

		if( tokens[i] == "0000" )
		{
			if( i+1 < tokens.size() )
			{
				if( mpvNum == 0 )
				{
					mpvStr[0] += " nullmove";
					//mpvStr[0] += tokens[i+1];//atoi(.c_str());
					mpvStr[0] += " ";
				}
				else
				{
					mpvStr[mpvNum-1] += " nullmove";
					//mpvStr[mpvNum-1] += tokens[i+1];//atoi(.c_str());
					mpvStr[mpvNum-1] += " ";
				}
			}
			i++;
			continue;
		}

		if( tokens[i] == "time" )
		{
			if( i+1 < tokens.size() )
			{
				this->m_textTime->Clear();
				int timeInMillis = atoi(tokens[i+1].c_str());
				int timeInSeconds = timeInMillis / 1000;
				int timeInMinutes = timeInSeconds / 60;
				int secondsTime = timeInSeconds - timeInMinutes*60;
				int timeInHours = timeInMinutes / 60;
				int minutesTime;
			//	char timeBuffer[30];
				this->m_textTime->AppendText(tokens[i].c_str());
				this->m_textTime->AppendText(" ");
				if( timeInMinutes > 60 )
				{
					minutesTime = timeInMinutes - timeInHours * 60;
				
					//sprintf(timeBuffer, "%d: %d:%d", timeInHours,
					//									minutesTime, secondsTime);

					this->m_textTime->AppendText(ZeroPadNumber<int>(timeInHours) + ":" + 
												 ZeroPadNumber<int>(minutesTime) + ":" +
												 ZeroPadNumber<int>(secondsTime));
					

				}
				else
				{
					minutesTime = timeInMinutes;
					//sprintf(timeBuffer, "00:%d:%d", minutesTime, secondsTime);

					this->m_textTime->AppendText("00:" + 
												 ZeroPadNumber<int>(minutesTime) + ":" +
												 ZeroPadNumber<int>(secondsTime));
				}

				if( logToFile )
				{
					logFile << " " <<tokens[i]<< ": "<<tokens[i+1];
				}

				
			}
			i++;
			continue;
		}

		if( tokens[i] == "multipv" )
		{
			if( i+1 < tokens.size() )
			{
				mpv = "";
				int tc = atoi(tokens[i+1].c_str());
				if( tc == 0 ) break;
				if( tc < MAX_LINES )
				{
					mpv += tokens[i+1];
					
					mpvNum = tc;//atoi(tokens[i+1].c_str());
					multiPvLines.GetCurrent()->ClearMPV(mpvNum -1);
					mpvStr[mpvNum-1] ="";
					mpvStr[mpvNum-1] += mpv + " ";
				}

				if( logToFile )
				{
					logFile << " " <<tokens[i]<< ": "<<tokens[i+1];
				}
			}
			mpvToggle = true;
			i++;
			continue;
		}

		if( tokens[i] == "bestmove" )
		{
			if( i+1 < tokens.size() )
			{
				m_textCurrMove->Clear();
				this->m_textCurrMove->AppendText(tokens[i+1].c_str());
				if( logToFile )
				{
					logFile << " " <<tokens[i]<< ": "<<tokens[i+1];
				}
				break;
			}
		}

		if( tokens[i] == "nps" )
		{
			if( i+1 < tokens.size() )
			{
				m_textErr->Clear();
				//m_textErr->AppendText("nps: ");
				int knps = atoi(tokens[i+1].c_str());
				knps /= 1000;
				//char bufNPS[30];
				//sprintf(bufNPS,"%d kN/s",knps);
				//m_textErr->AppendText(bufNPS);
				m_textErr->AppendText(to_string<int>(knps)+" kN/s");
				//i++;
			}
		}

		if( tokens[i] == "currmove" )
		{
		}

		if( tokens[i] == "currmovenumber")
		{
		}

		if( tokens[i] == "mate" )
		{

			//outBuff += " ";
			// divide by 100 and convert to string
			//char floatBuff[10];
			if( i+1 < tokens.size() )
			{
				int num = atoi(tokens[i+1].c_str());
				int sign = 1;
				if( white_black == BLACK )
				{
					sign = -1;
					if( mpvNum == 1 )
						multiPvLines.GetCurrent()->pMoveAnalyzed->move_cp_white = -int(num);
				}
				else
				{
					sign = 1;
					if( mpvNum == 1 )
						multiPvLines.GetCurrent()->pMoveAnalyzed->move_cp_black = int(num);
				}
				float score_cp = num;
				if( num == 0 )
				{
					
			//	sprintf(floatBuff, "0.00");
				}
				else
				{
					num /= 100.0f;
					if(  (internalBoard.mov == 1))
					{
						num = -num;
						score_cp = -score_cp;
					//	sprintf(floatBuff, "%.4g", num );
					}
					else
					{
					//	sprintf(floatBuff, "%.4g", num );
					}
				}
					
				string fltBuff = "mate:" + tokens[i+1];//ZeroPadNumber<float>(num);
				//string fltBuff = floatBuff;
				if( logToFile )
				{
					logFile << " " <<tokens[i]<< ": "<<fltBuff;
				}
				int score_cpi = (int) score_cp;
				float fnum = (float)num;
				if( mpvNum == 0 && num_kibitz_lines == 1) 
					multiPvLines.SetScore(score_cpi,fnum , fltBuff, 0);
				else if( mpvNum != 0 && mpvToggle == true)
					multiPvLines.SetScore(score_cpi, fnum, fltBuff, mpvNum-1);
			}
			else break;
		}
				

		if( tokens[i] == "cp" )
		{
			if( i+1 < tokens.size() )
			{

				{

					//outBuff += " ";
					// divide by 100 and convert to string
					//char floatBuff[10];
				
					float num = atof(tokens[i+1].c_str());
					if( white_black == BLACK )
					{
						if( mpvNum == 1 )
							multiPvLines.GetCurrent()->pMoveAnalyzed->move_cp_white = -int(num);
					}
					else
					{
						if( mpvNum == 1 )
							multiPvLines.GetCurrent()->pMoveAnalyzed->move_cp_black = int(num);
					}
					float score_cp = num;
					if( num == 0 )
					{
					
					//	sprintf(floatBuff, "0.00");
					}
					else
					{
						num /= 100.0f;
						if(  (internalBoard.mov == 1))
						{
							num = -num;
							score_cp = -score_cp;
						//	sprintf(floatBuff, "%.4g", num );
						}
						else
						{
						//	sprintf(floatBuff, "%.4g", num );
						}
					}
				
				/*	if( mpvNum == 0 )
					{
						mpvStr[0] += " (";
						if( num >= 0)
							mpvStr[0] += " ";
						mpvStr[0] += floatBuff;
					
						mpvStr[0] += ") ";
						if( strlen(floatBuff) < 4 && num > 0)
						{
							for( int vv = 0; vv < 4-strlen(floatBuff); vv++)
							 mpvStr[0] += " ";
						}
						else if( strlen(floatBuff) < 5 && num < 0)
						{
							for( int vv = 0; vv < 5-strlen(floatBuff); vv++)
							 mpvStr[0] += " ";
						}
					}
					else
					{
						mpvStr[mpvNum-1] += " (";
						if( num >= 0)
							mpvStr[mpvNum-1] += " ";
						mpvStr[mpvNum-1] += floatBuff;
						mpvStr[mpvNum-1] += ") ";
						if( strlen(floatBuff) < 4 && num > 0)
						{
							for( int vv = 0; vv < 4-strlen(floatBuff); vv++)
								mpvStr[mpvNum-1] += " ";
						}
						else if( strlen(floatBuff) < 5 && num < 0)
						{
							for( int vv = 0; vv < 5-strlen(floatBuff); vv++)
								mpvStr[mpvNum-1] += " ";
						}
					}*/
					string fltBuff = ZeroPadNumber<float>(num);
					//string fltBuff = floatBuff;
					if( logToFile )
					{
						logFile << " " <<tokens[i]<< ": "<<fltBuff;
					}
					int score_cpi = (int) score_cp;
					if( mpvNum == 0 && num_kibitz_lines == 1) 
						multiPvLines.SetScore(score_cpi, num, fltBuff, 0);
					else if( mpvNum != 0 && mpvToggle == true)
						multiPvLines.SetScore(score_cpi, num, fltBuff, mpvNum-1);
				}
			}
		}

		if( tokens[i] == "pv" )
		{
			int j = i+1;
			
			multiPvLines.PvChangedThisFrame(true);
			if( num_kibitz_lines != 1 )
			{
				if( mpvNum == 0 )
					break;
			}

			if( mpvToggle != true && num_kibitz_lines > 1)
				break;

			mpvToggle = false;


			int currMovNum = movNum;
			while(j < tokens.size())
			{
				bool promote = false;
				int tokenjl = tokens[j].length();
				if( (tokenjl == 4) || 
					((tokenjl==5) &&
					((tokens[j][tokenjl-1] == 'Q') || 
					(tokens[j][tokenjl-1] == 'R') ||
					(tokens[j][tokenjl-1] == 'N') ||
					(tokens[j][tokenjl-1] == 'B') ||
					(tokens[j][tokenjl-1] == 'q') || 
					(tokens[j][tokenjl-1] == 'r') || 
					(tokens[j][tokenjl-1] == 'n') || 
					(tokens[j][tokenjl-1] == 'b') )))
				{
					if( tokens[j]!= "info" && tokens[j] != "inf")
					{
						string out;
						string xxx = "";

						if( logToFile )
						{
							logFile << " " <<tokens[j]<< " ";
						}

						int promoteId = 3;
						if (tokenjl==5)
						{
							promote = true;
						}
					
						string inptstr = tokens[j];

						switch( inptstr[1] )
						{
						case '1': inptstr[1] = '0';break;
						case '2': inptstr[1] = '1';break;
						case '3': inptstr[1] = '2';break;
						case '4': inptstr[1] = '3';break;
						case '5': inptstr[1] = '4';break;
						case '6': inptstr[1] = '5';break;
						case '7': inptstr[1] = '6';break;
						case '8': inptstr[1] = '7';break;
						
						}

						switch( inptstr[3] )
						{
						case '1': inptstr[3] = '0';break;
						case '2': inptstr[3] = '1';break;
						case '3': inptstr[3] = '2';break;
						case '4': inptstr[3] = '3';break;
						case '5': inptstr[3] = '4';break;
						case '6': inptstr[3] = '5';break;
						case '7': inptstr[3] = '6';break;
						case '8': inptstr[3] = '7';break;
						
						}
						//tokens[j]+=" ";
						//text->AppendText(tokens[j].c_str() );
						
						string pgn = internalBoard.EPN_Move_to_PGN_Move(inptstr, promote, 3);
						string fen = internalBoard.Fen();
						MyChessMove *last_move = internalBoard.GetLastMove();
						internalBoard.SetBoard(fen);

						promote = false;

						/*char inf[4];
						if( white_black == 0 || white_black == 2)
						{
							
							_itoa(++currMovNum, inf, 10 );
							out+= inf;
							out+= ".";
							white_black = 1;
	
						}
						else
						{
							_itoa(currMovNum, inf, 10 );
							white_black = 0;
						}*/
						string inf;
						if( white_black == 0 || white_black == 2)
						{
							
							//_itoa(++currMovNum, inf, 10 );
							inf = to_string<int>(++currMovNum);
							out+= inf;
							out+= ".";
							white_black = 1;
	
						}
						else
						{
							//_itoa(currMovNum, inf, 10 );
							inf = to_string<int>(currMovNum);
							white_black = 0;
						}

						out+= pgn;
						//out+=tokens[j];
						out+=" ";
						//text->AppendText(out.c_str() );
						

						if( mpvNum == 0 && num_kibitz_lines == 1)
						{
							mpvStr[0] += out;
							multiPvLines.AddNode(VarMoveNode(pgn, *last_move, fen, white_black, inf),0);
							
						}
						else
						{
							mpvStr[mpvNum-1] += out;
							multiPvLines.AddNode(VarMoveNode(pgn, *last_move, fen, white_black, inf),mpvNum-1);
							
						}
						addedLine++;
					}
					else 
					{
						
						break;
					}


					
				}
				else 
				{
				
					break;
				}
				j++;
				
			}
			if( logToFile )
			{
				logFile << endl;
			}
			white_black = original_white_black;
			internalBoard.mov = original_white_black;
			internalBoard.SetBoard(old_fen);
			
			i=j;
			continue;
		}

	}

/*	if( addedLine != 0)
	{
		text->Clear();

		for( int i = 0; i < num_kibitz_lines; i++ )
		{

			text->AppendText(mpvStr[i].c_str());
			text->AppendText("\n");

		}
		text->SetInsertionPoint(0);
		text->ScrollWindow(0,0);		
	}*/
#if USE_TEXT_PANEL
	if( addedLine != 0)
	{
		text->Clear();

		for( int i = 0; i < num_kibitz_lines; i++ )
		{
			text->AppendText(char('0'+i));
			text->AppendText(" (");
			text->AppendText(multiPvLines.mpvs[i].score_str);
			text->AppendText(" ) ");
			for( int j = 0; j < multiPvLines.mpvs[i].moves.size(); j++ )
			{
				if( j==0 && !multiPvLines.mpvs[i].moves[j].whitesMove )
				{
					text->AppendText( multiPvLines.mpvs[i].moves[j].info);
					text->AppendText("...");
				}
				if( multiPvLines.mpvs[i].moves[j].whitesMove )
				{
					text->AppendText( multiPvLines.mpvs[i].moves[j].info);
					text->AppendText(".");
				}
				text->AppendText( multiPvLines.mpvs[i].moves[j].pgnMove);
				text->AppendText(" ");
			}
			text->AppendText("\n");
		}
		text->SetInsertionPoint(0);
		text->ScrollWindow(0,0);		



	}

#else
	pParent->m_scoreGraph->Refresh();
	analysisCanvas->Refresh(false);
	pParent->moveDataCanvas->Refresh();
#endif

	internalBoard.mov = original_white_black;
	internalBoard.SetBoard(old_fen);
}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoShutdown()
{
	if( !((mode == 1) && (this->timeMode != HYPER_BULLET))  )
		while( isReading == true ){}
	
	m_readTimer->Stop();
    wxString s4("quit");
    s4 += wxT('\n');
    (m_process)->GetOutputStream()->Write(s4.c_str(), s4.length());

    m_process->CloseOutput();

	engineLoaded=false;
    DisableInput();

    wxWindowDisabler disableAll;

	wxString loadingInfo = "Closing ";
	loadingInfo += currentLoadedEngine;

	wxBusyInfo info(loadingInfo, this->pParent);

    for ( int i = 0; i < 18; i++ )
    {
        wxMilliSleep(100);
//        wxTheApp->Yield();
    }

	//m_textErr->Clear();
	m_textCurrMove->Clear();
	m_textNodes->Clear();
	m_textErr->Clear();
	//m_textOut->Clear();
	m_textTime->Clear();
	
	wxString kbitzing_by = "Engine: ";
	


	currentLoadedEngine = "";

//	wxDELETE(m_process);

}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DoClose()
{
	if(!closeProblem )
	{
		if( this->timeMode != HYPER_BULLET )
			while( isReading == true ){}
	
		if( m_readTimer )
			m_readTimer->Stop();

		wxString s4("quit");
		s4 += wxT('\n');
		if( m_process && (m_process)->GetOutputStream() )
		{
			(m_process)->GetOutputStream()->Write(s4.c_str(), s4.length());

			m_process->CloseOutput();
		}
	}

	engineLoaded=false;
    DisableInput();

    wxWindowDisabler disableAll;

	wxString loadingInfo ;
	if( !closeProblem )
	{
		loadingInfo= "Closing ";
		loadingInfo += currentLoadedEngine;
	}
	else
	{
		loadingInfo = currentLoadedEngine;
		loadingInfo += " Closed Unexectedly";
	}

	wxBusyInfo info(loadingInfo, this->pParent);

    for ( int i = 0; i < 18; i++ )
    {
        wxMilliSleep(100);
//        wxTheApp->Yield();
    }

	//m_textErr->Clear();
	m_textCurrMove->Clear();
	m_textNodes->Clear();
	//m_textOut->Clear();
	m_textTime->Clear();
	
	wxString kbitzing_by = "Engine: ";
	
	if( wxTheApp->IsActive() )
	{
		pParent->m_mgr.GetPane(this).Name("Engine_Panel").Caption(kbitzing_by);
		pParent->m_mgr.Update();
	}

	currentLoadedEngine = "";

	if( analyzing )
	{
		
		((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);
		analyzing=  false;
		startedAnalyzing = false;
	}
	else
	{
		((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);
	}

//	wxDELETE(m_process);

}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DisableInput()
{

}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::DisableOutput()
{
//    FindWindow(Exec_Minus)->Disable();
}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnBtnMinus(wxCommandEvent& event)
{
	if( this->mode == 1 )return;

	if( this->engineLoaded == false )
		return;

	if( !m_process )
	{
		DoUnexpectedShutdown();
		return;
	}


	this->num_kibitz_lines--;
	if( num_kibitz_lines < 1 )
		num_kibitz_lines = 1;


	SendStop();

	//wxSleep(1);

    wxString mpv("setoption name MultiPv value ");
	char itoabuf[5];
	itoa(num_kibitz_lines, itoabuf, 10);
	mpv += itoabuf;
    mpv += wxT('\n');
    (m_process)->GetOutputStream()->Write(mpv.c_str(), mpv.length());





	//SendFen();
	SendMoves();

	SendGoInfinite();
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnBtnPlus(wxCommandEvent& event) 
{ 
	if( this->mode == 1 )return;

	if( this->engineLoaded == false )
		return;

	if( !m_process )
	{
		DoUnexpectedShutdown();
		return;
	}

	this->num_kibitz_lines++;
	if( num_kibitz_lines > MAX_LINES )
		num_kibitz_lines = MAX_LINES;

	SendStop();
		
	//if( bAnalyzing )
	//	OnBtnSendFile(event);
    wxString mpv("setoption name MultiPv value ");
	char itoabuf[5];
	itoa(num_kibitz_lines, itoabuf, 10);
	mpv += itoabuf;
    mpv += wxT('\n');
    (m_process)->GetOutputStream()->Write(mpv.c_str(), mpv.length());


	//SendFen();
	SendMoves();

	SendGoInfinite();
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnClose(wxCloseEvent& event)
{
	// ajourn game here

    if ( m_process )
    {
        // we're not interested in getting the process termination notification
        // if we are closing it ourselves

		
        wxProcess *process = m_process;
        
        process->SetNextHandler(NULL);

        process->CloseOutput();
    }

    event.Skip();
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnProcessTerm(wxProcessEvent& WXUNUSED(event))
{
	delete m_readTimer;
	m_readTimer = NULL;

    wxDELETE(m_process);

    wxLogWarning(wxT("The other process has terminated, closing"));
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::KibitzUserNewGame()
{
	SendUciNewGame();

	wxSleep(1);
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::KibitzStops()
{
	SendStop();

	if( !this->timeMode == HYPER_BULLET )
		while( isReading == true ){}

	if( m_readTimer && m_readTimer->IsRunning() )
		m_readTimer->Stop();

	this->analyzing = false;
	this->startedAnalyzing = false;

	((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::KibitzStartsMove()
{
	SendStop();

	if( !this->timeMode == HYPER_BULLET )
		while( isReading == true ){}

	m_readTimer->Stop();

	this->startedAnalyzing = false;

	((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(false);

}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::KibitzUserEndsMove()
{

	ChessBoard *board = pParent->board_canvas->GetBoard();
	internalBoard.SetBoard(board->Fen());


//	wxSleep(1);

	this->startedAnalyzing = true;

//	SendFen();
	SendMoves();
//	wxSleep(1);

	if( mode == 0 )
	{
		SendGoInfinite();
	}
	else if( mode == 1 )
	{
		this->SendMoveTime();
	}
	
	((wxBitmapToggleButton*)FindWindow(Exec_Btn_SendFile))->SetValue(true);

	m_readTimer->start();
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::CopyLineToGameTree(int mpv)
{
	if( this->mode == 1 )return;

	PGN::Move *pMovePtr = multiPvLines.GetCurrent()->pMoveAnalyzed;// pParent->currentGame->pCurrMove;
	for( int i = 0; i< multiPvLines.GetCurrent()->mpvs[mpv].moves.size(); i++)
	{
		string fen = multiPvLines.GetCurrent()->mpvs[mpv].moves[i].fen;
		MyChessMove epn = multiPvLines.GetCurrent()->mpvs[mpv].moves[i].move;
		int white_black = (int)!multiPvLines.GetCurrent()->mpvs[mpv].moves[i].whitesMove;
		string pgn_string = multiPvLines.GetCurrent()->mpvs[mpv].moves[i].pgnMove;
		if( i == 0 )
		{
			PGN::MoveInserter mv_inserter(  pgn_string,  white_black,  fen, &epn);
			int hits = -1;
			pMovePtr = pMovePtr->InsertVariation(&mv_inserter, &hits);
			pMovePtr->WriteScoresToAnnotation(multiPvLines.GetCurrent()->mpvs[mpv].score_str, !white_black);
		}
		else
		{
			PGN::MoveInserter mv_inserter(  pgn_string,  white_black,  fen, &epn);

			int hits = -1;
			pMovePtr = pMovePtr->InsertMoveToVariation(&mv_inserter, &hits);
		}
	}
	
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::CopyAllToGameTree()
{
	if( mode == 0 )
	{
		KibitzStartsMove();

		for( int mpv = 0; mpv < num_kibitz_lines; mpv++ )
		{
			CopyLineToGameTree(mpv);
		}
		pParent->my_canvas->needsRefresh = true;
		pParent->my_canvas->Refresh();
#ifdef USE_CANVAS_SET_FOCUS
		pParent->my_canvas->SetFocus();
#endif
		pParent->treeCanvas->Refresh();
	}
	else if( mode == 1 )
	{
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnBtnEngineOptions(wxCommandEvent& WXUNUSED(event))
{
	if (pParent->enginesMatch)
		return;

	if( !this->engineLoaded )
		return;

	if (!m_process)
	{
		DoUnexpectedShutdown();
		return;
	}

	KibitzStartsMove();
	EngineOptionsModalDialog *dlg = new EngineOptionsModalDialog(this, this, this->engineOptions, wxID_ANY,"Engine Options");
	if( dlg->ShowModal() == wxID_OK)
	{
	}

	delete dlg;
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::OnBtnLockEngine(wxCommandEvent& event)
{
	if( this->mode == 1 )return;

	if( !engineLoaded )
		return;

	if( !m_process )
	{
		DoUnexpectedShutdown();
		return;
	}

	this->LockEngineToMove = !this->LockEngineToMove;
	if( LockEngineToMove )
		wxMessageBox("Engine Locked", "fail message");
	/*if( event.IsChecked() )
		LockEngineToMove = true;
	else
	{
		LockEngineToMove = false;
		//this->LockEngineToMove = !this->LockEngineToMove;
	}*/
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MyPipePanel::ActivateDataLogging()
{

	logToFile = true;
	string pathName = wxStandardPaths::Get().GetResourcesDir().c_str() + "\\Session Data\\logs\\";
	string gameName = "logtest.log";//pParent->currentGame->whiteName + "_vs_" + pParent->currentGame->blackName +".log";
	string path = pathName + gameName;
	/*if( !wxRemoveFile(path))
	{
		wxMessageBox("Cannot Remove Log File","");
	}*/
	logFile.open(path);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ReadTimer::ReadTimer(MyPipePanel* panel) : wxTimer()
{
    ReadTimer::m_panel = panel;
	timeBetweenReads = 100;
}
 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void ReadTimer::Notify()
{
	//if( !m_panel->isReading )
	if( m_panel->ProcessLost() )
	{
		m_panel->DoUnexpectedShutdown();
	}
	else
	{
		m_panel->DoGet();
	}
}
 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void ReadTimer::start()
{
    wxTimer::Start(timeBetweenReads);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void ReadTimer::SetTimeBetweenReads(int n)
{
	timeBetweenReads = n;
}