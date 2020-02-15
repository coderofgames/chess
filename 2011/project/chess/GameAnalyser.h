#ifndef GAME_ANALYSER_H
#define GAME_ANALYSER_H


#include "headers.h"
#include "PgnLoad.h"
#include "MyPipePanel.h"

class MyFrame;

enum
{
	PROGRESS_ID = wxID_HIGHEST + 2000
};

class AnalysisInfo
{
public:
	AnalysisInfo(){}
	~AnalysisInfo(){}

	int numMoves;
	int totalTimeSeconds;
	int secondsPerMove;
};

class GameAnalyser : public wxScrolledWindow
{
public:

	GameAnalyser(MyFrame* parent,  MyPipePanel *pp);
	~GameAnalyser();


	void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);

	void CalculateTimeToCompletion( int &minutes, int& seconds)
	{
		gameAnalysis->numMoves = m_game->CountMoves();
		gameAnalysis->totalTimeSeconds = gameAnalysis->numMoves*secondsPerMove;

		minutes = gameAnalysis->totalTimeSeconds / 60;
		seconds = gameAnalysis->totalTimeSeconds - minutes *60;
	}

	MyFrame* m_parent;
	PGN::Move *pMove;
	PGN::Game *m_game;
	MyPipePanel *m_pipePanel;

	int secondsPerMove;
	wxGauge *m_Progress;

	AnalysisInfo* gameAnalysis;


	DECLARE_EVENT_TABLE()
};

#endif