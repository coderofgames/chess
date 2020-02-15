#ifndef BOARD_CANVAS_H
#define BOARD_CANVAS_H

#include "wx/wxprec.h"
//#include "Clipboard.h"
#include "wx/app.h"

//#include "Book.h"
//#include "Objects.h"
//#include "UCIEngine.h"
//#include "ChessRules.h"
//#include "Repository.h"
//#include "ByteBoard.h"

//using namespace thc;

//Objects objs;


#ifdef __BORLANDC__
    #pragma hdrstop
#endif


#include "wx/image.h"
#include "wx/colordlg.h"

#include "wx/imaglist.h"


#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

#include "wx/colordlg.h"
#include "wx/image.h"
#include "wx/artprov.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/filename.h"



struct BoardTheme
{
	wxString white;
	wxString black;
	int whiteAlpha;
	int blackAlpha;
};
/*
BoardTheme themes[14] = 
{ 
	{wxT("Burl Walnut"), wxT("Figured Makore"), 0, 0 },
	{wxT("Italian Poplar"), wxT("Walnut"), 0, 0 },
	{wxT("Clear Pine"), wxT("Cherry"), 0, 0 },
	{wxT("Ash"), wxT("Burl Walnut"), 0, 0 },
	{wxT("Honduras Mahogony"), wxT("Walnut"), 118, 50 },
	{wxT("Honduras Mahogony"), wxT("Kewazinga"), 118, 50 },
	{wxT("Honduras Mahogony"), wxT("Burl Walnut"), 118, 50 },
	{wxT("Burl Walnut"), wxT("Olive Ash Burl"), 118, 50 },
	{wxT("Italian Poplar"), wxT("Honduras Mahogony"), 118, 50 },
	{wxT("PurpleHeart"), wxT("Red Birch"), 118, 50 },
	{wxT("Cherry"), wxT("Rotary White Maple"), 118, 50 },
	{wxT("Cherry"), wxT("Walnut"), 100, 30 },
	{wxT("Walnut"), wxT("Honduras Mahogony"), 100, 30 },
	{wxT("Clear Pine"), wxT("Walnut"), 100, 30 }
};
  */

class MyFrame;

class MyPipePanel;

class BoardCanvas;

class GameBoardTile
{
public:

	GameBoardTile()
	{
		dirty = false;
	}

	void SetBitmap( wxBitmap *pBmp)
	{
		bmp = pBmp;
	}

	void SetCoords(double X, double Y)
	{
		x = X;
		y = Y;
	}

	double GetX()
	{
		return x;
	}

	double GetY()
	{
		return y;
	}

	void SetSize(double Size)
	{
		size = Size;
	}

	double GetSize()
	{
		return size;
	}

	wxBitmap* GetBitmap()
	{
		return bmp;
	}

	bool MarkForRefresh()
	{
		dirty = true;
	}

	bool IsDirty()
	{
		return dirty;
	}

	void SetIndexX( int xIndex )
	{
		xIndx = xIndex;
	}

	void SetIndexY( int yIndex )
	{
		yIndx = yIndex;
	}

	int GetIndexX()
	{
		return xIndx;
	}

	int GetIndexY()
	{
		return yIndx;
	}

	void SetColour( wxColour &col)
	{
		m_colour = col;
	}



	wxColour& GetColour()
	{
		return m_colour;
	}

	void SetOverlay(wxColour &col)
	{
		m_overlay = col;
	}

	wxColour& GetWhiteOverlay()
	{
		return m_overlay;
	}


	wxColour m_colour;

	wxColour m_overlay;


	wxBitmap *bmp;

	double x;
	double y;
	double size;

	int xIndx;
	int yIndx;

	bool dirty;
};




#include "BaseGraphicsBoard.h"

class GameResultMessageTimer : public wxTimer
{
public:

	GameResultMessageTimer (BoardCanvas* board);


	void Set(int type, wxString str);
	void Notify();
	void start();

	int m_dlgType;
	wxString m_string;
	BoardCanvas *m_board;
};



class BoardCanvas: public wxWindow, public BaseGraphicsBoard
{
public:
    BoardCanvas( MyFrame *parent, BaseGraphicsBoard* boardToCopy=NULL );
	~BoardCanvas();

	void Flip(){
		flip_board = !flip_board;
		this->Refresh();
	}

	void ChangeTextures(wxString &name){}

	void ResetBoard();

	void StopTimer()
	{
		if( this->m_rtimer	&& m_rtimer->IsRunning() )
			m_rtimer->Stop();
	}
	

	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event){}
	void mouseReleased(wxMouseEvent& event);



	void OnMouseWelcome(wxMouseEvent& event){MouseOverWin = true;}
	void OnMouseBye(wxMouseEvent& event){MouseOverWin = false;}


	bool PickSquare( double x, double y, int START_X, int END_X, 
												int START_Y, int END_Y, 
												int *sqX, int *sqY );

    void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);
	void OnEraseBackground(wxEraseEvent& event);
    void OnMouseMove(wxMouseEvent &event);
	//void OnMouse(wxMouseEvent& event);



    // set or remove the clipping region
    void Clip(bool clip) { m_clip = clip; Refresh(); }
#if wxUSE_GRAPHICS_CONTEXT
    void UseGraphicContext(bool use) { m_useContext = use; Refresh(); }

#endif

	void InitGraphics();


	void DisplayGameResult(wxString result, GameResultMessageTimer *timer);


	void OnSize(wxSizeEvent& event);
	void CalculateSizeParameters();
	void LogEvent(const wxString& name, wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event) { LogEvent("KeyDown", event); }
    void OnKeyUp(wxKeyEvent& event) { /*LogEvent("KeyUp", event);*/ }
	void OnChar(wxKeyEvent& event) { LogEvent("Char", event); }

	inline double degToRad(double deg) { return (deg * M_PI) / 180.0; }


	void SetBoardBlackSquare(wxString& name);
	void SetBoardWhiteSquare(wxString& name);
	void LoadDefaultWhiteSquares();
	void LoadDefaultDarkSquares();
	void LoadDefaultBorderImage();
	void LoadDefaultBackground();


	//bool CheckForRepetition();

	void SetBackgroundCol(wxColour& bkGround)
	{
		this->SetBackgroundColour(bkGround);
	}

	void SetBackgroundImage(wxString &name);

	void SetBorderImage(wxString &name);



	bool IsBoardFlipped()
	{
		return this->flip_board;
	}

	void MyRefresh(bool val) 
	{
		this->Refresh(val);
		needsRefresh = true;
	}











	void StartAnimation();
	void BackAnimation();

private:
    

	void DrawBackground(wxGraphicsContext *gc);
	void DrawBorder(wxGraphicsContext *gc);
	void CreateSquares(wxGraphicsContext *gc,wxGraphicsPath *whiteSquares, wxGraphicsPath *darkSquares);

	void DrawSquares(wxGraphicsContext *gc);
	void DrawPieces(wxGraphicsContext *gc);
	void DrawMovers(wxGraphicsContext *gc);
	void CreateSelectedSquarePath(wxGraphicsPath *path);
	
	void SetTileCoords();

	void DoMove(bool &promote);
	//void MarkDirtyLastMoveTiles(
	//wxGraphicsPath whiteSquares;// = gc->CreatePath() ;

	//wxGraphicsPath darkSquares;// = gc->CreatePath() ;

	void RenderToBackBuffer();

public:


	
	bool MouseOverWin;

	wxLongLong 	localTime;
	wxLongLong timeDelta;
	wxLongLong lastTime;


	

	RenderTimer *m_rtimer;
	bool Notified;




	
    


	wxLongLong 	animationStart;
	double 	timeSinceAnimStart;
	wxLongLong animationDelta;



	
	
	
	struct board_color
	{
		float r,g,b;
	};

	//GLFont *font;
	
	board_color light_squares;
	board_color dark_squares;
	




	wxBitmap whiteSquaresBmp;
	wxBitmap blackSquaresBmp;
	wxBitmap borderBmp;
	wxBitmap bmpBorder;
	wxBitmap backGroundImage;
	wxBitmap backBuffer;
	
	wxBitmap borderBitmap[4];
	wxBitmap defaultSquares[8][8];
	wxBitmap defaultSquaresWhite[8][8];

	GameBoardTile m_squareTiles[8][8];

	struct dblVec
	{
		double xy[2];
	};

	dblVec boardCoordTextCoords[16];
	
private:	
	
	//board_color border;
	float border_inset_tweak;

	
	



	int w, h;
	
	bool bResize;

	wxSize sz;

	 int BASE;//  = 80.0;
 int BASE2;// = BASE/2;
 int BASE4;// = BASE/4;
	
	//GLuint tex_2d;
	float mx;
	float my;



	//Drawable *chessbase_board;

	//Image *image;
	//Drawable *sprite;
	bool flip_board;


	bool fat_mode;



	//int gameStatus;

/*			float board_size;//=14.0f;
		float small_square_size;// = board_size/8.0f;
		float start_x;// = -board_size/2.0f;
		float start_y;// = -board_size/2.0f;
		float end_x;// = board_size/2.0f;
		float end_y;// = board_size/2.0f;
		//int loopx;//=0;
		//int loopy;//=0;
		float border_size;// = 0.55f;
		float border_outer_inset;// = 0.75f;
		float border_inner_size;// = 0.15f;
		float border_inner_inset;// = 0.075f;
		*/
    wxBitmap     m_smile_bmp;
    wxIcon       m_std_icon;
    bool         m_clip;
#if wxUSE_GRAPHICS_CONTEXT
    bool         m_useContext ;
	wxBitmap *gs_bmp4;
#endif

	wxFont font;


	
	double MouseX;
	double MouseY;

    DECLARE_EVENT_TABLE()
};


#endif