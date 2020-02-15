#ifndef BASE_GRAPHICS_BOARD_H
#define BASE_GRAPHICS_BOARD_H

#include "headers.h"
#include "byteboard.h"
#include "RenderTimer.h"


struct BoardCoord
{
	double x;
	double y;
};

class BaseGraphicsBoard 
{
public:

	BaseGraphicsBoard(){
		ChangingGraphicsMode = false;
		animationSpeed = 6;
		startEnginesOnKeyUp = false;
		newMaterials = true;
	}
	~BaseGraphicsBoard()
	{
		activeEngines.clear();
		
		if( !ChangingGraphicsMode )
		{
			delete chess_board;
		}
	}


	void FlagChangeGraphicsMode()
	{
		ChangingGraphicsMode = true;
	}
	
	void StartEnginesOnKeyUp()
	{
		startEnginesOnKeyUp = true;
	}
	
	inline ChessBoard* GetBoard(){return chess_board;} 


	inline void StopAnimating(){bAnimating = false;}

	inline void SetCapturePiece(int pcID)
	{
		capturePiece = pcID;
	}

	void AddActiveEngine(MyPipePanel* pPanel)
	{
		activeEngines.push_back(pPanel);
	}

	void SetGameMode(int Mode)
	{
		mode = Mode;
	}

	virtual void ChangeTextures(wxString &name)=0;


	inline void GetLastMoveInCaseOfCapture(MyChessMove *pCMove)
	{
		this->capturePiece = chess_board->GetPieceIDForTargetSquare(pCMove);
	}

	inline bool IsAnimating()
	{
		return bAnimating;
	}

	inline void SetAnimationSpeed(int speed)
	{
		animationSpeed = speed;
	}

	inline void SetLastMoveRectColour(wxColour &col)
	{
		lastMoveRectCol = col;
	}
		virtual void ResetBoard()=0;
	virtual void StartAnimation()=0;
	virtual void BackAnimation()=0;
	virtual bool IsBoardFlipped()=0;

	virtual void MyRefresh(bool val) = 0;

	virtual void Flip() = 0;
	virtual void SetBoardBlackSquare(wxString& name)=0;
	virtual void SetBoardWhiteSquare(wxString& name)=0;
	virtual void LoadDefaultWhiteSquares()=0;
	virtual void LoadDefaultDarkSquares()=0;
	virtual void LoadDefaultBorderImage()=0;
	virtual void LoadDefaultBackground()=0;
	virtual void SetBackgroundCol(wxColour& bkGround)=0;
	virtual void SetBackgroundImage(wxString &name)=0;
	virtual void SetBorderImage(wxString &name)=0;

	enum GameState
	{
		STATE_ZERO = 0,
		SELECTED_PIECE,
		MOVED_PIECE,
	};

	enum
	{
		YOUR_MOVE,
		WAIT_COMPUTER,
	};

	void PerformCopy(BaseGraphicsBoard *pgb)
	{
		whoseMove = pgb->whoseMove;

		droppedPieceLastMove = pgb->droppedPieceLastMove;
		movingPieceLastMove = pgb->movingPieceLastMove;
		lastMoveRectCol = pgb->lastMoveRectCol;
		moveForward = pgb->moveForward;

		gameState = pgb->gameState;
		selPieceID = pgb->selPieceID;
		theMove = pgb->theMove;
		pCurrMov = pgb->pCurrMov;

		chess_board = pgb->chess_board;
		//MyFrame *m_owner;
		//bool bAnimating;
		//bool bFowardsAnim;
		capturePiece = pgb->capturePiece;

		//currAnimX = pgb->;
		//currAnimY;

		colWhite = pgb->colWhite;
		colBlack = pgb->colBlack;
		colBorder = pgb->colBorder;
		colArrow = pgb->colArrow;
		colText = pgb->colArrow;

		bAnimating = pgb->bAnimating;
		//int pieceID;
		//brenderPiece;

		//double size;	
		//double base2_x;
	//	double base2_y;

		//double border;
		//squareSize = ;

		selX = pgb->selX;
		selY = pgb->selY;

		m_gameState = pgb->m_gameState;

		mode = pgb->mode; // 0 edit, 1 game

		for( int i = 0; i < pgb->activeEngines.size(); i++ )
		{
			activeEngines.push_back(pgb->activeEngines[i]);
		}

		//bool clearFlag;

		clearFlag = true;
		//renderMoveNow = true;
		needsRefresh = true;

		ChangingGraphicsMode = true;
		pgb->ChangingGraphicsMode = true;

		m_displayGameResult = pgb->m_displayGameResult;
		gameResultString = pgb->gameResultString;
	}

	int whoseMove;

	bool droppedPieceLastMove;
	BoardCoord movingPieceLastMove;
	wxColour lastMoveRectCol;
	bool moveForward;

	GameState gameState;
	int selPieceID;
	MyChessMove theMove;
	MyChessMove* pCurrMov;

	ChessBoard *chess_board;
	MyFrame *m_owner;
	bool bAnimating;
	bool bFowardsAnim;
	int capturePiece;

	double currAnimX;
	double currAnimY;

	wxColour colWhite;
	wxColour colBlack;
	wxColour colBorder;
	wxColour colArrow;
	wxColour colText;

	int pieceID;
	bool brenderPiece;

	double size;	
	double base2_x;
	double base2_y;

	double border;
	double squareSize;

	int selX;
	int selY;

	int m_gameState;

	int mode; // 0 edit, 1 game

	vector<MyPipePanel*> activeEngines;

	//bool clearFlag;

		bool clearFlag;
	bool renderMoveNow;
	bool needsRefresh;

	bool ChangingGraphicsMode;

	bool m_displayGameResult;
	string gameResultString;

	int animationSpeed;

	bool startEnginesOnKeyUp;

	bool newMaterials;
};

#endif