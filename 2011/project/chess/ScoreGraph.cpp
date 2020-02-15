#include "ScoreGraph.h"

#include "mdi.h"
#include "BoardCanvas.h"
#include "MyPipePanel.h"
#include "ScorePanel.h"


BEGIN_EVENT_TABLE(ScoreGraph, wxScrolledWindow)
//	EVT_CLOSE(PositionSetupFrame::OnClose)
	EVT_PAINT  (ScoreGraph::OnPaint)
	EVT_IDLE (ScoreGraph::OnIdle)
	EVT_LEFT_DOWN (ScoreGraph::OnMouseLeftClick)
	EVT_SCROLL(ScoreGraph::OnScroll)
	EVT_LEFT_UP(ScoreGraph::mouseReleased)
	
END_EVENT_TABLE()

ScoreGraph::ScoreGraph(wxWindow *parent, MyFrame* frame)
        : wxScrolledWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE |wxWANTS_CHARS )
{
	m_frame = frame;
	//m_useContext = true;
	//selection = 0;
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

#ifdef WINDOWS_7
	SetDoubleBuffered(true);
#endif
	//showVariationRoot = true;
	//numSelectionsThisMove = 0;
	global_offset = 0.0f;
	this->curr_draw_pos = 40.0f;

	this->SetScorePanel( (ScorePanel*) parent);

	//visitTreeCount = 0;
	SetScrollbars( 10.0f, 0.0f, 500, 0 );

	scrollControl = false;
#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif
}

ScoreGraph::~ScoreGraph()
{
}


void ScoreGraph::SetScorePanel( ScorePanel* scorePanel )
{
	m_scorePanel = scorePanel;
}

void ScoreGraph::OnPaint(wxPaintEvent &event)
{
	bool m_useContext = true;
    wxPaintDC pdc(this);

	wxSize sz(GetClientSize());
	
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

    m_frame->PrepareDC(dc);

	dc.Clear();

#if wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();
	//(wxINTERPOLATION_GOOD );
	gc->SetAntialiasMode(wxANTIALIAS_NONE);
//	DrawGraphics();
//	return;
	// gets the move strings from the start in inverse order
	int black_white;
	if( m_frame->board_canvas->GetBoard()->mov != 1 )
	{
		black_white = WHITE;
	}
	else black_white = BLACK;

	int maxValue = 0;
	int move_count = m_frame->currentGame->pCurrMove->CountMovesToStart(black_white, maxValue);



	//if( abs(maxValue) > 3000 )
	//	maxValue = 3000;
	int XX=0, YY=0;
	int xx = 0;
	/*CalcUnscrolledPosition	(0,0,&XX,&YY );

	int pixPerUnitX;
	int pixPerUnitY;
	GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);

	if( double(move_count * 10.0f) > double(sz.GetWidth() + XX))
		Scroll(double(move_count * 10.0f)/pixPerUnitX,0);

	//else if( double(move_count * 10.0f) < double(sz.GetWidth() + XX))
	//	Scroll(-(XX+sz.GetWidth()/2)/pixPerUnitX,0);


	CalcUnscrolledPosition	(0,0,&XX,&YY );*/

	double height_over_2 = (double)(sz.GetHeight())/2.0f;
	//height_over_2 -= 10; // removing this will make the graph fill 90% of the pane, 
						//and the numbers will go below the bottom of the graph
	//double 
	double start_offset = 5.0f;
	

	//wxFont fontBold = wxFont(7, wxMODERN, wxNORMAL, wxBOLD, false, "Microsoft Sans Serif");
	wxFont fontBold = wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL );
	gc->SetFont(fontBold, *wxBLACK);
	//Scroll(move_count*10/(2*pixPerUnitX), 0);
	
	gc->SetBrush(*wxWHITE_BRUSH);
	gc->SetPen(*wxTRANSPARENT_PEN);
	gc->DrawRectangle(start_offset, height_over_2 - height_over_2 , double(10000) + xx - start_offset, 2.0f*height_over_2);  
	
//	gc->SetPen(*wxLIGHT_GREY);
//	gc->SetBrush(*wxTRANSPARENT_BRUSH);
//	gc->DrawRectangle(start_offset, height_over_2 - height_over_2 , sz.GetWidth() + xx - start_offset-5, 2.0f*height_over_2);  
	//gc->DrawRectangle(start_offset, height_over_2 - height_over_2 , sz.GetWidth() + xx - start_offset-5, 2.0f*height_over_2);  
	
//	gc->SetBrush(*wxTRANSPARENT_BRUSH);
//	gc->SetPen(*wxBLACK_PEN);
//	gc->DrawRectangle(start_offset, height_over_2 - height_over_2 , double(10000) + xx - start_offset, 2.0f*height_over_2-1);  
	
	wxGraphicsPath linesPath = gc->CreatePath();

	//linesPath.MoveToPoint(45.0f, height_over_2*0.9 );
	//linesPath.AddLineToPoint(45.0f, height_over_2 - height_over_2 * 0.9f);

	linesPath.MoveToPoint(start_offset, height_over_2 );
	linesPath.AddLineToPoint(double(10000) + xx - start_offset, height_over_2);

	if( maxValue <= 0 ) 
	{
		this->m_scorePanel->txt1 = "0.1";
		this->m_scorePanel->txt2 = "-0.1";
		gc->SetPen(*wxLIGHT_GREY);
		gc->DrawPath(linesPath);
		return;
	}


	//scale = 1.0f;
	

/*	if( !(double(move_count)*10.0f > (double)(sz.GetWidth()*7/8+xx)  ))
	{
		start_offset = start_offset;
		//global_offset = start_offset;
	}
	else
	{
		double dividedVal = (double(move_count)*10.0f / (double)sz.GetWidth());
		double global_length = ((double)sz.GetWidth()-40.0f);
		double graph_length = double(move_count)*10.0f;
		double length_to_scroll_to = (double)(global_length / graph_length);

		////if( (double)(move_count)*10.0f-global_offset > (double)(sz.GetWidth()+XX))
		//{
		//}
		//else // something
		{
			start_offset = -(double(move_count)*10.0f-(double)(sz.GetWidth()*7/8+xx)) ;
		}
		//global_offset = start_offset;
	}	
	*/

//	linesPath.MoveToPoint(45.0f, height_over_2 );
//	linesPath.AddLineToPoint(40.0f, height_over_2);
	/*linesPath.MoveToPoint(0.0f, height_over_2 - height_over_2 * 0.9f );
	linesPath.AddLineToPoint(0.0f, height_over_2 - height_over_2 * 0.9f );
	linesPath.MoveToPoint(0.0f, height_over_2 + height_over_2 * 0.9f );
	linesPath.AddLineToPoint(0.0f, height_over_2 + height_over_2 * 0.9f );
	*/

	char buff[50];
	sprintf(buff, "%.2f",float(maxValue)/100.0f); 
	//gc->DrawText(buff,5.0f, height_over_2 - height_over_2 * 0.9f);
	this->m_scorePanel->txt1 = buff;
	sprintf(buff, "-%.2f",float(maxValue)/100.0f); 
	this->m_scorePanel->txt2 = buff;
	//gc->DrawText(buff,5.0f, height_over_2 + height_over_2 * 0.7f);
	//gc->DrawText("0.0",5.0f, height_over_2 - 10.0f);

	

	
	double scale = double(sz.GetHeight())/double(maxValue);
	scale *= 0.5;
	scale *= 0.7;

	double scrollPoint = start_offset+double(move_count)*10.0f;
	

	int localXX, localYY;
	CalcUnscrolledPosition( 0,0, &localXX, &localYY);	

	int pixPerUnitX;
	int pixPerUnitY;
	GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);


	double mid_point = localXX + (sz.GetWidth() )/2;

	//double difference_to_mid_point  = scrollPoint - mid_point;
	if( !scrollControl )
	{
		if( scrollPoint > mid_point )
		{
			double difference_to_mid_point  = scrollPoint - mid_point;
			Scroll( (localXX + difference_to_mid_point)/(pixPerUnitX),0);
		}
		else if ( scrollPoint < localXX )
		{
			Scroll( (scrollPoint)/(pixPerUnitX),0);
		}
	}
	
	// now draw
	{
		int inv_draw_count = move_count;
		string retVal = "";
		PGN::Move* temp = m_frame->currentGame->pCurrMove;
		
		if( black_white != 1 )
		{
			//string movBlack = " ";
			//movBlack += temp->blackMove.toString();
			//retVal += movBlack;   // = " e7e5\n";
			
			if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
			{
				gc->SetPen(*wxBLACK_PEN);
				double xStart = start_offset+double(inv_draw_count)*10.0f;

				//gc->SetBrush(wxBrush(wxColour(255, 0,0, 160)));
				gc->SetBrush(
					gc->CreateLinearGradientBrush	( xStart,
						0,
						xStart + 10,
						0,// + textHeight,
						wxColour(255,255,255,255),
						wxColour(255,0,0,255) 
						));

				if( temp->move_cp_black < 0 )
				{
					gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_black)*scale);
				}
				else
				{
					gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_black)*scale, 10.0f, (double)abs(temp->move_cp_black)*scale);
				}
			}


			
			
			inv_draw_count--;

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;


					if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
					{
						gc->SetPen(*wxBLACK_PEN);
						double xStart = start_offset+double(inv_draw_count)*10.0f;
						//gc->SetBrush(wxBrush(wxColour(0, 255,0, 160)));
						gc->SetBrush(
							gc->CreateLinearGradientBrush	( xStart,
								0,
								xStart + 10,
								0,// + textHeight,
								wxColour(255,255,255,255),
								wxColour(0,255,0,255) 
							));
						if( temp->move_cp_white < 0 )
						{
							gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_white)*scale);
						}
						else
						{
							gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_white)*scale, 10.0f, (double)abs(temp->move_cp_white)*scale);
						}

						gc->SetPen(*wxBLACK_PEN);
						gc->DrawText(temp->info.c_str(),start_offset+double(inv_draw_count)*10.0f,
							2.0f*height_over_2);


						linesPath.MoveToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2 - 5);
						linesPath.AddLineToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2);
						
					}
					
			
					inv_draw_count--;

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else
				{
					gc->SetPen(*wxLIGHT_GREY);
					gc->DrawPath(linesPath);
					return;
				}
				//else return "error";
				
			}
			else
			{

				if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
				{
					//gc->SetBrush(wxBrush(wxColour(0, 255,0, 160)));
					double xStart = start_offset+double(inv_draw_count)*10.0f;
					gc->SetPen(*wxBLACK_PEN);
					gc->SetBrush(
						gc->CreateLinearGradientBrush	( xStart,
							0,
							xStart + 10,
							0,// + textHeight,
							wxColour(255,255,255,255),
							wxColour(0,255,0,255) 
						));
					if( temp->move_cp_white < 0 )
					{
						gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_white)*scale);
					}
					else
					{
						gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_white)*scale, 10.0f, (double)abs(temp->move_cp_white)*scale);
					}

					gc->SetPen(*wxBLACK_PEN);
					gc->DrawText(temp->info.c_str(),start_offset+double(inv_draw_count)*10.0f,
						2.0f*height_over_2);
					
					linesPath.MoveToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2-5);
					linesPath.AddLineToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2);
				}
				
			
				inv_draw_count--;

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;

					if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
					{
						gc->SetPen(*wxBLACK_PEN);
						//gc->SetBrush(wxBrush(wxColour(0, 255,0, 160)));
						double xStart = start_offset+double(inv_draw_count)*10.0f;
						gc->SetBrush(
							gc->CreateLinearGradientBrush	( xStart,
								0,
								xStart + 10,
								0,// + textHeight,
								wxColour(255,255,255,255),
								wxColour(0,255,0,255) 
						));
						if( temp->move_cp_white < 0 )
						{
							gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_white)*scale);
						}
						else
						{
							gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_white)*scale, 10.0f, (double)abs(temp->move_cp_white)*scale);
						}
						gc->SetPen(*wxBLACK_PEN);
						gc->DrawText(temp->info.c_str(),start_offset+double(inv_draw_count)*10.0f,
							2.0f*height_over_2);

						linesPath.MoveToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2-5);
						linesPath.AddLineToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2);
					}
					
			
					inv_draw_count--;
					
					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else
				{
					gc->SetPen(*wxLIGHT_GREY);
					gc->DrawPath(linesPath);
					return;
				}
				//else return "error";
				
			}
			else
			{

				if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
				{
					gc->SetPen(*wxBLACK_PEN);
					//gc->SetBrush(wxBrush(wxColour(0, 255,0, 160)));
					double xStart = start_offset+double(inv_draw_count)*10.0f;
					gc->SetBrush(
						gc->CreateLinearGradientBrush	( xStart,
							0,
							xStart + 10,
							0,// + textHeight,
							wxColour(255,255,255,255),
							wxColour(0,255,0,255) 
					));
					if( temp->move_cp_white < 0 )
					{
						gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_white)*scale);
					}
					else
					{
						gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_white)*scale, 10.0f, (double)abs(temp->move_cp_white)*scale);
					}

					gc->SetPen(*wxBLACK_PEN);
					gc->DrawText(temp->info.c_str(),start_offset+double(inv_draw_count)*10.0f,
						2.0f*height_over_2);

					linesPath.MoveToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2-5);
					linesPath.AddLineToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2);
				}
				
			
				inv_draw_count--;

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( temp->pPrev == 0 )
		{
			gc->SetPen(*wxLIGHT_GREY);
			gc->DrawPath(linesPath);
			return;
		}

		while( temp->pPrev != NULL && inv_draw_count > 0)
		{


			if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
			{
				gc->SetPen(*wxBLACK_PEN);
				double xStart = start_offset+double(inv_draw_count)*10.0f;
				//gc->SetBrush(wxBrush(wxColour(255, 0,0, 160)));
					gc->SetBrush(
						gc->CreateLinearGradientBrush	( xStart,
							0,
							xStart + 10,
							0,// + textHeight,
							wxColour(255,255,255,255),
							wxColour(255,0,0,255) 
					));
				if( temp->move_cp_black < 0 )
				{
					gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_black)*scale);
				}
				else
				{
					gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_black)*scale, 10.0f, (double)abs(temp->move_cp_black)*scale);
				}
			}
			
			inv_draw_count--;

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;

					if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
					{
						gc->SetPen(*wxBLACK_PEN);
						//gc->SetBrush(wxBrush(wxColour(0, 255,0, 160)));
						double xStart = start_offset+double(inv_draw_count)*10.0f;
						//gc->SetBrush(wxBrush(wxColour(255, 0,0, 160)));
							gc->SetBrush(
								gc->CreateLinearGradientBrush	( xStart,
									0,
									xStart + 10,
									0,// + textHeight,
									wxColour(255,255,255,255),
									wxColour(0,255,0,255) 
							));
						if( temp->move_cp_white < 0 )
						{
							gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_white)*scale);
						}
						else
						{
							gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_white)*scale, 10.0f, (double)abs(temp->move_cp_white)*scale);
						}

						gc->SetPen(*wxBLACK_PEN);
						
						gc->DrawText(temp->info.c_str(),start_offset+double(inv_draw_count)*10.0f,
							2.0f*height_over_2);

						linesPath.MoveToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2-5);
						linesPath.AddLineToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2);
					}
					

					inv_draw_count--;

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else
				{
					gc->SetPen(*wxLIGHT_GREY);
					gc->DrawPath(linesPath);
					return;
				}
				//else return "error";
			}
			else
			{

				if( !(start_offset + double(inv_draw_count)*10.0f < global_offset) )
				{
					gc->SetPen(*wxBLACK_PEN);
					//gc->SetBrush(wxBrush(wxColour(0, 255,0, 160)));
						double xStart = start_offset+double(inv_draw_count)*10.0f;
						//gc->SetBrush(wxBrush(wxColour(255, 0,0, 160)));
							gc->SetBrush(
								gc->CreateLinearGradientBrush	( xStart,
									0,
									xStart + 10,
									0,// + textHeight,
									wxColour(255,255,255,255),
									wxColour(0,255,0,255) 
							));
					if( temp->move_cp_white < 0 )
					{
						gc->DrawRectangle(xStart, height_over_2, 10.0f, (double)abs(temp->move_cp_white)*scale);
					}
					else
					{
						gc->DrawRectangle(xStart, height_over_2 - (double)(temp->move_cp_white)*scale, 10.0f, (double)abs(temp->move_cp_white)*scale);
					}

					gc->SetPen(*wxBLACK_PEN);
					gc->DrawText(temp->info.c_str(),start_offset+double(inv_draw_count)*10.0f,
						2.0f*height_over_2);

					linesPath.MoveToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2-5);
					linesPath.AddLineToPoint(start_offset+double(inv_draw_count)*10.0f + 5,
											  2.0f*height_over_2);
				}
				

				inv_draw_count--;

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else
			{
				gc->SetPen(*wxLIGHT_GREY);
				gc->DrawPath(linesPath);
				return;
			}
		}
	}

	gc->SetPen(*wxLIGHT_GREY);
	gc->DrawPath(linesPath);
		
		
	//scrollControl = false;
	

#endif
}


void ScoreGraph::OnIdle(wxIdleEvent &event)
{
	event.Skip();
}


void ScoreGraph::OnMouseLeftClick(wxMouseEvent &event)
{
}