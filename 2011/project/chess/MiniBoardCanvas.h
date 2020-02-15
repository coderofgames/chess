#ifndef MINI_BOARD_CANVAS_H
#define MINI_BOARD_CANVAS_H

#include "headers.h"
#include "ByteBoard.h"

class ChessBoard;


class MiniBoardCanvas: public wxScrolledWindow
{
public:
    MiniBoardCanvas( MyFrame *parent, wxWindow *winParent );
	~MiniBoardCanvas();

	void Flip(){
		flip_board = !flip_board;
		this->Refresh();
	}

	void ResetBoard();
	

	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event){}
	void mouseReleased(wxMouseEvent& event);

    void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);
	void OnEraseBackground(wxEraseEvent& event);
    void OnMouseMove(wxMouseEvent &event);
	//void OnMouse(wxMouseEvent& event);

	bool PickSquare( double x, double y, int START_X, int END_X, 
												int START_Y, int END_Y, 
												int *sqX, int *sqY );


    // set or remove the clipping region
    void Clip(bool clip) { m_clip = clip; Refresh(); }

#if wxUSE_GRAPHICS_CONTEXT
    void UseGraphicContext(bool use) { m_useContext = use; Refresh(); }

#endif

	void InitGraphics();

	void OnSize(wxSizeEvent& event);
	void CalculateSizeParameters();

	void LogEvent(const wxString& name, wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event) { LogEvent("KeyDown", event); }
    void OnKeyUp(wxKeyEvent& event) { LogEvent("KeyUp", event); }
	//void OnChar(wxKeyEvent& event) { LogEvent("Char", event); }

	inline double degToRad(double deg) { return (deg * M_PI) / 180.0; }

private:
    MyFrame *m_owner;
public:

	bool keyDownCtrl;

	enum GameState
	{
		STATE_ZERO = 0,
		SELECTED_PIECE,
		MOVED_PIECE,
	};

	enum SelectionState
	{
		SEL_NONE,
		SELECTED_SQUARE,
		SELECTED_NEW_PIECE,
	};

	SelectionState selState;

	GameState gameState;
	int selPieceID;

public:

	ChessBoard* GetBoard()
	{
		return &internalBoard;
	}

	bool only_show_pawns;
private:

	struct board_color
	{
		float r,g,b;
	};

	//GLFont *font;
	ChessBoard internalBoard;

	board_color light_squares;
	board_color dark_squares;
	//board_color border;
	float border_inset_tweak;

	
	int w, h;
	int pieceID;
	bool brenderPiece;
	bool bResize;

	wxSize sz;

	int BASE;
	int BASE2;
	int BASE4;
	double size;
	double base2_x;
	double base2_y;
	double border;
	double squareSize;

	bool flip_board;
	int selX;
	int selY;
    bool         m_clip;

	double addPieces_x;
	double addPieces_y;

	int newPieceX;
	int newPieceY;

	int newSelectedPieceIndex;

	

#if wxUSE_GRAPHICS_CONTEXT
    bool         m_useContext ;
#endif

	double MouseX;
	double MouseY;

    DECLARE_EVENT_TABLE()
};

#endif