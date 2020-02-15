#include "BoardCanvas.h"
#include "mdi.h"
#include "ChessSquare.h"
#include "MyPipePanel.h"
#include "PromotionSelectionDialog.h"
#include "OptionsFrame.h"
#include "GameClock.h"
#include "MoveDataCanvas.h"
#include "NotationFrame.h"



bool SquareOverLapsSquare( double x1, double y1 , double x2, double y2, double squareSize)
{
	if( x1 > x2 && x1 < x2 + squareSize )
		return true;

	if( y1 > y2 && y1 < y2+ squareSize )
		return true;

	if( x1+ squareSize > x2 && x1 + squareSize < x2 + squareSize )
		return true;

	if( y1+ squareSize > y2 && y1 + squareSize < y2 + squareSize )
		return true;

	return false;
}


GameResultMessageTimer::GameResultMessageTimer(BoardCanvas* board) : wxTimer()
{
    m_dlgType = 0;
	m_string = "";
}



void GameResultMessageTimer::Set(int type, wxString str)
{
    m_dlgType = type;
	m_string = str;
}
 
void GameResultMessageTimer::Notify()
{
//	wxMessageBox(m_string, "Game Over");
	m_board->DisplayGameResult(m_string, this);
//	this->Stop();
	
}
 
void GameResultMessageTimer::start()
{
    wxTimer::Start(1000);
}

//GameResultMessageTimer g_gameMessageTimer;

// ----------------------------------------------------------------------------
// BoardCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(BoardCanvas, wxWindow)
    EVT_PAINT  (BoardCanvas::OnPaint)
	EVT_IDLE(BoardCanvas::OnIdle)
    EVT_MOTION (BoardCanvas::OnMouseMove)
    EVT_SIZE(BoardCanvas::OnSize)

   // EVT_MOUSE_EVENTS(BoardCanvas::OnMouse)
	EVT_LEFT_DOWN(BoardCanvas::mouseDown)
	EVT_LEFT_UP(BoardCanvas::mouseReleased)
	EVT_MOUSEWHEEL(BoardCanvas::mouseWheelMoved)
    EVT_ERASE_BACKGROUND(BoardCanvas::OnEraseBackground)
	EVT_ENTER_WINDOW(                  BoardCanvas::OnMouseWelcome)
	EVT_LEAVE_WINDOW(                  BoardCanvas::OnMouseBye)
END_EVENT_TABLE()

BoardCanvas::BoardCanvas(MyFrame *parent, BaseGraphicsBoard* boardToCopy)
//:wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                               wxWANTS_CHARS  )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	if( boardToCopy )
	{
		this->PerformCopy(boardToCopy);
	}

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

#ifdef WINDOWS_7
	SetDoubleBuffered(true);
#endif	
	SetDoubleBuffered(true);
//	wxMessageBox("entering board canvas");
	m_useContext = true;
    m_owner = parent;
    
//	wxMessageBox("getting icon");
   // m_smile_bmp = wxBitmap(smile_xpm);
    //m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
	//this->EnableScrolling(false, true);
	//SetScrollbar(wxVERTICAL, 0, 16, 50);
	//wxSize sz(GetClientSize());
	//SetVirtualSize(sz.GetWidth(), sz.GetHeight()*4);
//	wxMessageBox("getting client size");
	wxSize sz = GetClientSize();
//	wxMessageBox("setting w and h");
    w = sz.GetWidth();
    h = sz.GetHeight();
//	wxMessageBox("done setting w and h");

	//   this->gs_bmp4 = new wxBitmap;
	      /* 4 colour bitmap */
   // gs_bmp4->LoadFile("board.bmp", wxBITMAP_TYPE_BMP);
        /* init scrolled area size, scrolling speed, etc. */
    //   SetScrollbars(1,1, w, h*2, 0, 0);
    m_clip = false;

//	wxMessageBox("setting variables");
light_squares.r = 3.0f;
	light_squares.g = 3.0f;
	light_squares.b = 3.0f;

	dark_squares.r = 1.5f;
	dark_squares.g = 1.5f;
	dark_squares.b = 2.0f;

	//border.r = 1.2f;
	//border.g = 0.9f;
	//border.b = 0.5f;

	size = 200;// = (sz.GetHeight() < sz.GetWidth() ? sz.GetHeight()*0.85 : sz.GetWidth()*0.85);
	//BASE2 = (sz.GetHeight()  < sz.GetWidth()? sz.GetHeight()*0.075 : sz.GetWidth()*0.075);
	base2_x=30;// = sz.GetWidth()/2 - size/2;
	base2_y=30;// = sz.GetHeight()/2 - size/2;

	border=10;// = size*18/500;//17.75;
	squareSize = (size-size*20.0f/200)/8;

	fat_mode=true;

	mx=my=0.0f;
	w=h=0;
	
	BASE  = 80.0;
	BASE2 = BASE/2;
	BASE4 = BASE/4;

	selX=0;
	selY=0;

	bResize = false;

//	wxMessageBox("creating font");
	font = wxFont(9, wxFONTFAMILY_DEFAULT , wxNORMAL, wxBOLD, false); 
	


//	wxMessageBox("creating board");
	if( boardToCopy == NULL)
		chess_board = new ChessBoard();

//	wxMessageBox("calling load textures");
	if( !chess_board->svgTextures[0] )
		chess_board->LoadTextures();
	
//	wxMessageBox("calling change textures");
	//chess_board->ChangeTextures("images/tango/48x48/apps/");
	//chess_board->ChangeTextures("images/glow/Chess Pieces/");
	//chess_board->ChangeTextures("images/dgkit/");
	//chess_board->ChangeTextures("images/PieceThemes/Alpha/");
	//chess_board->ChangeTextures("images/PieceThemes/Merida2/");
	//chess_board->ChangeTextures("images/PieceThemes/Liepzig/");
	//chess_board->ChangeTextures("images/PieceThemes/Smart/");
	//chess_board->ChangeTextures("images/PieceThemes/Alpha/dropshadow/");
	//chess_board->ChangeTextures("images/PieceThemes/Alpha/dropshadow2/");
	chess_board->ChangeTextures(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+string("/images/PieceThemes/Fritz/100/dropshadow/"));
//	wxMessageBox("textures changed, setting states");
	
	gameState = GameState::STATE_ZERO;
	selPieceID = -1;

	bAnimating = false; 
	animationDelta = 70;
	bFowardsAnim = false;

	//this->
#if wxUSE_GRAPHICS_CONTEXT
    m_useContext = false;
#endif

	m_rtimer = new RenderTimer(this);
	m_rtimer->start();
	Notified = false;


	//if(is_first_time)
	{
		
		
		//is_first_time = false;
	}
//	wxMessageBox("getting time");

	localTime = wxGetLocalTimeMillis ();
	lastTime = localTime;
	
	timeDelta = 50;

//	wxMessageBox("setting variables");



//	wxMessageBox("Loading border texture");
	//bmpBorder.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/border_frame.png",wxBITMAP_TYPE_PNG);;
	LoadDefaultBorderImage();
	
//		wxMessageBox("setting states");
	//whiteSquaresBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/images/Squares/Walnut/white.png",wxBITMAP_TYPE_PNG);
	//blackSquaresBmp.LoadFile("images/Squares/Walnut/black.png",wxBITMAP_TYPE_PNG);
	
	//whiteSquaresBmp.LoadFile("images/Squares/indi/h7.png",wxBITMAP_TYPE_PNG);
	//blackSquaresBmp.LoadFile("images/Squares/indi/b9.png",wxBITMAP_TYPE_PNG);
	//borderBmp.LoadFile("images/Squares/Walnut/panel.png",wxBITMAP_TYPE_PNG);
	//borderBmp.LoadFile("images/Squares/Walnut/black.png",wxBITMAP_TYPE_PNG);


	

	SetBackgroundColour(wxColour(145,156,156,255));//);

//	wxMessageBox("loading squares");

	if( this->m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps )
	{
		LoadDefaultWhiteSquares();
		LoadDefaultDarkSquares();
	}



//	wxMessageBox("loading more squares");



//	wxMessageBox("loading background");


//	wxMessageBox("calculating size");
	CalculateSizeParameters();

		colWhite = this->m_owner->optionsFrame->m_BoardOptions.colWhite;//wxColour(255,  255,  255, 255);	
		colBlack = this->m_owner->optionsFrame->m_BoardOptions.colBlack;//wxColour(150,  150,  255, 255);

	if( boardToCopy == NULL )
	{
//	wxMessageBox("connecting event handlers");
		lastMoveRectCol = *wxRED;
		droppedPieceLastMove = false;
		whoseMove = YOUR_MOVE;
		capturePiece = 0;
		pCurrMov = (MyChessMove*)0xFFFFFFFF;
		mode = 0;
		m_gameState = PLAYING;
		renderMoveNow = false;
		MouseOverWin = false;

		clearFlag = true;
	
		m_displayGameResult = false;

		pieceID = 0;
		brenderPiece = false;

		flip_board = true;

		moveForward = true;
	}

    // connect event handlers for the blue input window
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(BoardCanvas::OnKeyDown),
                        NULL, this);
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(BoardCanvas::OnKeyUp),
                        NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(BoardCanvas::OnChar),
                        NULL, this);

//	wxMessageBox("done loading");
}

void BoardCanvas::LoadDefaultWhiteSquares()
{
	for( char a = 'a'; a < 'i'; a++ )
	{
		for( char n = '1'; n < '9'; n++ )
		{
			wxString path = "/images/Squares/indi/Burl Walnut/";
		


			path+=a;
				path+= n;

			path+= ".png";
			unsigned int Xi = a-'a';
			unsigned int Yi = n-'1';
			//defaultSquaresWhite[ a-'a' ][ n - '1' ].FreeResource(true);
			defaultSquaresWhite[ Xi ][ Yi ].LoadFile(wxStandardPaths::Get().GetResourcesDir()+path,wxBITMAP_TYPE_PNG);

			
			if( !defaultSquaresWhite[ Xi ][ Yi ].IsOk() )
			{
				wxMessageBox("Problem loading square: /images/Squares/indi/Burl Walnut/" + a + n + wxString(".png"));
			}
			else
			{
				if( (Xi+Yi)%2==1 )
					m_squareTiles[ Xi ][ 7-Yi ].SetBitmap( &defaultSquaresWhite[ Xi ][ Yi ] ); 
			}
		}
	}
}

void BoardCanvas::LoadDefaultDarkSquares()
{
	for( char a = 'a'; a < 'i'; a++ )
	{
		for( char n = '1'; n < '9'; n++ )
		{
			wxString path = "/images/Squares/indi/Fiddleback Makore/";
		


			path+=a;
				path+= n;

			path+= ".png";
			
			unsigned int Xi = a-'a';
			unsigned int Yi = n-'1';
			defaultSquares[ Xi ][ Yi ].LoadFile(wxStandardPaths::Get().GetResourcesDir()+path,wxBITMAP_TYPE_PNG);
			if( !defaultSquares[ Xi ][ Yi ].IsOk() )
			{
				wxMessageBox("Problem loading square: /images/Squares/indi/Fiddleback Makore/" + a + n +wxString(".png"));
			}
			else
			{
				if( (Xi+Yi)%2!=1 )
					m_squareTiles[ Xi ][ 7-Yi ].SetBitmap( &defaultSquares[ Xi ][ Yi ] ); 
			}
		}
	}
}

void BoardCanvas::LoadDefaultBorderImage()
{
	if( bmpBorder.GetBitmapData())
		return;

	bmpBorder.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/border_frame.png",wxBITMAP_TYPE_PNG);;
	if( !bmpBorder.IsOk() )
	{
		wxMessageBox("problem loading default border texture: /images/border_frame.png");
	}
}

void BoardCanvas::LoadDefaultBackground()
{
	backGroundImage.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/images/Squares/Walnut/panel.png", wxBITMAP_TYPE_PNG);
	if( !backGroundImage.IsOk() )
	{
		wxMessageBox("Problem Loading Background: /images/Squares/Walnut/panel.png");
	}
}

BoardCanvas::~BoardCanvas()
{
	/*if( gs_bmp4 )
	{
		delete gs_bmp4;
		gs_bmp4 = NULL;
	}*/
	//delete chess_board;
	//if( this->r
	if( m_rtimer )
	{
		if(m_rtimer->IsRunning())
			m_rtimer->Stop();
		delete m_rtimer;
		m_rtimer = NULL;
	}
	whiteSquaresBmp.FreeResource(true);
	blackSquaresBmp.FreeResource(true);
	borderBmp.FreeResource(true);

	for( int i = 0; i < 4; i++ )
	{
		borderBitmap[i].FreeResource(true);
	}

	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			defaultSquares[ i][ j].FreeResource(true);
			defaultSquaresWhite[ i][ j].FreeResource(true);
		}
	}

	bmpBorder.FreeResource(true);
}


void BoardCanvas::SetBackgroundImage(wxString &name)
{
	//wxString path = name;
	//path += "/";
	backGroundImage.FreeResource(true);
	backGroundImage.LoadFile(name, wxBITMAP_TYPE_PNG);
}


void BoardCanvas::SetBorderImage(wxString &name)
{
	//wxString path = name;
	//path += "/";
	borderBmp.FreeResource(true);

	for( int i = 0; i < 4; i++ )
	{
		borderBitmap[i].FreeResource(true);
	}
	borderBmp.LoadFile(name, wxBITMAP_TYPE_PNG);
	CalculateSizeParameters();
}

void BoardCanvas::DisplayGameResult(wxString result,GameResultMessageTimer *timer)
{
/*	wxMessageBox(result, "Game Over");
	timer->Stop();
	delete timer;
	timer = NULL;*/
}


bool BoardCanvas::PickSquare( double x, double y, int START_X, int END_X, 
												int START_Y, int END_Y, 
												int *sqX, int *sqY )
{
	// start_x = -board_size/2
	// so in this case it is -7;
	// so small_square_size is board_size/8
	// start_x + board_size/2 = 0;
	// x + board_size/2 >0
	if( (mode == 1) && (whoseMove == WAIT_COMPUTER) )
		return false;

	if ( x >= START_X && x < END_X )
	{
		if( y >= START_Y && y < END_Y )
		{
			*sqX = (int)((x - START_X)/squareSize );
			*sqY = (int)((y - START_Y)/squareSize );

			if( !flip_board )
			{
				*sqX = 7-*sqX;
				*sqY = 7-*sqY;
			}
			return true;
		}
	}
	return false;
}

void BoardCanvas::OnEraseBackground(wxEraseEvent& evt)
{
    // intentionally empty
	//Refresh();
//	evt.Skip();
}

void BoardCanvas::InitGraphics()
{
	/*if ( fat_mode )OnP
		board_size= (w>h? h*0.85 : w*0.85 );//.0f;
	else
		board_size = 14.0f;
	*/

	//ChangeScreenResolution (800, 600, 32);
				
	
}


void BoardCanvas::ResetBoard()
{
	sz = GetClientSize();
	chess_board->SetBoard(this->m_owner->currentGame->pBase->m_fenBlack);
	CalculateSizeParameters();
	gameResultString = "";//Drawn, 50 moves without a pawn move or capture!";
	m_displayGameResult = false;
	m_gameState = PLAYING;
	if( mode == 1 )
	{
		whoseMove = YOUR_MOVE;
		if( m_owner->playersOpponent )
		{
			m_owner->playersOpponent->SetEngineColor(BLACK);
		}
	}
}

static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }

void BoardCanvas::DrawBackground(wxGraphicsContext *gc)
{
	if( m_owner->optionsFrame->m_BoardOptions.useBackgroundImage == true )
	{
		gc->DrawBitmap(this->backGroundImage, 0,0, sz.GetWidth(), sz.GetHeight());
	}
	else
	{
		//gc->Clear();
		//gc->DrawRectangle(0, 0, w, base2_y);
		//gc->DrawRectangle(0, base2_y, base2_x, sz.GetHeight()- base2_y*2);
		//gc->DrawRectangle(0, base2_y+border*2 + squareSize*8, w, base2_y);
		//gc->DrawRectangle(base2_x + border*2 + squareSize*8, base2_y, base2_x,sz.GetHeight()- base2_y*2 );
	}
}

void BoardCanvas::DrawBorder(wxGraphicsContext *gc)
{
	wxGraphicsPath borderPath = gc->CreatePath() ;
	if( m_owner->optionsFrame->m_BoardOptions.useBorderBitmap == false )
	{
		borderPath.AddRectangle(base2_x, base2_y, squareSize*8+2*border, squareSize*8+2*border);
		gc->PushState(); // save current translation/scale/other state
		{
			
			wxColour colBlackNoAlpha = colBlack;
			colBlackNoAlpha.Set(colBlackNoAlpha.Red(), colBlackNoAlpha.Green(), colBlackNoAlpha.Blue(), 255 );
			//gc->SetPen(wxPen("navy", 1));
			gc->SetBrush(wxBrush(colBlackNoAlpha));// 128 == half transparent
			//gc->Rotate(degToRad((double)rot));
			gc->DrawPath(borderPath);

		}
		gc->PopState();
	}
	else
	{
		gc->PushState();
		//gc->DrawBitmap(borderBmp, base2_x, base2_y, squareSize*8+2*border, squareSize*8+2*border);
		gc->DrawBitmap(borderBitmap[0], base2_x, base2_y, squareSize*8+2*border, border );
		gc->DrawBitmap(borderBitmap[1], base2_x, base2_y+border, border, squareSize*8 );
		gc->DrawBitmap(borderBitmap[2], base2_x, base2_y+squareSize*8 + border, squareSize*8+2*border, border );
		gc->DrawBitmap(borderBitmap[3], base2_x+squareSize*8 + border, base2_y+border, border, squareSize*8 );
		gc->PopState();
	}

	if( m_owner->optionsFrame->m_BoardOptions.showCoordinates == true)
	{
		if( this->flip_board )
		{
			//gc->SetPen(*wxWHITE);
			double textHeight = 7.5;
			double textWidth = 3.75;
			gc->DrawText("A", boardCoordTextCoords[0].xy[0], boardCoordTextCoords[0].xy[1] );//base2_y +squareSize*8 + border + textWidth);
			gc->DrawText("B", boardCoordTextCoords[1].xy[0], boardCoordTextCoords[1].xy[1] );
			gc->DrawText("C", boardCoordTextCoords[2].xy[0], boardCoordTextCoords[2].xy[1] );
			gc->DrawText("D", boardCoordTextCoords[3].xy[0], boardCoordTextCoords[3].xy[1] );
			gc->DrawText("E", boardCoordTextCoords[4].xy[0], boardCoordTextCoords[4].xy[1] );
			gc->DrawText("F", boardCoordTextCoords[5].xy[0], boardCoordTextCoords[5].xy[1] );
			gc->DrawText("G", boardCoordTextCoords[6].xy[0], boardCoordTextCoords[6].xy[1] );
			gc->DrawText("H", boardCoordTextCoords[7].xy[0], boardCoordTextCoords[7].xy[1] );


			gc->DrawText("8",boardCoordTextCoords[8].xy[0], boardCoordTextCoords[8].xy[1] );
			gc->DrawText("7", boardCoordTextCoords[9].xy[0], boardCoordTextCoords[9].xy[1] );
			gc->DrawText("6", boardCoordTextCoords[10].xy[0], boardCoordTextCoords[10].xy[1] );
			gc->DrawText("5", boardCoordTextCoords[11].xy[0], boardCoordTextCoords[11].xy[1] );
			gc->DrawText("4", boardCoordTextCoords[12].xy[0], boardCoordTextCoords[12].xy[1] );
			gc->DrawText("3", boardCoordTextCoords[13].xy[0], boardCoordTextCoords[13].xy[1] );
			gc->DrawText("2", boardCoordTextCoords[14].xy[0], boardCoordTextCoords[14].xy[1] );
			gc->DrawText("1", boardCoordTextCoords[15].xy[0], boardCoordTextCoords[15].xy[1] );

			//gc->SetPen(*wxBLACK);
		}
		else
		{
			gc->DrawText("H", boardCoordTextCoords[0].xy[0], boardCoordTextCoords[0].xy[1] );//base2_y +squareSize*8 + border + textWidth);
			gc->DrawText("G", boardCoordTextCoords[1].xy[0], boardCoordTextCoords[1].xy[1] );
			gc->DrawText("F", boardCoordTextCoords[2].xy[0], boardCoordTextCoords[2].xy[1] );
			gc->DrawText("E", boardCoordTextCoords[3].xy[0], boardCoordTextCoords[3].xy[1] );
			gc->DrawText("D", boardCoordTextCoords[4].xy[0], boardCoordTextCoords[4].xy[1] );
			gc->DrawText("C", boardCoordTextCoords[5].xy[0], boardCoordTextCoords[5].xy[1] );
			gc->DrawText("B", boardCoordTextCoords[6].xy[0], boardCoordTextCoords[6].xy[1] );
			gc->DrawText("A", boardCoordTextCoords[7].xy[0], boardCoordTextCoords[7].xy[1] );


			gc->DrawText("1",boardCoordTextCoords[8].xy[0], boardCoordTextCoords[8].xy[1] );
			gc->DrawText("2", boardCoordTextCoords[9].xy[0], boardCoordTextCoords[9].xy[1] );
			gc->DrawText("3", boardCoordTextCoords[10].xy[0], boardCoordTextCoords[10].xy[1] );
			gc->DrawText("4", boardCoordTextCoords[11].xy[0], boardCoordTextCoords[11].xy[1] );
			gc->DrawText("5", boardCoordTextCoords[12].xy[0], boardCoordTextCoords[12].xy[1] );
			gc->DrawText("6", boardCoordTextCoords[13].xy[0], boardCoordTextCoords[13].xy[1] );
			gc->DrawText("7", boardCoordTextCoords[14].xy[0], boardCoordTextCoords[14].xy[1] );
			gc->DrawText("8", boardCoordTextCoords[15].xy[0], boardCoordTextCoords[15].xy[1] );
		}
	}

	/*gc->PushState();
		wxPoint2DDouble points[4];
		points[0].m_x = base2_x + border ;// + squareSize*8;
		points[0].m_y = base2_y + border + squareSize*8;
		points[1].m_x = base2_x + border-1;// + squareSize*8;
		points[1].m_y = base2_y + border-1;// + squareSize*8;
		points[2].m_x = base2_x + border;// + squareSize*8;
		points[2].m_y = base2_y + border;// + squareSize*8;
		points[3].m_x = base2_x + border + squareSize*8+1;
		points[3].m_y = base2_y + border-1;

		wxGraphicsPath pth_borderLines = gc->CreatePath();
		pth_borderLines.MoveToPoint( base2_x + border-1, base2_y + border + squareSize*8+1);
		pth_borderLines.AddLineToPoint(points[1]);
		pth_borderLines.AddLineToPoint(points[3]);
		gc->SetPen(*wxBLACK);
		gc->StrokePath(pth_borderLines);
	gc->PopState();
	*/
}
void BoardCanvas::CreateSquares(wxGraphicsContext *gc, wxGraphicsPath *whiteSquares, wxGraphicsPath *darkSquares)
{
	
	int xx = 0;
	int yy=0;
	for( double x = base2_x+border; x < base2_x+border+squareSize*8; x+= squareSize )
	{
		yy=0;
		for( double y = base2_y+border; y < base2_y+ border+squareSize*8; y+=squareSize)
		{
			if( (xx<8) && (yy<8)) 
			{
				if( (xx+yy)%2!=1 )
				{
					if( m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == true )
					{
						//gc->DrawBitmap(whiteSquaresBmp, x,y,squareSize,squareSize);
						gc->DrawBitmap( defaultSquaresWhite[xx][7-yy],x,y,squareSize,squareSize);
						//defaultSquares
						//gc->DrawBitmap( defaultSquares[xx][yy],x,y,squareSize,squareSize);
						whiteSquares->AddRectangle(x, y, squareSize, squareSize);
					}
					else
					{
						whiteSquares->AddRectangle(x, y, squareSize, squareSize);
					}
				}
				else
				{
					if( m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == true )
					{
						//gc->DrawBitmap(blackSquaresBmp, x,y,squareSize,squareSize);
						gc->DrawBitmap( defaultSquares[xx][7-yy],x,y,squareSize,squareSize);
						darkSquares->AddRectangle(x, y, squareSize, squareSize);
					}
					else
					{
						darkSquares->AddRectangle(x, y, squareSize, squareSize);
					}
				}
			}
			yy++;
		}
		xx++;
	}

	if( m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == false )
	{
		// Draw the light Squares
		gc->PushState(); // save current translation/scale/other state
		{
			//gc->SetPen(wxPen("navy", 1));
			if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
			{
				gc->SetPen(*wxTRANSPARENT_PEN);
			}
			gc->SetBrush(wxBrush(colWhite));// 128 == half transparent
			gc->DrawPath(*whiteSquares);
		}
		gc->PopState();

		// draw the dark squares
		gc->PushState(); // save current translation/scale/other state
		{

			//gc->SetPen(wxPen("navy", 1));
			if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
			{
				gc->SetPen(*wxTRANSPARENT_PEN);
			}
			gc->SetBrush(wxBrush(colBlack));// 128 == half transparent
			gc->DrawPath(*darkSquares);

		}
		gc->PopState();
	}
	else
	{
		// Draw the light Squares
		gc->PushState(); // save current translation/scale/other state
		{

			//gc->SetPen(wxPen("navy", 1));
			if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
			{
				gc->SetPen(*wxTRANSPARENT_PEN);
			}
			gc->SetBrush(wxBrush(wxColour(255,255,255,128)));// 128 == half transparent
			gc->DrawPath(*whiteSquares);
		}
		gc->PopState();


		gc->PushState(); // save current translation/scale/other state
		{

			//gc->SetPen(wxPen("navy", 1));
			if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
			{
				gc->SetPen(*wxTRANSPARENT_PEN);
			}
			gc->SetBrush(wxBrush(wxColour(0,0,0,50)));// 128 == half transparent
			gc->DrawPath(*darkSquares);
		}
		gc->PopState();
	}
}



void BoardCanvas::DrawSquares(wxGraphicsContext *gc)
{
}
void BoardCanvas::DrawPieces(wxGraphicsContext *gc)
{
	gc->PushState();

	int i, j;
	i=(flip_board? 0 : 7);
 	for( double x = base2_x+border ; x < base2_x+border+ squareSize*8; x+= squareSize )
	{
		
		j=(flip_board? 7 : 0);
		for( double y = base2_y+border; y < base2_y+border+ squareSize*8; y+=squareSize)
		{
			//path.AddRectangle(x, y, squareSize, squareSize);
			int pcID = -1;
			if(chess_board->SquareHasPiece(i,j,&pcID ))
			{
				if( gameState == GameState::SELECTED_PIECE  )
				{
					if( ((7-j == selY)&& (i==selX)) )
					{
//#ifdef _WIN32

						//Refresh();
					}

					else
					{
						
						chess_board->svgTextures[pcID-1]->UseAlpha();
						gc->DrawBitmap(*chess_board->svgTextures[pcID-1], x, y-squareSize/32, squareSize, squareSize);
						
					}
				}
				else if (bAnimating)
				{
					if( !((j == selY)&& (i==selX)) )
					{
							chess_board->svgTextures[pcID-1]->UseAlpha();
							gc->DrawBitmap(*chess_board->svgTextures[pcID-1], x, y-squareSize/32, squareSize, squareSize);
					}
				}
				else
				{
					chess_board->svgTextures[pcID-1]->UseAlpha();
					gc->DrawBitmap(*chess_board->svgTextures[pcID-1], x, y-squareSize/32, squareSize, squareSize);
				}
			}
			j = (flip_board? j-1 : j+1);
		}
		
		i= (flip_board? i+1 : i-1);
	}

	gc->PopState();
}
void BoardCanvas::DrawMovers(wxGraphicsContext *gc)
{
}
void BoardCanvas::CreateSelectedSquarePath(wxGraphicsPath *path)
{
}

void BoardCanvas::OnPaint(wxPaintEvent &event)
{

	/*static bool first_time = true;
	if( first_time == true || clearFlag == true)
	{
		first_time = false;
		clearFlag = false;
		RenderToBackBuffer();
	}*/
	static bool animationFinishedLastFrame = false;
    wxPaintDC pdc(this);
	
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

//    m_owner->PrepareDC(dc);

  //  dc.SetBackgroundMode( m_owner->m_backgroundMode );
//    if ( m_owner->m_backgroundBrush.Ok() )
  //      dc.SetBackground( m_owner->m_backgroundBrush );
 //   if ( m_owner->m_colourForeground.Ok() )
 //       dc.SetTextForeground( m_owner->m_colourForeground );
  //  if ( m_owner->m_colourBackground.Ok() )
  //      dc.SetTextBackground( m_owner->m_colourBackground );

  /*  if ( m_owner->m_textureBackground) {
        if ( ! m_owner->m_backgroundBrush.Ok() ) {
            wxColour clr(0,128,0);
            wxBrush b(clr, wxSOLID);
            dc.SetBackground(b);
        }
    }*/

    if ( m_clip )
        dc.SetClippingRegion(100, 100, 100, 100);

	
//    dc.Clear();
	
/*    if ( m_owner->m_textureBackground )
    {
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        for ( int i = 0; i < 200; i++ )
            dc.DrawLine(0, i*10, i*10, 0);
    }*/
	//m_show = Show_Text;

#if wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();

    gc->SetFont(font,*wxWHITE);
	bool highlight_white = false;
	MyChessMove* pMov = NULL;
	gc->SetAntialiasMode(wxAntialiasMode::wxANTIALIAS_DEFAULT);
#ifdef WINDOWS_7
	if( bAnimating )
#endif
	{
		if( this->moveForward )
		{
			if( chess_board->mov == BLACK )
			{
				highlight_white = true;
				if( m_owner->currentGame != NULL )
				{
					pMov = &m_owner->currentGame->pCurrMove->whiteMove;
					if( ((m_owner->currentGame->pCurrMove->white == "O-O") || 
						(m_owner->currentGame->pCurrMove->white == "O-O-O")))// &&
						//droppedPieceLastMove)
					{
						clearFlag = true;
					}
				}
			}
			else
			{
				if( m_owner->currentGame != NULL )
				{
					pMov = &m_owner->currentGame->pCurrMove->blackMove;
					if( ((m_owner->currentGame->pCurrMove->black == "O-O") || 
						(m_owner->currentGame->pCurrMove->black == "O-O-O")))
						//&&
						//droppedPieceLastMove)
					{
						clearFlag = true;
					}
				}
			}
		}
		else
		{
			if( chess_board->mov == BLACK )
			{
				if( m_owner->currentGame != NULL )
				{
					pMov = &m_owner->currentGame->pCurrMove->blackMove;
					if( ((m_owner->currentGame->pCurrMove->black == "O-O") || 
						(m_owner->currentGame->pCurrMove->black == "O-O-O")))// &&
						//droppedPieceLastMove)
					{
						clearFlag = true;
					}
				}
			}
			else
			{
				if( m_owner->currentGame != NULL && m_owner->currentGame->pCurrMove->pNext)
				{
					pMov = &m_owner->currentGame->pCurrMove->pNext->whiteMove;
					if( ((m_owner->currentGame->pCurrMove->pNext->white == "O-O") || 
						(m_owner->currentGame->pCurrMove->pNext->white == "O-O-O")))// &&
						//droppedPieceLastMove)
					{
						clearFlag = true;
					}
				}
			}
		}
	}
#ifdef WINDOWS_7
	else

	{

		if( chess_board->mov == BLACK )
		{
			highlight_white = true;
			if( m_owner->currentGame != NULL )
				pMov = &m_owner->currentGame->pCurrMove->whiteMove;
		}
		else
		{
			if( m_owner->currentGame != NULL )
				pMov = &m_owner->currentGame->pCurrMove->blackMove;
		}
	}
#endif



	wxGraphicsPath  whiteSquares= gc->CreatePath() ;
	wxGraphicsPath  darkSquares= gc->CreatePath() ;
	
	wxGraphicsPath lastMovePath = gc->CreatePath() ;
	wxGraphicsPath selectedSquarePath = gc->CreatePath() ;
	wxGraphicsPath matedKingPath = gc->CreatePath();



	//whiteSquares.MoveToPoint(base2_x+border, base2_y+border);
	//whiteSquares.AddLineToPoint(base2_x+size-border, base2_y+size-border);
	
	// diagonal top right to bottom left
	//whiteSquares.MoveToPoint(base2_x+size-border, base2_y+border);
	//whiteSquares.AddLineToPoint(base2_x+border, base2_y+size-border);
	

	/*//top center to bottom center
	path.MoveToPoint(base2_x+250, base2_y);
	path.AddLineToPoint(base2_x+250, base2_y+500);


	
	
	// left middle to right middle
	path.MoveToPoint(BASE2, BASE2+250);
	path.AddLineToPoint(BASE2+500, BASE2+250);*/
	/*double selectedX;
	double selectedY;

	if( flip_board )
	{
		selectedX = base2_x+border +selX*squareSize;
		selectedY = base2_y+border+selY*squareSize;
	}
	else
	{
		// flip board counter
		selectedX = base2_x+border +(selX)*squareSize;
		selectedY = base2_y+border+(selY)*squareSize;
	}


	selectedSquarePath.AddRectangle(selectedX, selectedY,
									squareSize, squareSize);
									*/

	int rot = 0;

	if( pMov!= NULL )
	{
		double fromX; 
		double fromY;


		double toX;
		double toY;

		if( flip_board )
		{

			fromX = base2_x+border+squareSize*pMov->from.file;
			fromY = base2_y+border+squareSize*(7-pMov->from.rank);


			toX = base2_x+border+squareSize*pMov->to.file;
			toY = base2_y+border+squareSize*(7-pMov->to.rank);
		}
		else
		{
			fromX = base2_x+border+squareSize*(7-pMov->from.file);
			fromY = base2_y+border+squareSize*(pMov->from.rank);


			toX = base2_x+border+squareSize*(7-pMov->to.file);
			toY = base2_y+border+squareSize*(pMov->to.rank);
		}

		lastMovePath.AddRectangle(fromX, fromY, squareSize, squareSize);
		lastMovePath.AddRectangle(toX,toY , squareSize, squareSize);
		
		bool bAddArrows = false;		
	}	
#ifndef WINDOWS_7
	if( clearFlag )
#endif
	{
		dc.Clear();
		this->DrawBackground(gc);
		this->DrawBorder(gc);

		this->CreateSquares(gc, &whiteSquares, &darkSquares);
		this->DrawPieces(gc);
#ifndef WINDOWS_7
		clearFlag = false;
#endif
	}
#ifndef WINDOWS_7
	else
	{
		if( gameState == GameState::SELECTED_PIECE || (droppedPieceLastMove == true) || bAnimating || animationFinishedLastFrame)
		{
			droppedPieceLastMove = false;

			if( animationFinishedLastFrame )
				animationFinishedLastFrame = false;

			GameBoardTile* tiles[4];
			int c1x, c1y;
			int c2x, c2y;
			int c3x, c3y;
			int c4x, c4y;

			c1x = (int)((movingPieceLastMove.x-(base2_x+border))/squareSize);
			c1y = (int)((movingPieceLastMove.y-(base2_y+border))/squareSize);

			c2x = (int)((movingPieceLastMove.x+squareSize-(base2_x+border))/squareSize);
			c2y = (int)((movingPieceLastMove.y-(base2_y+border))/squareSize);

			c3x = (int)((movingPieceLastMove.x-(base2_x+border))/squareSize);
			c3y = (int)((movingPieceLastMove.y+squareSize-(base2_y+border))/squareSize);

			c4x = (int)((movingPieceLastMove.x+squareSize-(base2_x+border))/squareSize);
			c4y = (int)((movingPieceLastMove.y+squareSize-(base2_y+border))/squareSize);

			int spareTile = -1;

			if( c1x >= 0 && c1x < 8 && c1y >= 0 && c1y < 8 )
			{
				tiles[0] = &m_squareTiles[c1x][c1y];
			}
			else 
			{
				spareTile = 0;
				tiles[0]= NULL;
			}

			if( c2x >= 0 && c2x < 8 && c2y >= 0 && c2y < 8 )
			{
				tiles[1] = &m_squareTiles[c2x][c2y];
			}
			else
			{
				spareTile = 1;
					tiles[1]= NULL;
			}

			if( c3x >= 0 && c3x < 8 && c3y >= 0 && c3y < 8 )
			{
				tiles[2] = &m_squareTiles[c3x][c3y];
			}
			else
			{
				spareTile = 2;
				tiles[2]= NULL;
			}

			if( c4x >= 0 && c4x < 8 && c4y >= 0 && c4y < 8 )
			{
				tiles[3] = &m_squareTiles[c4x][c4y];
			}
			else
			{
				spareTile = 3;
				tiles[3]= NULL;
			}
			
			//if( spareTile != 0 && 

			if( (movingPieceLastMove.x  < base2_x) || 
				(movingPieceLastMove.x  > base2_x + squareSize*8 + 2*border) ||
				(movingPieceLastMove.y  < base2_y) ||
				(movingPieceLastMove.y  > base2_y + squareSize*8 + 2*border) )
			{
				this->DrawBackground(gc);
				this->DrawBorder(gc);
			}

			else if( (movingPieceLastMove.x  < base2_x + border) || 
				(movingPieceLastMove.x  > base2_x + squareSize*8 + border) ||
				(movingPieceLastMove.y  < base2_y + border) ||
				(movingPieceLastMove.y  > base2_y + squareSize*8 + border) )
			{
				this->DrawBorder(gc);
			}

			if( m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == false )
			{

				for( int jj = 0; jj < 4; jj++ )
				{
					if(tiles[jj] == NULL )
						continue;

					gc->SetBrush(wxBrush(tiles[jj]->GetColour()));
					if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
						gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawRectangle(tiles[jj]->x, tiles[jj]->y, squareSize, squareSize);

					gc->SetPen(*wxBLACK);
					//gc->SetBrush(wxBrush(tiles[jj]->GetWhiteOverlay()));
					//gc->SetPen(*wxTRANSPARENT_PEN);
					//gc->DrawRectangle(tiles[jj]->x, tiles[jj]->y, squareSize, squareSize);
					int i = tiles[jj]->GetIndexX();
					int j = tiles[jj]->GetIndexY();
					
					i=(flip_board? i : 7-i);
					j=(flip_board? 7-j : j);

					int pcID;
					if( chess_board->SquareHasPiece(i,j, &pcID))
					{
						if( ((7-j == selY)&& (i==selX)) )
						{
						}
						else
						{
							chess_board->svgTextures[pcID-1]->UseAlpha();
							gc->DrawBitmap(*chess_board->svgTextures[pcID-1], tiles[jj]->x, tiles[jj]->y-squareSize/32, squareSize, squareSize);
						}
					}
				}


			}
			else
			{
				for( int jj = 0; jj < 4; jj++ )
				{
					if(tiles[jj] == NULL )
						continue;

					gc->SetBrush(wxBrush(tiles[jj]->GetColour()));
					if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
						gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawBitmap(*tiles[jj]->GetBitmap(),tiles[jj]->x, tiles[jj]->y, squareSize, squareSize);

					gc->SetBrush(wxBrush(tiles[jj]->GetWhiteOverlay()));
					gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawRectangle(tiles[jj]->x, tiles[jj]->y, squareSize, squareSize);
					gc->SetPen(*wxBLACK);

					int i = tiles[jj]->GetIndexX();
					int j = tiles[jj]->GetIndexY();
					
					i=(flip_board? i : 7-i);
					j=(flip_board? 7-j : j);

					int pcID;
					if( chess_board->SquareHasPiece(i,j, &pcID))
					{
						if( ((7-j == selY)&& (i==selX)) )
						{
						}
						else
						{
							chess_board->svgTextures[pcID-1]->UseAlpha();
							gc->DrawBitmap(*chess_board->svgTextures[pcID-1], tiles[jj]->x, tiles[jj]->y-squareSize/32, squareSize, squareSize);
						}
					}
				}

			}
		}
		
		if( droppedPieceLastMove || pCurrMov != pMov || renderMoveNow || bAnimating )
		{

			renderMoveNow = false;


		//	if(  )//&& !this-> )
			{

				//this->DrawPieces(gc);
				int indxX1 = pMov->from.file;
				int indxY1 = pMov->from.rank;

				int indxX1pure = indxX1;
				int indxY1pure = indxY1;

				int indxX2 = pMov->to.file;
				int indxY2 = pMov->to.rank;	

				int indxX2pure = indxX2;
				int indxY2pure = indxY2;

				indxX1=(flip_board? indxX1 : 7-indxX1);
				indxY1=(flip_board? 7-indxY1 : indxY1);

				indxX2=(flip_board? indxX2 : 7-indxX2);
				indxY2=(flip_board? 7-indxY2 : indxY2);

				if( m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == false )
				{
					gc->SetBrush(wxBrush(m_squareTiles[indxX1][indxY1].GetColour()));
					if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
						gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawRectangle(m_squareTiles[indxX1][indxY1].x, 
										m_squareTiles[indxX1][indxY1].y,
										squareSize, squareSize);

					gc->SetPen(*wxBLACK);

					gc->SetBrush(wxBrush(m_squareTiles[indxX2][indxY2].GetColour()));
					if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
						gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawRectangle(m_squareTiles[indxX2][indxY2].x, 
										m_squareTiles[indxX2][indxY2].y,
										squareSize, squareSize);

					gc->SetPen(*wxBLACK);
				}
				else
				{
					if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
						gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawBitmap(*m_squareTiles[indxX1][indxY1].GetBitmap(),
						m_squareTiles[indxX1][indxY1].x, 
						m_squareTiles[indxX1][indxY1].y, 
						squareSize, squareSize);

					gc->SetBrush(wxBrush(m_squareTiles[indxX1][indxY1].GetWhiteOverlay()));
					gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawRectangle(m_squareTiles[indxX1][indxY1].x, 
						m_squareTiles[indxX1][indxY1].y, squareSize, squareSize);
					gc->SetPen(*wxBLACK);


					if( m_owner->optionsFrame->m_BoardOptions.showLines == false )
						gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawBitmap(*m_squareTiles[indxX2][indxY2].GetBitmap(),
						m_squareTiles[indxX2][indxY2].x, 
						m_squareTiles[indxX2][indxY2].y, 
						squareSize, squareSize);

					gc->SetBrush(wxBrush(m_squareTiles[indxX2][indxY2].GetWhiteOverlay()));
					gc->SetPen(*wxTRANSPARENT_PEN);
					gc->DrawRectangle(m_squareTiles[indxX2][indxY2].x, 
						m_squareTiles[indxX2][indxY2].y, squareSize, squareSize);
					gc->SetPen(*wxBLACK);
				}

				/*int pcID = -1;
				if( chess_board->SquareHasPiece(indxX1,indxY1, &pcID))
				{

					{
						chess_board->svgTextures[pcID-1]->UseAlpha();
						gc->DrawBitmap(*chess_board->svgTextures[pcID-1], 
							base2_x+border+squareSize*indxX1, base2_x+border+squareSize*indxY1, 
							squareSize, squareSize);
					}
				}

				pcID = -1;
				if( chess_board->SquareHasPiece(indxX2,indxY2, &pcID))
				{

					{
						chess_board->svgTextures[pcID-1]->UseAlpha();
						gc->DrawBitmap(*chess_board->svgTextures[pcID-1], 
							base2_x+border+squareSize*indxX2, base2_x+border+squareSize*indxY2, 
							squareSize, squareSize);
					}
				}*/
				
				double newX1 = base2_x+border+squareSize*indxX1;
				double newY1 = base2_y+border+squareSize*indxY1 - squareSize/32;

				double newX2 = base2_x+border+squareSize*indxX2;
				double newY2 = base2_y+border+squareSize*indxY2- squareSize/32;

				if( moveForward == false )
				{
					double tempX = newX1;
					newX1 = newX2;
					newX2 = tempX;

					double tempY = newY1;
					newY1 = newY2;
					newY2 = tempY;

					int tempIndxX = indxX1pure;
					indxX1pure = indxX2pure;
					indxX2pure = tempIndxX;

					int tempIndxY = indxY1pure;
					indxY1pure = indxY2pure;
					indxY2pure = tempIndxY;
				}

				int pcID = -1;
				if( chess_board->SquareHasPiece(indxX1pure,indxY1pure, &pcID) )
				{

					{
						chess_board->svgTextures[pcID-1]->UseAlpha();
						gc->DrawBitmap(*chess_board->svgTextures[pcID-1], 
							newX1, newY1, 
							squareSize, squareSize);
					}
				}

				pcID = -1;
				if( chess_board->SquareHasPiece(indxX2pure,indxY2pure, &pcID) )
				{

					{
						chess_board->svgTextures[pcID-1]->UseAlpha();
						gc->DrawBitmap(*chess_board->svgTextures[pcID-1], 
							newX2, newY2, 
							squareSize, squareSize);
					}
				}


				//this->CreateSquares(gc, &whiteSquares, &darkSquares);
				//this->DrawPieces(gc);
			}
		}
	}
#endif
	//gc->PushState();
	//gc->DrawBitmap(backBuffer, 0,0,sz.GetWidth(), sz.GetHeight());
	//gc->PopState();



	






		// draw the dark squares
	gc->PushState(); // save current translation/scale/other state
	{

		//gc->SetPen(wxPen("navy", 1));
	
		gc->SetBrush(wxBrush(wxColour(255,  150,  150, 100)));// 128 == half transparent
		gc->FillPath(selectedSquarePath);

	}
	gc->PopState();


	// draw the border



	int i = 0;//=0;
	int j;


	/*gc->PushState(); // save current translation/scale/other state
	if( pMov!= NULL && (m_owner->optionsFrame->m_BoardOptions.showLastMoveRect == true))
	{
		gc->SetBrush(wxBrush(wxColour(255,  0,  0 , 180)));// 128 == half transparent
		gc->SetPen(wxPen(lastMoveRectCol, 1));
		gc->StrokePath(lastMovePath);

	}
	gc->PopState();
	*/


	if( this->m_gameState == CHECKMATE && m_owner->optionsFrame->m_GameOptions.highlightsAtEnd == true )
	{
		MyChessPiece *PC;
		if( chess_board->mov == 1 )
		{
			PC = chess_board->blackPcs[4];
		}
		else
		{
			PC = chess_board->whitePcs[4];
		}
		double kingX = base2_x+border+squareSize*PC->x;
		double kingY = base2_y+border+squareSize*(7-PC->y);

		wxGraphicsPath kingPath = gc->CreatePath();
		kingPath.AddRectangle(kingX, kingY, squareSize, squareSize);

		gc->PushState();
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(wxColour(255,  0,  0, 128)));
			gc->DrawPath(kingPath);
		gc->PopState();

	}

	//droppedPieceLastMove
	if( animationFinishedLastFrame || droppedPieceLastMove )
	{
		needsRefresh = false;
	}
	

	if( m_owner->optionsFrame->m_BoardOptions.fasterMouse == true )
	{
			POINT p;
		::GetCursorPos(&p);
		::ScreenToClient((HWND)this->GetHWND(), &p);
		MouseX = p.x;
		MouseY = p.y;
	}

	if( gameState == GameState::SELECTED_PIECE || this->bAnimating )
	{		
		needsRefresh = true;
		//	POINT p;
		//::GetCursorPos(&p);
		//::ScreenToClient((HWND)this->GetHWND(), &p);
		//MouseX = p.x;
		//MouseY = p.y;
//#endif			
		if( this->bAnimating && selPieceID != -1 )
		{
			double fromX; //= base2_x+border+squareSize*pMov->from.file;
			double fromY; //= base2_y+border+squareSize*(7-pMov->from.rank);


			double toX; //= base2_x+border+squareSize*pMov->to.file;
			double toY ;//= base2_y+border+squareSize*(7-pMov->to.rank);

			

			if( flip_board )
			{
				if( bFowardsAnim )
				{
					fromX = base2_x+border+squareSize*pMov->from.file;
					fromY = base2_y+border+squareSize*(7-pMov->from.rank);

					toX = base2_x+border+squareSize*pMov->to.file;
					toY = base2_y+border+squareSize*(7-pMov->to.rank);
					if( capturePiece != 0 )
					{
						chess_board->svgTextures[selPieceID-1]->UseAlpha();
						gc->DrawBitmap(*chess_board->svgTextures[capturePiece-1], toX, toY-squareSize/32, squareSize, squareSize);
						//capturePiece = -1;
					}
				}
				else
				{

					fromX = base2_x+border+squareSize*pMov->to.file;
					fromY = base2_y+border+squareSize*(7-pMov->to.rank);

					toX = base2_x+border+squareSize*pMov->from.file;
					toY = base2_y+border+squareSize*(7-pMov->from.rank);
				}
			}
			else
			{
				if( bFowardsAnim )
				{
					fromX = base2_x+border+squareSize*(7-pMov->from.file);
					fromY = base2_y+border+squareSize*(pMov->from.rank);

					toX = base2_x+border+squareSize*(7-pMov->to.file);
					toY = base2_y+border+squareSize*(pMov->to.rank);
					if( capturePiece != 0 )
					{
						chess_board->svgTextures[selPieceID-1]->UseAlpha();
						gc->DrawBitmap(*chess_board->svgTextures[capturePiece-1], toX, toY-squareSize/32, squareSize, squareSize);
						//capturePiece = -1;
					}
				}
				else
				{

					fromX = base2_x+border+squareSize*(7-pMov->to.file);
					fromY = base2_y+border+squareSize*(pMov->to.rank);

					toX = base2_x+border+squareSize*(7-pMov->from.file);
					toY = base2_y+border+squareSize*(pMov->from.rank);
				}
			}



			double DirX = (toX - fromX)/sqrtf( (toX-fromX)*(toX-fromX) + (toY - fromY)*(toY - fromY));
			double DirY = (toY - fromY)/sqrtf( (toX-fromX)*(toX-fromX) + (toY - fromY)*(toY - fromY));
			if( toX == fromX ) DirX = 0;
			if( toY == fromY ) DirY = 0;
			double DistanceX = abs(toX - fromX);
			double DistanceY = abs(toY - fromY);

			double ticksPerSecond = 1000/animationDelta.ToDouble();
			double fps = 1000/timeDelta.ToDouble();
			double ticksPerFrame = 1/double(animationSpeed);//0.3;//ticksPerSecond / (4*fps);

			double totalDist = sqrtf(DistanceX * DistanceX + DistanceY * DistanceY);
			double tickDist = ticksPerFrame *  totalDist;

			

			double PosX = currAnimX + tickDist*DirX;
			double PosY = currAnimY + tickDist*DirY;

			currAnimX=PosX;
			currAnimY=PosY;
			movingPieceLastMove.x =currAnimX;//MouseX-squareSize/2;
			movingPieceLastMove.y =currAnimY;//MouseY-squareSize/2;
			
			chess_board->svgTextures[selPieceID-1]->UseAlpha();
			gc->DrawBitmap(*chess_board->svgTextures[selPieceID-1], PosX, PosY-squareSize/32, squareSize, squareSize);

			//Refresh(false);
			double distToTarget = sqrtf( (PosX-toX)*(PosX-toX) + (PosY-toY)*(PosY-toY));
			double distFromSource = sqrtf( (PosX-fromX)*(PosX-fromX) + (PosY-fromY)*(PosY-fromY));
			if( (distToTarget <= tickDist) || (distFromSource > totalDist))
			{
				bAnimating = false;
				selX =selY=-1;
				selPieceID=-1;
				capturePiece = 0;
				animationFinishedLastFrame = true;

				if( !(m_owner->m_gameTraverseTimer) || 
					(m_owner->m_gameTraverseTimer && !m_owner->m_gameTraverseTimer->IsRunning()) )
				{
					if( startEnginesOnKeyUp )
					{
						m_owner->StartEnginesAfterMoveChange();
						startEnginesOnKeyUp = false;
					}
				}
				else
				{
					Refresh(false);
				}
				//clearFlag = true;
				//Refresh(false);
			}
			else
			{
				if( !(m_owner->m_gameTraverseTimer) || 
					(m_owner->m_gameTraverseTimer && !m_owner->m_gameTraverseTimer->IsRunning()) )
				{
					Refresh(false);
				}
			}
		}
		else
		{
			

			chess_board->svgTextures[selPieceID-1]->UseAlpha();
			gc->DrawBitmap(*chess_board->svgTextures[selPieceID-1], MouseX-squareSize/2, MouseY-squareSize/2, squareSize, squareSize);
			movingPieceLastMove.x =MouseX-squareSize/2;
			movingPieceLastMove.y =MouseY-squareSize/2;
		}
	//	Refresh();
	}					


    //gc->PopState(); // restore saved state




	if( m_owner->optionsFrame->m_BoardOptions.showMouseCircle == true )
	{
		gc->PushState();
			gc->SetBrush(wxBrush(wxColour(0,  255,  255 , 90)));// 128 == half transparent
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->DrawEllipse(MouseX-squareSize/2, MouseY-squareSize/2, squareSize, squareSize);
		gc->PopState();
	}
#endif  

//	gc->SetFont(font, *wxBLACK);
//	wxGraphicsBrush br;
	
//	gc->DrawText("QRKqrk", base2_x + squareSize*4/2, base2_y + squareSize*8.5 + border, 0);

	/*if( m_displayGameResult )
	{
		double textWidth;
		double textHeight;
		gc->GetTextExtent(gameResultString,&textWidth, &textHeight);
		gc->DrawText(gameResultString, base2_x + squareSize*4/2 , base2_y + squareSize*8.5 + border,0);
//		gameResultString = "Drawn, 50 moves without a pawn move or capture!";
						//m_displayGameResult = true;
	}*/
	//event.RequestMore() ;

	pCurrMov = pMov;
}


void BoardCanvas::RenderToBackBuffer()
{
	wxMemoryDC pdc(this->backBuffer);
	
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

	dc.Clear();

#if wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();
//	DrawGraphics();

   // wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    gc->SetFont(font,*wxWHITE);
	bool highlight_white = false;
	MyChessMove* pMov = NULL;

    wxGraphicsPath whiteSquares = gc->CreatePath() ;
	wxGraphicsPath darkSquares = gc->CreatePath() ;
	
	wxGraphicsPath lastMovePath = gc->CreatePath() ;
	wxGraphicsPath selectedSquarePath = gc->CreatePath() ;
	wxGraphicsPath matedKingPath = gc->CreatePath();



	//whiteSquares.MoveToPoint(base2_x+border, base2_y+border);
	//whiteSquares.AddLineToPoint(base2_x+size-border, base2_y+size-border);
	
	// diagonal top right to bottom left
	//whiteSquares.MoveToPoint(base2_x+size-border, base2_y+border);
	//whiteSquares.AddLineToPoint(base2_x+border, base2_y+size-border);
	

	/*//top center to bottom center
	path.MoveToPoint(base2_x+250, base2_y);
	path.AddLineToPoint(base2_x+250, base2_y+500);


	
	
	// left middle to right middle
	path.MoveToPoint(BASE2, BASE2+250);
	path.AddLineToPoint(BASE2+500, BASE2+250);*/
	/*double selectedX;
	double selectedY;

	if( flip_board )
	{
		selectedX = base2_x+border +selX*squareSize;
		selectedY = base2_y+border+selY*squareSize;
	}
	else
	{
		// flip board counter
		selectedX = base2_x+border +(selX)*squareSize;
		selectedY = base2_y+border+(selY)*squareSize;
	}


	selectedSquarePath.AddRectangle(selectedX, selectedY,
									squareSize, squareSize);
									*/

	this->DrawBackground(gc);

	DrawBorder(gc);


	CreateSquares(gc, &whiteSquares, &darkSquares);

	DrawPieces(gc);
#endif
}



void BoardCanvas::OnIdle(wxIdleEvent &event)
{
//	wxPaintEvent evt;

	/*if( bAnimating )
	{
		wxLongLong 	currTime = wxGetLocalTimeMillis ();
		if( currTime - lastTime > timeDelta )
		{
			lastTime = currTime;
			Refresh();
			
		}
		if( currTime - animationStart > this->animationDelta )
		{
			bAnimating = false;
		}
		else event.RequestMore(true);
	}*/
	
//	OnPaint(evt);
	//event.RequestMore(false);
}

/*void BoardCanvas::OnMouse(wxMouseEvent& event)
{
	 wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);
wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );


//	PickSquare(x,y, base2_x+border, base2_x+border+squareSize*8,
//					base2_y+border, base2_y+border+squareSize*8,
//					&selX,&selY);
	PickSquare(pos.x,pos.y, base2_x+border, base2_x+border+squareSize*8,
					base2_y+border, base2_y+border+squareSize*8,
					&selX,&selY);

	Refresh();
}*/

void BoardCanvas::mouseDown(wxMouseEvent& event)
{
	 wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);
	wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );




	PickSquare(x,y, base2_x+border, base2_x+border+squareSize*8,
					base2_y+border, base2_y+border+squareSize*8,
					&selX,&selY);

	if ((whoseMove == WAIT_COMPUTER)&&(this->mode == 1))
	{
		return;
	}
	
	if( gameState == GameState::STATE_ZERO )
	{
		if( chess_board->SquareHasPiece(selX,7-selY, &selPieceID) )
		{
			if( chess_board->IsThisMyMove(chess_board->PieceColor( selX,7-selY )))
			{
				theMove.from.file=selX;
				theMove.from.rank=7-selY;
				gameState = GameState::SELECTED_PIECE;
				needsRefresh = true;
			}
			else
			{
				// Not My Move
			}
		}
	}


	//Refresh(false);
}


void BoardCanvas::CalculateSizeParameters()
{
	size = (double)(int)(sz.GetHeight() < sz.GetWidth() ? sz.GetHeight()*0.95 : sz.GetWidth()*0.95);
	BASE2 = (double)(int)(sz.GetHeight()  < sz.GetWidth()? sz.GetHeight()*0.075 : sz.GetWidth()*0.075);
	base2_x = (double)(int)(sz.GetWidth()/2 - size/2);
	base2_y = (double)(int)(sz.GetHeight()/2 - size/2);
	chess_board->SetBoardDisplaySize(size);
 
	border = (double)(int)(size*20/500);//17.75;
	squareSize = (double)(int)((size-size*36.0f/500)/8);

	if( (sz.GetHeight() <= 300) || (sz.GetWidth() <= 300) )
		font = wxFont(5, wxFONTFAMILY_DEFAULT , wxNORMAL, wxBOLD, false);
	else if( (sz.GetHeight() <= 400) || (sz.GetWidth() <= 400) )
		font = wxFont(7, wxFONTFAMILY_DEFAULT , wxNORMAL, wxBOLD, false);
	else
		font = wxFont(9, wxFONTFAMILY_DEFAULT , wxNORMAL, wxBOLD, false);

	double textHeight = 7.5;
	double textWidth = 3.75;
	boardCoordTextCoords[0].xy[0] = base2_x + border +squareSize/2-textWidth; boardCoordTextCoords[0].xy[1] = base2_y +squareSize*8 + border + textWidth;
	boardCoordTextCoords[1].xy[0]= base2_x + border + squareSize +squareSize/2-textWidth; boardCoordTextCoords[1].xy[1] =  base2_y + squareSize*8+ border+ textWidth;
	boardCoordTextCoords[2].xy[0]=base2_x + border + 2*squareSize+squareSize/2-textWidth;boardCoordTextCoords[2].xy[1]= base2_y +squareSize*8+ border+ textWidth;
	boardCoordTextCoords[3].xy[0]=base2_x + border + 3*squareSize+squareSize/2-textWidth;boardCoordTextCoords[3].xy[1] = base2_y +squareSize*8+ border+ textWidth;
	boardCoordTextCoords[4].xy[0] = base2_x + border + 4*squareSize+squareSize/2-textWidth; boardCoordTextCoords[4].xy[1]= base2_y +squareSize*8+ border+ textWidth;
	boardCoordTextCoords[5].xy[0] = base2_x + border + 5*squareSize+squareSize/2-textWidth;boardCoordTextCoords[5].xy[1]= base2_y +squareSize*8+ border+ textWidth;
	boardCoordTextCoords[6].xy[0] = base2_x + border + 6*squareSize+squareSize/2-textWidth; boardCoordTextCoords[6].xy[1] =  base2_y +squareSize*8+ border+ textWidth;
	boardCoordTextCoords[7].xy[0] = base2_x + border + 7*squareSize+squareSize/2-textWidth; boardCoordTextCoords[7].xy[1] =  base2_y +squareSize*8+ border+ textWidth;


	boardCoordTextCoords[8].xy[0] = base2_x +border/2-textWidth; boardCoordTextCoords[8].xy[1]= base2_y + border+squareSize/2-textHeight;
	boardCoordTextCoords[9].xy[0] = base2_x +border/2-textWidth; boardCoordTextCoords[9].xy[1]=  base2_y + border+ squareSize+squareSize/2-textHeight;
	boardCoordTextCoords[10].xy[0] = base2_x +border/2-textWidth; boardCoordTextCoords[10].xy[1]= base2_y + border+ 2*squareSize+squareSize/2-textHeight;
	boardCoordTextCoords[11].xy[0] = base2_x +border/2-textWidth; boardCoordTextCoords[11].xy[1]=  base2_y + border+ 3*squareSize+squareSize/2-textHeight;
	boardCoordTextCoords[12].xy[0]= base2_x +border/2-textWidth; boardCoordTextCoords[12].xy[1]= base2_y + border+ 4*squareSize+squareSize/2-textHeight;
	boardCoordTextCoords[13].xy[0] = base2_x +border/2-textWidth; boardCoordTextCoords[13].xy[1]= base2_y + border+ 5*squareSize+squareSize/2-textHeight;
	boardCoordTextCoords[14].xy[0] = base2_x +border/2-textWidth; boardCoordTextCoords[14].xy[1]=  base2_y + border+ 6*squareSize+squareSize/2-textHeight;
	boardCoordTextCoords[15].xy[0] = base2_x +border/2-textWidth;boardCoordTextCoords[15].xy[1] = base2_y + border+ 7*squareSize+squareSize/2-textHeight;

	for( int i = 0; i < 4; i++ )
	{
		borderBitmap[i].FreeResource(true);
	}
	

	double rectBorder = bmpBorder.GetSize().GetHeight()*20/500;
	double rectSquareSize = (double)(int)((bmpBorder.GetSize().GetHeight()-rectBorder*2))/8;
	wxRect rect1 = wxRect( 0, 0, rectSquareSize*8+2*rectBorder, rectBorder );

	wxRect rect2 = wxRect( 0,rectBorder , rectBorder, rectSquareSize*8 );
	wxRect rect3 = wxRect(  0, rectSquareSize*8 + rectBorder, rectSquareSize*8+2*rectBorder, rectBorder );
	wxRect rect4 = wxRect(  rectSquareSize*8 + rectBorder, rectBorder, rectBorder, rectSquareSize*8 );

	//wxRect rct1(
	borderBitmap[0]= bmpBorder.GetSubBitmap(rect1);
	borderBitmap[1]= bmpBorder.GetSubBitmap(rect2);
	borderBitmap[2] = bmpBorder.GetSubBitmap(rect3);//.LoadFile("images/Squares/PanelBottom.png",wxBITMAP_TYPE_PNG);;
	borderBitmap[3] = bmpBorder.GetSubBitmap(rect4);//.LoadFile("images/Squares/PanelRight.png",wxBITMAP_TYPE_PNG);;

	SetTileCoords();
}


void BoardCanvas::SetTileCoords()
{
	int xx = 0;
	int yy=0;
	for( double x = base2_x+border; x < base2_x+border+squareSize*8; x+= squareSize )
	{
		yy=0;
		for( double y = base2_y+border; y < base2_y+ border+squareSize*8; y+=squareSize)
		{
			if( (xx<8) && (yy<8)) 
			{
				m_squareTiles[xx][yy].SetCoords(x,y);
				m_squareTiles[xx][yy].SetSize(squareSize);
				m_squareTiles[xx][yy].SetIndexX(xx);
				m_squareTiles[xx][yy].SetIndexY(yy);
				if( (xx+yy)%2!=1 )
				{
					m_squareTiles[xx][yy].SetColour(colWhite);
					m_squareTiles[xx][yy].SetOverlay(wxColour(255,255,255,128));
				}
				else
				{
					m_squareTiles[xx][yy].SetColour(colBlack);
					m_squareTiles[xx][yy].SetOverlay(wxColour(0,0,0,50));
				}
			}
			yy++;
		}
		xx++;
	}
}

void BoardCanvas::OnSize(wxSizeEvent& event)
{
	bResize = true;
	sz = GetClientSize();
	CalculateSizeParameters();
	//backBuffer.FreeResource(true);
	//backBuffer.Create(sz);
	clearFlag = true;
	//Update();
	Refresh(true);
}

void BoardCanvas::OnMouseMove(wxMouseEvent &event)
{
/*#if wxUSE_STATUSBAR
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);

    wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );
    wxString str;
    str.Printf( wxT("Current mouse position: %d,%d"), (int)x, (int)y );
    m_owner->SetStatusText( str );
#else
    wxUnusedVar(event);
#endif // wxUSE_STATUSBAR*/
	wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);
wxPoint pos = event.GetPosition();
    MouseX = pos.x;//dc.DeviceToLogicalX( pos.x );
    MouseY = pos.y;//dc.DeviceToLogicalY( pos.y );

	//event.reque
	//event.
	//event.(false);
	if( MouseOverWin && this->gameState == GameState::SELECTED_PIECE)
	{
		wxLongLong 	currTime = wxGetLocalTimeMillis ();
		if( currTime - lastTime > timeDelta )
		{
			lastTime = currTime;
			//Refresh(false);
		}
	}

}

void BoardCanvas::mouseReleased(wxMouseEvent& event)
{
	 wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);
	wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );


//	PickSquare((double)pos.x,(double)pos.y, base2_x+border, base2_x+border+squareSize*8,
//					base2_y+border, base2_y+border+squareSize*8,
//					&selX,&selY);

	PickSquare(x,y, base2_x+border, base2_x+border+squareSize*8,
					base2_y+border, base2_y+border+squareSize*8,
					&selX,&selY);

	if( gameState == GameState::SELECTED_PIECE  )
	{
		// check square is a valid move
		theMove.to.file=selX;
		theMove.to.rank=7-selY;
		bool promote = false;
		bool pinned = false;
		bool moveFound = false;

		// more feedback
		if( chess_board->IsMoveValid(&theMove, &promote, &pinned) )
		{
			DoMove(promote);
			

		}
		else if( this->m_owner->optionsFrame->m_GameOptions.useSnaps == true ) 
		{
			
			long coordsX[4];
			long coordsY[4];

			coordsX[0] = x-squareSize/2;
			coordsY[0] = y-squareSize/2;

			coordsX[1] = x+squareSize/2;
			coordsY[1] = y-squareSize/2;

			coordsX[2] = x-squareSize/2;
			coordsY[2] = y+squareSize/2;

			coordsX[3] = x+squareSize/2;
			coordsY[3] = y+squareSize/2;
			// for each corner position
			for( int i = 0; i < 4; i++ )
			{
				
				PickSquare(coordsX[i],coordsY[i], base2_x+border, base2_x+border+squareSize*8,
					base2_y+border, base2_y+border+squareSize*8,
					&selX,&selY);

				theMove.to.file=selX;
				theMove.to.rank=7-selY;
				promote = false;
				pinned = false;
					
				if( chess_board->IsMoveValid(&theMove, &promote, &pinned) )
				{
					DoMove(promote);
				}



	
			}
		}
		//if( !moveFound )
		{
			gameState = GameState::STATE_ZERO;
		}
		if( pinned )
		{
			gameState = GameState::STATE_ZERO;
		}

	}
	else if( gameState == GameState::STATE_ZERO )
	{
		if( chess_board->SquareHasPiece(selX,7-selY, &selPieceID) )
		{
			if( chess_board->IsThisMyMove(chess_board->PieceColor( selX,7-selY )))
			{
				theMove.from.file=selX;
				theMove.from.rank=7-selY;
				gameState = GameState::SELECTED_PIECE;
				//clearFlag = true;
				needsRefresh = true;
			}
			else
			{
				// Not My Move
			}
		}
	}

	//clearFlag = true;
	//Refresh(false);
}

void BoardCanvas::DoMove(bool &promote)
{
			//moveFound = true;
			for( int j = 0; j < activeEngines.size(); j++ )
			{
				if( activeEngines[j]->IsActive() &&
					activeEngines[j]->IsAnalysing() && mode == 0)
				{
					activeEngines[j]->KibitzStartsMove();
			//		activeEngines[j]->KibitzUserEndsMove();
				}
			}

			string epn="    ";
			char buff[10];
			
			switch( theMove.from.file )
			{
			case 0: epn[0] = 'a'; break;
			case 1:epn[0] = 'b'; break;
			case 2: epn[0] = 'c'; break;
			case 3: epn[0] = 'd'; break;
			case 4: epn[0] = 'e'; break;
			case 5: epn[0] = 'f'; break;
			case 6: epn[0] = 'g'; break;
			case 7: epn[0] = 'h'; break;
			
			}
			itoa(theMove.from.rank, buff, 10);
			epn[1] = buff[0];
			switch( theMove.to.file )
			{
			case 0: epn[2] = 'a'; break;
			case 1:epn[2] = 'b'; break;
			case 2: epn[2] = 'c'; break;
			case 3: epn[2] = 'd'; break;
			case 4: epn[2] = 'e'; break;
			case 5: epn[2] = 'f'; break;
			case 6: epn[2] = 'g'; break;
			case 7: epn[2] = 'h'; break;
			
			}
			itoa(theMove.to.rank, buff, 10);
			epn[3] = buff[0];
			int white_black = 0;
			if( chess_board->mov == 0 || chess_board->mov == 2 )
				white_black = 0;
			else white_black = 1;

			int promotePieceID = 0;
			if( promote && (m_owner->optionsFrame->m_GameOptions.alwaysPromoteToQueen == false))
			{
				PromotionSelectionDialaog dialog(this->m_parent, chess_board,wxID_ANY,
					wxT("Select Piece to Promote to"));

				dialog.SetPromoteID( &promotePieceID );
				dialog.ShowModal();

				//switch( promotePieceID )
				//epn+=
			}

			
			
			string pgn = chess_board->EPN_Move_to_PGN_Move(epn, promote, promotePieceID);
			
			string fen = chess_board->Fen();
			PGN::MoveInserter mv(pgn,white_black, fen, chess_board->GetLastMove());

			if( white_black == WHITE )
			{
				m_owner->gameClock->SetWhiteBlack(BLACK);
			}
			else
			{
				m_owner->gameClock->SetWhiteBlack(WHITE);
			}

			int repetitions = 0;
			if( this->m_owner->currentGame->pCurrMove == this->m_owner->currentGame->pBase )
				this->m_owner->currentGame->pCurrMove = this->m_owner->currentGame->pRoot->InsertMove( &mv, &repetitions);
			else
				this->m_owner->currentGame->pCurrMove = this->m_owner->currentGame->GetCurrentMove()->InsertMove( &mv, &repetitions);
			


			int gameStatus = chess_board->GetGameStatus();

			switch( gameStatus )
			{
			case PLAYING:
			case KINGSAFE:
				break;
			case CHECKMATE:
				{
					m_gameState = CHECKMATE;

					if( chess_board->mov == 0 || chess_board->mov == 2 )
					{
						m_displayGameResult = true;
						gameResultString = "Checkmate";
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "Black "+gameResultString+"s!","Game Over");
						}
						m_owner->currentGame->result = "0-1";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					else
					{
						gameResultString = "Checkmate";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "White "+gameResultString+"s!","Game Over");
						}
						m_owner->currentGame->result = "1-0";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					m_owner->currentGame->WriteResultIntoHeader();
					m_owner->gameClock->Stop();
					m_owner->RecreateGameList();
				}
				break;
			case STALEMATE:
				{
					m_gameState = STALEMATE;

					if( chess_board->mov == 0 || chess_board->mov == 2 )
					{
						gameResultString = "Stalemate";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "White "+gameResultString+"d!","Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					else
					{
						gameResultString = "Stalemate";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "Black" + gameResultString +"d!","Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					m_owner->currentGame->WriteResultIntoHeader();
					m_owner->RecreateGameList();
					m_owner->gameClock->Stop();
					
				}
				break;
			case DRAW_BY_50_MOVE:
				{
					m_gameState = DRAW_BY_50_MOVE;
					if( chess_board->mov == 0 || chess_board->mov == 2 )
					{
						gameResultString = "Drawn, 50 moves without a pawn move or capture!";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( gameResultString,"Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteResultIntoHeader();
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					else
					{
						gameResultString = "Drawn, 50 moves without a pawn move or capture!";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( gameResultString,"Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteResultIntoHeader();
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					m_owner->gameClock->Stop();
					m_owner->RecreateGameList();
				}
				break;
			case CHECK:
				{
					m_gameState = CHECK;

				}
				break;
			}	

			if( repetitions >= 3 )
			{
				gameResultString = "Draw By Repetition";
				wxMessageBox("Draw By Repetition","");
				m_gameState = DRAW_BY_REPETITION;
				m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
				m_owner->gameClock->Stop();
			}

			chess_board->SetBoard(fen);
			//m_owner->currentGame->EnumerateVariations();
			this->m_owner->my_canvas->move_count++;
			if( m_owner->my_canvas->white_black_move == WHITE )
				m_owner->my_canvas->white_black_move = BLACK;
			else m_owner->my_canvas->white_black_move = WHITE;
			
			if( this->m_owner->my_canvas->move_count < 10 )
				this->m_owner->my_canvas->Scroll(0,0);
			this->m_owner->my_canvas->ScrollToCursor();
			this->m_owner->my_canvas->Refresh(true);
			this->m_owner->my_canvas->needsRefresh = true;
			this->m_owner->moveDataCanvas->Refresh(true);
			this->m_owner->noteFrame->Refresh(true);
			//Refresh(false);
			needsRefresh = true;
			droppedPieceLastMove = true;
			
			

			if( (mode == 1) && (m_owner->engineVsHumanMatch) )
			{
				whoseMove = WAIT_COMPUTER;
				if( (m_owner->playersOpponent) != 0 && (m_owner->playersOpponent->IsActive()) )
				{

						m_owner->playersOpponent->PlayerMoves();
					
				}

				if( m_owner->gameClock->IsPaused() )
				{
					m_owner->gameClock->StartGame();
					m_owner->gameClock->UnPause();
				}
			}
			else
			{

				for( int j = 0; j < activeEngines.size(); j++ )
				{
					if( activeEngines[j]->IsActive() && 
						activeEngines[j]->IsAnalysing() && mode == 0)
					{
				//		activeEngines[j]->KibitzStartsMove();
						activeEngines[j]->KibitzUserEndsMove();
					}
				}
			}
#ifdef USE_CANVAS_SET_FOCUS
		//	this->m_owner->my_canvas->SetFocus();
#endif
}


void BoardCanvas::LogEvent(const wxString& name, wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	
	switch ( keycode )
	{
		case WXK_BACK:break;
        case WXK_TAB:break;
        case WXK_RETURN:
			{
			}
			break;
        case WXK_ESCAPE:break;
        case WXK_SPACE:break;
        case WXK_DELETE:break;
        case WXK_START:break;
        case WXK_LBUTTON:break;
        case WXK_RBUTTON:break;
        case WXK_CANCEL:break;
        case WXK_MBUTTON:break;
        case WXK_CLEAR:break;
        case WXK_SHIFT:break;
        case WXK_ALT:break;
        case WXK_CONTROL:break;
        case WXK_MENU:break;
        case WXK_PAUSE:break;
        case WXK_CAPITAL:break;
        case WXK_END:break;
        case WXK_HOME:break;
        case WXK_LEFT:
			{
				m_owner->my_canvas->LogEvent(name, event);
			}break;
        case WXK_UP:break;
        case WXK_RIGHT:
			{
				m_owner->my_canvas->LogEvent(name, event);
				break;
			}
        case WXK_DOWN:break;
        case WXK_SELECT:break;
        case WXK_PRINT:break;
        case WXK_EXECUTE:break;
        case WXK_SNAPSHOT:break;
        case WXK_INSERT:break;
        case WXK_HELP:break;
        case WXK_NUMPAD0:break;
        case WXK_NUMPAD1:break;
        case WXK_NUMPAD2:break;
        case WXK_NUMPAD3:break;
        case WXK_NUMPAD4:break;
        case WXK_NUMPAD5:break;
        case WXK_NUMPAD6:break;
        case WXK_NUMPAD7:break;
        case WXK_NUMPAD8:break;
        case WXK_NUMPAD9:break;
        case WXK_MULTIPLY:break;
        case WXK_ADD:break;
        case WXK_SEPARATOR:break;
        case WXK_SUBTRACT:break;
        case WXK_DECIMAL:break;
        case WXK_DIVIDE:break;
        case WXK_F1:break;
        case WXK_F2:break;
        case WXK_F3:break;
        case WXK_F4:break;
        case WXK_F5:break;
        case WXK_F6:break;
        case WXK_F7:break;
        case WXK_F8:break;
        case WXK_F9:break;
        case WXK_F10:break;
        case WXK_F11:break;
        case WXK_F12:break;
        case WXK_F13:break;
        case WXK_F14:break;
        case WXK_F15:break;
        case WXK_F16:break;
        case WXK_F17:break;
        case WXK_F18:break;
        case WXK_F19:break;
        case WXK_F20:break;
        case WXK_F21:break;
        case WXK_F22:break;
        case WXK_F23:break;
        case WXK_F24:break;
        case WXK_NUMLOCK:break;
        case WXK_SCROLL:break;
        case WXK_PAGEUP:break;
        case WXK_PAGEDOWN:break;
        case WXK_NUMPAD_SPACE:break;
        case WXK_NUMPAD_TAB:break;
        case WXK_NUMPAD_ENTER:break;
        case WXK_NUMPAD_F1:break;
        case WXK_NUMPAD_F2:break;
        case WXK_NUMPAD_F3:break;
        case WXK_NUMPAD_F4:break;
        case WXK_NUMPAD_HOME:break;
        case WXK_NUMPAD_LEFT:break;
        case WXK_NUMPAD_UP:break;
        case WXK_NUMPAD_RIGHT:break;
        case WXK_NUMPAD_DOWN:break;
        case WXK_NUMPAD_PAGEUP:break;
        case WXK_NUMPAD_PAGEDOWN:break;
        case WXK_NUMPAD_END:break;
        case WXK_NUMPAD_BEGIN:break;
        case WXK_NUMPAD_INSERT:break;
        case WXK_NUMPAD_DELETE:break;
        case WXK_NUMPAD_EQUAL:break;
        case WXK_NUMPAD_MULTIPLY:break;
        case WXK_NUMPAD_ADD:break;
        case WXK_NUMPAD_SEPARATOR:break;
        case WXK_NUMPAD_SUBTRACT:break;
        case WXK_NUMPAD_DECIMAL:break;
        case WXK_NUMPAD_DIVIDE:break;

    default: break;

    }
}


void BoardCanvas::SetBoardBlackSquare(wxString& name)
{
	//blackSquaresBmp.FreeResource(true);
	//blackSquaresBmp.LoadFile(name,wxBITMAP_TYPE_PNG);
	for( char a = 'a'; a < 'i'; a++ )
	{
		for( char n = '1'; n < '9'; n++ )
		{
			wxString path = name;
			path += '/';


			path+=a;
				path+= n;

			path+= ".png";
			defaultSquares[ a-'a' ][ n - '1' ].FreeResource(true);
			defaultSquares[ a-'a' ][ n - '1' ].LoadFile(path,wxBITMAP_TYPE_PNG);
		}
	}
}

void BoardCanvas::SetBoardWhiteSquare(wxString& name)
{
	//whiteSquaresBmp.FreeResource(true);
	//whiteSquaresBmp.LoadFile(name,wxBITMAP_TYPE_PNG);
	for( char a = 'a'; a < 'i'; a++ )
	{
		for( char n = '1'; n < '9'; n++ )
		{
			wxString path = name;
		
			path += '/';

			path+=a;
				path+= n;

			path+= ".png";
			defaultSquaresWhite[ a-'a' ][ n - '1' ].FreeResource(true);
			defaultSquaresWhite[ a-'a' ][ n - '1' ].LoadFile(path,wxBITMAP_TYPE_PNG);
		}
	}
}


void BoardCanvas::StartAnimation()
{
	if( this->m_owner->optionsFrame->m_GameOptions.animate == false || this->mode == 1)
	{
		bAnimating = false;
		return;
	}
	bAnimating = true;
	bFowardsAnim = true;
	needsRefresh = true;
	animationStart = wxGetLocalTimeMillis ();
	MyChessMove* pMov = NULL;
	if( chess_board->mov == BLACK )
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->whiteMove;
	}
	else
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->blackMove;
	}

	if( flip_board )
	{
		currAnimX = base2_x+border+squareSize*pMov->from.file;
		currAnimY = base2_y+border+squareSize*(7-pMov->from.rank);
	}
	else
	{
		currAnimX = base2_x+border+squareSize*(7-pMov->from.file);
		currAnimY = base2_y+border+squareSize*(pMov->from.rank);
	}

	selX = pMov->to.file;
	selY = pMov->to.rank;

	chess_board->SquareHasPiece(pMov->to.file, pMov->to.rank, &selPieceID );
}


void BoardCanvas::BackAnimation()
{
	if( this->m_owner->optionsFrame->m_GameOptions.animate == false || this->mode == 1)
	{
		this->bAnimating = false;
		return;
	}
	needsRefresh = true;
	bAnimating = true;
	bFowardsAnim = false;
	animationStart = wxGetLocalTimeMillis ();
	MyChessMove* pMov = NULL;
	if( chess_board->mov == BLACK )
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->blackMove;

	}
	else
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->pNext->whiteMove;
	}

	if( flip_board )
	{
		currAnimX = base2_x+border+squareSize*pMov->to.file;
		currAnimY = base2_y+border+squareSize*(7-pMov->to.rank);
	}
	else
	{
		currAnimX = base2_x+border+squareSize*(7-pMov->to.file);
		currAnimY = base2_y+border+squareSize*(pMov->to.rank);
	}

	selX = pMov->from.file;
	selY = pMov->from.rank;

	chess_board->SquareHasPiece(pMov->from.file, pMov->from.rank, &selPieceID );
}


