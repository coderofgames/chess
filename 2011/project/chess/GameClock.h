#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H

#include "headers.h"


class GameClock
{
public:

	GameClock(){}
	~GameClock(){}

	int startTimeSeconds;
	int currTimeSecondsWhite;
	int currTimeSecondsBlack;

	long currTimeCentiSecondsWhite;
	long currTimeCentiSecondsBlack;

	int white_black;

	int SetMinutes;
	int SetSeconds;

	bool m_paused;

	wxLongLong 	lastTime;// = wxGetLocalTimeMillis ();
	//wxLongLong 	localTime;
//lastTime = localTime;
	
	wxLongLong timeDelta;// = 40;
};


	class MyFrame;

enum
{
	HYPER_BULLET,
	BULLET,
	BLITZ,
	RAPID,
	STANDARD,
	LONG_TIME,
};

	class GameClockPanel : public wxScrolledWindow
	{
	public:

		GameClockPanel(MyFrame* parent);
		GameClockPanel(wxWindow* parent);
		~GameClockPanel();


		void OnPaint(wxPaintEvent &event);
		void OnSize(wxSizeEvent& event);

		void Start();

		void Update();

		void Stop();

		void SetTimeControl(int Minutes, int Seconds, int Incr_millis, int tick_millis);

		bool CheckTimeOutWhite();

		bool CheckTimeOutBlack();

		void PauseGame();

		void UnPause()
		{
			m_paused = false;
		}

		bool IsPaused()
		{
			return m_paused;
		}

		void OnIdle(wxIdleEvent &event);

		void SetWhiteBlack(int wb) ;


		void ResetTime();


		inline int GetEngineMilliSecondsWhite()
		{
			return (unsigned int)currTimeMilliSecondsWhite;
		}

		inline int GetEngineMilliSecondsBlack()
		{
			return (unsigned int)currTimeMilliSecondsBlack;
		}

		void TickClock();

		void StartGame();

		void SetNumMovesBeforeTimeControl(int numMovesBeforeTimeControl)
		{
			movesBeforeTimeControl = numMovesBeforeTimeControl;
		}

		int GetNumMovesBeforeTimeControl()
		{
			return movesBeforeTimeControl;
		}

		void UseFideTimeControl()
		{
			bFideTimeControl = true;
		}

		bool IsFideTimeControl()
		{
			return bFideTimeControl;
		}

		void SetIncrementAfterTimeControl(int inc )
		{
			incrementAfterTimeControl = inc;
		}

		int GetIncrementAfterTimeControl()
		{
			return incrementAfterTimeControl;
		}

		void SetNumMillisAfterTimeControl(int n)
		{ 
			numMilliSecondsAfterTimeControl = n;
		}

		int m_timeMode;

		long startTimeSeconds;
		long currTimeSecondsWhite;
		long currTimeSecondsBlack;

		long currTimeCentiSecondsWhite;
		long currTimeCentiSecondsBlack;

		long currTimeMilliSecondsWhite;
		long currTimeMilliSecondsBlack;

		int white_black;

		int SetMinutes;
		int SetSeconds;

		int increment;

		bool m_paused;

		wxLongLong 	lastTime;// = wxGetLocalTimeMillis ();
		wxLongLong lastRenderTime;
		//wxLongLong 	localTime;
	//lastTime = localTime;
	
		wxLongLong timeDelta;// = 40;
 
		MyFrame *m_owner;
		bool m_useContext;

		wxLongLong currTime ;//= wxGetLocalTimeMillis ();


		bool bFideTimeControl;

		int plyCount ;
		int blackMoves;
		int whiteMoves;
		int movesBeforeTimeControl;
		int incrementAfterTimeControl;

		int numSecondsAfterTimeControl;
		int numMilliSecondsAfterTimeControl;



		DECLARE_EVENT_TABLE()

	};









#endif