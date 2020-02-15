#include "TimeControlSettingsDialog.h"
#include "mdi.h"
//#include "PgnLoad.h"
#include "MiscUtils.h"
//#include "NotationFrame.h"
//#include "SetDateDialog.h"

enum
{
	////GUI Enum Control ID Start
	ID_WXSTATICTEXT10 = wxID_HIGHEST +7810,
	ID_WXSTATICTEXT8,// = 1030,
	ID_WXSTATICTEXT7,// = 1029,
	ID_WXSTATICTEXT6,// = 1028,
	ID_WXSTATICTEXT5,// = 1027,
	ID_WXSTATICTEXT4,// = 1026,
	ID_WXSTATICTEXT3,/// = 1025,
	ID_WXSTATICTEXT2,// = 1024,
	ID_WXSTATICTEXT1,// = 1023,
	ID_WXCOMBOBOX5,// = 1022,
	ID_WXCOMBOBOX4,// = 1021,
	ID_WXCOMBOBOX3,// = 1020,
	ID_WXCOMBOBOX7,// = 1019,
	ID_WXCOMBOBOX2,// = 1018,
	ID_WXSPINCTRL7,// = 1016,
	ID_WXSPINCTRL6,// = 1013,
	ID_WXSPINCTRL5,// = 1012,
	ID_WXSPINCTRL4,// = 1011,
	ID_WXRADIOBUTTON2,// = 1009,
	ID_WXRADIOBUTTON1,// = 1007,
	ID_WXCOMBOBOX1,// = 1006,
	ID_WXSPINCTRL3,// = 1005,
	ID_WXSPINCTRL2,// = 1004,
	ID_WXSPINCTRL1,// = 1003,
	ID_WXBUTTON2,// = 1002,
	ID_WXSTATICBOX1,// = 1033,
	ID_WXBUTTON1,// = 1001,
	ID_WXSTATICBOX2,// = 1034,
	////GUI Enum Control ID End
	//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
};

BEGIN_EVENT_TABLE(TimeControlDialog, wxDialog)
	EVT_BUTTON(wxID_OK, TimeControlDialog::OnOk)
	//EVT_BUTTON(wxID_CANCEL, GameInfoModalDialog::OnCancel)
	//EVT_BUTTON(EDIT_LIST, GameInfoModalDialog::OnEditElem)
	//EVT_BUTTON(ADD_TO_LIST, GameInfoModalDialog::OnEditElem)
	//EVT_BUTTON(DELETE_ELEM, GameInfoModalDialog::OnEditElem)

END_EVENT_TABLE()


TimeControlDialog::TimeControlDialog(MyFrame* pFrame, PGN::Game* game, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{


	m_pFrame = pFrame;
	//pGame = game;



	WxStaticText10 = new wxStaticText(this, ID_WXSTATICTEXT10, wxT("For the Rest of the Game"), wxPoint(168, 208), wxDefaultSize, 0, wxT("WxStaticText10"));
	WxStaticText10->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxBOLD, false));

	WxStaticText8 = new wxStaticText(this, ID_WXSTATICTEXT8, wxT("Moves"), wxPoint(352, 104), wxDefaultSize, 0, wxT("WxStaticText8"));
	WxStaticText8->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxBOLD, false));

	WxStaticText7 = new wxStaticText(this, ID_WXSTATICTEXT7, wxT("For The First"), wxPoint(168, 104), wxDefaultSize, 0, wxT("WxStaticText7"));
	WxStaticText7->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxBOLD, false));

	WxStaticText6 = new wxStaticText(this, ID_WXSTATICTEXT6, wxT("Increment"), wxPoint(352, 272), wxDefaultSize, 0, wxT("WxStaticText6"));

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, wxT("Initial Time"), wxPoint(352, 240), wxDefaultSize, 0, wxT("WxStaticText5"));

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("Increment"), wxPoint(352, 48), wxDefaultSize, 0, wxT("WxStaticText4"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Initial Time"), wxPoint(352, 16), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Increment"), wxPoint(352, 168), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Initial Time"), wxPoint(352, 136), wxDefaultSize, 0, wxT("WxStaticText1"));

	wxArrayString arrayStringFor_WxComboBox5;
	arrayStringFor_WxComboBox5.Add(wxT("Seconds"));
	arrayStringFor_WxComboBox5.Add(wxT("Milli-Seconds"));
	WxComboBox5 = new wxComboBox(this, ID_WXCOMBOBOX5, wxT("WxComboBox5"), wxPoint(256, 272), wxSize(81, 23), arrayStringFor_WxComboBox5, 0, wxDefaultValidator, wxT("WxComboBox5"));
	WxComboBox5->SetSelection(1);

	wxArrayString arrayStringFor_WxComboBox4;
	arrayStringFor_WxComboBox4.Add(wxT("Minutes"));
	arrayStringFor_WxComboBox4.Add(wxT("Seconds"));
	WxComboBox4 = new wxComboBox(this, ID_WXCOMBOBOX4, wxT("WxComboBox4"), wxPoint(256, 240), wxSize(81, 23), arrayStringFor_WxComboBox4, 0, wxDefaultValidator, wxT("WxComboBox4"));
	WxComboBox4->SetSelection(1);

	wxArrayString arrayStringFor_WxComboBox3;
	arrayStringFor_WxComboBox3.Add(wxT("Seconds"));
	arrayStringFor_WxComboBox3.Add(wxT("Milli-Seconds"));
	WxComboBox3 = new wxComboBox(this, ID_WXCOMBOBOX3, wxT("WxComboBox3"), wxPoint(256, 168), wxSize(81, 23), arrayStringFor_WxComboBox3, 0, wxDefaultValidator, wxT("WxComboBox3"));
	WxComboBox3->SetSelection(1);

	wxArrayString arrayStringFor_WxComboBox7;
	arrayStringFor_WxComboBox7.Add(wxT("Minutes"));
	arrayStringFor_WxComboBox7.Add(wxT("Seconds"));
	WxComboBox7 = new wxComboBox(this, ID_WXCOMBOBOX7, wxT("WxComboBox7"), wxPoint(256, 136), wxSize(81, 23), arrayStringFor_WxComboBox7, 0, wxDefaultValidator, wxT("WxComboBox7"));
	WxComboBox7->SetSelection(1);

	wxArrayString arrayStringFor_WxComboBox2;
	arrayStringFor_WxComboBox2.Add(wxT("Seconds"));
	arrayStringFor_WxComboBox2.Add(wxT("Milli-Seconds"));
	WxComboBox2 = new wxComboBox(this, ID_WXCOMBOBOX2, wxT("WxComboBox2"), wxPoint(256, 48), wxSize(81, 23), arrayStringFor_WxComboBox2, 0, wxDefaultValidator, wxT("WxComboBox2"));
	WxComboBox2->SetSelection(1);
	WxSpinCtrl7 = new wxSpinCtrl(this, ID_WXSPINCTRL7, wxT("0"), wxPoint(160, 272), wxSize(89, 24), wxSP_ARROW_KEYS, 0, 1000, 0);

	WxSpinCtrl6 = new wxSpinCtrl(this, ID_WXSPINCTRL6, wxT("0"), wxPoint(160, 240), wxSize(89, 24), wxSP_ARROW_KEYS, 0, 1440, 0);

	WxSpinCtrl5 = new wxSpinCtrl(this, ID_WXSPINCTRL5, wxT("0"), wxPoint(160, 168), wxSize(89, 24), wxSP_ARROW_KEYS, 0, 1000, 0);

	WxSpinCtrl4 = new wxSpinCtrl(this, ID_WXSPINCTRL4, wxT("0"), wxPoint(256, 104), wxSize(89, 24), wxSP_ARROW_KEYS, 0, 1440, 0);

	WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, wxT("FIDE style"), wxPoint(16, 104), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxRadioButton2"));

	WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, wxT("Standard"), wxPoint(16, 16), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxRadioButton1"));
	WxRadioButton1->SetValue(true);

	wxArrayString arrayStringFor_WxComboBox1;
	arrayStringFor_WxComboBox1.Add(wxT("Minutes"));
	arrayStringFor_WxComboBox1.Add(wxT("Seconds"));
	WxComboBox1 = new wxComboBox(this, ID_WXCOMBOBOX1, wxT("WxComboBox1"), wxPoint(256, 16), wxSize(81, 23), arrayStringFor_WxComboBox1, 0, wxDefaultValidator, wxT("WxComboBox1"));
	WxComboBox1->SetSelection(1);
	WxSpinCtrl3 = new wxSpinCtrl(this, ID_WXSPINCTRL3, wxT("0"), wxPoint(160, 136), wxSize(89, 24), wxSP_ARROW_KEYS, 0, 1440, 0);

	WxSpinCtrl2 = new wxSpinCtrl(this, ID_WXSPINCTRL2, wxT("0"), wxPoint(160, 48), wxSize(89, 24), wxSP_ARROW_KEYS, 0, 1000, 0);

	WxSpinCtrl1 = new wxSpinCtrl(this, ID_WXSPINCTRL1, wxT("0"), wxPoint(160, 16), wxSize(89, 24), wxSP_ARROW_KEYS, 1, 100, 0);
	WxSpinCtrl1->SetValue(10);

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(120, 328), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT(""), wxPoint(8, 88), wxSize(433, 225));

	WxButton1 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(16, 328), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT(""), wxPoint(8, 0), wxSize(433, 89));

	SetTitle(wxT("TimeControlDialog"));
	SetIcon(wxNullIcon);
	SetSize(8,8,465,410);
	Center();


	res = 0;

	bFischer = true;

	//edited= false;
	
	//p.x += 110;
	//wxButton * c = new wxButton( this, wxID_CANCEL, _("Cancel"), p, wxDefaultSize );
}

TimeControlDialog::~TimeControlDialog()
{
}

void TimeControlDialog::OnOk( wxCommandEvent & event )
{
	bFischer = WxRadioButton1->GetValue();
	//PGN::Game* pGame = m_pFrame->currentGame;
	if( WxComboBox1->GetValue() == "Minutes" )
	{
		start_minutes = WxSpinCtrl1->GetValue();
		start_seconds = start_minutes*60;
	}
	else
	{
		start_seconds = WxSpinCtrl1->GetValue();
		start_minutes = start_seconds / 60;
	}

	if( WxComboBox2->GetValue() == "Seconds" )
	{
		start_inc_seconds = WxSpinCtrl2->GetValue();
		start_inc_millis = start_inc_seconds * 1000;
	}
	else
	{
		start_inc_millis = WxSpinCtrl2->GetValue();
		start_inc_seconds = start_inc_millis / 1000;
	}

//	wxMessageBox(wxString::Format("%d minutes, %d seconds",start_minutes ,start_seconds),"");
//	wxMessageBox(wxString::Format("%d inc seconds, %d inc millis",start_inc_seconds ,start_inc_millis),"");

	if( !bFischer )
	{
		time_control_one_num_moves = WxSpinCtrl4->GetValue();

		wxMessageBox(wxString::Format("%d num moves for first time control",time_control_one_num_moves),"");

		if( WxComboBox7->GetValue() == "Minutes" )
		{
			start_minutes = WxSpinCtrl3->GetValue();
			start_seconds = start_minutes*60;
		}
		else
		{
			start_seconds = WxSpinCtrl3->GetValue();
			start_minutes = start_seconds / 60;
		}

		if( WxComboBox3->GetValue() == "Seconds" )
		{
			start_inc_seconds = WxSpinCtrl5->GetValue();
			start_inc_millis = start_inc_seconds * 1000;
		}
		else
		{
			start_inc_millis = WxSpinCtrl5->GetValue();
			start_inc_seconds = start_inc_millis / 1000;
		}

	//	wxMessageBox(wxString::Format("%d minutes, %d seconds",start_minutes ,start_seconds),"");
	//	wxMessageBox(wxString::Format("%d inc seconds, %d inc millis",start_inc_seconds ,start_inc_millis),"");

		


		if( WxComboBox4->GetValue() == "Minutes" )
		{
			time_control_one_minutes = WxSpinCtrl6->GetValue();
			time_control_one_seconds = time_control_one_minutes*60;
		}
		else
		{
			time_control_one_seconds = WxSpinCtrl6->GetValue();
			time_control_one_minutes = time_control_one_seconds / 60;
		}

		if( WxComboBox5->GetValue() == "Seconds" )
		{
			time_control_one_inc_seconds = WxSpinCtrl7->GetValue();
			time_control_one_inc_millis = time_control_one_inc_seconds * 1000;
		}
		else
		{
			time_control_one_inc_millis = WxSpinCtrl7->GetValue();
			time_control_one_inc_seconds = time_control_one_inc_millis / 1000;
		}

	//	wxMessageBox(wxString::Format("%d minutes, %d seconds",time_control_one_minutes ,time_control_one_seconds),"");
	//	wxMessageBox(wxString::Format("%d inc seconds, %d inc millis",time_control_one_inc_seconds ,time_control_one_inc_millis),"");



	}

	res = wxID_OK;

	Close(true);
}

void TimeControlDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}