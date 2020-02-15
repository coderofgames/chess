#include "VariationBoardCanvas.h"





#include "BoardCanvas.h"
#include "mdi.h"
#include "ChessSquare.h"
#include "MyPipePanel.h"
#include "PromotionSelectionDialog.h"

// ----------------------------------------------------------------------------
// BoardCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(VariationBoardCanvas, wxScrolledWindow)
    EVT_PAINT  (VariationBoardCanvas::OnPaint)
	EVT_IDLE(VariationBoardCanvas::OnIdle)
    EVT_MOTION (VariationBoardCanvas::OnMouseMove)
    EVT_SIZE(VariationBoardCanvas::OnSize)

   // EVT_MOUSE_EVENTS(BoardCanvas::OnMouse)
	EVT_LEFT_DOWN(VariationBoardCanvas::mouseDown)
	EVT_LEFT_UP(VariationBoardCanvas::mouseReleased)
	EVT_MOUSEWHEEL(VariationBoardCanvas::mouseWheelMoved)
    EVT_ERASE_BACKGROUND(VariationBoardCanvas::OnEraseBackground)
END_EVENT_TABLE()

VariationBoardCanvas::VariationBoardCanvas(MyFrame *parent, wxWindow *winParent)
//:wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
        : wxScrolledWindow(winParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE |wxWANTS_CHARS )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	SetDoubleBuffered(true);
	m_useContext = true;
    m_owner = parent;
    
	wxSize sz(GetClientSize());
       int w = sz.GetWidth();
       int h = 2*sz.GetHeight()/3;
 
    //   SetScrollbars(1,1, w, h*2, 0, 0);
    m_clip = false;


	light_squares.r = 3.0f;
	light_squares.g = 3.0f;
	light_squares.b = 3.0f;

	dark_squares.r = 1.5f;
	dark_squares.g = 1.5f;
	dark_squares.b = 2.0f;

	size = 800;
	base2_x=30;
	base2_y=30;

	border=10;
	squareSize = (size-size*20.0f/200)/8;

	w=h=0;
	
	BASE  = 80.0;
	BASE2 = BASE/2;
	BASE4 = BASE/4;

	selX=0;
	selY=0;

	bResize = false;


	pieceID = 0;
	brenderPiece = false;

	flip_board = true;

	//chess_board = new ChessBoard();
	//chess_board->LoadTextures();


	selState = SelectionState::SEL_NONE;
	


	gameState = GameState::STATE_ZERO;
	selPieceID = -1;

	//this->
#if wxUSE_GRAPHICS_CONTEXT
    m_useContext = false;
#endif

	keyDownCtrl = false;


	 bmpBackToStart.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Back All.png", wxBITMAP_TYPE_PNG);
	 
	 bmpBack.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Back.png", wxBITMAP_TYPE_PNG);
	 bmpForward.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Play.png", wxBITMAP_TYPE_PNG);
	 
	 bmpForwardToEnd.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Play All.png", wxBITMAP_TYPE_PNG);


    // connect event handlers for the blue input window
  //  Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(VariationBoardCanvas::OnKeyDown),
   //                     NULL, this);
  //  Connect(wxEVT_KEY_UP, wxKeyEventHandler(VariationBoardCanvas::OnKeyUp),
  //                      NULL, this);
    //Connect(wxEVT_CHAR, wxKeyEventHandler(MiniBoardCanvas::OnChar),
     //                   NULL, this);
}

VariationBoardCanvas::~VariationBoardCanvas()
{

//	delete chess_board;
}

bool VariationBoardCanvas::PickSquare( double x, double y, int START_X, int END_X, 
												int START_Y, int END_Y, 
												int *sqX, int *sqY )
{
	// start_x = -board_size/2
	// so in this case it is -7;
	// so small_square_size is board_size/8
	// start_x + board_size/2 = 0;
	// x + board_size/2 >0




	
	if(x >= addPieces_x && x < addPieces_x +squareSize*5)
	{
		
		if(y >= addPieces_y && y < addPieces_y +squareSize*2)
		{
			
			newPieceX = (int)((x-addPieces_x)/squareSize );
			newPieceY = (int)((y-addPieces_y)/squareSize );

			if( selState == SelectionState::SELECTED_NEW_PIECE )
			{
				selPieceID = 0;
			}
			selState = SelectionState::SELECTED_NEW_PIECE;


			if( newPieceY == 0 )
			{
				this->newSelectedPieceIndex = newPieceX;
			}
			else
			{
				this->newSelectedPieceIndex = newPieceX+6;
			}
			//int idx_x = x - addPieces_x
		}
	}
	else
	{
		selState = SelectionState::SEL_NONE;
		return false;
	}

// addPieces_x+squareSize*i, addPieces_y, squareSize, squareSize);
//(*m_owner->board_canvas->GetBoard()->svgTextures[i+6], addPieces_x+squareSize*i, addPieces_y+squareSize, squareSize, squareSize);
	/*(*m_owner->board_canvas->GetBoard()->svgTextures[i+6]*/
	return true;
}

void VariationBoardCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(evt))
{
}

void VariationBoardCanvas::InitGraphics()
{
}


void VariationBoardCanvas::ResetBoard()
{
	sz = GetClientSize();
	internalBoard.SetInitialPosition();
	CalculateSizeParameters();
}

static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }


void VariationBoardCanvas::OnPaint(wxPaintEvent &event)
{
    wxPaintDC pdc(this);
	
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

    m_parent->PrepareDC(dc);

    //if ( m_clip )
     //   dc.SetClippingRegion(100, 100, 100, 100);

    dc.Clear();


#if wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();
//	DrawGraphics();

    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    gc->SetFont(font,*wxBLACK);
	bool highlight_white = false;

	

    wxGraphicsPath whiteSquares = gc->CreatePath() ;
	wxGraphicsPath darkSquares = gc->CreatePath() ;
	wxGraphicsPath borderPath = gc->CreatePath() ;
	wxGraphicsPath lastMovePath = gc->CreatePath() ;
	//wxGraphicsPath selectedSquarePath = gc->CreatePath() ;
	wxGraphicsPath whiteBorderBox = gc->CreatePath();
	wxGraphicsPath blackBorderBox = gc->CreatePath();


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
					whiteSquares.AddRectangle(x, y, squareSize, squareSize);
				}
				else
				{
					darkSquares.AddRectangle(x, y, squareSize, squareSize);
				}
			}
			yy++;
		}
		xx++;
	}



	borderPath.AddRectangle(base2_x, base2_y, squareSize*8+2*border, squareSize*8+2*border);


	addPieces_x = base2_x +border+squareSize*2;
	addPieces_y = base2_y + 2*border+squareSize*8.5;

	whiteBorderBox.AddRectangle( addPieces_x, addPieces_y, squareSize*4, squareSize);

	blackBorderBox.AddRectangle( addPieces_x, addPieces_y + squareSize,squareSize*5, squareSize);
	
	int rot = 0;





	gc->PushState(); // save current translation/scale/other state
	{

		//gc->SetPen(wxPen("navy", 1));
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(wxColour(150,  150,  255, 255)));// 128 == half transparent
		//gc->Rotate(degToRad((double)rot));
		gc->DrawPath(borderPath);

	}
	gc->PopState();

	// Draw the light Squares
    gc->PushState(); // save current translation/scale/other state
	{
		gc->SetPen(*wxTRANSPARENT_PEN);
		//gc->SetPen(wxPen("navy", 1));
		gc->SetBrush(wxBrush(wxColour(255,  255,  255, 255)));// 128 == half transparent
		gc->DrawPath(whiteSquares);
	}
	gc->PopState();

	// draw the dark squares
	gc->PushState(); // save current translation/scale/other state
	{

		//gc->SetPen(wxPen("navy", 1));
	
		gc->SetBrush(wxBrush(wxColour(244,  85,  32, 255)));// 128 == half transparent
		gc->DrawPath(darkSquares);

	}
	gc->PopState();


	int i = 0;//=0;
	int j;


	gc->PushState();

	i=(flip_board? 0 : 7);
 	for( double x = base2_x+border ; x < base2_x+border+ squareSize*8; x+= squareSize )
	{
		
		j=(flip_board? 7 : 0);
		for( double y = base2_y+border; y < base2_y+border+ squareSize*8; y+=squareSize)
		{
			//path.AddRectangle(x, y, squareSize, squareSize);
			int pcID = -1;
			if(internalBoard.SquareHasPiece(i,j,&pcID ))
			{
				if( gameState == GameState::SELECTED_PIECE )
				{
					if( ((7-j == selY)&& (i==selX)) )
					{
					}
					else
					{
						m_owner->board_canvas->GetBoard()->svgTextures[pcID-1]->UseAlpha();
						gc->DrawBitmap(*m_owner->board_canvas->GetBoard()->svgTextures[pcID-1], x, y, squareSize, squareSize);
					}
				}
				else
				{
					m_owner->board_canvas->GetBoard()->svgTextures[pcID-1]->UseAlpha();
					gc->DrawBitmap(*m_owner->board_canvas->GetBoard()->svgTextures[pcID-1], x, y, squareSize, squareSize);
				}
			}
			j = (flip_board? j-1 : j+1);
		}
		
		i= (flip_board? i+1 : i-1);
	}

/*	if( gameState == GameState::SELECTED_PIECE )
	{			
			POINT p;
		::GetCursorPos(&p);
		::ScreenToClient((HWND)this->GetHWND(), &p);
		MouseX = p.x;
		MouseY = p.y;
//#endif			
		m_owner->board_canvas->GetBoard()->svgTextures[selPieceID-1]->UseAlpha();
		gc->DrawBitmap(*m_owner->board_canvas->GetBoard()->svgTextures[selPieceID-1], MouseX-squareSize/2, MouseY-squareSize/2, squareSize, squareSize);
		Refresh();
	}*/					


    gc->PopState(); // restore saved state


	gc->PushState();
			{
				gc->SetPen(wxPen("blue", 1));
				//gc->SetBrush(wxBrush(wxColour(150,  150,  255, 255)));// 128 == half transparent
				gc->DrawPath(whiteBorderBox);
			}
	gc->PopState();




	

	gc->PushState();
		/*for( int i = 0; i < 5; i++ )
		{
			gc->DrawBitmap(*m_owner->board_canvas->GetBoard()->svgTextures[i], addPieces_x+squareSize*i, addPieces_y, squareSize, squareSize);
			gc->DrawBitmap(*m_owner->board_canvas->GetBoard()->svgTextures[i+6], addPieces_x+squareSize*i, addPieces_y+squareSize, squareSize, squareSize);

		}*/
			gc->DrawBitmap(bmpBackToStart, addPieces_x, addPieces_y, squareSize, squareSize);
			gc->DrawBitmap(bmpBack, addPieces_x+squareSize, addPieces_y, squareSize, squareSize);
			gc->DrawBitmap(bmpForward, addPieces_x+squareSize*2, addPieces_y, squareSize, squareSize);
			gc->DrawBitmap(bmpForwardToEnd, addPieces_x+squareSize*3, addPieces_y, squareSize, squareSize);


		/*if( selState == SelectionState::SELECTED_NEW_PIECE )
		{
			gc->DrawBitmap(*m_owner->board_canvas->GetBoard()->svgTextures[this->newSelectedPieceIndex], MouseX-squareSize/2, MouseY-squareSize/2, squareSize, squareSize);
		}
		else if( gameState == GameState::SELECTED_PIECE )
		{
			gc->DrawBitmap(*m_owner->board_canvas->GetBoard()->svgTextures[this->selPieceID-1], MouseX-squareSize/2, MouseY-squareSize/2, squareSize, squareSize);

		}*/

	gc->PopState();
//	whiteBorderBox.AddRectangle(base2_x + border+squareSize*9, base2_y +border+squareSize*1.5, squareSize*5, squareSize);



#endif  




}












void VariationBoardCanvas::OnIdle(wxIdleEvent &event)
{
	wxPaintEvent evt;
	OnPaint(evt);
	//event.RequestMore(false);
}



void VariationBoardCanvas::mouseDown(wxMouseEvent& event)
{
	 wxClientDC dc(this);
    PrepareDC(dc);
    m_parent->PrepareDC(dc);
wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );

	if( selState == SELECTED_NEW_PIECE )
		return;


//	PickSquare((double)pos.x,(double)pos.y, base2_x+border, base2_x+border+squareSize*8,
//					base2_y+border, base2_y+border+squareSize*8,
//					&selX,&selY);

	bool hitTest = PickSquare(x,y, base2_x+border, base2_x+border+squareSize*8,
					base2_y+border, base2_y+border+squareSize*8,
					&selX,&selY);

	if( internalBoard.SquareHasPiece(selX,7-selY, &selPieceID) &&
		gameState == GameState::STATE_ZERO && 
		hitTest )
	{
		internalBoard.board[selX][7-selY] = 0;
			gameState = GameState::SELECTED_PIECE;

	}

	
/*	if( gameState == GameState::SELECTED_PIECE )
	{
		// check square is a valid move
		theMove.to.file=selX;
		theMove.to.rank=7-selY;
		bool promote = false;
		bool pinned = false;

		// more feedback
		if( chess_board->IsMoveValid(&theMove, &promote, &pinned) )
		{

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
			
			
			string pgn = chess_board->EPN_Move_to_PGN_Move(epn, promote,2 );
		//	chess_board->makemove2(theMove.from.file, theMove.from.rank,
		//							theMove.to.file, theMove.to.rank, promote, 2);
			string fen = chess_board->Fen();
			PGN::MoveInserter mv(pgn,white_black, fen);


			this->m_owner->currentGame->pCurrMove = this->m_owner->currentGame->GetCurrentMove()->InsertMove( &mv);
			//m_owner->currentGame->EnumerateVariations();
			this->m_owner->my_canvas->move_count++;
			if( m_owner->my_canvas->white_black_move == WHITE )
				m_owner->my_canvas->white_black_move = BLACK;
			else m_owner->my_canvas->white_black_move = WHITE;
			this->m_owner->my_canvas->Refresh(true);

		}
		gameState = GameState::STATE_ZERO;
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
			}
			else
			{
				// Not My Move
			}
		}
	}
	*/

	Refresh();
}


void VariationBoardCanvas::CalculateSizeParameters()
{
	size = (double)(int)(sz.GetHeight() < sz.GetWidth() ? sz.GetHeight()*0.75 : sz.GetWidth()*0.75);
	BASE2 = (double)(int)(sz.GetHeight()  < sz.GetWidth()? sz.GetHeight()*0.075 : sz.GetWidth()*0.075);
	base2_x = 10;//(double)(int)(sz.GetWidth()/2 - size/2);
	base2_y = 10;//(double)(int)(sz.GetHeight()/2 - size/2);
	internalBoard.SetBoardDisplaySize(size);
 
	border = (double)(int)(size*18/500);//17.75;
	squareSize = (double)(int)((size-size*36.0f/500)/8);
}

void VariationBoardCanvas::OnSize(wxSizeEvent& event)
{
	bResize = true;
	sz = GetClientSize();
	CalculateSizeParameters();
	//Update();
	//Refresh(true);
}

void VariationBoardCanvas::OnMouseMove(wxMouseEvent &event)
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

	Refresh(true);
	//event.reque
	//event.
	//event.(false);

}

void VariationBoardCanvas::mouseReleased(wxMouseEvent& event)
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

	bool hitTest = PickSquare(x,y, base2_x+border, base2_x+border+squareSize*8,
						base2_y+border, base2_y+border+squareSize*8,
						&selX,&selY);

	if( !hitTest )
	{
		selPieceID = 0;
		newSelectedPieceIndex = 0;
		gameState = GameState::STATE_ZERO;
		selState = SelectionState::SEL_NONE;
		return;
	}

	if( gameState == GameState::SELECTED_PIECE )
	{
		// move the piece
		internalBoard.DropPiece(selPieceID, selX, 7-selY );
		selPieceID = 0;
		newSelectedPieceIndex = 0;
		gameState = GameState::STATE_ZERO;
		selState = SelectionState::SEL_NONE;
	}
	else if( selState == SelectionState::SELECTED_NEW_PIECE )
	{
		// selected new piece, wait for drop
		/*internalBoard.DropPiece(newSelectedPieceIndex+1, selX, 7-selY );
		if( !keyDownCtrl )
		{
			selPieceID = 0;
			newSelectedPieceIndex = 0;
			gameState = GameState::STATE_ZERO;
			selState = SelectionState::SEL_NONE;
		}*/

		if( newSelectedPieceIndex == 0 )
		{
			if( this->selectedMove > 0 )
			{
				this->selectedMove--;

				if( this->m_moves.size() > this->selectedMove )
				{
					internalBoard.SetBoard(this->m_moves[ selectedMove ].fen);
					Refresh(false);
				}
			}
			else
			{
				this->selectedMove = 0;				
				
				if( this->m_moves.size() > 0 )
				{
					internalBoard.SetBoard(this->m_moves[ selectedMove ].fen);
					Refresh(false);
				}

			}


		}
		else if( newSelectedPieceIndex == 1 )
		{
			if( this->selectedMove < this->m_moves.size() -1 )
			{
				this->selectedMove++;
				if( this->m_moves.size() > this->selectedMove )
				{
					
					internalBoard.SetBoard(this->m_moves[ selectedMove ].fen);
					Refresh(false);
				}

			}
			else
			{
				if( this->m_moves.size() > 0 )
				{
					this->selectedMove = this->m_moves.size()-1;
					internalBoard.SetBoard(this->m_moves[ selectedMove ].fen);
					Refresh(false);
				}
				else
				{
					this->selectedMove  = 0;
				}
			}

		}


	}


	/*if( gameState == GameState::SELECTED_PIECE )
	{
		// check square is a valid move
		theMove.to.file=selX;
		theMove.to.rank=7-selY;
		bool promote = false;
		bool pinned = false;

		// more feedback
		if( chess_board->IsMoveValid(&theMove, &promote, &pinned) )
		{

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

			int promotePieceID = 3;
			if( promote )
			{
				PromotionSelectionDialaog dialog(this->m_parent, chess_board,wxID_ANY,
					wxT("Select Piece to Promote to"));

				dialog.SetPromoteID( &promotePieceID );
				dialog.ShowModal();
			}
			
			string pgn = chess_board->EPN_Move_to_PGN_Move(epn, promote, promotePieceID);
			
			string fen = chess_board->Fen();
			PGN::MoveInserter mv(pgn,white_black, fen, chess_board->GetLastMove());


			this->m_owner->currentGame->pCurrMove = this->m_owner->currentGame->GetCurrentMove()->InsertMove( &mv);
			chess_board->SetBoard(fen);
			//m_owner->currentGame->EnumerateVariations();
			this->m_owner->my_canvas->move_count++;
			if( m_owner->my_canvas->white_black_move == WHITE )
				m_owner->my_canvas->white_black_move = BLACK;
			else m_owner->my_canvas->white_black_move = WHITE;
			if( this->m_owner->my_canvas->move_count < 10 )
				this->m_owner->my_canvas->Scroll(0,0);
			this->m_owner->my_canvas->Refresh(true);

			for( int j = 0; j < activeEngines.size(); j++ )
			{
				if( activeEngines[j]->IsActive() &&
					activeEngines[j]->IsAnalysing())
				{
					activeEngines[j]->KibitzStartsMove();
					activeEngines[j]->KibitzUserEndsMove();
				}
			}

		}
		gameState = GameState::STATE_ZERO;
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
			}
			else
			{
				// Not My Move
			}
		}
	}*/


	Refresh();
}


void VariationBoardCanvas::SetCurrentPipePanel( MyPipePanel *pp )
{
	pCurrentPipePanel = pp;
}

void VariationBoardCanvas::LogEvent(const wxString& name, wxKeyEvent& event)
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
        case WXK_CONTROL:
			{
				if( name == "KeyDown" )
				{
					keyDownCtrl = true;
				}
				else
				{
					keyDownCtrl = false;
				}
			}	
			break;
        case WXK_MENU:break;
        case WXK_PAUSE:break;
        case WXK_CAPITAL:break;
        case WXK_END:break;
        case WXK_HOME:break;
        case WXK_LEFT:
			{
			//	m_owner->my_canvas->LogEvent(name, event);
			}break;
        case WXK_UP:break;
        case WXK_RIGHT:
			{
			//	m_owner->my_canvas->LogEvent(name, event);
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