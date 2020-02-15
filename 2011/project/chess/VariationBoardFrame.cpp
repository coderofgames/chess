#include "VariationBoardFrame.h"


#include "VariationBoardCanvas.h"
#include "D3D9VariationBoard.h"
#include "mdi.h"
#include "OptionsFrame.h"

BEGIN_EVENT_TABLE(VariationBoardFrame, wxPanel)
	EVT_PAINT  (VariationBoardFrame::OnPaint)
    EVT_SIZE(VariationBoardFrame::OnSize)

END_EVENT_TABLE()

VariationBoardFrame::VariationBoardFrame(MyFrame *parent):
				wxPanel((wxWindow*)parent)
{
	m_parent = parent;

	m_variationBoard = new D3D9VariationBoard(parent, this);
	//m_variationBoard = new VariationBoardCanvas(parent, this);
	
	//((VariationBoardCanvas*)m_variationBoard)->SetSize(360,380);
	//((D3D9VariationBoard*)m_variationBoard)->SetSize(360,380);
	m_variationBoard->ResetBoard();
	m_variationBoard->GetBoard()->SetBoard(m_parent->board_canvas->GetBoard()->Fen());
	((D3D9VariationBoard*)m_variationBoard)->Show(true);
	
	//((VariationBoardCanvas*)m_variationBoard)->Show(true);

    int w, h;
    GetClientSize(&w, &h);


    ((D3D9VariationBoard*)m_variationBoard)->SetSize(0, 0, 300, 300);
	//((VariationBoardCanvas*)m_variationBoard)->SetSize(0, 0, 300, 300);



#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif

	m_font =  wxFont(10, wxMODERN, wxNORMAL,wxBOLD, false/*,"Arial Baltic"*/);
}


void VariationBoardFrame::OnSize(wxSizeEvent& event)
{
	DoSize();
	//Update();
	//Refresh(true);
}

void VariationBoardFrame::DoSize()
{
    int w, h;
    GetClientSize(&w, &h);


	//if( !removeHeaderInfo )
		//((VariationBoardCanvas*)m_variationBoard)->SetSize(0, 0, w, h);
		((D3D9VariationBoard*)m_variationBoard)->SetSize(0, 0, w, h);
	//else
	//m_variationBoard->SetSize(0, 40, w, h-40);
}


void VariationBoardFrame::OnPaint(wxPaintEvent &event)
{

    int w, h;
    GetClientSize(&w, &h);
	wxPaintDC pdc(this);
#if wxUSE_GRAPHICS_CONTEXT
	bool m_useContext = true;
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

	

#if wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *gc = gdc.GetGraphicsContext();
#endif
  //  PrepareDC(dc);


	

	char buff[1024];
	gc->SetFont( m_font,*wxBLACK);
	




}

