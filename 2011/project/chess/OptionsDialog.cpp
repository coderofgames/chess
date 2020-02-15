#include "OptionsDialog.h"



BEGIN_EVENT_TABLE(OptionsDialog, wxFrame)

	
END_EVENT_TABLE()

OptionsDialog::OptionsDialog(MyFrame *parent)
	:wxFrame((wxWindow*)parent, wxID_ANY, "Choose a Variation", wxPoint(150,150), wxDefaultSize, wxWANTS_CHARS|wxWS_EX_BLOCK_EVENTS|/*wxSTAY_ON_TOP*/wxFRAME_FLOAT_ON_PARENT)
{
	
	this->pParent = parent;
	
	SetSize(200,400);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
 //   sizerTop->Add(m_lbox, 200, wxGROW | wxALL, 5);

    wxSizer *sizerBtns = new wxBoxSizer(wxHORIZONTAL);


    this->SetSizer(sizerTop);
    sizerTop->Fit(this);

  //  Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(VariationSelectionFrame::OnKeyDown),
   //                     NULL, this);
   // Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(VariationSelectionFrame::OnKeyDown),
    //                    NULL, m_lbox);
}

OptionsDialog::~OptionsDialog()
{

}