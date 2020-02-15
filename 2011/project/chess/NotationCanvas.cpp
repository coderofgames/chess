#include "NotationCanvas.h"
#include "mdi.h"
#include "OptionsFrame.h"
#include "MyPipePanel.h"
#include "TreeCanvas.h"
#include "MoveDataCanvas.h"
#include "BaseGraphicsBoard.h"
#include "MiscUtils.h"
#include "NotationFrame.h"
#include "ScoreGraph.h"
#include "ScorePanel.h"



// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    File_Quit = wxID_EXIT,
    File_About = wxID_ABOUT,

    MenuShow_First = wxID_HIGHEST,
    File_ShowDefault = MenuShow_First,
    File_ShowText,
    File_ShowLines,
    File_ShowBrushes,
    File_ShowPolygons,
    File_ShowMask,
    File_ShowOps,
    File_ShowRegions,
    File_ShowCircles,
    File_ShowSplines,
#if wxUSE_GRAPHICS_CONTEXT
    File_ShowAlpha,
#endif
    File_ShowGradients,
    MenuShow_Last = File_ShowGradients,

    File_Clip,
#if wxUSE_GRAPHICS_CONTEXT
    File_GraphicContext,
#endif

    MenuOption_First,

    MapMode_Text = MenuOption_First,
    MapMode_Lometric,
    MapMode_Twips,
    MapMode_Points,
    MapMode_Metric,

    UserScale_StretchHoriz,
    UserScale_ShrinkHoriz,
    UserScale_StretchVertic,
    UserScale_ShrinkVertic,
    UserScale_Restore,

    AxisMirror_Horiz,
    AxisMirror_Vertic,

    LogicalOrigin_MoveDown,
    LogicalOrigin_MoveUp,
    LogicalOrigin_MoveLeft,
    LogicalOrigin_MoveRight,
    LogicalOrigin_Set,
    LogicalOrigin_Restore,

#if wxUSE_COLOURDLG
    Colour_TextForeground,
    Colour_TextBackground,
    Colour_Background,
#endif // wxUSE_COLOURDLG
    Colour_BackgroundMode,
	SET_VARIATION_COLOR,
	SET_MOVE_COLOR,
	PROMOTE_VARIATION,
	DELETE_VARIATION,
	DELETE_REMAINING_MOVES,
	ADD_ANNOTATION,
	ADD_GLYPH,
	SET_GLYPH,
	TOGGLE_VARIATION_FOLDS,
	ID_EXCLAMATION,// "!", "Insert an Exclamation Mark Symbol");
	ID_DOUBLE_EXCLAMATION, //"!!", "Insert a Double Exclamation Mark Symbol");
	ID_EXCLAMATION_QUESTION, //"!?", "Insert an Exclamation Mark Followed by a Question Mark Symbol");
	ID_QUESTION, //"?", "Insert an Question Mark Symbol");
	ID_DOUBLE_QUESTION, //"??", "Insert a Double Question Mark Symbol");
	ID_QUESTION_EXCLAMATION, //"?!", "Insert a Question Mark Followed by an Exclamation Mark Symbol");
	
    Colour_TextureBackgound,

    MenuOption_Last = Colour_TextureBackgound
};


// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT  (MyCanvas::OnPaint)
	EVT_IDLE(MyCanvas::OnIdle)
	EVT_SIZE(MyCanvas::OnSize)
    EVT_LEFT_DOWN (MyCanvas::OnMouseMove)
	EVT_RIGHT_DOWN (MyCanvas::rightClick)
	EVT_MENU(SET_VARIATION_COLOR, MyCanvas::SetVariationColor)
	EVT_MENU(SET_MOVE_COLOR, MyCanvas::SetMoveColor)
	EVT_MENU(PROMOTE_VARIATION, MyCanvas::PromoteVariation)
	EVT_MENU(DELETE_VARIATION, MyCanvas::DeleteVariation)
	EVT_MENU(DELETE_REMAINING_MOVES, MyCanvas::DeleteRemainingMoves)
	EVT_MENU(ADD_ANNOTATION, MyCanvas::AddAnnotation)
	EVT_MENU(ADD_GLYPH, MyCanvas::OnAddGlyph)
	EVT_MENU(TOGGLE_VARIATION_FOLDS, MyCanvas::OnToggleAllVariationFolds)
	
	//OnAddGlyph(wxCommandEvent& evt)

	EVT_MENU(ID_EXCLAMATION, MyCanvas::OnAddGlyphExclamation)
	EVT_MENU(ID_DOUBLE_EXCLAMATION, MyCanvas::OnSetGlyphDoubleExclamation)
	EVT_MENU(ID_EXCLAMATION_QUESTION, MyCanvas::OnAddGlyphExclamationQuestion)
	EVT_MENU(ID_QUESTION, MyCanvas::OnSetGlyphQuestion)
	EVT_MENU(ID_DOUBLE_QUESTION, MyCanvas::OnAddGlyphDoubleQuestion)
	EVT_MENU(ID_QUESTION_EXCLAMATION, MyCanvas::OnSetGlyphQuestionExclamation)
END_EVENT_TABLE()



MyCanvas::MyCanvas( MyFrame* parent, wxWindow *owner):
 wxScrolledWindow(owner, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE )//| wxBORDER_THEME )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	SetDoubleBuffered(true);
	m_useContext = true;
    m_owner = parent;
    m_show = Show_Default;
   // m_smile_bmp = wxBitmap(smile_xpm);
    m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
	//this->EnableScrolling(false, true);
	//SetScrollbar(wxVERTICAL, 0, 16, 50);
	//wxSize sz(GetClientSize());
	//SetVirtualSize(sz.GetWidth(), sz.GetHeight()*4);
	bCheckVirtualSize = true;
	this->SetBackgroundColour(*wxWHITE);
	wxSize sz(GetClientSize());
       w = sz.GetWidth();
       h = sz.GetHeight();

	   needsRefresh = true;
	   MoveToCursor = false;
	
SetVirtualSize(1025, 100000);
	scrollX = 0;
	scrollY = 1;
 
	Scroll(0,-10);
	  // SetSize(0, 40, w,h-40);
	  // this->gs_bmp4 = new wxBitmap;
	      /* 4 colour bitmap */
    //gs_bmp4->LoadFile("board.bmp", wxBITMAP_TYPE_BMP);
        /* init scrolled area size, scrolling speed, etc. */
       //SetScrollbars(1,1, w, h*2, 0, 0);
	move_count = 0;
#ifdef USE_CANVAS_SET_FOCUS
	SetFocus();
#endif
	   SetSizer(NULL,false);
    m_clip = true;
	selectedMove = 0;
	white_black_move = 0;
	      
	/*wxFont font = m_owner->optionsFrame->m_NotationOptions.notationFont;
		wxFont f1 = font;//m_owner->optionsFrame->m_NotationOptions.notationFont;
		wxFont f2 = font;//m_owner->optionsFrame->m_NotationOptions.notationFont;
		wxFont f3 = font;//m_owner->optionsFrame->m_NotationOptions.notationFont;
		f1.SetWeight(wxNORMAL);
		f2.SetWeight(wxBOLD);
		f3.SetStyle(wxITALIC);
		//f1.SetPixelSize();
	SetFonts(f1, f2, f3);*/
	fontSize = 10;

	fontBold = wxFont(fontSize, wxMODERN, wxNORMAL, wxBOLD, false);
	fontItalic = wxFont(fontSize, wxMODERN, wxFONTSTYLE_ITALIC, wxNORMAL, false);
	fontNormal = wxFont(fontSize, wxMODERN, wxNORMAL, wxNORMAL, false);
	

	annotateTextColor = m_owner->optionsFrame->m_NotationOptions.AnnotateTextColor;

	rememberedScrollPos = false;
	remXX = 0;
	remYY = 0;

#if wxUSE_GRAPHICS_CONTEXT
    m_useContext = false;
#endif

	//SetBackgroundColour(*wxLIGHT_GREY);
	//SetBackgroundColour(wxColour(109,202,242,0));
#ifdef THEME_BUILD
	//SetBackgroundColour(*wxLIGHT_GREY);
#endif
    // connect event handlers for the blue input window
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyCanvas::OnKeyDown),
                        NULL, this);
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyCanvas::OnKeyUp),
                        NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(MyCanvas::OnChar),
                        NULL, this);
}

MyCanvas::MyCanvas(MyFrame *parent)
//:wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	//SetDoubleBuffered(true);
	m_useContext = true;
    m_owner = parent;
    m_show = Show_Default;
   // m_smile_bmp = wxBitmap(smile_xpm);
    m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
	//this->EnableScrolling(false, true);
	//SetScrollbar(wxVERTICAL, 0, 16, 50);
	//wxSize sz(GetClientSize());
	//SetVirtualSize(sz.GetWidth(), sz.GetHeight()*4);
	bCheckVirtualSize = true;
	wxSize sz(GetClientSize());
       w = sz.GetWidth();
       h = sz.GetHeight();

	scrollX = 0;
	scrollY = 1;
	//wxPanel *panel = new wxPanel(this);

	//panel->SetSize(w,120);
	
	//SetSize(0, 120, w, h-120);
 
	  // this->gs_bmp4 = new wxBitmap;
	      /* 4 colour bitmap */
    //gs_bmp4->LoadFile("board.bmp", wxBITMAP_TYPE_BMP);
        /* init scrolled area size, scrolling speed, etc. */
       //SetScrollbars(1,1, w, h*2, 0, 0);
	move_count = 0;
	Scroll(0,0);
#ifdef USE_CANVAS_SET_FOCUS
	SetFocus();
#endif
	   SetSizer(NULL,false);
    m_clip = true;
	selectedMove = 0;
	white_black_move = 0;

#if wxUSE_GRAPHICS_CONTEXT
    m_useContext = false;
#endif

    // connect event handlers for the blue input window
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyCanvas::OnKeyDown),
                        NULL, this);
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyCanvas::OnKeyUp),
                        NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(MyCanvas::OnChar),
                        NULL, this);
}


void MyCanvas::SetFonts(wxFont norm, wxFont bold, wxFont italic)
{
	fontBold = bold;
	fontNormal = norm;
	fontItalic = italic;
	this->fontSize = norm.GetPointSize();
}

void MyCanvas::SetTextViewportBackground(wxColor col)
{
	this->SetBackgroundColour(col);
}


void MyCanvas::DrawTestBrushes(wxDC& dc)
{
    static const wxCoord WIDTH = 200;
    static const wxCoord HEIGHT = 80;

    wxCoord x = 10,
            y = 10;

    dc.SetBrush(wxBrush(*wxGREEN, wxSOLID));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Solid green"), x + 10, y + 10);

    y += HEIGHT;
    dc.SetBrush(wxBrush(*wxRED, wxCROSSDIAG_HATCH));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Hatched red"), x + 10, y + 10);

    y += HEIGHT;
//    dc.SetBrush(wxBrush(*gs_bmpMask));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Stipple mono"), x + 10, y + 10);

    y += HEIGHT;
//    dc.SetBrush(wxBrush(*gs_bmpNoMask));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Stipple colour"), x + 10, y + 10);
}

void MyCanvas::DrawTestPoly(wxDC& dc)
{
    wxBrush brushHatch(*wxRED, wxFDIAGONAL_HATCH);
    dc.SetBrush(brushHatch);

    wxPoint star[5];
    star[0] = wxPoint(100, 60);
    star[1] = wxPoint(60, 150);
    star[2] = wxPoint(160, 100);
    star[3] = wxPoint(40, 100);
    star[4] = wxPoint(140, 150);

    dc.DrawText(_T("You should see two (irregular) stars below, the left one ")
                _T("hatched"), 10, 10);
    dc.DrawText(_T("except for the central region and the right ")
                _T("one entirely hatched"), 10, 30);
    dc.DrawText(_T("The third star only has a hatched outline"), 10, 50);

    dc.DrawPolygon(WXSIZEOF(star), star, 0, 30);
    dc.DrawPolygon(WXSIZEOF(star), star, 160, 30, wxWINDING_RULE);

    wxPoint star2[10];
    star2[0] = wxPoint(0, 100);
    star2[1] = wxPoint(-59, -81);
    star2[2] = wxPoint(95, 31);
    star2[3] = wxPoint(-95, 31);
    star2[4] = wxPoint(59, -81);
    star2[5] = wxPoint(0, 80);
    star2[6] = wxPoint(-47, -64);
    star2[7] = wxPoint(76, 24);
    star2[8] = wxPoint(-76, 24);
    star2[9] = wxPoint(47, -64);
    int count[2] = {5, 5};

    dc.DrawPolyPolygon(WXSIZEOF(count), count, star2, 450, 150);
}

void MyCanvas::DrawTestLines( int x, int y, int width, wxDC &dc )
{
    dc.SetPen( wxPen( wxT("black"), width, wxSOLID) );
    dc.SetBrush( *wxRED_BRUSH );
    dc.DrawText(wxString::Format(wxT("Testing lines of width %d"), width), x + 10, y - 10);
    dc.DrawRectangle( x+10, y+10, 100, 190 );

    dc.DrawText(_T("Solid/dot/short dash/long dash/dot dash"), x + 150, y + 10);
    dc.SetPen( wxPen( wxT("black"), width, wxSOLID) );
    dc.DrawLine( x+20, y+20, 100, y+20 );
    dc.SetPen( wxPen( wxT("black"), width, wxDOT) );
    dc.DrawLine( x+20, y+30, 100, y+30 );
    dc.SetPen( wxPen( wxT("black"), width, wxSHORT_DASH) );
    dc.DrawLine( x+20, y+40, 100, y+40 );
    dc.SetPen( wxPen( wxT("black"), width, wxLONG_DASH) );
    dc.DrawLine( x+20, y+50, 100, y+50 );
    dc.SetPen( wxPen( wxT("black"), width, wxDOT_DASH) );
    dc.DrawLine( x+20, y+60, 100, y+60 );

    dc.DrawText(_T("Misc hatches"), x + 150, y + 70);
    dc.SetPen( wxPen( wxT("black"), width, wxBDIAGONAL_HATCH) );
    dc.DrawLine( x+20, y+70, 100, y+70 );
    dc.SetPen( wxPen( wxT("black"), width, wxCROSSDIAG_HATCH) );
    dc.DrawLine( x+20, y+80, 100, y+80 );
    dc.SetPen( wxPen( wxT("black"), width, wxFDIAGONAL_HATCH) );
    dc.DrawLine( x+20, y+90, 100, y+90 );
    dc.SetPen( wxPen( wxT("black"), width, wxCROSS_HATCH) );
    dc.DrawLine( x+20, y+100, 100, y+100 );
    dc.SetPen( wxPen( wxT("black"), width, wxHORIZONTAL_HATCH) );
    dc.DrawLine( x+20, y+110, 100, y+110 );
    dc.SetPen( wxPen( wxT("black"), width, wxVERTICAL_HATCH) );
    dc.DrawLine( x+20, y+120, 100, y+120 );

    dc.DrawText(_T("User dash"), x + 150, y + 140);
    wxPen ud( wxT("black"), width, wxUSER_DASH );
    wxDash dash1[6];
    dash1[0] = 8;  // Long dash  <---------+
    dash1[1] = 2;  // Short gap            |
    dash1[2] = 3;  // Short dash           |
    dash1[3] = 2;  // Short gap            |
    dash1[4] = 3;  // Short dash           |
    dash1[5] = 2;  // Short gap and repeat +
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+140, 100, y+140 );
    dash1[0] = 5;  // Make first dash shorter
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+150, 100, y+150 );
    dash1[2] = 5;  // Make second dash longer
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+160, 100, y+160 );
    dash1[4] = 5;  // Make third dash longer
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+170, 100, y+170 );
}

void MyCanvas::DrawDefault(wxDC& dc)
{
    // mark the origin
    dc.DrawCircle(0, 0, 10);

#if !defined(wxMAC_USE_CORE_GRAPHICS) || !wxMAC_USE_CORE_GRAPHICS
    // GetPixel and FloodFill not supported by Mac OS X CoreGraphics
    // (FloodFill uses Blit from a non-wxMemoryDC)
    //flood fill using brush, starting at 1,1 and replacing whatever colour we find there
    dc.SetBrush(wxBrush(wxColour(128,128,0), wxSOLID));

    wxColour tmpColour ;
    dc.GetPixel(1,1, &tmpColour);
    dc.FloodFill(1,1, tmpColour, wxFLOOD_SURFACE);
#endif

    dc.DrawCheckMark(5, 80, 15, 15);
    dc.DrawCheckMark(25, 80, 30, 30);
    dc.DrawCheckMark(60, 80, 60, 60);

    // this is the test for "blitting bitmap into DC damages selected brush" bug
    wxCoord rectSize = m_std_icon.GetWidth() + 10;
    wxCoord x = 100;
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxGREEN_BRUSH );
    dc.DrawRectangle(x, 10, rectSize, rectSize);
    dc.DrawBitmap(m_std_icon, x + 5, 15, true);
    x += rectSize + 10;
    dc.DrawRectangle(x, 10, rectSize, rectSize);
    dc.DrawIcon(m_std_icon, x + 5, 15);
    x += rectSize + 10;
    dc.DrawRectangle(x, 10, rectSize, rectSize);

    // test for "transparent" bitmap drawing (it intersects with the last
    // rectangle above)
    //dc.SetBrush( *wxTRANSPARENT_BRUSH );

  //  if (m_smile_bmp.Ok())
   //     dc.DrawBitmap(m_smile_bmp, x + rectSize - 20, rectSize - 10, true);

    dc.SetBrush( *wxBLACK_BRUSH );
    dc.DrawRectangle( 0, 160, 1000, 300 );

    // draw lines
    wxBitmap bitmap(20,70);
    wxMemoryDC memdc;
    memdc.SelectObject( bitmap );
    memdc.SetBrush( *wxBLACK_BRUSH );
    memdc.SetPen( *wxWHITE_PEN );
    memdc.DrawRectangle(0,0,20,70);
    memdc.DrawLine( 10,0,10,70 );

    // to the right
    wxPen pen = *wxRED_PEN;
    memdc.SetPen(pen);
    memdc.DrawLine( 10, 5,10, 5 );
    memdc.DrawLine( 10,10,11,10 );
    memdc.DrawLine( 10,15,12,15 );
    memdc.DrawLine( 10,20,13,20 );

/*
    memdc.SetPen(*wxRED_PEN);
    memdc.DrawLine( 12, 5,12, 5 );
    memdc.DrawLine( 12,10,13,10 );
    memdc.DrawLine( 12,15,14,15 );
    memdc.DrawLine( 12,20,15,20 );
*/

    // same to the left
    memdc.DrawLine( 10,25,10,25 );
    memdc.DrawLine( 10,30, 9,30 );
    memdc.DrawLine( 10,35, 8,35 );
    memdc.DrawLine( 10,40, 7,40 );

    // XOR draw lines
    dc.SetPen(*wxWHITE_PEN);
    memdc.SetLogicalFunction( wxINVERT );
    memdc.SetPen( *wxWHITE_PEN );
    memdc.DrawLine( 10,50,10,50 );
    memdc.DrawLine( 10,55,11,55 );
    memdc.DrawLine( 10,60,12,60 );
    memdc.DrawLine( 10,65,13,65 );

    memdc.DrawLine( 12,50,12,50 );
    memdc.DrawLine( 12,55,13,55 );
    memdc.DrawLine( 12,60,14,60 );
    memdc.DrawLine( 12,65,15,65 );

    memdc.SelectObject( wxNullBitmap );
    dc.DrawBitmap( bitmap, 10, 170 );
    wxImage image = bitmap.ConvertToImage();
    image.Rescale( 60,210 );
    bitmap = wxBitmap(image);
    dc.DrawBitmap( bitmap, 50, 170 );

    // test the rectangle outline drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRectangle(150, 170, 49, 29);
    dc.DrawRectangle(200, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(250, 210, 250, 170);
    dc.DrawLine(260, 200, 150, 200);

    // test the rectangle filled drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 170, 49, 29);
    dc.DrawRectangle(350, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(400, 170, 400, 210);
    dc.DrawLine(300, 200, 410, 200);

    // a few more tests of this kind
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 220, 1, 1);
    dc.DrawRectangle(310, 220, 2, 2);
    dc.DrawRectangle(320, 220, 3, 3);
    dc.DrawRectangle(330, 220, 4, 4);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 230, 1, 1);
    dc.DrawRectangle(310, 230, 2, 2);
    dc.DrawRectangle(320, 230, 3, 3);
    dc.DrawRectangle(330, 230, 4, 4);

    // and now for filled rect with outline
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(500, 170, 49, 29);
    dc.DrawRectangle(550, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(600, 170, 600, 210);
    dc.DrawLine(500, 200, 610, 200);

    // test the rectangle outline drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRoundedRectangle(150, 270, 49, 29, 6);
    dc.DrawRoundedRectangle(200, 270, 49, 29, 6);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(250, 270, 250, 310);
    dc.DrawLine(150, 300, 260, 300);

    // test the rectangle filled drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRoundedRectangle(300, 270, 49, 29, 6);
    dc.DrawRoundedRectangle(350, 270, 49, 29, 6);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(400, 270, 400, 310);
    dc.DrawLine(300, 300, 410, 300);

    // Added by JACS to demonstrate bizarre behaviour.
    // With a size of 70, we get a missing red RHS,
    // and the height is too small, so we get yellow
    // showing. With a size of 40, it draws as expected:
    // it just shows a white rectangle with red outline.
    int totalWidth = 70;
    int totalHeight = 70;
    wxBitmap bitmap2(totalWidth, totalHeight);

    wxMemoryDC memdc2;
    memdc2.SelectObject(bitmap2);

    wxColour clr(255, 255, 0);
    wxBrush yellowBrush(clr, wxSOLID);
    memdc2.SetBackground(yellowBrush);
    memdc2.Clear();

    wxPen yellowPen(clr, 1, wxSOLID);

    // Now draw a white rectangle with red outline. It should
    // entirely eclipse the yellow background.
    memdc2.SetPen(*wxRED_PEN);
    memdc2.SetBrush(*wxWHITE_BRUSH);

    memdc2.DrawRectangle(0, 0, totalWidth, totalHeight);

    memdc2.SetPen(wxNullPen);
    memdc2.SetBrush(wxNullBrush);
    memdc2.SelectObject(wxNullBitmap);

    dc.DrawBitmap(bitmap2, 500, 270);

    // Repeat, but draw directly on dc
    // Draw a yellow rectangle filling the bitmap

    x = 600; int y = 270;
    dc.SetPen(yellowPen);
    dc.SetBrush(yellowBrush);
    dc.DrawRectangle(x, y, totalWidth, totalHeight);

    // Now draw a white rectangle with red outline. It should
    // entirely eclipse the yellow background.
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);

    dc.DrawRectangle(x, y, totalWidth, totalHeight);
}

int MyCanvas::DrawVariation(int &currPos, int *depth, int height,
							PGN::Move *pMove, wxDC& dc, wxSize *sz)
{
	int curr_pos = currPos;
	//dc.SetPen( *wxGREEN );
//	dc.SetTextForeground(*wxGREEN);

	dc.SetFont( fontNormal );
	if( pMove->pVarWhite != 0 )
	{
		CodeFold *codeFold = new CodeFold();
		codeFold->SetWhiteBlack(-1);

		codeFold->SetMovePtr(pMove);
		codeFold->SetRect( currPos,*depth,height,height );

		codeFold->SetType( CODE_FOLD_DISP_NODE );
		currLine->AddNode(codeFold);
		codeFold->SetLine(currLine);
					
				
		currPos += 20;

		if( pMove->IsTrueDisplayVariations() )
		{
			DrawVariationWhite(currPos, depth, height, pMove, dc, sz, 10);
			currPos = 0;
		
			m_dataPage.AddLine(currLine);
			currLine = new Line();
		}
		else 
		{
			codeFold->SetState(1);
		}


		//depth += height;

	}
	
	if( pMove->pVarBlack != 0 )
	{
		if(  pMove->pVarWhite == 0 )
		{
			CodeFold *codeFold = new CodeFold();
			codeFold->SetWhiteBlack(-1);

			codeFold->SetMovePtr(pMove);
			codeFold->SetRect( currPos,*depth,height,height );

			codeFold->SetType( CODE_FOLD_DISP_NODE );
			currLine->AddNode(codeFold);
			codeFold->SetLine(currLine);
			currPos += 20;

			//	depth += height;
			if( pMove->IsTrueDisplayVariations() )
			{
				DrawVariationBlack(currPos, depth, height, pMove, dc, sz, 10);
				currPos = 0;
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			}
			else codeFold->SetState(1);
			
		}
		else
		{
			//	depth += height;
			if( pMove->IsTrueDisplayVariations() )
			{
				DrawVariationBlack(currPos, depth, height, pMove, dc, sz, 10);
				currPos = 0;
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			}
			
		}


		
	}

	if( ((pMove->pVarWhite != 0) || (pMove->pVarBlack != 0)) && pMove->IsTrueDisplayVariations())
	{
		*depth += height;
	}

	dc.SetFont( fontBold );
	//dc.SetPen( *wxBLACK );
	//dc.SetTextForeground(*wxBLACK);

	return 0;
}

 int MyCanvas::DrawVariationBlack(int &currPos, int *depth, int height,
							PGN::Move *pMove, wxDC& dc, wxSize *sz, 
							int offset, char start, char end)
{

	int length;
	int descent;
	int localH;
	char buff2[512];
	//wxRichTextCtrl *m_richTextCtrl = m_owner->m_richTextCtrl;
	char annotationText[1024];
	bool renderInverse = false;
	int rectLeftOffset = 5;
	int rectRightOffset = 2;
	int varCount = 0;
	PGN::Move *pMoveLocal = pMove->pVarBlack;

	while( pMoveLocal )
	{
		PGN::Move *pNextIter = pMoveLocal;
		int lcurrPos = offset;
		*depth += height;
		m_dataPage.AddLine(currLine);
		currLine = new Line();
		bool firstMove = true;
		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxBOLD, false) );
		char varNum[10];
		varNum[0] = 'A'+varCount;//, varNum, 10);
		varNum[1] = '.';
		varNum[2] = 0;
	//	dc.DrawText(wxString(varNum), offset, depth);
		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, false) );
		lcurrPos+=10;
		
		while( pNextIter && 
			 ((pNextIter->white.length()>0)||
			  (pNextIter->black.length() > 0)||
			  (pNextIter->result.length()>0)))
		{

			if( firstMove )
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());

				}
				else
				{
					/*if( pNextIter->pUp->variation_count_black > 1 )
					{
						sprintf(buff2, "%s) %c%s...", pNextIter->variation_id, start, pNextIter->info.c_str());
					}
					else
					{*/
					//sprintf(buff2, "%c ", start );
					//dc.GetTextExtent( buff2, &length, &height, &descent );
					if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
					{
					//	dc.DrawText(wxString(start), lcurrPos, *depth);
					}
					currDataNode->SetWhiteBlack(BLACK);
					string startStr = ""; 
					startStr  += start;
					currDataNode->SetText( startStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					currLine->AddNode(currDataNode);
					//PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					//currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
						lcurrPos += 20;
					sprintf(buff2, "%s... ", pNextIter->info.c_str());
					currDataNode->SetWhiteBlack(BLACK);
					RenderMoveColor *mvCol = pNextIter->GetRenderColorBlack(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currDataNode->SetType( NUMBER_ONLY_DISP_NODE );
					//}
					
					
				}
				firstMove = false;
			}
			else
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
					currDataNode->SetWhiteBlack(WHITE);
					RenderMoveColor *mvCol = pNextIter->GetRenderColorWhite(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				}
				else
				{
					sprintf(buff2, "%s.%s ", pNextIter->info.c_str(),
						pNextIter->white.c_str());

					RenderMoveColor *mvCol = pNextIter->GetRenderColorWhite(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currDataNode->SetWhiteBlack(WHITE);
				}
			}
			dc.GetTextExtent( buff2, &length, &localH, &descent );


			renderInverse = false;
			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( (white_black_move == BLACK) ||((white_black_move==WHITE)&&(move_count==0)) )
				{
					renderInverse =true;
					//ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}
			
			
			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
				//	dc.SetBrush(*wxBLACK_BRUSH);
				//	dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
				//	dc.SetTextForeground(*wxWHITE);
				//	dc.DrawText(wxString(buff2), lcurrPos, *depth);
				//	dc.SetTextForeground(*wxBLACK);
				}
				//else
				//	dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			//currDataNode->SetWhiteBlack(WHITE);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			currLine->AddNode(currDataNode);

			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();
			
			renderInverse = false;
			int indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;


			lcurrPos += length;

			if( m_owner->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
			{
				DrawAnnotationWhite(lcurrPos, depth, height,
					pNextIter, dc, sz, offset);
			}

			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, "%s ", pNextIter->result.c_str());
					// ready to break
				}
				else if( pNextIter->black != "" )
				{
					sprintf(buff2, "%s ", pNextIter->black.c_str());
				}
				else
				{
				//	sprintf(buff2, " %c", end);
					// ready to break;
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, "%s ", pNextIter->result.c_str());
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
						sprintf(buff2, "%s ", pNextIter->black.c_str());
					else
					{
						sprintf(buff2, "%s ", pNextIter->black.c_str());
					}
				}
			}
			
			dc.GetTextExtent( buff2, &length, &localH, &descent );

			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( this->white_black_move == WHITE )
				{
					renderInverse =true;
					//ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}

			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
					//dc.SetBrush(*wxBLACK_BRUSH);
					//dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
					//dc.SetTextForeground(*wxWHITE);
					//dc.DrawText(wxString(buff2), lcurrPos, *depth);
					//dc.SetTextForeground(*wxBLACK);
				}
				//else
					//dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			currDataNode->SetWhiteBlack(BLACK);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			RenderMoveColor *mvCol = pNextIter->GetRenderColorBlack(m_owner->m_moveColorPallete);
			currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currLine->AddNode(currDataNode);
			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			renderInverse = false;

			lcurrPos += length;

			indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;

			if( m_owner->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
			{
				DrawAnnotationBlack(lcurrPos, depth, height,
					pNextIter, dc, sz, offset);
			}

			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					//dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					currLine->AddNode(currDataNode);
					RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
					lcurrPos += length;
				}
				else
				{
					//dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
					lcurrPos += length;
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					//dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
					lcurrPos += length;
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
					{
					//	dc.DrawText(wxString(end), lcurrPos, *depth);
						currDataNode->SetWhiteBlack(BLACK);
						string endStr = ""; 
						endStr  += end;
						currDataNode->SetText( endStr.c_str() );
						currDataNode->SetMovePtr(pNextIter);
						currDataNode->SetRect( lcurrPos,*depth,length,height );
						if( start == '[' )
							currDataNode->SetStyle(TEXT_DISP_NORMAL);
						else currDataNode->SetStyle(TEXT_DISP_ITALIC);
						currDataNode->SetType( BRACKET_DISP_NODE );
						RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
						currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
						currLine->AddNode(currDataNode);
						currDataNode->SetLine(currLine);
						pNextIter->SetLine(currLine);
						currDataNode = new TextDisplayNode();
						dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
						lcurrPos += length;
					}
					//else
					//	dc.DrawText(wxString(end), lcurrPos, depth);
				}
			}

				if( pNextIter->pVarWhite )
				{

					dc.SetFont( fontItalic );
					//dc.SetPen( *wxRED);
					//dc.SetTextForeground( *wxRED);
					CodeFold *codeFold = new CodeFold();
					codeFold->SetWhiteBlack(-1);

					codeFold->SetMovePtr(pNextIter);

					codeFold->SetRect( lcurrPos,*depth,height,height );

					codeFold->SetType( CODE_FOLD_DISP_NODE );
					currLine->AddNode(codeFold);
					codeFold->SetLine(currLine);
					
				
					lcurrPos += 20;

					if( pNextIter->IsTrueDisplayVariations() )
					{
						//DrawVariationWhite(currPos, depth, height, pMove, dc, sz, 10);
						//currPos = 0;
		
						int cpos = lcurrPos;
						DrawVariationWhite(cpos, depth, height,
								pNextIter, dc, sz, offset+10, '(',')');
					//lcurrPos = offset;
					//depth +=height;
						if( end == ')' )
						{
							dc.SetFont( fontItalic );
						}
						else
						{
							dc.SetFont( fontNormal );
						}
						//dc.SetPen( *wxGREEN );
						//dc.SetTextForeground( *wxGREEN);
						//*depth+= height;
						if( (pNextIter->pNext != NULL) && !(pNextIter->pVarBlack) && (pNextIter != pMoveLocal) && 
							(pNextIter->pNext->info!= ""))
						{
								*depth+= height;
							m_dataPage.AddLine(currLine);
							currLine = new Line();
						}
						lcurrPos = offset+30;
						//	m_dataPage.AddLine(currLine);
						//	currLine = new Line();
					}
					else 
					{
						codeFold->SetState(1);
					}

					
				}

			if( pNextIter != pMoveLocal )
			{


				if( pNextIter->pVarBlack )
				{
					dc.SetFont( fontItalic );
					//dc.SetTextForeground( *wxRED);

					if(  pNextIter->pVarWhite == 0  )
					{
						CodeFold *codeFold = new CodeFold();
						codeFold->SetWhiteBlack(-1);

						codeFold->SetMovePtr(pNextIter);
						codeFold->SetRect( lcurrPos,*depth,height,height );

						codeFold->SetType( CODE_FOLD_DISP_NODE );
						currLine->AddNode(codeFold);
						codeFold->SetLine(currLine);
					
				
						lcurrPos += 20;
					

						if( pNextIter->IsTrueDisplayVariations() )
						{

							int cpos = offset+10;
							DrawVariationBlack(cpos, depth, height,
									pNextIter, dc, sz, offset+10, '(',')');
							//lcurrPos = offset;
							//depth +=height;
							if( end == ')' )
							{
								dc.SetFont( fontItalic );
							}
							else
							{
								dc.SetFont( fontNormal );
							}
							//dc.SetTextForeground( *wxGREEN);
							//*depth+= height;
							if( pNextIter->pNext != NULL && (pNextIter->pNext->info!= ""))
							{
									*depth+= height;
									m_dataPage.AddLine(currLine);
								currLine = new Line();
							}
							lcurrPos = offset+30;
						}
						else 
						{
							codeFold->SetState(1);
						}
					}
					else
					{
						if( pNextIter->IsTrueDisplayVariations() )
						{

							int cpos = offset+10;
							DrawVariationBlack(cpos, depth, height,
									pNextIter, dc, sz, offset+10, '(',')');
							//lcurrPos = offset;
							//depth +=height;
							if( end == ')' )
							{
								dc.SetFont( fontItalic );
							}
							else
							{
								dc.SetFont( fontNormal );
							}
							//dc.SetTextForeground( *wxGREEN);
							//*depth+= height;
							if( pNextIter->pNext != NULL && (pNextIter->pNext->info!= ""))
							{
									*depth+= height;
									m_dataPage.AddLine(currLine);
								currLine = new Line();
							}
							lcurrPos = offset+30;
						}
					}
				}

				//if( pNextIter->pVarWhite || pNextIter->pVarBlack )
					
			}
			/*else
			{
				if( pNextIter->pVarBlack )
				{
						CodeFold *codeFold = new CodeFold();
						codeFold->SetWhiteBlack(-1);

						codeFold->SetMovePtr(pNextIter);
						codeFold->SetRect( lcurrPos,*depth,height,height );

						codeFold->SetType( CODE_FOLD_DISP_NODE );
						currLine->AddNode(codeFold);
						codeFold->SetLine(currLine);
					
				
						lcurrPos += 20;
				}
			}*/




			
			
			pNextIter = pNextIter->pNext;
		}
		lcurrPos = offset;

		//*depth +=height;
		varCount++;
//		PGN::Move* pLocalTemp = pMoveLocal;
		pMoveLocal = pMoveLocal->pVarBlack;


	
	}
	
//	currLine->AddNode(currDataNode);
//	m_dataPage.AddLine(currLine);
//	currLine = new Line();

	return 0;
}


 int MyCanvas::DrawVariationWhite(int &currPos, int *depth, int height,
							PGN::Move *pMove, wxDC& dc, wxSize *sz, 
							int offset, char start, char end)
{
	int length;
	int descent;
	int localH;
	char buff2[512];
	//wxRichTextCtrl *m_richTextCtrl = m_owner->m_richTextCtrl;
	char annotationText[1024];
	bool renderInverse = false;
	int rectLeftOffset = 5;
	int rectRightOffset = 2;
	int varCount = 0;
	
	PGN::Move *pMoveLocal = pMove->pVarWhite;


	while( pMoveLocal )
	{
		PGN::Move *pNextIter = pMoveLocal;
		int lcurrPos = offset;
		*depth += height;
		m_dataPage.AddLine(currLine);
		currLine = new Line();
		bool firstMove = true;

		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxBOLD, false) );
		char varNum[10];
		varNum[0] = 'A'+varCount;//, varNum, 10);
		varNum[1] = '.';
		varNum[2] = 0;
	//	dc.DrawText(wxString(varNum), offset, depth);
		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, false) );
		lcurrPos+=10;
		
		while( pNextIter && 
			 ((pNextIter->white.length() > 0)||(pNextIter->result.length()>0)) )
		{
			
			if( firstMove )
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					/*if( pNextIter->pUp->variation_count_white > 1 )
					{
						sprintf(buff2, "%s) %c%s.%s", pNextIter->variation_id, start, pNextIter->info.c_str(),
								pNextIter->white.c_str());
					}
					else
					{*/
					//sprintf(buff2, "%c ", start );
					//dc.GetTextExtent( buff2, &length, &height, &descent );
					//dc.DrawText(wxString(start), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(WHITE);
					string endStr = ""; 
					endStr  += start;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					mvCol = pNextIter->GetRenderColorWhite(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					lcurrPos += 20;

						sprintf(buff2, "%s.%s ", pNextIter->info.c_str(),
								pNextIter->white.c_str());
					//}
				}
				firstMove = false;
			}
			else
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					sprintf(buff2, "%s.%s ", pNextIter->info.c_str(),
						pNextIter->white.c_str());
					RenderMoveColor *mvCol = pNextIter->GetRenderColorWhite(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				}
			}

			dc.GetTextExtent( buff2, &length, &localH, &descent );


			renderInverse = false;
			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( (white_black_move == BLACK) ||((white_black_move==WHITE)&&(move_count==0)) )
				{
					renderInverse =true;
					//ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}
			
			
			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
				//	dc.SetBrush(*wxBLACK_BRUSH);
					//if( firstMove )
					//	dc.DrawRectangle(lcurrPos-rectLeftOffset+10,depth,length+rectRightOffset,height);
					//else
				//		dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
				//	dc.SetTextForeground(*wxWHITE);
				//	dc.DrawText(wxString(buff2), lcurrPos, *depth);
				//	dc.SetTextForeground(*wxBLACK);
				}
				//else
				//	dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			currDataNode->SetWhiteBlack(WHITE);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			currLine->AddNode(currDataNode);
			//PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
			//currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			int indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;
	//		if( firstMove )
		//		firstMove = false;



			
			renderInverse = false;


			lcurrPos += length;

			if( m_owner->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
			{
				DrawAnnotationWhite(lcurrPos, depth, height,
					pNextIter, dc, sz, offset);
			}

			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					sprintf(buff2, "%s ", pNextIter->black.c_str());
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
						sprintf(buff2, "%s ", pNextIter->black.c_str());
					else
						sprintf(buff2, "%s ", pNextIter->black.c_str());
				}
			}
			


			dc.GetTextExtent( buff2, &length, &localH, &descent );

			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( this->white_black_move == WHITE )
				{
					renderInverse =true;
					//ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}

			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
				//	dc.SetBrush(*wxBLACK_BRUSH);
				//	dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
				//	dc.SetTextForeground(*wxWHITE);
				//	dc.DrawText(wxString(buff2), lcurrPos, *depth);
				//	dc.SetTextForeground(*wxBLACK);
				}
				//else
					//dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			currDataNode->SetWhiteBlack(BLACK);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			
			RenderMoveColor *mvCol = pNextIter->GetRenderColorBlack(m_owner->m_moveColorPallete);
			currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currLine->AddNode(currDataNode);
			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			renderInverse = false;
			indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;


			lcurrPos += length;
			
			if( m_owner->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
			{
				DrawAnnotationBlack(lcurrPos, depth, height,
					pNextIter, dc, sz, offset);
			}

			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					//dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
					lcurrPos += length;
				}
				else
				{
					//dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
					lcurrPos += length;
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
				//	dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
					lcurrPos += length;
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
					{
				//		dc.DrawText(wxString(end), lcurrPos, *depth);
						currDataNode->SetWhiteBlack(BLACK);
						string endStr = ""; 
						endStr  += end;
						currDataNode->SetText( endStr.c_str() );
						currDataNode->SetMovePtr(pNextIter);
						currDataNode->SetRect( lcurrPos,*depth,length,height );
						if( start == '[' )
							currDataNode->SetStyle(TEXT_DISP_NORMAL);
						else currDataNode->SetStyle(TEXT_DISP_ITALIC);
						currDataNode->SetType( BRACKET_DISP_NODE );
						RenderMoveColor *mvCol = pNextIter->GetRenderColor(m_owner->m_moveColorPallete);
						currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
						currLine->AddNode(currDataNode);
						currDataNode->SetLine(currLine);
						pNextIter->SetLine(currLine);
						currDataNode = new TextDisplayNode();
						dc.GetTextExtent( endStr.c_str(), &length, &localH, &descent );
						lcurrPos += length;
					}
					//else
					//	dc.DrawText(wxString(end), lcurrPos, depth);
				}
			}

			if( pNextIter != pMoveLocal )
			{
				if( pNextIter->pVarWhite )
				{
										
					dc.SetFont( fontItalic );
					//dc.SetPen( *wxRED);
					//dc.SetTextForeground( *wxRED);
					CodeFold *codeFold = new CodeFold();
					codeFold->SetWhiteBlack(-1);

					codeFold->SetMovePtr(pNextIter);
					codeFold->SetRect( lcurrPos,*depth,height,height );

					codeFold->SetType( CODE_FOLD_DISP_NODE );
					currLine->AddNode(codeFold);
					codeFold->SetLine(currLine);
					
				
					lcurrPos += 20;

					if( pNextIter->IsTrueDisplayVariations() )
					{
						int cpos = lcurrPos;
						DrawVariationWhite(cpos, depth, height,
								pNextIter, dc, sz, offset+10, '(',')');
						//lcurrPos = offset;
						//depth +=height;
						if( end == ')' )
						{
							dc.SetFont( fontItalic );
						}
						else
						{
							dc.SetFont( fontNormal );
						}
						//dc.SetTextForeground( *wxGREEN);
				//		*depth+= height;
						if( (pNextIter->pNext != NULL) && ((pNextIter->pNext->info!= "")) && (!pNextIter->pVarBlack))
						{
							*depth+= height;
							m_dataPage.AddLine(currLine);
							currLine = new Line();
						}
						lcurrPos = offset+30;
					}
					else
					{
						codeFold->SetState(1);
					}
				}

				//if( pNextIter->pVarWhite || pNextIter->pVarBlack )
				//	lcurrPos = offset+10;
			}

				if( pNextIter->pVarBlack )
				{
					dc.SetFont( fontItalic );
					//dc.SetPen( *wxRED);
					//dc.SetTextForeground( *wxRED);
					if( (pNextIter->pVarWhite == 0) || (pNextIter == pMoveLocal) )
					{
						CodeFold *codeFold = new CodeFold();
						codeFold->SetWhiteBlack(-1);

						codeFold->SetMovePtr(pNextIter);
						codeFold->SetRect( lcurrPos,*depth,height,height );

						codeFold->SetType( CODE_FOLD_DISP_NODE );
						currLine->AddNode(codeFold);
						codeFold->SetLine(currLine);
					
				
						lcurrPos += 20;

						if( pNextIter->IsTrueDisplayVariations() )
						{
							int cpos = offset+10;
							DrawVariationBlack(cpos, depth, height,
									pNextIter, dc, sz, offset+10, '(',')');
							//lcurrPos = offset;
							//depth +=height;
							if( end == ')' )
							{
								dc.SetFont( fontItalic );
							}
							else
							{
								dc.SetFont( fontNormal );
							}
							//dc.SetTextForeground( *wxGREEN);
							//*depth+= height;
							if( pNextIter->pNext != NULL && ((pNextIter->pNext->info!= "")))
							{
								*depth+= height;
								m_dataPage.AddLine(currLine);
								currLine = new Line();
							}
							lcurrPos = offset+30;
						}
						else
						{
							codeFold->SetState(1);
						}
					}
					else
					{
						if( pNextIter->IsTrueDisplayVariations() )
						{
							int cpos = offset+10;
							DrawVariationBlack(cpos, depth, height,
									pNextIter, dc, sz, offset+10, '(',')');
							//lcurrPos = offset;
							//depth +=height;
							if( end == ')' )
							{
								dc.SetFont( fontItalic );
							}
							else
							{
								dc.SetFont( fontNormal );
							}
							//dc.SetTextForeground( *wxGREEN);
							//*depth+= height;
							if( pNextIter->pNext != NULL && ((pNextIter->pNext->info!= "")))
							{
								*depth+= height;
								m_dataPage.AddLine(currLine);
								currLine = new Line();
							}
							lcurrPos = offset+30;
						}
					}
					
				}

			pNextIter = pNextIter->pNext;
		}
		lcurrPos = offset;
		//*depth +=height;
		varCount++;

		//PGN::Move* pLocalTemp = pMoveLocal;
		pMoveLocal = pMoveLocal->pVarWhite;


	}
//	currLine->AddNode(currDataNode);
//	m_dataPage.AddLine(currLine);
//	currLine = new Line();
	
	return 0;
 }


 int MyCanvas::DrawAnnotationWhite(int &currPos, int *depth, int height,
	 PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset)
 {
	int buff5len=0;
	int buff5h, buff5ext;
	for( int aa = 0; aa < pMove->annotation_white.text.size(); aa++ )
	{
		//if(( pMove->annotation_white.text[aa] == "{") || ( pMove->annotation_white.text[aa] == "}"))
		//	continue;

		dc.GetTextExtent( pMove->annotation_white.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
		if (currPos + buff5len > sz->GetWidth()) 
		{
			if (currPos > 0) 
			{ 
				// newline
				*depth += height;//*1.5;
				currPos = offset+10; 
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			} 
		}
		//dc.SetFont( f1 );
		dc.SetTextForeground(*wxBLUE);
	//	if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
		{
			//dc.DrawText(wxString(pMove->annotation_white.text[aa].c_str()), currPos, *depth);
			//AnnotationDislayNode
		}
		AnnotationDislayNode *textNode = new AnnotationDislayNode();
		textNode->SetWhiteBlack(-1);

		textNode->SetMovePtr(pMove);
		textNode->SetRect( currPos,*depth,buff5len,buff5h );
		textNode->SetText(pMove->annotation_white.text[aa].c_str());
		textNode->SetColour(annotateTextColor);
		textNode->SetType( ANNOTATION_DISP_NODE );
		currLine->AddNode(textNode);
		textNode->SetLine(currLine);			

		currPos += buff5len;

		/*currDataNode->SetWhiteBlack(WHITE);
		currDataNode->SetText( pMove->annotation_white.text[aa].c_str( );
		currDataNode->SetMovePtr(pNextIter);
		currDataNode->SetRect( lcurrPos,*depth,length,height );
		currDataNode->SetStyle(TEXT_DISP_NORMAL);
		currLine->AddNode(currDataNode);
			//PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
			//currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
		currDataNode->SetLine(currLine);
		pNextIter->SetLine(currLine);
		currDataNode = new AnnotationDislayNode();
						*/
	}
	currPos += buff5len;

	dc.SetTextForeground(*wxBLACK);

	return 0;
 }

  int MyCanvas::DrawAnnotationBlack(int &currPos, int *depth, int height,
	 PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset)
 {
	int buff5len=0;
	int buff5h, buff5ext;
	for( int aa = 0; aa < pMove->annotation_black.text.size(); aa++ )
	{
		//if(( pMove->annotation_black.text[aa] == "{") || ( pMove->annotation_black.text[aa] == "}"))
		//	continue;			
		dc.GetTextExtent( pMove->annotation_black.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
		if (currPos + buff5len > sz->GetWidth()) 
		{
			if (currPos > 0) 
			{ 
				// newline
				*depth += height;//*1.5;
				currPos = offset+10; 
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			} 
		}
		//dc.SetFont( f1 );
		dc.SetTextForeground(*wxBLUE);
		//if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
		{
			//dc.DrawText(wxString(pMove->annotation_black.text[aa].c_str()), currPos, *depth);
			//wnd10->AppendText( wxString(pMove->annotation_black.text[aa].c_str()));
		}
		AnnotationDislayNode *textNode = new AnnotationDislayNode();
		textNode->SetWhiteBlack(-1);
		textNode->SetText(pMove->annotation_black.text[aa].c_str());
		textNode->SetMovePtr(pMove);
		textNode->SetRect( currPos,*depth,buff5len,buff5h );
		textNode->SetColour(annotateTextColor);
		textNode->SetType( ANNOTATION_DISP_NODE );
		currLine->AddNode(textNode);
		textNode->SetLine(currLine);	
		currPos += buff5len;
						
	}
	currPos += buff5len;

	dc.SetTextForeground(*wxBLACK);

	return 0;
 }

  /*
  |
  |_______________  depth- sz->GetHeight()  scrollY
  |
  |
  |______________   sz->GetHeight()
  |
  |   X             depth
  |
  |
  |________________ 

  depth > sz->GetHeight()

  */
void MyCanvas::ScrollTest(int depth, wxSize *sz)
{
	static bool scrolled = false;
	static bool scrolledTwice = false;

	int pixPerUnitX, pixPerUnitY;
	//this->GetScro
	GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);

	//scrollX = 0;
	//scrollY = 0;
	//int distanceToScroll = depth - sz->GetHeight();
	if(!scrolled)
	{
		if( (depth  > sz->GetHeight()) )
		{
		//if( scrollY < depth - sz->GetHeight() )
			this->Scroll(0, depth / pixPerUnitY );
			scrollY++;
			//this->SetScrollPos(0, scrollY);
			scrolled = true;
		}
	}

	else if( depth  > scrollY* sz->GetHeight() )
		{
			//if( !scrolledTwice )
		//if( scrollY < depth - sz->GetHeight() )
			//this->Scroll(0, sz->GetHeight() - scrollY);
			//scrollY += sz->GetHeight();
			{
				this->Scroll(0, depth / pixPerUnitY);
				//scrolledTwice = true;
				scrollY++;
			}
			
			//this->SetScrollPos(0, scrollY);
			
		}
	else if( depth < (scrollY-1) *sz->GetHeight() )
	{
			{
				this->Scroll(0, -depth / pixPerUnitY);
				//scrolledTwice = true;
				scrollY--;
			}
	}
	/*else if( depth < scrollY + sz->GetHeight() - 20)
	{
		this->Scroll(0, - depth + sz->GetHeight() + scrollY - 20);
		scrollY = -depth + sz->GetHeight() + scrollY - 20;
	}*/
	
}
/* 
 
 */
// dave NOTE: TEXTCALL
// wxBufferedPaintDC?
void MyCanvas::DrawText(wxDC& dc)
{
	wxSize sz(GetClientSize());

	w = sz.GetWidth();
	h = sz.GetHeight();
	//CalcUnscrolledPosition	(0,0,&xx,&yy );	

	//this->ScrollWindow(0,0);
    // set underlined font for testing
    dc.SetFont( fontNormal );
	char* buff = new char[360];
	int i = 0;
	char buff2[360];
	char buff3[100];
	char buff4[100];
	char buff5[360];
	
	int xx = 0;
	int yy=0;
	int curLineLength = 0;

	//wxNativeFontInfo f1Info;
	//f1Info.
	//wxFONTWEIGHT_MAX
						//wxFONTSTYLE_ITALIC
	wxFont f1(fontBold);

	m_dataPage.Clear();

	currLine = new Line();
	currDataNode = new TextDisplayNode();

	
	int down = 10;
	PGN::Move* pMove;
	if( m_owner->currentGame != NULL )
	{
		pMove= this->m_owner->currentGame->pRoot;
		
		int length;
		int height;
		int descent;

		dc.SetFont( f1);
		dc.GetTextExtent( "Some Text", &length, &height, &descent );
		while ( pMove )
		{
		
			bool bAddedResultThisPass = false;

			if( pMove == NULL )
				break;

			dc.SetFont( f1);



			sprintf(buff2, "%s. %s %s",pMove->info.c_str(),
				pMove->white.c_str(),
				pMove->black.c_str());


			


			
			/*if( pMove->atEndBlack && pMove->result != "" )
			{
				sprintf( buff3, " %s", pMove->result.c_str());
			}
			else*/
			{
				if( pMove->white == "" )
				{
					if( pMove->result != "" )
					{
						sprintf( buff3, "%s ", pMove->result.c_str());
						RenderMoveColor *mvCol = pMove->GetRenderColorBlack(m_owner->m_moveColorPallete);
						currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
						bAddedResultThisPass = true;
					}
					else
					{
						if( pMove == this->m_owner->currentGame->pRoot )
						{
							sprintf( buff3, "%s... ", pMove->info.c_str());
							RenderMoveColor *mvCol = pMove->GetRenderColorBlack(m_owner->m_moveColorPallete);
							currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
						}
						else
						{

							sprintf( buff3, "* ");
							RenderMoveColor *mvCol = pMove->GetRenderColorBlack(m_owner->m_moveColorPallete);
							currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
							bAddedResultThisPass = true;
						}
					}
				}
				else
				{
					sprintf( buff3, "%s.%s ", pMove->info.c_str(),
					pMove->white.c_str());
					RenderMoveColor *mvCol = pMove->GetRenderColorWhite(m_owner->m_moveColorPallete);
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				}
			}


			int buff3len, buff3h, buff3ext;
			dc.GetTextExtent( buff3, &buff3len, &buff3h, &buff3ext );

			line_height = buff3h;

			if (curLineLength + buff3len > sz.GetWidth()) 
			{
				if (curLineLength > 0) 
				{ 
					// newline
					down += height;//*1.5;
					curLineLength = 0; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}

			bool renderInverse = false;
		
			if( pMove == this->m_owner->currentGame->pCurrMove )
			{
				if( (white_black_move == BLACK) ||((white_black_move==WHITE)&&(move_count==0)) )
				{
					renderInverse =true;
					//ScrollTest(down, &sz);
				}
			}

		
			int rectLeftOffset = 5;
			int rectRightOffset = 2;

			if( (down > YY) && (down < sz.GetHeight() + YY))
			{
				if( renderInverse )
				{
					//dc.SetBrush(*wxBLACK_BRUSH);
					//dc.DrawRectangle(curLineLength-rectLeftOffset,down,buff3len+rectRightOffset,buff3h);
					//dc.SetTextForeground(*wxWHITE);
					//dc.DrawText(wxString(buff3), curLineLength, down);
			
				}
				//else
					//dc.DrawText(wxString(buff3), curLineLength, down);
			}

			currDataNode->SetWhiteBlack(WHITE);
			currDataNode->SetText( buff3 );
			currDataNode->SetMovePtr(pMove);
			currDataNode->SetRect( curLineLength,down,buff3len,buff3h );
			currDataNode->SetStyle(TEXT_DISP_BOLD);
			currLine->AddNode(currDataNode);
			//PGN::RenderMoveColor *mvCol = pMove->GetRenderColor();
			//currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currDataNode->SetLine(currLine);
			pMove->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			renderInverse =false;

			curLineLength+= buff3len;
			/*int buff5len=0;
			int buff5h, buff5ext;
			for( int aa = 0; aa < pMove->annotation_white.text.size(); aa++, curLineLength += buff5len )
			{
			
				dc.GetTextExtent( pMove->annotation_white.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
				if (curLineLength + buff5len > sz.GetWidth()) 
				{
					if (curLineLength > 0) 
					{ 
						// newline
						down += height;//*1.5;
						curLineLength = 0; 
					} 
				}
				dc.SetFont( f1 );
				dc.SetTextForeground(*wxBLUE);
				dc.DrawText(wxString(pMove->annotation_white.text[aa].c_str()), curLineLength, down);
						
			}*/

			if( m_owner->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
			{
				DrawAnnotationWhite(curLineLength, &down, height,
					pMove, dc, &sz, 0);
			}

	//		sprintf( buff5, "%s", pMove->text_white);

		
			//dc.GetTextExtent( buff5, &buff5len, &buff5h, &buff5ext );



			dc.SetFont( f1 );
			dc.SetTextForeground(*wxBLACK);

			int buff4len, buff4h, buff4ext;

			if( (pMove->black == "") && 
				!(/*pMove == this->m_owner->currentGame->pRoot &&*/ (pMove->white == "")) )
			{
				if( pMove->result != "" )
				{
					sprintf( buff4, "%s ",
						pMove->result.c_str());
					bAddedResultThisPass = true;
				}
				else
				{
					sprintf( buff4, "* ");
					bAddedResultThisPass = true;
				}
			}
			else
			{
				sprintf( buff4, "%s ",
					pMove->black.c_str());
			}

			dc.GetTextExtent( buff4, &buff4len, &buff4h, &buff4ext );

		

			if( pMove == this->m_owner->currentGame->pCurrMove )
			{
				if( this->white_black_move == WHITE )
				{
					renderInverse =true;
					//ScrollTest(down, &sz);
				}
			}

			//if( (pMove->pNext) && (pMove->pNext)->atEndBlack )
			//	renderInverse =true;



			if (curLineLength + buff4len > sz.GetWidth()) 
			{
				if (curLineLength > 0) 
				{ 
						// newline
					down += height;//*1.5;
					curLineLength = 0; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 			
			}

			if( (down > YY) && (down < sz.GetHeight() + YY))
			{
				//if( renderInverse )
				//{
				///	dc.SetBrush(*wxBLACK_BRUSH);
				//	dc.DrawRectangle(curLineLength-rectLeftOffset,down,buff4len+rectRightOffset,buff4h);
				//	dc.SetTextForeground(*wxWHITE);
				//	dc.DrawText(wxString(buff4), curLineLength, down);
				//}
				//else
				//	dc.DrawText(wxString(buff4), curLineLength, down);
			}

			currDataNode->SetWhiteBlack(BLACK);
			currDataNode->SetText( buff4 );
			currDataNode->SetMovePtr(pMove);
			currDataNode->SetRect( curLineLength,down,buff4len,buff4h );
			currLine->AddNode(currDataNode);
			currDataNode->SetLine(currLine);
			currDataNode->SetStyle(TEXT_DISP_BOLD);
			RenderMoveColor *mvCol = pMove->GetRenderColorBlack(m_owner->m_moveColorPallete);
			currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			pMove->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			curLineLength += buff4len;

			/*for( int aa = 0; aa < pMove->annotation_black.text.size(); aa++, curLineLength += buff5len )
			{
			
				dc.GetTextExtent( pMove->annotation_black.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
				if (curLineLength + buff5len > sz.GetWidth()) 
				{
					if (curLineLength > 0) 
					{ 
						// newline
						down += height;//*1.5;
						curLineLength = 0; 
					} 
				}
				dc.SetFont( f1 );
				dc.SetTextForeground(*wxBLUE);
				dc.DrawText(wxString(pMove->annotation_black.text[aa].c_str()), curLineLength, down);
			
			
			}*/
			if( m_owner->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
			{
				DrawAnnotationBlack(curLineLength, &down, height,
					pMove, dc, &sz, 0);
			}
//			curLineLength += buff5len;
		
		
			dc.SetTextForeground(*wxBLACK);

			DrawVariation( curLineLength, &down, height, pMove, dc, &sz );
		
			needsRefresh = false;

			if( (pMove->pNext == NULL) && (bAddedResultThisPass == false))
			{
				currDataNode->SetWhiteBlack(BLACK);
				currDataNode->SetText( "* " );
				currDataNode->SetType(BRACKET_DISP_NODE);
				currDataNode->SetMovePtr(pMove);
				currDataNode->SetRect( curLineLength,down,buff4len,buff4h );
				currLine->AddNode(currDataNode);
				currDataNode->SetLine(currLine);
				currDataNode->SetStyle(TEXT_DISP_BOLD);
				RenderMoveColor *mvCol = pMove->GetRenderColorBlack(m_owner->m_moveColorPallete);
				currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				pMove->SetLine(currLine);
				currDataNode = new TextDisplayNode();
			}

			pMove = pMove->pNext;

			i++;
		}
		currLine->AddNode(currDataNode);
		m_dataPage.AddLine(currLine);
		currLine = new Line();


		//m_dataPage.MergeEqualNodesInLines();
	}
	int pixPerUnitX;
	int pixPerUnitY;
	GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);
	SetScrollbars( 10, 10, 50, down/pixPerUnitY + sz.GetHeight()/pixPerUnitY);	

}




static const struct
{
    const wxChar *name;
    int           rop;
} rasterOperations[] =
{
    { wxT("wxAND"),          wxAND           },
    { wxT("wxAND_INVERT"),   wxAND_INVERT    },
    { wxT("wxAND_REVERSE"),  wxAND_REVERSE   },
    { wxT("wxCLEAR"),        wxCLEAR         },
    { wxT("wxCOPY"),         wxCOPY          },
    { wxT("wxEQUIV"),        wxEQUIV         },
    { wxT("wxINVERT"),       wxINVERT        },
    { wxT("wxNAND"),         wxNAND          },
    { wxT("wxNO_OP"),        wxNO_OP         },
    { wxT("wxOR"),           wxOR            },
    { wxT("wxOR_INVERT"),    wxOR_INVERT     },
    { wxT("wxOR_REVERSE"),   wxOR_REVERSE    },
    { wxT("wxSET"),          wxSET           },
    { wxT("wxSRC_INVERT"),   wxSRC_INVERT    },
    { wxT("wxXOR"),          wxXOR           },
};

void MyCanvas::DrawImages(wxDC& dc)
{
    dc.DrawText(_T("original image"), 0, 0);
//    dc.DrawBitmap(*gs_bmpNoMask, 0, 20, 0);
    dc.DrawText(_T("with colour mask"), 0, 100);
//    dc.DrawBitmap(*gs_bmpWithColMask, 0, 120, true);
    dc.DrawText(_T("the mask image"), 0, 200);
//    dc.DrawBitmap(*gs_bmpMask, 0, 220, 0);
    dc.DrawText(_T("masked image"), 0, 300);
//    dc.DrawBitmap(*gs_bmpWithMask, 0, 320, true);

//    int cx = gs_bmpWithColMask->GetWidth(),
  //      cy = gs_bmpWithColMask->GetHeight();

    wxMemoryDC memDC;
    for ( size_t n = 0; n < WXSIZEOF(rasterOperations); n++ )
    {
        wxCoord x = 120 + 150*(n%4),
                y =  20 + 100*(n/4);

        dc.DrawText(rasterOperations[n].name, x, y - 20);
//        memDC.SelectObject(*gs_bmpWithColMask);
   //     dc.Blit(x, y, cx, cy, &memDC, 0, 0, rasterOperations[n].rop, true);
    }
}


#if wxUSE_GRAPHICS_CONTEXT
#ifdef __WXGTK20__
void MyCanvas::DrawAlpha(wxDC& no_dc)
#else
void MyCanvas::DrawAlpha(wxDC& dc)
#endif
{
#ifdef __WXGTK__
    wxGCDC dc( this );
    PrepareDC( dc );
#endif

    wxDouble margin = 20 ;
    wxDouble width = 180 ;
    wxDouble radius = 30 ;
    
    dc.SetPen( wxPen( wxColour( 128, 0, 0, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 255, 0, 0, 255),wxSOLID));
    
    wxRect r(margin,margin+width*0.66,width,width) ;
    
    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;
    
    dc.SetPen( wxPen( wxColour( 0, 0, 128, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 255),wxSOLID));
    
    r.Offset( width * 0.8 , - width * 0.66 ) ;
    
    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;
    
    dc.SetPen( wxPen( wxColour( 128, 128, 0, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 192, 192, 0, 255),wxSOLID));

    r.Offset( width * 0.8 , width *0.5 ) ;
    
    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;
    
    dc.SetPen( *wxTRANSPARENT_PEN ) ;
    dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
    dc.DrawRoundedRectangle( 0 , margin + width / 2 , width * 3 , 100 , radius) ;
    
    dc.SetTextForeground( wxColour(255,255,0,128) );
    dc.SetFont( wxFont( 40, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
    dc.DrawText( wxT("Hello!"), 120, 80 );
}

#endif

void MyCanvas::DrawCircles(wxDC& dc)
{
    int x = 100,
        y = 100,
        r = 20;

    dc.SetPen( *wxRED_PEN );
    dc.SetBrush( *wxGREEN_BRUSH );

    dc.DrawText(_T("Some circles"), 0, y);
    dc.DrawCircle(x, y, r);
    dc.DrawCircle(x + 2*r, y, r);
    dc.DrawCircle(x + 4*r, y, r);

    y += 2*r;
    dc.DrawText(_T("And ellipses"), 0, y);
    dc.DrawEllipse(x - r, y, 2*r, r);
    dc.DrawEllipse(x + r, y, 2*r, r);
    dc.DrawEllipse(x + 3*r, y, 2*r, r);

    y += 2*r;
    dc.DrawText(_T("And arcs"), 0, y);
    dc.DrawArc(x - r, y, x + r, y, x, y);
    dc.DrawArc(x + 4*r, y, x + 2*r, y, x + 3*r, y);
    dc.DrawArc(x + 5*r, y, x + 5*r, y, x + 6*r, y);

    y += 2*r;
    dc.DrawEllipticArc(x - r, y, 2*r, r, 0, 90);
    dc.DrawEllipticArc(x + r, y, 2*r, r, 90, 180);
    dc.DrawEllipticArc(x + 3*r, y, 2*r, r, 180, 270);
    dc.DrawEllipticArc(x + 5*r, y, 2*r, r, 270, 360);
    
    // same as above, just transparent brush
    
    dc.SetPen( *wxRED_PEN );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );

    y += 2*r;
    dc.DrawText(_T("Some circles"), 0, y);
    dc.DrawCircle(x, y, r);
    dc.DrawCircle(x + 2*r, y, r);
    dc.DrawCircle(x + 4*r, y, r);

    y += 2*r;
    dc.DrawText(_T("And ellipses"), 0, y);
    dc.DrawEllipse(x - r, y, 2*r, r);
    dc.DrawEllipse(x + r, y, 2*r, r);
    dc.DrawEllipse(x + 3*r, y, 2*r, r);

    y += 2*r;
    dc.DrawText(_T("And arcs"), 0, y);
    dc.DrawArc(x - r, y, x + r, y, x, y);
    dc.DrawArc(x + 4*r, y, x + 2*r, y, x + 3*r, y);
    dc.DrawArc(x + 5*r, y, x + 5*r, y, x + 6*r, y);

    y += 2*r;
    dc.DrawEllipticArc(x - r, y, 2*r, r, 0, 90);
    dc.DrawEllipticArc(x + r, y, 2*r, r, 90, 180);
    dc.DrawEllipticArc(x + 3*r, y, 2*r, r, 180, 270);
    dc.DrawEllipticArc(x + 5*r, y, 2*r, r, 270, 360);
    
}

void MyCanvas::DrawSplines(wxDC& dc)
{
#if wxUSE_SPLINES
    dc.DrawText(_T("Some splines"), 10, 5);

    // values are hardcoded rather than randomly generated
    // so the output can be compared between native
    // implementations on platforms with different random
    // generators

    const int R = 300;
    const wxPoint center( R + 20, R + 20 );
    const int angles[7] = { 0, 10, 33, 77, 13, 145, 90 };
    const int radii[5] = { 100 , 59, 85, 33, 90 };
    const int n = 200;
    wxPoint pts[n];

    // background spline calculation
    unsigned int radius_pos = 0;
    unsigned int angle_pos = 0;
    int angle = 0;
    for ( int i = 0; i < n; i++ )
    {
        angle += angles[ angle_pos ];
        int r = R * radii[ radius_pos ] / 100;
        pts[ i ].x = center.x + (wxCoord)( r * cos( M_PI * angle / 180.0) );
        pts[ i ].y = center.y + (wxCoord)( r * sin( M_PI * angle / 180.0) );

        angle_pos++;
        if ( angle_pos >= WXSIZEOF(angles) ) angle_pos = 0;

        radius_pos++;
        if ( radius_pos >= WXSIZEOF(radii) ) radius_pos = 0;
    }

    // background spline drawing
    dc.SetPen(*wxRED_PEN);
    dc.DrawSpline(WXSIZEOF(pts), pts);

    // less detailed spline calculation
    wxPoint letters[4][5];
    // w
    letters[0][0] = wxPoint( 0,1); //  O           O
    letters[0][1] = wxPoint( 1,3); //   *         *
    letters[0][2] = wxPoint( 2,2); //    *   O   *
    letters[0][3] = wxPoint( 3,3); //     * * * *
    letters[0][4] = wxPoint( 4,1); //      O   O
    // x1
    letters[1][0] = wxPoint( 5,1); //  O*O
    letters[1][1] = wxPoint( 6,1); //     *
    letters[1][2] = wxPoint( 7,2); //      O
    letters[1][3] = wxPoint( 8,3); //       *
    letters[1][4] = wxPoint( 9,3); //        O*O
    // x2
    letters[2][0] = wxPoint( 5,3); //        O*O
    letters[2][1] = wxPoint( 6,3); //       *
    letters[2][2] = wxPoint( 7,2); //      O
    letters[2][3] = wxPoint( 8,1); //     *
    letters[2][4] = wxPoint( 9,1); //  O*O
    // W
    letters[3][0] = wxPoint(10,0); //  O           O
    letters[3][1] = wxPoint(11,3); //   *         *
    letters[3][2] = wxPoint(12,1); //    *   O   *
    letters[3][3] = wxPoint(13,3); //     * * * *
    letters[3][4] = wxPoint(14,0); //      O   O

    const int dx = 2 * R / letters[3][4].x;
    const int h[4] = { -R/2, 0, R/4, R/2 };

    for ( int m = 0; m < 4; m++ )
    {
        for ( int n = 0; n < 5; n++ )
        {
            letters[m][n].x = center.x - R + letters[m][n].x * dx;
            letters[m][n].y = center.y + h[ letters[m][n].y ];
        }

        dc.SetPen( wxPen( wxT("blue"), 1, wxDOT) );
        dc.DrawLines(5, letters[m]);
        dc.SetPen( wxPen( wxT("black"), 4, wxSOLID) );
        dc.DrawSpline(5, letters[m]);
    }

#else
    dc.DrawText(_T("Splines not supported."), 10, 5);
#endif
}

void MyCanvas::DrawGradients(wxDC& dc)
{
    static const int TEXT_HEIGHT = 15;

    // LHS: linear
    wxRect r(10, 10, 50, 50);
    dc.DrawText(_T("wxRIGHT"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxRIGHT);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("wxLEFT"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxLEFT);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("wxDOWN"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxDOWN);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("wxUP"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxUP);


    // RHS: concentric
    r = wxRect(200, 10, 50, 50);
    dc.DrawText(_T("Blue inside"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("White inside"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxWHITE, *wxBLUE);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("Blue in top left corner"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE, wxPoint(0, 0));

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("Blue in bottom right corner"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE, wxPoint(r.width, r.height));
}

void MyCanvas::DrawRegions(wxDC& dc)
{
    dc.DrawText(_T("You should see a red rect partly covered by a cyan one ")
                _T("on the left"), 10, 5);
    dc.DrawText(_T("and 5 smileys from which 4 are partially clipped on the right"),
                10, 5 + dc.GetCharHeight());
    dc.DrawText(_T("The second copy should be identical but right part of it ")
                _T("should be offset by 10 pixels."),
                10, 5 + 2*dc.GetCharHeight());

    DrawRegionsHelper(dc, 10, true);
    DrawRegionsHelper(dc, 350, false);
}

void MyCanvas::DrawRegionsHelper(wxDC& dc, wxCoord x, bool firstTime)
{
    wxCoord y = 100;

    dc.DestroyClippingRegion();
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.SetPen( *wxTRANSPARENT_PEN );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.SetClippingRegion( x + 10, y + 10, 100, 270 );

    dc.SetBrush( *wxRED_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.SetClippingRegion( x + 10, y + 10, 100, 100 );

    dc.SetBrush( *wxCYAN_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.DestroyClippingRegion();

    wxRegion region(x + 110, y + 20, 100, 270);
#if !defined(__WXMOTIF__) && !defined(__WXMAC__)
    if ( !firstTime )
        region.Offset(10, 10);
#endif
    dc.SetClippingRegion(region);

    dc.SetBrush( *wxGREY_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

   /* if (m_smile_bmp.Ok())
    {
        dc.DrawBitmap( m_smile_bmp, x + 150, y + 150, true );
        dc.DrawBitmap( m_smile_bmp, x + 130, y + 10,  true );
        dc.DrawBitmap( m_smile_bmp, x + 130, y + 280, true );
        dc.DrawBitmap( m_smile_bmp, x + 100, y + 70,  true );
        dc.DrawBitmap( m_smile_bmp, x + 200, y + 70,  true );
    }*/
}

void MyCanvas::OnPaint(wxPaintEvent &event)
{
	m_useContext = false;
    wxPaintDC pdc(this);
	wxSize sz(GetClientSize());
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

    m_owner->PrepareDC(dc);

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

    /*if ( m_clip )
	{
		
		CalcUnscrolledPosition	(0,0,&XX,&YY );	
		dc.SetClippingRegion(XX, YY, sz.GetWidth(), sz.GetHeight());
	}*/
	//dc.
    //dc.Clear();

/*    if ( m_owner->m_textureBackground )
    {
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        for ( int i = 0; i < 200; i++ )
            dc.DrawLine(0, i*10, i*10, 0);
    }*/
	m_show = Show_Text;

	if( needsRefresh )
	{
		switch ( m_show )
		{
			case Show_Default:
				DrawDefault(dc);
				break;

			case Show_Circles:
				DrawCircles(dc);
				break;

			case Show_Splines:
				DrawSplines(dc);
				break;

			case Show_Regions:
				DrawRegions(dc);
				break;

			case Show_Text:
				DrawText(dc);
				Refresh(false);
				//DrawGraphics(gdc.GetGraphicsContext());
				break;

			case Show_Lines:
				DrawTestLines( 0, 100, 0, dc );
				DrawTestLines( 0, 320, 1, dc );
				DrawTestLines( 0, 540, 2, dc );
				DrawTestLines( 0, 760, 6, dc );
				break;

			case Show_Brushes:
				DrawTestBrushes(dc);
				break;

			case Show_Polygons:
				DrawTestPoly(dc);
				break;

			case Show_Mask:
				DrawImages(dc);
				break;

			case Show_Ops:
			  //  DrawWithLogicalOps(dc);
				break;
        
	#if wxUSE_GRAPHICS_CONTEXT
			case Show_Alpha:
				DrawAlpha(dc);
				break;
	#endif

			case Show_Gradient:
				DrawGradients(dc);
				break;

			default:
				break;
		}
	}
	else
	{
#ifdef USE_GRAPHICS_CONTEXT
wxGraphicsContext *gc = gdc.GetGraphicsContext();
		Scroll(0, 0);
		wxFont f3(10, wxMODERN, wxITALIC,wxNORMAL, false/*,"Arial Baltic"*/);
		wxFont f2(10, wxMODERN, wxNORMAL,wxBOLD, false/*,"Arial Baltic"*/);
		wxFont f1(10, wxMODERN, wxNORMAL,wxNORMAL, false/*,"Arial Baltic"*/);
		//wxGraphicsFont f1;
		gc->SetFont(f1, *wxBLACK);
		gc->SetPen(*wxBLACK);

		int fontHeight; 
		int fontWidth;
		int depth;

		wxSize textSize = dc.GetTextExtent("ABC");
		

		int lines_per_page = sz.GetHeight() / textSize.GetHeight();
		int pixPerUnitX;
		int pixPerUnitY;
		GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);
		
		int localXX, localYY;
		CalcUnscrolledPosition( 0,0, &localXX, &localYY);	
		
		int start = 0;
		int lineY = 0;
		// scroll here
		for( int i = 0; i < m_dataPage.lines.size(); i++ )
		{
			if( m_dataPage.lines[i] == m_owner->currentGame->pCurrMove->m_line )
			{
				start = i;
				lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
				break;
			}
		}
		if( lineY-localYY > sz.GetHeight() - textSize.GetHeight() )
		{
			Scroll(0, lineY/pixPerUnitY);
		}
		else if( localYY == 0 )
		{
			start = 0;
		}
		else 
		{
			for( int i = 0; i < m_dataPage.lines.size(); i++ )
			{
				if( m_dataPage.lines[i]->nodes[0]->m_rect.y == localYY )
				{
					start = i;
					//lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
					break;
				}
			}
		}

		for( int i = start; i < m_dataPage.lines.size(); i++ )
		{
			if( m_dataPage.lines[i]->nodes.size() > 0 )
			{
				if( m_dataPage.lines[i]->nodes[0]->m_rect.y > lineY + sz.GetHeight() )
				{
					break;
				}
			}
			for( int j = 0; j < m_dataPage.lines[i]->nodes.size(); j++ )
			{
				if( m_dataPage.lines[i]->nodes[j]->GetStyle() ==  TEXT_DISP_NORMAL )
				{
				//	dc.SetFont(f1);
					gc->SetFont(f1, *wxBLACK);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_BOLD ) 
				{
				//	dc.SetFont(f2);
					gc->SetFont(f2, *wxBLACK);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_ITALIC )
				{
				//	dc.SetFont(f3);
					gc->SetFont(f3, *wxBLACK);
				}

				if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == BLACK &&
					 (m_owner->board_canvas->GetBoard()->mov == WHITE || 
						m_owner->board_canvas->GetBoard()->mov == 2 )) &&
						(m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
/*					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack == WHITE && 
						(m_owner->board_canvas->GetBoard()->mov == WHITE || 
						m_owner->board_canvas->GetBoard()->mov == 2 )) &&
					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK && 
						(m_owner->board_canvas->GetBoard()->mov != BLACK)))) ||
					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack != WHITE && 
						(m_owner->board_canvas->GetBoard()->mov != WHITE || 
						m_owner->board_canvas->GetBoard()->mov != 2 )) &&
					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack == BLACK && 
						(m_owner->board_canvas->GetBoard()->mov == BLACK)))))*/
				{
					int rectLeftOffset = 5;
					int rectRightOffset = 2;
					gc->SetBrush(*wxBLACK_BRUSH);
					gc->DrawRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height);

					gc->SetPen(*wxWHITE);
					gc->DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y);
					gc->SetPen(*wxBLACK);
				}
				else if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == WHITE &&
					 (m_owner->board_canvas->GetBoard()->mov == BLACK )) &&
					 (m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
				{
					int rectLeftOffset = 5;
					int rectRightOffset = 2;
					gc->SetBrush(*wxBLACK_BRUSH);
					gc->DrawRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height);

					gc->SetPen(*wxWHITE);
					gc->DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y);
					gc->SetPen(*wxBLACK);
				}
				else
				{
					gc->DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y);
				}
			}
		}
#endif

#ifndef USE_GRAPHICS_CONTEXT
		wxGraphicsContext *gc = gdc.GetGraphicsContext();
		//Scroll(0, 0);
		int localXX, localYY;
		CalcUnscrolledPosition( 0,0, &localXX, &localYY);	
		
		//wxFont f3(10, wxMODERN, wxITALIC,wxNORMAL, false/*,"Arial Baltic"*/);
		//wxFont f2(10, wxMODERN, wxNORMAL,wxBOLD, false/*,"Arial Baltic"*/);
		//wxFont f1(10, wxMODERN, wxNORMAL,wxNORMAL, false/*,"Arial Baltic"*/);
		dc.SetFont(fontNormal);

		int fontHeight; 
		int fontWidth;
		int depth;

		wxSize textSize = dc.GetTextExtent("A");
		

		int lines_per_page = sz.GetHeight() / textSize.GetHeight();
		int pixPerUnitX;
		int pixPerUnitY;
		GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);
		

		
		int start = 0;
		int lineY = 0;
		// scroll here

		if( this->rememberedScrollPos )
		{
			rememberedScrollPos = false;

				Scroll(0, 0);
				Scroll(0, remYY/pixPerUnitY);
				//start = 0;

				for( int i = 0; i < m_dataPage.lines.size(); i++ )
				{
					if(m_dataPage.lines[i]->nodes.size() == 0)
						continue;

					if( (m_dataPage.lines[i]->nodes[0]->m_rect.y > remYY-textSize.GetHeight()) && 
						(m_dataPage.lines[i]->nodes[0]->m_rect.y < remYY+textSize.GetHeight()))
					{
						start = i;
						lineY=remYY;//m_dataPage.lines[i]->nodes[0]->m_rect.y;
						break;
					}
				}
			
		}
		else if( MoveToCursor )
		{
			MoveToCursor = false;
			for( int i = 0; i < m_dataPage.lines.size(); i++ )
			{
				if( m_dataPage.lines[i] == m_owner->currentGame->pCurrMove->m_line &&
					m_dataPage.lines[i]->nodes.size() != 0 )

				{
					start = i;
					lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
					break;
				}
			}
			if( lineY-localYY > sz.GetHeight() - textSize.GetHeight()*2 )//???
			{

				Scroll(0, lineY/pixPerUnitY);
				/*for( int i = 0; i < m_dataPage.lines.size(); i++ )
				{
					if( (m_dataPage.lines[i]->nodes[0]->m_rect.y > localYY-10) && 
						(m_dataPage.lines[i]->nodes[0]->m_rect.y < localYY+10))
					{
						start = i;
						//lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
						break;
					}
				}*/

			}
			else if (lineY < localYY)
			{
				Scroll(0, 0);
				Scroll(0, lineY/pixPerUnitY);
				//start = 0;
			}
			else if( localYY == 0 )
			{
				start = 0;
			}	
			else
			{
				for( int i = 0; i < m_dataPage.lines.size(); i++ )
				{
					if(m_dataPage.lines[i]->nodes.size() == 0)
						continue;

					if( (m_dataPage.lines[i]->nodes[0]->m_rect.y > localYY-textSize.GetHeight()) && 
						(m_dataPage.lines[i]->nodes[0]->m_rect.y < localYY+textSize.GetHeight()))
					{
						start = i;
						//lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
						break;
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < m_dataPage.lines.size(); i++ )
			{
				if(m_dataPage.lines[i]->nodes.size() == 0)
					continue;
				if( (m_dataPage.lines[i]->nodes[0]->m_rect.y >= localYY-textSize.GetHeight()) && 
					(m_dataPage.lines[i]->nodes[0]->m_rect.y <= localYY+textSize.GetHeight()))
				{
					start = i;
					lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
					break;
				}
			}
		}
		for( int i = start; i < m_dataPage.lines.size(); i++ )
		{
			if( m_dataPage.lines[i]->nodes.size() > 0 )
			{
				if( m_dataPage.lines[i]->nodes[0]->m_rect.y > lineY + sz.GetHeight() )
				{
					break;
				}
			}
			else continue;

//			CodeFold *cfNode = NULL;
//			bool in_code_fold = false;
			for( int j = 0; j < m_dataPage.lines[i]->nodes.size(); j++ )
			{


				if( m_dataPage.lines[i]->nodes[j]->GetStyle() ==  TEXT_DISP_NORMAL )
				{
					dc.SetFont(fontNormal);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_BOLD ) 
				{
					dc.SetFont(fontBold);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_ITALIC )
				{
					dc.SetFont(fontItalic);
				}

				if( m_dataPage.lines[i]->nodes[j]->GetType() == ANNOTATION_DISP_NODE &&
					m_owner->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false)
				{
					dc.SetTextForeground(m_dataPage.lines[i]->nodes[j]->GetColor());
					dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y));
					dc.SetTextForeground(*wxBLACK);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetType() == CODE_FOLD_DISP_NODE )
				{
					if( m_dataPage.lines[i]->nodes[j]->GetState() == 1 )
					{
						dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
						dc.SetBrush( wxBrush( wxColour( 100, 255, 100, 128),wxSOLID));
						//cfNode = (CodeFold*)m_dataPage.lines[i]->nodes[j];
						//in_code_fold = true;
					}
					else
					{
						dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
						dc.SetBrush( wxBrush( wxColour( 255, 100, 100, 128),wxSOLID));
						//cfNode = NULL;
						//in_code_fold = false;
					}

					dc.DrawRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x + m_dataPage.lines[i]->nodes[j]->m_rect.height/4,
									m_dataPage.lines[i]->nodes[j]->m_rect.y + m_dataPage.lines[i]->nodes[j]->m_rect.height/4,
									m_dataPage.lines[i]->nodes[j]->m_rect.height/2,
									m_dataPage.lines[i]->nodes[j]->m_rect.height/2

									);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetType() == BRACKET_DISP_NODE )
				{
					dc.SetTextForeground(*wxBLACK);
					dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y));
					dc.SetTextForeground(*wxBLACK);
					dc.SetPen(*wxBLACK_PEN);
				}
				else if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == BLACK &&
					 (m_owner->board_canvas->GetBoard()->mov == WHITE || 
						m_owner->board_canvas->GetBoard()->mov == 2 )) &&
						(m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
				{
					int rectLeftOffset = textSize.GetWidth()/4;
					int rectRightOffset = 0;

					dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
					dc.SetBrush( wxBrush( wxColour( 100, 100, 255, 128),wxSOLID));
					dc.DrawRoundedRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height,2);


					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y));
					dc.SetTextForeground(*wxBLACK);
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.SetPen(*wxBLACK_PEN);
				}
				else if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == WHITE &&
					 (m_owner->board_canvas->GetBoard()->mov == BLACK )) &&
					 (m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
				{
					int rectLeftOffset = textSize.GetWidth()/4;
					int rectRightOffset = 0;//textSize.GetWidth()/4;

					dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
					dc.SetBrush( wxBrush( wxColour( 100, 100, 255, 128),wxSOLID));

					dc.DrawRoundedRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height,2);



					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y));
					dc.SetTextForeground(*wxBLACK);
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.SetPen(*wxBLACK_PEN);
				}
				else
				{


					dc.SetTextForeground(m_dataPage.lines[i]->nodes[j]->GetColor());
					dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y));
					dc.SetTextForeground(*wxBLACK);
					dc.SetPen(*wxBLACK_PEN);
				}
			}
		}
#endif
	}

	//event.RequestMore() ;
}


#if wxUSE_GRAPHICS_CONTEXT

const int BASE  = 80.0;
const int BASE2 = BASE/2;
const int BASE4 = BASE/4;

static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }


// modeled along Robin Dunn's GraphicsContext.py sample

void MyCanvas::DrawGraphics(wxGraphicsContext* gc)
{
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    gc->SetFont(font,*wxBLACK);

   gc->PushState();
   // gc->Translate(60, 400);
    gc->DrawText("Scaled smiley inside a square", 0, 0);
    gc->DrawRectangle(BASE2, BASE2, 500, 500);
    gc->DrawBitmap(*gs_bmp4, BASE2, BASE2, 500, 500);
    gc->PopState();

    // make a path that contains a circle and some lines, centered at 0,0
    wxGraphicsPath path = gc->CreatePath() ;
    path.AddCircle( 0, 0, BASE2 );
    path.MoveToPoint(0, -BASE2);
    path.AddLineToPoint(0, BASE2);
    path.MoveToPoint(-BASE2, 0);
    path.AddLineToPoint(BASE2, 0);
    path.CloseSubpath();
    path.AddRectangle(-BASE4, -BASE4/2, BASE2, BASE4);

    // Now use that path to demonstrate various capbilites of the grpahics context
    gc->PushState(); // save current translation/scale/other state
    gc->Translate(60, 75); // reposition the context origin

    gc->SetPen(wxPen("navy", 1));
    gc->SetBrush(wxBrush("pink"));

	//gc->DrawA
    for( int i = 0 ; i < 3 ; ++i )
    {
        wxString label;
        switch( i )
        {
            case 0 :
                label = "StrokePath";
                break;
            case 1 :
                label = "FillPath";
                break;
            case 2 :
                label = "DrawPath";
                break;
        }
        wxDouble w, h;
        gc->GetTextExtent(label, &w, &h, NULL, NULL);
        gc->DrawText(label, -w/2, -BASE2-h-4);
        switch( i )
        {
            case 0 :
                gc->StrokePath(path);
                break;
            case 1 :
                gc->FillPath(path);
                break;
            case 2 :
                gc->DrawPath(path);
                break;
        }
        gc->Translate(2*BASE, 0);
    }

    gc->PopState(); // restore saved state
    gc->PushState(); // save it again
    gc->Translate(60, 200); // offset to the lower part of the window

    gc->DrawText("Scale", 0, -BASE2);
    gc->Translate(0, 20);

    gc->SetBrush(wxBrush(wxColour(178,  34,  34, 128)));// 128 == half transparent
    for( int i = 0 ; i < 8 ; ++i )
    {
        gc->Scale(1.08, 1.08); // increase scale by 8%
        gc->Translate(5,5);
        gc->DrawPath(path);
    }

    gc->PopState(); // restore saved state
    gc->PushState(); // save it again
    gc->Translate(400, 200);

    gc->DrawText("Rotate", 0, -BASE2);

    // Move the origin over to the next location
    gc->Translate(0, 75);

    // draw our path again, rotating it about the central point,
    // and changing colors as we go
    for ( int angle = 0 ; angle < 360 ; angle += 30 )
    {
        gc->PushState(); // save this new current state so we can
        //  pop back to it at the end of the loop
        wxImage::RGBValue val = wxImage::HSVtoRGB(wxImage::HSVValue(float(angle)/360, 1, 1));
        gc->SetBrush(wxBrush(wxColour(val.red, val.green, val.blue, 64)));
        gc->SetPen(wxPen(wxColour(val.red, val.green, val.blue, 128)));

        // use translate to artfully reposition each drawn path
        gc->Translate(1.5 * BASE2 * cos(DegToRad(angle)),
                     1.5 * BASE2 * sin(DegToRad(angle)));

        // use Rotate to rotate the path
        gc->Rotate(DegToRad(angle));

        // now draw it
        gc->DrawPath(path);
        gc->PopState();
    }
    gc->PopState();

 
}
#endif // wxUSE_GRAPHICS_CONTEXT

void MyCanvas::OnIdle(wxIdleEvent &event)
{
	//wxPaintEvent evt;

/*	if( m_owner->board_canvas->IsAnimating() )
	{
		m_owner->board_canvas->OnIdle(event);
	}
*/	//evt.Skip();
	//OnPaintruet(evt);
	//event.RequestMore(false);
}

void MyCanvas::OnMouseMove(wxMouseEvent &event)
{
#if wxUSE_STATUSBAR
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);

	if( m_owner->enginesMatch )
		return;

    wxString str;
   // str.Printf( wxT("Current mouse position: %d,%d"), (int)x, (int)y );
   // m_owner->SetStatusText( str );
#else
    wxUnusedVar(event);
#endif // wxUSE_STATUSBAR
    wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );

	for( int i = 0; i < m_dataPage.lines.size(); i++ )
	{
		if( m_dataPage.lines[i]->nodes.size() == 0 )
			continue;
		
		if(((y > m_dataPage.lines[i]->nodes[0]->m_rect.y) &&
		   (y < m_dataPage.lines[i]->nodes[0]->m_rect.y + m_dataPage.lines[i]->nodes[0]->m_rect.GetHeight())))
		{
			for( int j = 0; j < m_dataPage.lines[i]->nodes.size(); j++ )
			{
				if( x > m_dataPage.lines[i]->nodes[j]->m_rect.x &&
					(x< m_dataPage.lines[i]->nodes[j]->m_rect.x + 
					m_dataPage.lines[i]->nodes[j]->m_rect.GetWidth()))
				{
					if( m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE )
					{
						if( variation_auto_fold )
						{
							if( !m_dataPage.lines[i]->nodes[j]->pMove->IsThisBranchOfMove(m_owner->currentGame->pCurrMove))
							{
								this->m_owner->currentGame->pCurrMove->SetDisplayVariations(false);
							}
						}

						this->m_owner->currentGame->pCurrMove = m_dataPage.lines[i]->nodes[j]->pMove;
						
						if( variation_auto_fold )
						{
							this->m_owner->currentGame->pCurrMove->SetDisplayVariations(true);
							if( m_owner->currentGame->pCurrMove->pNext )
							{
								if( m_owner->currentGame->pCurrMove->pNext->pVarWhite )
								{
									m_owner->currentGame->pCurrMove->pNext->SetDisplayVariations(true);
									this->needsRefresh = true;
									this->Refresh(false);
								}
							}							
							if( m_owner->currentGame->pCurrMove->pVarWhite || 
								m_owner->currentGame->pCurrMove->pVarBlack )
							{
								this->needsRefresh = true;
								this->Refresh(false);
							}
						}
					
						this->white_black_move = m_dataPage.lines[i]->nodes[j]->bWhiteBlack;
						if( this->white_black_move == 1 )
						{
							m_owner->board_canvas->GetBoard()->SetBoard(m_owner->currentGame->pCurrMove->m_fenBlack );
							m_owner->treeCanvas->SetSelection(0);
							m_owner->DoLookupFromBook();
							this->white_black_move = 0;
							int maxVal;
							this->move_count = this->m_owner->currentGame->pCurrMove->CountMovesToStart(white_black_move,maxVal);
						}
						else
						{
							m_owner->board_canvas->GetBoard()->SetBoard(m_owner->currentGame->pCurrMove->m_fenWhite );
							m_owner->treeCanvas->SetSelection(0);
							m_owner->DoLookupFromBook();
							int maxVal;
							this->move_count = this->m_owner->currentGame->pCurrMove->CountMovesToStart(white_black_move,maxVal);
							this->white_black_move = 1;
						}
					
						//Refresh();
						
						//m_owner->board_canvas->StopAnimating();
						m_owner->board_canvas->clearFlag = true;
						m_owner->board_canvas->MyRefresh(false);
						
						//m_owner->treeCanvas->Refresh();
						m_owner->DoRefresh();
						m_owner->moveDataCanvas->Refresh();
						
						if( m_owner->m_mgr.GetPane("scoreGraph").IsShown() && (m_owner->optionsFrame->m_GeneralAppOptions.updateScoreGraphOnKeyUp) )
						{
							//m_owner->m_scoreGraph->Refresh();
							m_owner->m_scoreGraphPanel->Refresh();
						}
						for( int j = 0; j < activeEngines.size(); j++ )
						{
							if( activeEngines[j]->IsActive() &&
								activeEngines[j]->IsAnalysing())
							{
								activeEngines[j]->KibitzStartsMove();
								activeEngines[j]->KibitzUserEndsMove();
							}
						}
						//SetFocus();
						break;
					}
					else if(m_dataPage.lines[i]->nodes[j]->GetType() == CODE_FOLD_DISP_NODE)
					{
						this->rememberedScrollPos = true;
						CalcUnscrolledPosition( 0,0, &remXX, &remYY);	
						if( m_dataPage.lines[i]->nodes[j]->GetState() == 1 )
						{
							m_dataPage.lines[i]->nodes[j]->SetState(0);
							m_dataPage.lines[i]->nodes[j]->pMove->SetDisplayVariations(true);
							this->needsRefresh = true;
						}
						else if( !m_owner->currentGame->pCurrMove->MoveIsVariationRoot(m_dataPage.lines[i]->nodes[j]->pMove) &&
							      !m_owner->currentGame->pCurrMove->IsThisBranchOfMove(m_dataPage.lines[i]->nodes[j]->pMove))
						{
							m_dataPage.lines[i]->nodes[j]->SetState(1);
							m_dataPage.lines[i]->nodes[j]->pMove->SetDisplayVariations(false);
							this->needsRefresh = true;
						}
						Refresh(false);
						//SetFocus();
						break;
					}
				}
			}
		}
	}
#ifdef USE_CANVAS_SET_FOCUS
	SetFocus();
#endif
}

void MyCanvas::ResetBoard()
{
	m_owner->currentGame->ResetMoveList();
					
	m_owner->board_canvas->ResetBoard();//->chess_board.SetInitialPosition();
					
	move_count = 0;
	this->selectedMove = 0;
	this->white_black_move = 0;
					
//	m_owner->ctrl->Refresh(true);
	needsRefresh = true;
	m_owner->board_canvas->MyRefresh(false);
	this->Refresh(false);
}


void MyCanvas::LogEvent(const wxString& name, wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	
	if( m_owner->m_gameTraverseTimer )
		m_owner->m_gameTraverseTimer->Stop();

	int mode = 0;

	if( name == "KeyDown" )
	{
		mode = 0;
	}
	else if( name == "KeyUp" )
	{
		mode = 1;

		if( m_owner->optionsFrame->m_NotationOptions.dynamicClassifyECO == true )
		{
			m_owner->ClassifyECO();
			m_owner->noteFrame->Refresh();
		}

		if(m_owner->optionsFrame->m_GeneralAppOptions.updateScoreGraphOnKeyUp)
		{
			m_owner->DoLookupFromBook();
		}

		m_owner->moveDataCanvas->Refresh();
		m_owner->treeCanvas->MyRefresh(true);
	
		if( m_owner->m_mgr.GetPane("scoreGraph").IsShown() && (m_owner->optionsFrame->m_GeneralAppOptions.updateScoreGraphOnKeyUp) )
		{
			//m_owner->m_scoreGraph->Refresh();
			m_owner->m_scoreGraphPanel->Refresh();
		}

		if( m_owner->board_canvas->bAnimating)
		{
			m_owner->board_canvas->StartEnginesOnKeyUp();
		}
		else
		{
			//m_owner->StartEnginesAfterMoveChange();
			m_owner->KickStartEnginesAfterMoveChange();
		}
		return;
	}
	

	switch ( keycode )
	{
		case WXK_BACK:break;
        case WXK_TAB:break;
        case WXK_RETURN:break;
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
		case WXK_UP:
			{
				if( m_owner->enginesMatch )
					break;
				m_owner->treeCanvas->DecrementSelection();
			}
			break;
        case WXK_DOWN:
			{
				if( m_owner->enginesMatch )
					break;
				m_owner->treeCanvas->IncrementSelection();
			}
			break;
        case WXK_LEFT:
			{
				m_owner->bookLookupCount = 0;

				if( m_owner->enginesMatch )
					break;
				
				if( m_owner->board_canvas->IsAnimating() && !m_owner->optionsFrame->m_NotationOptions.updateMoveFast )
					break;

				if( m_owner->currentGame->gameBeginsAtStartPos == false )
				{
					if( m_owner->currentGame->pCurrMove == m_owner->currentGame->pRoot )
					{
						if( /*(m_owner->currentGame->pRoot->x == 1)*/
							m_owner->currentGame->pRoot->MoveFlagIsSet(MOVE_FLAG_X) &&
							(m_owner->currentGame->pRoot->white == "" ))
						{
							if( m_owner->board_canvas->GetBoard()->mov == 1 )
							{
								white_black_move = WHITE;
								
								m_owner->board_canvas->MyRefresh(false);
								m_owner->treeCanvas->MyRefresh(true);
								this->Refresh(true);
								break;
							}
							else
							{
								//m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->pRoot->m_fenBlack );
								m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->pBase->m_fenBlack );

								white_black_move = BLACK;
								m_owner->board_canvas->MyRefresh(false);
								m_owner->treeCanvas->MyRefresh(true);
								this->Refresh(true);
								break;
							}

						}
					}
				}

				/*if( m_owner->board_canvas->moveForward == true )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;
				}*/
				
				bool isMainLine = m_owner->currentGame->pCurrMove->IsThisMainLine();

				bool bStartAnimation = true;
				
				//if( !m_owner->board_canvas->bAnimating)
				m_owner->StopEnginesBeforeMoveChange();
				

				if( m_owner->pgnLoad.move_counter== 0)
				{
					m_owner->treeCanvas->SetSelection(0);
					
					//if( !m_owner->board_canvas->bAnimating)
					//	m_owner->StartEnginesAfterMoveChange();
					
					break;
				}

				if( (white_black_move == BLACK) && (move_count > 0))
				{
					if(m_owner->currentGame->pCurrMove->pUp != NULL )
					{
						if( isMainLine )
						{
							m_owner->currentGame->PrevMove();
							m_owner->currentGame->PrevMove();
							if( variation_auto_fold )
							{

								if( (m_owner->currentGame->pCurrMove->pVarWhite || 
									 m_owner->currentGame->pCurrMove->pVarBlack) ||
									(m_owner->currentGame->pCurrMove->pNext &&
									 m_owner->currentGame->pCurrMove->pNext->pVarWhite))
								{
									this->needsRefresh = true;
									this->Refresh(false);
								}
							}
						}
						else
						{
							if( m_owner->treeCanvas->GetSelection()!= 0 &&
								m_owner->treeCanvas->GetSelection() == m_owner->treeCanvas->numSelectionsThisMove)
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
							}
							else
							{
								m_owner->currentGame->PrevMove();
								m_owner->currentGame->PrevMove();
								
								if( variation_auto_fold )
								{
									if( (m_owner->currentGame->pCurrMove->pVarWhite || 
										m_owner->currentGame->pCurrMove->pVarBlack) ||
										(m_owner->currentGame->pCurrMove->pNext &&
										m_owner->currentGame->pCurrMove->pNext->pVarWhite))
									{
										this->needsRefresh = true;
										this->Refresh(false);
									}
								}
							}
						}
						
					}
					else
					{
						m_owner->currentGame->PrevMove();
						if( variation_auto_fold )
						{
							if( (m_owner->currentGame->pCurrMove->pVarWhite || 
									m_owner->currentGame->pCurrMove->pVarBlack) ||
								(m_owner->currentGame->pCurrMove->pNext &&
									m_owner->currentGame->pCurrMove->pNext->pVarWhite))
							{
								this->needsRefresh = true;
								this->Refresh(false);
							}
						}
						//white_black_move = WHITE;
					}
				}

				if( (white_black_move == WHITE) && (move_count>0))
				{
					if( m_owner->currentGame->pCurrMove->white == "" )
					{
						if( isMainLine )
						{
							m_owner->currentGame->PrevMove();
							if( variation_auto_fold )
							{
								if( (m_owner->currentGame->pCurrMove->pVarWhite || 
									 m_owner->currentGame->pCurrMove->pVarBlack) ||
									(m_owner->currentGame->pCurrMove->pNext &&
									 m_owner->currentGame->pCurrMove->pNext->pVarWhite))
								{
									this->needsRefresh = true;
									this->Refresh(false);
								}
							}	
						}
						else
						{
							if( m_owner->treeCanvas->GetSelection()!= 0 &&
								m_owner->treeCanvas->GetSelection() == m_owner->treeCanvas->numSelectionsThisMove)
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
							}
							else
							{
								m_owner->currentGame->PrevMove();
								//m_owner->currentGame->PrevMove();
								if( variation_auto_fold )
								{
									if( (m_owner->currentGame->pCurrMove->pVarWhite || 
										 m_owner->currentGame->pCurrMove->pVarBlack) ||
										(m_owner->currentGame->pCurrMove->pNext &&
										 m_owner->currentGame->pCurrMove->pNext->pVarWhite))
									{
										this->needsRefresh = true;
										this->Refresh(false);
									}
								}
							}
						}

					}
				}


				if( m_owner->currentGame->IsInBase() )
				{
					//m_owner->currentGame->NextMove();
					white_black_move = BLACK;
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;
					bStartAnimation = false;
				}
				/*if( (selectedMove ==0 ))// m_owner->currentGame->move_counter)
					//|| m_owner->board_canvas->GetBoard()->postReset)
				{
					//m_owner->ctrl->chess_board.mov = 0;
					//PGN::Move *pMove = m_owner->currentGame->pRoot;
					//while(pMove->pNext)
					//	pMove = pMove->pNext;
					
					//ResetBoard();

					
					return;
				}		*/		

				//MyChessMove mv = 
				if( white_black_move == WHITE )
				{
					//white_black_move = move_count;
					//ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->white );
					//m_owner->ctrl->chess_board.parseMove( m_owner->currentGame->GetNextMove()->white );
					//if( m_owner->currentGame->pCurrMove->hasFen )
					
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenWhite() );
					white_black_move = BLACK;
					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wxString inpt ="";

						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str());
							//inpt+= " ";
							//m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str()));
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}
					//else
					//{
					//	m_owner->board_canvas->GetBoard()->parseMove(m_owner->currentGame->GetPreviousMove()->white);
					//	m_owner->currentGame->pCurrMove->SetFen( m_owner->board_canvas->GetBoard()->Fen());
					//}

					/*if( m_owner->optionsFrame->m_NotationOptions.dynamicClassifyECO == true )
					{
						m_owner->ClassifyECO();
						m_owner->noteFrame->Refresh();
					}*/

					//m_owner->DoLookupFromBook();
				}
				else
				{
					//white_black_move = move_count;
				//	ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->black );
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenBlack() );
					white_black_move = WHITE;
					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wxString inpt = "";
						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str());
							//inpt += " ";
						//	m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str()));
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}

					/*if( m_owner->optionsFrame->m_NotationOptions.dynamicClassifyECO == true )
					{
						m_owner->ClassifyECO();
						m_owner->noteFrame->Refresh();
					}*/

					//m_owner->DoLookupFromBook();
				}
				
				m_owner->treeCanvas->SetSelection(0);
				ScrollToCursor();
				wxRect clear(XX,YY,w, h);
				//Refresh(/*, &clear*/ );
				//m_owner->board_canvas->clearFlag = true;
				m_owner->board_canvas->moveForward = false;
				m_owner->board_canvas->renderMoveNow = true;
				m_owner->board_canvas->MyRefresh(false);
				
				if( bStartAnimation )
					m_owner->board_canvas->BackAnimation();
				//m_owner->treeCanvas->Refresh(true);
				
				m_owner->DoRefresh();

				//if( m_owner->currentGame->

				//if( !m_owner->board_canvas->bAnimating)
					

				
				//Refresh();
			//	ctrl->chess_board.
			}break;
 
        case WXK_RIGHT:
			{

				/*if( m_owner->board_canvas->moveForward == false )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;
				}	*/			
				/*if( m_owner->variationSelectionBox &&
					m_owner->variationSelectionBox->IsVisible() )
					m_owner->variationSelectionBox->Close();
				*/

				if( m_owner->enginesMatch )
					break;

				if( m_owner->board_canvas->IsAnimating() && !m_owner->optionsFrame->m_NotationOptions.updateMoveFast )
				{
					break;
				}

				bool bStartAnimation = true;

				if( m_owner->pgnLoad.move_counter== 0)
					break;

					bool treeUsed = false;

				m_owner->bookLookupCount = 0;

				m_owner->StopEnginesBeforeMoveChange();

				if( (white_black_move == WHITE) && !(move_count == 0))
				{
					if( m_owner->currentGame->pCurrMove->pNext &&
						m_owner->currentGame->pCurrMove->pNext->white != "" &&
						!m_owner->currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK)
						/*!m_owner->currentGame->pCurrMove->atEndBlack*/)
					{
						m_owner->currentGame->NextMove();
						
						if( variation_auto_fold )
						{
							if( (m_owner->currentGame->pCurrMove->pVarWhite || 
									m_owner->currentGame->pCurrMove->pVarBlack) ||
								(m_owner->currentGame->pCurrMove->pNext &&
									m_owner->currentGame->pCurrMove->pNext->pVarWhite))
							{
								this->needsRefresh = true;
								this->Refresh(false);
							}
						}
						//		m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
							//	m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();
					}
					else 
					{
						if( m_owner->currentGame->pRoot->white == "" )//&&
							//m_owner->currentGame->pCurrMove->pNext->white != "")
						{
							if( (m_owner->currentGame->IsInBase()) ||
								((m_owner->currentGame->pCurrMove->pNext)&&(m_owner->currentGame->pCurrMove->pNext->white != "")))
							{
								m_owner->currentGame->NextMove();
								if( m_owner->currentGame->pCurrMove->white != "" )
									white_black_move = WHITE;
								else
									white_black_move = BLACK;
							}
							else
							{
								//if( !m_owner->board_canvas->bAnimating)
								//	m_owner->StartEnginesAfterMoveChange();
								//m_owner->StartEnginesAfterMoveChange();
								break;
							}
							

						}
						else
						{
							//if( !m_owner->board_canvas->bAnimating)
							//	m_owner->StartEnginesAfterMoveChange();
							if( (m_owner->treeCanvas->selectedElem->pMove == NULL) &&
								(m_owner->treeCanvas->selectedElem->isBookMove == true))
							{
								string pgnMove = m_owner->treeCanvas->selectedElem->bookMove;
								m_owner->board_canvas->GetBoard()->parseMove(pgnMove);
								string fen = m_owner->board_canvas->GetBoard()->Fen();
								//MyChessMove mv = *board_canvas->GetBoard()->GetLastMove();
					
								int white_black = 0;
								if( m_owner->board_canvas->GetBoard()->mov == 0 || m_owner->board_canvas->GetBoard()->mov == 2 )
									white_black = 1;
								else white_black = 0;

								if( m_owner->treeCanvas->selectedElem->white_black != white_black )
								{
						
								
						
								PGN::MoveInserter mv(pgnMove,white_black, fen, m_owner->board_canvas->GetBoard()->GetLastMove());
								m_owner->board_canvas->GetBoard()->SetBoard(fen);
								int hits = -1;

								m_owner->currentGame->pCurrMove = m_owner->currentGame->pCurrMove->InsertMove(&mv, &hits); 
								}
								m_owner->my_canvas->needsRefresh = true;
								m_owner->DoRefresh();

								this->m_owner->my_canvas->move_count++;
								if( m_owner->my_canvas->white_black_move == WHITE )
									m_owner->my_canvas->white_black_move = BLACK;
								else m_owner->my_canvas->white_black_move = WHITE;

								m_owner->DoLookupFromBook();
								m_owner->treeCanvas->Refresh(true);
							}
							break;
						}
					}
				}
				/*else if( (white_black_moves == WHITE) && (move_count == 0) &&
					(m_owner->currentGame->pRoot->white == "") &&
					(!m_owner->currentGame->pRoot->black == ""))
				{

				}*/

			

				if( (white_black_move == WHITE) )
				{

					if( m_owner->treeCanvas->numElems > 1)
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
						m_owner->treeCanvas->SetSelection(0);
					
						

						if( bStartAnimation )
							m_owner->board_canvas->StartAnimation();
						//m_owner->treeCanvas->Refresh(true);
						//if( !m_owner->board_canvas->bAnimating)
						//	m_owner->StartEnginesAfterMoveChange();
						
						break;
					}

					if( m_owner->currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					
					{
						if( m_owner->currentGame->AtEnd())
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								/*(m_owner->treeCanvas->GetSelection() != 0))*/
								(m_owner->treeCanvas->selectedElem->bRoot))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								//if( !m_owner->board_canvas->bAnimating)
								//	m_owner->StartEnginesAfterMoveChange();
								
								break;
							}
							else if( m_owner->currentGame->pCurrMove->white != "" )
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								
								if( bStartAnimation )
								{
									//if( 
									m_owner->board_canvas->StartAnimation();
								}

								white_black_move = WHITE;

								treeUsed = true;
								move_count++; // ??
								//break;
							}

						}
						else
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								/*(m_owner->treeCanvas->GetSelection() != 0))*/
								(m_owner->treeCanvas->selectedElem->bRoot))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								//if( !m_owner->board_canvas->bAnimating)
								//	m_owner->StartEnginesAfterMoveChange();
								
								break;
							}
						}

					}
					else treeUsed = true;
				}

				if(white_black_move == BLACK && !treeUsed)
				{

					if( m_owner->treeCanvas->numElems > 1 ) 
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
						m_owner->treeCanvas->SetSelection(0);
					
						if( bStartAnimation )
							m_owner->board_canvas->StartAnimation();

						move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
						//if( !m_owner->board_canvas->bAnimating)
						//	m_owner->StartEnginesAfterMoveChange();
						
						break;
					}

					if(m_owner->currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					{
						if( m_owner->currentGame->AtEnd())
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								/*(m_owner->treeCanvas->GetSelection() != 0))*/
								(m_owner->treeCanvas->selectedElem->bRoot))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								//if( !m_owner->board_canvas->bAnimating)
								//	m_owner->StartEnginesAfterMoveChange();
								
								break;
							}
							else if( m_owner->currentGame->pCurrMove->white != "" )
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								if( bStartAnimation )
								{
									if( m_owner->currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE) )
										//m_owner->currentGame->pCurrMove->atEndWhite == true )
									{
										//m_owner->board_canvas->StartAnimation();
									}
									else
									{
										m_owner->board_canvas->StartAnimation();
									}
									
								}

								//white_black_move = WHITE;

								move_count++; // ??
							}

						}
						else
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								/*(m_owner->treeCanvas->GetSelection() != 0))*/
								(m_owner->treeCanvas->selectedElem->bRoot))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								//if( !m_owner->board_canvas->bAnimating)
								//	m_owner->StartEnginesAfterMoveChange();
								
								break;
							}
						}

					
					}
				}

				if( m_owner->currentGame->IsInBase() )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;

					m_owner->currentGame->NextMove();
					if( m_owner->currentGame->pCurrMove->white != "" )
						white_black_move = WHITE;
					else
						white_black_move = BLACK;
					if( /*m_owner->currentGame->GetCurrentMove()->pVarWhite*/
						(m_owner->treeCanvas->numElems > 1) || 
						(m_owner->currentGame->GetCurrentMove()->GetVariationRoot()!=NULL))
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
						m_owner->treeCanvas->SetSelection(0);
						if( bStartAnimation )
							m_owner->board_canvas->StartAnimation();

						//if( !m_owner->board_canvas->bAnimating)
						//	m_owner->StartEnginesAfterMoveChange();
						break;
					}
				}

				if( m_owner->currentGame->AtEnd() )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;

					if( m_owner->currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE))
						//m_owner->currentGame->pCurrMove->atEndWhite )
					{
						if( white_black_move == BLACK )
						{
							//white_black_move = WHITE;
							//if( !m_owner->board_canvas->bAnimating)
							//	m_owner->StartEnginesAfterMoveChange();
							//if( bStartAnimation )
							//	m_owner->board_canvas->StartAnimation();

							break;
						}
						white_black_move = WHITE;
					}
					else if( m_owner->currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK))
						//m_owner->currentGame->pCurrMove->atEndBlack )
					{
						if( white_black_move == WHITE && !treeUsed)
						{
							//white_black_move = BLACK;
							//if( !m_owner->board_canvas->bAnimating)
							//	m_owner->StartEnginesAfterMoveChange();
							//if( bStartAnimation )
							//	m_owner->board_canvas->StartAnimation();
							
							break;
						}
						
					}

					bStartAnimation = false;

				}
		


				if( white_black_move == WHITE )
				{
					
					if( (m_owner->optionsFrame->m_GameOptions.animate == true) && (this->m_owner->gameMode!= 1))
						m_owner->board_canvas->GetLastMoveInCaseOfCapture(m_owner->currentGame->pCurrMove->GetWhiteMove());
					
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenWhite() );
				
					white_black_move = BLACK;


					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wxString inpt ="";

						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str());
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}

					/*if( m_owner->optionsFrame->m_NotationOptions.dynamicClassifyECO == true )
					{
						m_owner->ClassifyECO();
						m_owner->noteFrame->Refresh();
					}*/
					//m_owner->DoLookupFromBook();
				}
				else
				{
					
					if( (m_owner->optionsFrame->m_GameOptions.animate == true) && (this->m_owner->gameMode!= 1))
						m_owner->board_canvas->GetLastMoveInCaseOfCapture(m_owner->currentGame->pCurrMove->GetBlackMove());
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenBlack() );
					white_black_move = WHITE;
					
					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wxString inpt = "";
						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str());
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}
					move_count++;

					/*if( m_owner->optionsFrame->m_NotationOptions.dynamicClassifyECO == true )
					{
						m_owner->ClassifyECO();
						m_owner->noteFrame->Refresh();
					}*/

					//m_owner->DoLookupFromBook();
				}
				
				

				
				ScrollToCursor();

				wxRect clear(XX,YY,w, h);
				//Refresh(/*, &clear*/ );
				m_owner->board_canvas->moveForward = true;
				m_owner->board_canvas->renderMoveNow = true;
				m_owner->board_canvas->MyRefresh(false);
				if( bStartAnimation )
					m_owner->board_canvas->StartAnimation();

				m_owner->DoRefresh();



			
			}break;

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
        case WXK_F1:
			{
				wxCommandEvent tgmEvt;
				m_owner->m_parent->ToggleMaximize(tgmEvt);
			}
			break;
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


  /*  EVT_MOTION (MyCanvas::OnMouseMove)
	EVT_LEFT_DOWN(MyCanvas::mouseDown)
	EVT_RIGHT_DOWN(MyCanvas::rightClick)
	EVT_LEFT_UP(MyCanvas::mouseReleased)
	EVT_MOUSEWHEEL(MyCanvas::mouseWheelMoved)
END_EVENT_TABLE()*/
void MyCanvas::mouseDown(wxMouseEvent& event)
{
}
void MyCanvas::mouseWheelMoved(wxMouseEvent& event)
{
}
void MyCanvas::mouseReleased(wxMouseEvent& event)
{
}
void MyCanvas::rightClick(wxMouseEvent& evt)
{
	wxMenu menu;
	menu.Append(PROMOTE_VARIATION, "Promote Variation", "Moves the Current Variation up in the Tree");
	menu.Append(DELETE_VARIATION, "Delete Variation", "Completely Deletes the Current Variation, this action cannot be undone");
	menu.Append(DELETE_REMAINING_MOVES, "Delete Remaining Moves", "Deletes all Moves after the current Move");
	menu.Append(ADD_ANNOTATION, "Add Annotation To This Move", "Annotates the Current Move");
	menu.Append(SET_VARIATION_COLOR, "Set Variation Colour", "Sets the Colour of the Current Variation");
	menu.Append(SET_MOVE_COLOR, "Set Move Colour", "Sets the Colour of the Current Move");
	wxMenuItem *item = menu.Append(ADD_GLYPH, "Add Glyph", "Inserts A Glyph into the move");
	menu.Append(TOGGLE_VARIATION_FOLDS, "Fold All Variations", "Sets the Colour of the Current Move");
	

	
    PopupMenu(&menu);
}

void MyCanvas::OnAddGlyph(wxCommandEvent& evt)
{
	wxMenu glyphs;
	glyphs.Append(ID_EXCLAMATION, "!", "Insert an Exclamation Mark Symbol");
	glyphs.Append(ID_DOUBLE_EXCLAMATION, "!!", "Insert a Double Exclamation Mark Symbol");
	glyphs.Append(ID_EXCLAMATION_QUESTION, "!?", "Insert an Exclamation Mark Followed by a Question Mark Symbol");
	glyphs.Append(ID_QUESTION, "?", "Insert an Question Mark Symbol");
	glyphs.Append(ID_DOUBLE_QUESTION, "??", "Insert a Double Question Mark Symbol");
	glyphs.Append(ID_QUESTION_EXCLAMATION, "?!", "Insert a Question Mark Followed by an Exclamation Mark Symbol");
	
	PopupMenu(&glyphs);
}


void MyCanvas::SetMoveColor(wxCommandEvent& WXUNUSED(evt))
{
	wxColourDialog colorDlg(this);
	
	if( colorDlg.ShowModal() == wxID_OK )
	{
		wxColourData colData = colorDlg.GetColourData();
	
		if( m_owner->board_canvas->GetBoard()->mov == 1 )
		{
			m_owner->currentGame->pCurrMove->SetRenderColorWhite(colData.GetColour().Red(),
										colData.GetColour().Green(), colData.GetColour().Blue(),
										m_owner->m_moveColorPallete);
		}
		else
		{
			m_owner->currentGame->pCurrMove->SetRenderColorBlack(colData.GetColour().Red(),
										colData.GetColour().Green(), colData.GetColour().Blue(),
										m_owner->m_moveColorPallete);
		}
		this->needsRefresh = true;
		Refresh(true);
	}
}

void MyCanvas::SetVariationColor(wxCommandEvent& WXUNUSED(evt))
{
	wxColourDialog colorDlg(this);
	
	if( colorDlg.ShowModal() == wxID_OK )
	{
		wxColourData colData = colorDlg.GetColourData();
		
		m_owner->currentGame->pCurrMove->SetVariationRenderColor(colData.GetColour().Red(),
									colData.GetColour().Green(), colData.GetColour().Blue(), 
									m_owner->m_moveColorPallete);

		this->needsRefresh = true;
		Refresh(true);
	}
}

void MyCanvas::PromoteVariation(wxCommandEvent& evt)
{
	m_owner->PromoteVariation();
}

void MyCanvas::DeleteVariation(wxCommandEvent& evt)
{
	m_owner->DeleteVariation();
}

void MyCanvas::DeleteRemainingMoves(wxCommandEvent& evt)
{
	m_owner->DeleteRemainingMoves();
}

void MyCanvas::AddAnnotation(wxCommandEvent& evt)
{
	m_owner->EditAnnotation();
}

void MyCanvas::OnToggleAllVariationFolds(wxCommandEvent& evt)
{
	m_owner->currentGame->ToggleAllVariationFolds();
	this->needsRefresh = true;
	Refresh(true);
}


void MyCanvas::OnAddGlyphExclamation(wxCommandEvent& evt)
{
	if( white_black_move == BLACK )
	{
		int findExclamation = m_owner->currentGame->pCurrMove->white.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->white.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->white += "!";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->white.length();
			string newstr = m_owner->currentGame->pCurrMove->white.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->white.substr(0,posAtStart);
			oldstr += "!";
			m_owner->currentGame->pCurrMove->white = oldstr;
		}
		
	}
	else
	{
		int findExclamation = m_owner->currentGame->pCurrMove->pPrev->black.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->pPrev->black.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->black += "!";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->pPrev->black.length();
			string newstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(0,posAtStart);
			oldstr += "!";
			m_owner->currentGame->pCurrMove->pPrev->black = oldstr;
		}
	}
	this->needsRefresh = true;
	Refresh(true);
}
void MyCanvas::OnSetGlyphDoubleExclamation(wxCommandEvent& evt)
{
	if( white_black_move == BLACK )
	{
		int findExclamation = m_owner->currentGame->pCurrMove->white.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->white.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->white += "!!";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->white.length();
			string newstr = m_owner->currentGame->pCurrMove->white.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->white.substr(0,posAtStart);
			oldstr += "!!";
			m_owner->currentGame->pCurrMove->white = oldstr;
		}
		
	}
	else
	{
		int findExclamation = m_owner->currentGame->pCurrMove->pPrev->black.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->pPrev->black.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->black += "!!";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->pPrev->black.length();
			string newstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(0,posAtStart);
			oldstr += "!!";
			m_owner->currentGame->pCurrMove->pPrev->black = oldstr;
		}
	}
	this->needsRefresh = true;
	Refresh(true);
}
void MyCanvas::OnAddGlyphExclamationQuestion(wxCommandEvent& evt)
{
	if( white_black_move == BLACK )
	{
		int findExclamation = m_owner->currentGame->pCurrMove->white.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->white.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->white += "!?";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->white.length();
			string newstr = m_owner->currentGame->pCurrMove->white.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->white.substr(0,posAtStart);
			oldstr += "!?";
			m_owner->currentGame->pCurrMove->white = oldstr;
		}
		
	}
	else
	{
		int findExclamation = m_owner->currentGame->pCurrMove->pPrev->black.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->pPrev->black.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->black += "!?";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->pPrev->black.length();
			string newstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(0,posAtStart);
			oldstr += "!?";
			m_owner->currentGame->pCurrMove->pPrev->black = oldstr;
		}
	}
	this->needsRefresh = true;
	Refresh(true);
}
void MyCanvas::OnSetGlyphQuestion(wxCommandEvent& evt)
{
	if( white_black_move == BLACK )
	{
		int findExclamation = m_owner->currentGame->pCurrMove->white.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->white.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->white += "?";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->white.length();
			string newstr = m_owner->currentGame->pCurrMove->white.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->white.substr(0,posAtStart);
			oldstr += "?";
			m_owner->currentGame->pCurrMove->white = oldstr;
		}
		
	}
	else
	{
		int findExclamation = m_owner->currentGame->pCurrMove->pPrev->black.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->pPrev->black.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->black += "?";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->pPrev->black.length();
			string newstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(0,posAtStart);
			oldstr += "?";
			m_owner->currentGame->pCurrMove->pPrev->black = oldstr;
		}
	}
	this->needsRefresh = true;
	Refresh(true);
}
void MyCanvas::OnAddGlyphDoubleQuestion(wxCommandEvent& evt)
{
	if( white_black_move == BLACK )
	{
		int findExclamation = m_owner->currentGame->pCurrMove->white.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->white.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->white += "??";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->white.length();
			string newstr = m_owner->currentGame->pCurrMove->white.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->white.substr(0,posAtStart);
			oldstr += "??";
			m_owner->currentGame->pCurrMove->white = oldstr;
		}
		
	}
	else
	{
		int findExclamation = m_owner->currentGame->pCurrMove->pPrev->black.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->pPrev->black.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->black += "??";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->pPrev->black.length();
			string newstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(0,posAtStart);
			oldstr += "??";
			m_owner->currentGame->pCurrMove->pPrev->black = oldstr;
		}
	}
	this->needsRefresh = true;
	Refresh(true);
}
void MyCanvas::OnSetGlyphQuestionExclamation(wxCommandEvent& evt)
{
	if( white_black_move == BLACK )
	{
		int findExclamation = m_owner->currentGame->pCurrMove->white.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->white.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->white += "?!";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->white.length();
			string newstr = m_owner->currentGame->pCurrMove->white.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->white.substr(0,posAtStart);
			oldstr += "?!";
			m_owner->currentGame->pCurrMove->white = oldstr;
		}
		
	}
	else
	{
		int findExclamation = m_owner->currentGame->pCurrMove->pPrev->black.find("!");
		int findQuestion = m_owner->currentGame->pCurrMove->pPrev->black.find("?");

		if( findExclamation == -1 && findQuestion == -1)
		{
			m_owner->currentGame->pCurrMove->black += "?!";
		}
		else
		{
			int posAtStart = 0;
			if( (findExclamation > 0) && (findExclamation < findQuestion) )
			{
				posAtStart = findExclamation;
			}
			else if( (findExclamation > 0) && (findQuestion == -1) )
			{
				posAtStart = findExclamation;
			}
			else if( (findQuestion > 0) && (findQuestion < findExclamation))
			{
				posAtStart = findQuestion;
			}
			else if( (findQuestion > 0) && (findExclamation == -1) )
			{
				posAtStart = findQuestion;
			}

			int len = m_owner->currentGame->pCurrMove->pPrev->black.length();
			string newstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(posAtStart, len - posAtStart+1);
			string oldstr = m_owner->currentGame->pCurrMove->pPrev->black.substr(0,posAtStart);
			oldstr += "?!";
			m_owner->currentGame->pCurrMove->pPrev->black = oldstr;
		}
	}
	this->needsRefresh = true;
	Refresh(true);
}