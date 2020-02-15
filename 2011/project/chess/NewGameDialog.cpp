#include "NewGameDialog.h"
#include "GameInfoDialog.h"
#include "mdi.h"

BEGIN_EVENT_TABLE(NewGameDialog, wxDialog)
	EVT_BUTTON(ID_WXBUTTON1, NewGameDialog::OnOk)
	EVT_BUTTON(ID_WXBUTTON3, NewGameDialog::EditHeader)
	//EVT_BUTTON(wxID_CANCEL, GameInfoModalDialog::OnCancel)

	//EVT_BUTTON(ADD_ENGINE, EngineListModalDialog::OnAddEngine)
	
END_EVENT_TABLE()

NewGameDialog::NewGameDialog(MyFrame * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog((wxWindow*)parent, id, title, pos, size, style)
{
	m_frame = parent;

	game.CreateEmptyHeader();

	WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, wxT("Add To Game List"), wxPoint(20, 170), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton1"));

	WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, wxT("Clear Game List"), wxPoint(20, 200), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton2"));


	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT(""), wxPoint(11, 155), wxSize(238, 85));

	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Edit Header"), wxPoint(76, 121), wxSize(97, 26), 0, wxDefaultValidator, wxT("WxButton3"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Event"), wxPoint(21, 79), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxEdit3 = new wxTextCtrl(this, ID_WXEDIT3, wxT(""), wxPoint(98, 79), wxSize(140, 18), 0, wxDefaultValidator, wxT("WxEdit3"));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Black Player"), wxPoint(21, 53), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("White Player"), wxPoint(21, 29), wxDefaultSize, 0, wxT("WxStaticText1"));

	WxEdit2 = new wxTextCtrl(this, ID_WXEDIT2, wxT(""), wxPoint(98, 53), wxSize(140, 20), 0, wxDefaultValidator, wxT("WxEdit2"));

	WxEdit1 = new wxTextCtrl(this, ID_WXEDIT1, wxT(""), wxPoint(98, 29), wxSize(140, 18), 0, wxDefaultValidator, wxT("WxEdit1"));

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(115, 256), wxSize(87, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Add Game"), wxPoint(15, 256), wxSize(87, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT(""), wxPoint(11, 9), wxSize(238, 104));


	SetTitle(wxT("NewGame"));
	SetIcon(wxNullIcon);
	SetSize(8,8,275,335);
	Center();
	retCode = wxID_CANCEL;

	whiteText = "";
	blackText = "";
	eventText = "";

	
	editedHeader = false;
}

NewGameDialog::~NewGameDialog()
{
}


void NewGameDialog::OnOk( wxCommandEvent & event )
{
	if( this->editedHeader == false )
	{
		whiteText = ((wxTextCtrl*)FindWindow(ID_WXEDIT1))->GetValue();
		blackText = ((wxTextCtrl*)FindWindow(ID_WXEDIT2))->GetValue();
		eventText = ((wxTextCtrl*)FindWindow(ID_WXEDIT3))->GetValue();
	}
	if(((wxRadioButton*)FindWindow(ID_WXRADIOBUTTON2))->GetValue() == true )
	{
		retCode = ADD_GAME_CLEAR_LIST;
	}
	else
	{
		retCode = ADD_GAME_TO_LIST;
	}
	Close(true);
}


void NewGameDialog::AddGameClearList( wxCommandEvent & event )
{
	//this->SetReturnCode(ADD_GAME_CLEAR_LIST);
	retCode = ADD_GAME_CLEAR_LIST;
	Close(true);
}

void NewGameDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}


void NewGameDialog::EditHeader(wxCommandEvent &event)
{
	GameInfoModalDialog dialog(this->m_frame, &game, this, wxID_ANY, "Edit Header");
	int ret = dialog.ShowModal();
	if( dialog.edited )
		this->editedHeader = true;
	else
		this->editedHeader = false;

}
