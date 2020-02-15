#include "FindDialog.h"

#include "mdi.h"

BEGIN_EVENT_TABLE(FindDialog, wxDialog)
	EVT_BUTTON(wxID_OK, FindDialog::OnOk)

END_EVENT_TABLE()


enum
{
	ID_WXLISTBOX1 = wxID_HIGHEST +8333
};


FindDialog::FindDialog(MyFrame* pFrame,  wxWindow * parent, wxWindowID id, const wxString & title,int *sel,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)

{
	m_pFrame = pFrame;

	m_selection = sel;
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start
	
		
	WxButton8 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(152, 272), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton8"));

	WxButton7 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(32, 272), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton7"));

	wxArrayString arrayStringFor_WxListBox1;
	for( int i = 0; i < m_pFrame->find_possibilities.size(); i++ )
	{
		arrayStringFor_WxListBox1.Add(m_pFrame->find_possibilities[i].str );
	}
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(16, 16), wxSize(249, 225), arrayStringFor_WxListBox1, wxLB_SINGLE);

	SetTitle(wxT("Find"));
	SetIcon(wxNullIcon);
	SetSize(8,8,298,349);
	Center();

	////GUI Items Creation End
}

void FindDialog::OnClose(wxCloseEvent & evt)
{

	Destroy();
}

void FindDialog::OnOk( wxCommandEvent & evt )
{
	stringSelection = WxListBox1->GetStringSelection();
	*m_selection = WxListBox1->GetSelection();
	Close(true);
}
	
void FindDialog::OnCancel( wxCommandEvent & evt )
{
	Close(true);
}
