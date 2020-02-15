#ifndef MY_XBOARD_PIPE_PANEL
#define MY_XBOARD_PIPE_PANEL

class EngineAnalysisCanvas;

class MyPipePanel : public wxPanel
{
public:
	MyPipePanel(){}
    MyPipePanel(MyFrame *parent);
	~MyPipePanel();

	// 	wxString strText;
	
   // wxCriticalSection m_csStrText;        // protects m_bmp
	ReadTimer *m_readTimer;		
	void LoadEngine(const wxString& cmd,
                         wxProcess *process);

	void KibitzStops();
	void KibitzStartsMove();
	void KibitzUserEndsMove();
	void KibitzUserNewGame();

	bool IsActive(){ return engineLoaded; }
	bool IsAnalysing() {
		if( LockEngineToMove )
			return false;
		else return (analyzing); 
	}
	bool EngineIsLoaded() { return engineLoaded;}

	vector<string>* GetTokenizedOptionsVector()
	{
		return &tokenizedOptions;
	}

	void SendMoves();
	void SendStop();
	void SendGoInfinite();
	void SendFen();
	void SendUciNewGame();
	void SendMoveTime();
	void SendOptionsString(string opts);
	void SendStartGameMode();
	void SendStartAnalyzeMode();
	
    void DoGet();
    void DoClose();
	void DoShutdown();

	int GetNumKbtzLines()
	{
		return num_kibitz_lines;
	}

	void OptionsSetNumKibitzLines(int nkbl)
	{
		num_kibitz_lines = nkbl;
	}

	MPVMoveNodeVectorArray* GetMPVNodeVectorArray()
	{
		return &multiPvLines;
	}

	void SetEngineColor( int white_black )
	{
		engineColor = white_black;
	}

	int GetEngineColor()
	{
		return engineColor;
	}

	void SetGameMode()
	{
		mode = 1;
		if( this->engineLoaded )
			SendStartGameMode();
	}

	void SetAnalyzeMode()
	{
		gameModeDepthPV.Clear();
		mode = 0;
		if( this->engineLoaded )
			SendStartAnalyzeMode();
	}

	int GetMode()
	{
		return mode;
	}

	MoveNodeVector* GetGameModePV()
	{
		return &gameModePv;
	}

	MoveNodeDepthArray * GetGameModeDepthPV()
	{
		return &gameModeDepthPV;
	}


	bool IsEngineLockedToMove()
	{
		return LockEngineToMove;
	}

	void SetPanelName(wxString pn)
	{
		panel_name = pn;
	}

	wxString GetPanelName()
	{
		return panel_name;
	}

	void PlayerMoves();
protected:
    void OnTextEnter(wxCommandEvent& WXUNUSED(event)) { DoSend(); }
    void OnBtnPlus(wxCommandEvent& event);
    void OnBtnSendFile(wxCommandEvent& WXUNUSED(event));
    void OnBtnMinus(wxCommandEvent& WXUNUSED(event));
    void OnBtnClose(wxCommandEvent& WXUNUSED(event)) 
	{ 
		DoClose(); 
	}
    void OnBtnCopyUp(wxCommandEvent& WXUNUSED(event))
	{
		this->CopyAllToGameTree();
	}
	void OnBtnEngineOptions(wxCommandEvent& WXUNUSED(event));

	void OnBtnLockEngine(wxCommandEvent& event);

	void OnGUIThreadEvent(wxThreadEvent& event);
	void OnIdle(wxIdleEvent &event);
    void OnClose(wxCloseEvent& event);

    void OnProcessTerm(wxProcessEvent& event);

    void DoSend();

	void CopyLineToGameTree(int mpv);
	void CopyAllToGameTree();




	//PGN::Move* pMovePtr;



private:
    void DoGetFromStream(wxTextCtrl *text, wxInputStream& in);
    void DisableInput();
    void DisableOutput();

	void ProcessOutput(char* buf, wxTextCtrl *text);
	void WaitBestMove(char* buf, wxTextCtrl *text);

	MyFrame *pParent;
    wxProcess *m_process;

	bool LockEngineToMove;

 //   wxOutputStream m_out;
//    wxInputStream m_in,
 //                 m_err;

	int num_kibitz_lines;

	wxTextCtrl *m_textOut,
				*m_textIn,
				*m_textErr,
				*m_textNodes,
				*m_textCurrMove,
				*m_textTime;

	wxListBox *m_lbox;

	wxBitmap analyzeBitmap;
	wxBitmap analyzeBitmapSelected;

	wxBitmap stopBitmap;
	wxBitmap stopBitmapSelected;

	wxBitmap plusBitmap;
	wxBitmap minusBitmap;

	wxBitmap copyAnalysisToNotationBitmap;

	wxBitmap optionsBitmap;
	wxBitmap lockEngineBitmap;

	string mpvStr[MAX_LINES];
	VarMoveNode *nodes[MAX_LINES];

	MPVMoveNodeVectorArray multiPvLines;
	MoveNodeVector gameModePv;
	MoveNodeDepthArray gameModeDepthPV;

	bool analyzing;
	bool isReading;

	ChessBoard internalBoard;

	wxString currentLoadedEngine;

	wxString panel_name;

	int storedMovNum;

	string engineOptions;
	vector<string> tokenizedOptions;

	int mode; // analyze = 0, play = 1
	int engineColor;

	EngineAnalysisCanvas * analysisCanvas;

   // MyGUIThread m_thread;
	 //MyEngineGUIThread *m_thread;
	// MyEngineThread *m_thread;
    int m_nCurrentProgress;
	bool engineLoaded;

    DECLARE_EVENT_TABLE()
};

#endif