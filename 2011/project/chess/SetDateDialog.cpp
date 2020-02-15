#include "SetDateDialog.h"


BEGIN_EVENT_TABLE(SetDateDialog, wxDialog)
	EVT_BUTTON(wxID_OK, SetDateDialog::OnOk)
	
END_EVENT_TABLE()

SetDateDialog::SetDateDialog(wxWindow * parent, wxWindowID id, const wxString & title, wxString text,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{

	//m_text = text;
	//QueenBtn->SetStyle
	wxSizer *sizerBtns = new wxBoxSizer(wxHORIZONTAL);
	wxSizer *sizerDlg = new wxBoxSizer(wxVERTICAL);

	dateCtrl = new wxDatePickerCtrl(this, wxID_ANY);//, const wxDateTime &dt=wxDefaultDateTime, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxDP_DEFAULT|wxDP_SHOWCENTURY, const wxValidator &validator=wxDefaultValidator, const wxString &name="datectrl")
 	//Initializes the object and calls Create() with all the parameters. 
	wxButton * b = new wxButton( this, wxID_OK, _("OK") );
	
	wxButton * c = new wxButton( this, wxID_CANCEL, _("CANCEL") );
	/*sizerBtns->
        Add(QueenBtn, 0, wxALL, 5);
	sizerBtns->
        Add(BishopBtn, 0, wxALL, 5);*/
	sizerBtns->
        Add(b, 0, wxEXPAND, 5);
	sizerBtns->
        Add(c, 0, wxEXPAND, 5);
		
	sizerDlg->Add(dateCtrl,0,wxEXPAND,5);
	sizerDlg->Add(sizerBtns,0,wxEXPAND,5);

	    this->SetSizer(sizerDlg);
    sizerDlg->Fit(this);
	
}

SetDateDialog::~SetDateDialog()
{
}


wxString SetDateDialog::GetString()
{
	//m_text = dateCtrl->GetValue().FormatDate();
	return dateCtrl->GetValue().FormatDate();
}

void SetDateDialog::OnOk( wxCommandEvent & event )
{
	m_text = dateCtrl->GetValue().FormatDate();//FormatISODate();
	Close(true);
}

void SetDateDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}
