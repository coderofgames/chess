///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ECO_FilterDialog.h"

///////////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(ECO_FilterDialog, wxDialog)
	EVT_BUTTON(wxID_OK, ECO_FilterDialog::OnOk)
	//EVT_BUTTON(wxID_CANCEL, GameInfoModalDialog::OnCancel)
	//EVT_BUTTON(EDIT_LIST, GameInfoModalDialog::OnEditElem)
	//EVT_BUTTON(ADD_TO_LIST, GameInfoModalDialog::OnEditElem)
	//EVT_BUTTON(DELETE_ELEM, GameInfoModalDialog::OnEditElem)

END_EVENT_TABLE()

ECO_FilterDialog::ECO_FilterDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxSize(wxDefaultSize.GetWidth()/2, wxDefaultSize.GetHeight()) );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 2, 0, 0 );
	
	wxString ECO_Letter[5];
	ECO_Letter[0] = "A";
	ECO_Letter[1] = "B";
	ECO_Letter[2] = "C";
	ECO_Letter[3] = "D";
	ECO_Letter[4] = "E";
	m_comboBox1 = new wxComboBox( this, wxID_ANY, wxT("A"), wxDefaultPosition, wxDefaultSize, 5, ECO_Letter, 0 ); 
	gSizer1->Add( m_comboBox1, 0, wxALL, 5 );
	
	m_spinCtrl2 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0 );
	gSizer1->Add( m_spinCtrl2, 0, wxALL, 5 );
	
	m_comboBox3 = new wxComboBox( this, wxID_ANY, wxT("A"), wxDefaultPosition, wxDefaultSize, 5, ECO_Letter, 0 ); 
	gSizer1->Add( m_comboBox3, 0, wxALL, 5 );
	
	m_spinCtrl3 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0 );
	gSizer1->Add( m_spinCtrl3, 0, wxALL, 5 );
	
	m_button5 = new wxButton( this, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button5, 0, wxALL, 5 );
	
	m_button6 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button6, 0, wxALL, 5 );
	
	this->SetSizer( gSizer1 );
	this->Layout();

	Ok = false;
	
	this->Centre( wxBOTH );
}

ECO_FilterDialog::~ECO_FilterDialog()
{
}

void ECO_FilterDialog::OnOk( wxCommandEvent & event )
{
	string combo1Sel = this->m_comboBox1->GetValue();
	string combo2Sel = this->m_comboBox3->GetValue();

	int num1 = m_spinCtrl2->GetValue();
	int num2 = m_spinCtrl3->GetValue();

	char buff1[10];
	if( num1 < 10 )
	{
		sprintf( buff1, "0%d", num1 );
	}
	else
	{
		sprintf( buff1, "%d", num1 );
	}

	char buff2[10];
	if( num2 < 10 )
	{
		sprintf( buff2, "0%d", num2 );
	}
	else
	{
		sprintf( buff2, "%d", num2 );
	}

	eco1 = combo1Sel + buff1;
	eco2 = combo2Sel + buff2;

	Ok = true;

	Close(true);
}
