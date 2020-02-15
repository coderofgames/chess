#ifndef PIPE_PANEL_H
#define PIPE_PANEL_H

#include "headers.h"
#include "ByteBoard.h"
//#include "ProcessReadThread.h"

class MyPipePanel;
class EngineAnalysisCanvas;
class EngineManager;


	enum {
		GO_INFINITE,
		GO_TIME,
		GO_FIXED_TIME,
		GO_FIXED_DEPTH,
	};



class ReadTimer : public wxTimer
{
    MyPipePanel* m_panel;
public:
    ReadTimer(MyPipePanel* panel);

    void Notify();
    void start();

	void SetTimeBetweenReads(int n);
	inline int GetTimeBetweenReads(){return timeBetweenReads;}
	int timeBetweenReads;


};


#define MAX_LINES 30
#define MAX_DEPTHS 250

class MoveNodeVector
{
public:

	MoveNodeVector()
	{
		depth = 0;
		depthStr = "";
		score_cp = 0;
		moves = new vector<VarMoveNode>();
	}

	~MoveNodeVector()
	{
		delete moves;
		moves = NULL;
	}

	void Clear()
	{
		delete moves;
		moves = new vector<VarMoveNode>();
	}
	void SetDepth( int Depth )
	{
		depth = Depth;
	}
	void AddNode( VarMoveNode &node )
	{
		moves->push_back(node);
	}

	void operator=(MoveNodeVector &a)
	{
		depth = a.depth;
		depthStr = a.depthStr;
		score_cp = a.score_cp;
		score_p = a.score_p;
		score_str = a.score_str;
	
	
		for( int i = 0; i < a.moves->size(); i++ )
		{
			moves->push_back((*a.moves)[i]);
		}

	}

	VarMoveNode* GetNode(int idx)
	{
		return &(*moves)[idx];
	}


	int GetDepth()
	{
		return depth;
	}

	void SetScoreValues(int& cp, float& sc_p, string& str)
	{
		score_cp = cp;
		score_p = sc_p;
		score_str = str;
	}

	void SetDepthString(string dpthStr)
	{
		depthStr = dpthStr;
	}

	void PvChangedThisFrame(bool val)
	{
		pvChangedThisFrame = val;
	}

	bool HasPVChangedThisFrame()
	{
		return pvChangedThisFrame;
	}

	int depth;
	string depthStr;
	int score_cp;
	float score_p;
	string score_str;
	
	bool pvChangedThisFrame;
	vector<VarMoveNode> *moves;
};


class MoveNodeDepthArray
{
public:

	MoveNodeDepthArray()
	{
		currDepth = 0;
	}

	~MoveNodeDepthArray()
	{
		Clear();
	}

	void Clear()
	{
		currDepth = 0;
		for( unsigned int i = 0; i < MAX_DEPTHS; i++ )
		{
			m_moves[i].Clear();
		}
		
	}
	void SetDepth( int dep, int Depth )
	{
		currDepth = dep;
		m_moves[dep].depth = Depth;
	}
	void AddNode( int depth, VarMoveNode &node )
	{
		currDepth = depth;
		m_moves[depth].moves->push_back(node);
	}

	VarMoveNode* GetNode(int depth, int idx)
	{

		return &(*m_moves[depth].moves)[idx];
	}


	int GetDepth(int dep)
	{
		return m_moves[dep].depth;
	}

	void SetScoreValues(int depth, int& cp, float& sc_p, string& str)
	{
		if( depth < 0 )
			return;
		currDepth = depth;
		m_moves[depth].score_cp = cp;
		m_moves[depth].score_p = sc_p;
		m_moves[depth].score_str = str;
	}

	void SetDepthString(int depth, string dpthStr)
	{
		if( depth < 0 )
			return;
		currDepth = depth;
		m_moves[depth].depthStr = dpthStr;
	}

	void PvChangedThisFrame(bool val)
	{
		pvChangedThisFrame = val;
	}

	bool HasPVChangedThisFrame()
	{
		return pvChangedThisFrame;
	}

	int GetMaxDepth()
	{
		return currDepth+1;
	}



	void operator=(MoveNodeDepthArray &a)
	{
		currDepth = a.currDepth;
		for( int i = 0; i < a.currDepth+1; i++ )
		{
			m_moves[i] = a.m_moves[i];
		}
	}

	int currDepth;
	bool pvChangedThisFrame;
	MoveNodeVector m_moves[MAX_DEPTHS];
};

class MPVMoveNodeVectorArray
{
public:

	MPVMoveNodeVectorArray(){
		pvChangedThisFrame = false;
	}
	~MPVMoveNodeVectorArray()
	{
		for( int i = 0; i < MAX_LINES; i++ )
			mpvs[i].Clear();
	}


	void AddNode( VarMoveNode& move, int mpv )
	{
		mpvs[mpv].AddNode(move);
	}
	
	VarMoveNode* GetMove( int MPV, int idx )
	{
		return mpvs[MPV].GetNode(idx);
	}

	void SetScore( int& cp, float& sc_p, string& scoreStr, int MPV)
	{
		mpvs[MPV].SetScoreValues( cp, sc_p, scoreStr);
	}

	void ClearMPV(int mpv)
	{
		mpvs[mpv].Clear();
	}

	void PvChangedThisFrame(bool val)
	{
		pvChangedThisFrame = val;
	}

	bool HasPVChangedThisFrame()
	{
		return pvChangedThisFrame;
	}

	void operator=(MPVMoveNodeVectorArray &a)
	{
		pMoveAnalyzed = a.pMoveAnalyzed;
		
		for( int i=0; i < MAX_LINES;i++)
		{
			mpvs[i] = a.mpvs[i];
		}
	}

	bool pvChangedThisFrame;
	PGN::Move* pMoveAnalyzed;
	MoveNodeVector mpvs[MAX_LINES];
};

class MoveNodeMPVDepthArray
{
public:

	void AddNode( VarMoveNode &node, int depth, int mpv )
	{
		depths[depth].AddNode(node, mpv);
	}
	
	void PvChangedThisFrame(bool val)
	{
		pvChangedThisFrame = val;
	}

	bool HasPVChangedThisFrame()
	{
		return pvChangedThisFrame;
	}

	void operator=(MoveNodeMPVDepthArray &a)
	{
		pMoveAnalyzed = a.pMoveAnalyzed;
		for( int i = 0; i < MAX_DEPTHS; i++ )
		{
			depths[i] = a.depths[i];
		}
	}

	bool pvChangedThisFrame;
	PGN::Move* pMoveAnalyzed;
	MPVMoveNodeVectorArray depths[MAX_DEPTHS]; // greater than practical usage expectations
};



enum
{
	HYPER_BULLET,
	BULLET,
	BLITZ,
	RAPID,
	STANDARD,
	LONG_TIME,
};





class BoardCanvas;

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

	//string GetEngineOptions();

	inline bool IsActive(){ return engineLoaded; }
	bool IsAnalysing() {
		if( LockEngineToMove )
			return false;
		else return (analyzing); 
	}
	inline bool EngineIsLoaded() { return engineLoaded;}

	inline vector<string>* GetTokenizedOptionsVector()
	{
		return &tokenizedOptions;
	}

	void SendMoves();
	void SendStop();
	void SendGoInfinite();
	void SendFen();
	void SendUciNewGame();
	void SendMoveTime();
	void SendMoveDepth();
	void SendFixedMoveTime();
	void SendOptionsString(string opts);
	void SendStartGameMode();
	void SendStartAnalyzeMode();

	void DoFinishGame();

    void DoGet();
    void DoClose();
	void DoShutdown();

	inline int GetNumKbtzLines()
	{
		return num_kibitz_lines;
	}

	inline void OptionsSetNumKibitzLines(int nkbl)
	{
		num_kibitz_lines = nkbl;
	}

	inline MPVMoveNodeVectorArray* GetMPVNodeVectorArray()
	{
		return &multiPvLines;
	}

	inline void SetEngineColor( int white_black )
	{
		engineColor = white_black;
	}

	inline int GetEngineColor()
	{
		return engineColor;
	}

	inline void SetGameMode()
	{
		mode = 1;
		if( this->engineLoaded )
			SendStartGameMode();
	}

	inline void SetAnalyzeMode()
	{
		gameModeDepthPV.Clear();
		mode = 0;
		if( this->engineLoaded )
			SendStartAnalyzeMode();
	}

	inline int GetMode()
	{
		return mode;
	}

	inline MoveNodeVector* GetGameModePV()
	{
		return &gameModePv;
	}

	inline MoveNodeDepthArray * GetGameModeDepthPV()
	{
		return &gameModeDepthPV;
	}


	inline bool IsEngineLockedToMove()
	{
		return LockEngineToMove;
	}

	inline void SetPanelName(wxString pn)
	{
		panel_name = pn;
	}

	inline wxString GetPanelName()
	{
		return panel_name;
	}

	/*string& GetEngineName()
	{
		return panelName;
	}*/

	inline wxString& GetEngineName()
	{
		return panelName;
	}

	void PlayerMoves();


	inline void SetEngineManager(EngineManager* pEngineManager, int indx)
	{
		m_engineManager = pEngineManager;
		indxToEngineManager = indx;
	}

	inline EngineManager* GetEngineManager()
	{
		return m_engineManager;
	}


	inline void SetOpponent(MyPipePanel *pOther)
	{
		opponent = pOther;
		EngineVsEngineMode = true;
	}

	inline void StopEngineVsEngineMode()
	{
		opponent = NULL;
		EngineVsEngineMode = false;
	}

	inline void SetTimeMode(int TimeMode)
	{
		timeMode = TimeMode;
	}

	inline int GetTimeMode()
	{
		return timeMode;
	}

	int GetGoMode()
	{
		return goMode;
	}

	void SetGoMode( int i )
	{
		goMode = i;
	}

	inline MyFrame* GetParent()
	{
		return this->pParent;
	}


	void SetPanelNameIntoCaption();


	bool isReading;
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

	bool CheckTime();


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
	

	ChessBoard internalBoard;

	wxString currentLoadedEngine;

	

	int storedMovNum;

	string engineOptions;
	vector<string> tokenizedOptions;

	int timeMode;
	int mode; // analyze = 0, play = 1
	int engineColor;

	MyPipePanel *opponent;
	bool EngineVsEngineMode;
	
	int fixedDepth;
	int fixedTimeMillis;
	


	int goMode;

	EngineAnalysisCanvas * analysisCanvas;
	EngineManager *m_engineManager;
	int indxToEngineManager;

	wxString panel_name; // the name of the window
	wxString panelName;	// the name of the engine
   // MyGUIThread m_thread;
	 //MyEngineGUIThread *m_thread;
	// MyEngineThread *m_thread;
    int m_nCurrentProgress;
	bool engineLoaded;

    DECLARE_EVENT_TABLE()
};

#endif