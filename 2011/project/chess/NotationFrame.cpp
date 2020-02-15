#include "NotationFrame.h"
#include "NotationCanvas.h"
#include "mdi.h"
#include "OptionsFrame.h"

BEGIN_EVENT_TABLE(NotationFrame, wxPanel)
	EVT_PAINT  (NotationFrame::OnPaint)
    EVT_SIZE(NotationFrame::OnSize)

END_EVENT_TABLE()

NotationFrame::NotationFrame(MyFrame *parent, MyCanvas **canvas):
				wxPanel((wxWindow*)parent)
{
	m_parent = parent;
	//wxMessageBox("Exiting Canvas");
	//m_panel = new wxPanel(this);
	m_canvas = new MyCanvas(parent, this);
	*canvas = m_canvas;
//	m_canvas = *canvas;

	//wxMessageBox("Exiting Canvas");
    int w, h;
    GetClientSize(&w, &h);

	//m_panel->SetSize(w,120);
   // m_textWindow->SetSize(0, 0, 200, h);
    m_canvas->SetSize(0, 40, w, h-40);


	removeHeaderInfo = false;

#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif

	m_font =  wxFont(10, wxMODERN, wxNORMAL,wxBOLD, false/*,"Arial Baltic"*/);
}


void NotationFrame::OnSize(wxSizeEvent& event)
{
	DoSize();
	//Update();
	//Refresh(true);
}

void NotationFrame::DoSize()
{
    int w, h;
    GetClientSize(&w, &h);

//	m_panel->SetSize(w,120);
   // m_textWindow->SetSize(0, 0, 200, h);
	if( !removeHeaderInfo )
		m_canvas->SetSize(0, 40, w, h-40);
	else
		m_canvas->SetSize(0, 0, w, h);
}


void NotationFrame::OnPaint(wxPaintEvent &event)
{
	if( removeHeaderInfo )
		return;

//	if( m_canvas->m_owner->optionsFrame->m_GameOptions.

	

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
	
	//dc.SetTextForeground(*wxBLACK);
	
	// debug
	sprintf(buff, "%s vs %s",
		m_canvas->m_owner->currentGame->whiteName.c_str(), 
		m_canvas->m_owner->currentGame->blackName.c_str());
	//	m_canvas->m_owner->currentGame->whiteName,
		//m_canvas->m_owner->currentGame->blackName);

	double length, height, descent;
	gc->GetTextExtent( buff, &length, &height, &descent );


	gc->SetBrush(wxBrush(wxColour(150,  150,  255, 128)));
	gc->DrawRectangle(w/2-length/2-5, 6, length+10, height+4); // was h=8
	gc->DrawText(wxString(buff),w/2-length/2, 8);			// was h=10

	//gc->GetTextExtent( m_canvas->m_owner->currentECO, &length, &height, &descent );

	//gc->SetBrush(wxBrush(wxColour(150,  150,  255, 128)));
	//gc->DrawRectangle(w/2-length/2-5, 30, length+10, height+4); // height adjusted to fit above
	//gc->DrawText(m_canvas->m_owner->currentECO,w/2-length/2,32);
	
   // m_owner->PrepareDC(dc);
}

