#include "ScorePanel.h"
#include "ScoreGraph.h"

#include "mdi.h"
#include "OptionsFrame.h"

BEGIN_EVENT_TABLE(ScorePanel, wxPanel)
	EVT_PAINT  (ScorePanel::OnPaint)
    EVT_SIZE(ScorePanel::OnSize)

END_EVENT_TABLE()

ScorePanel::ScorePanel(MyFrame *parent ):
				wxPanel((wxWindow*)parent)
{
	m_parent = parent;
	//wxMessageBox("Exiting Canvas");
	//m_panel = new wxPanel(this);

//	m_canvas = *canvas;

	//wxMessageBox("Exiting Canvas");
    int w, h;
    GetClientSize(&w, &h);

	m_scoreGraph = new ScoreGraph(this, parent);

	//m_panel->SetSize(w,120);
   // m_textWindow->SetSize(0, 0, 200, h);
    m_scoreGraph->SetSize(60, 0, w-60, h);

	txt1 = "0.0";
	txt2 = "0.0";


#ifdef THEME_BUILD
	SetBackgroundColour(THEME_COLOR_B);
#endif

	m_font =  wxFont(10, wxMODERN, wxNORMAL,wxBOLD, false/*,"Arial Baltic"*/);
}


void ScorePanel::OnSize(wxSizeEvent& event)
{
	DoSize();
	//Update();
	//Refresh(true);
}

void ScorePanel::DoSize()
{
    int w, h;
    GetClientSize(&w, &h);

//	m_panel->SetSize(w,120);
   // m_textWindow->SetSize(0, 0, 200, h);

		m_scoreGraph->SetSize(60, 0, w-60, h);

}


void ScorePanel::OnPaint(wxPaintEvent &event)
{


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


	

	
	wxFont fontBold = wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL );
	gc->SetFont(fontBold, *wxBLACK);

	//sprintf(buff, "%.2f",float(maxValue)/100.0f); 
	gc->DrawText(txt1,5.0f, 0.0f);
	//this->m_scorePanel->txt1 = buff;
	//sprintf(buff, "-%.2f",float(maxValue)/100.0f); 
	//this->m_scorePanel->txt2 = buff;
	double twidth = 0;
	double theight = 0;
	gc->GetTextExtent(txt2, &twidth, &theight);
	gc->DrawText(txt2,5.0f, h - theight*2);
	gc->DrawText("0.0",5.0f, h/2- theight );
	//dc.SetTextForeground(*wxBLACK);
	
	// debug
/*	sprintf(buff, "%s vs %s",
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
	
   // m_owner->PrepareDC(dc);*/
}
