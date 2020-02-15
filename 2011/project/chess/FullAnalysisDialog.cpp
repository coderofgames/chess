#include "FullAnalysisDialog.h"
#include "mdi.h"

enum
{
	////GUI Enum Control ID Start
	ID_WXCHECKBOX16 = wxID_HIGHEST + 4024,
	ID_WXCHECKBOX15,
	ID_WXBUTTON11,
	ID_WXBUTTON10,
	ID_WXBUTTON9,
	ID_WXBUTTON8,
	ID_WXSTATICTEXT9,
	ID_WXCHOICE6,
	ID_WXSTATICBOX5,
	ID_WXSTATICTEXT8,
	ID_WXCHOICE5,
	ID_WXBUTTON7,
	ID_WXBUTTON6,
	ID_WXBUTTON5,
	ID_WXBUTTON4,
	ID_WXSTATICTEXT7,
	ID_WXCHOICE4,
	ID_WXSTATICTEXT6,
	ID_WXCHOICE3,
	ID_WXFILEPICKERCTRL1,
	ID_WXCHECKBOX14,
	ID_WXCHECKBOX13,
	ID_WXBUTTON3,
	ID_WXSTATICTEXT5,
	ID_WXCHOICE2,
	ID_WXCHECKBOX12,
	ID_WXCHECKBOX11,
	ID_WXCHECKBOX10,
	ID_WXCHECKBOX9,
	ID_WXSTATICTEXT4,
	ID_WXSPINCTRL3,
	ID_WXCHECKBOX8,
	ID_WXCHECKBOX7,
	ID_WXCHECKBOX6,
	ID_WXCHECKBOX3,
	ID_WXCHECKBOX5,
	ID_WXCHECKBOX4,
	ID_WXCHECKBOX2,
	ID_WXBUTTON2,
	ID_WXBUTTON1,
	ID_WXCHECKBOX1,
	ID_WXSTATICTEXT3 ,
	ID_WXSPINCTRL2 ,
	ID_WXSTATICTEXT2,
	ID_WXSPINCTRL1,
	ID_WXSTATICTEXT1 ,
	ID_WXCHOICE1,
	ID_WXSTATICBOX2 ,
	ID_WXSTATICBOX4 ,
	ID_WXSTATICBOX1 ,
	ID_WXSTATICBOX3 ,
	////GUI Enum Control ID End
	ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
};

BEGIN_EVENT_TABLE(FullAnalysisDialog, wxDialog)
	EVT_BUTTON(wxID_OK, FullAnalysisDialog::OnOk)


END_EVENT_TABLE()


FullAnalysisDialog::FullAnalysisDialog(MyFrame* pFrame,  wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{


	m_pFrame = pFrame;

	WxCheckBox16 = new wxCheckBox(this, ID_WXCHECKBOX16, wxT("  Create Graphs"), wxPoint(272, 48), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxCheckBox16"));

	WxCheckBox15 = new wxCheckBox(this, ID_WXCHECKBOX15, wxT("  Save Depth Scores"), wxPoint(272, 24), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxCheckBox15"));

	WxButton11 = new wxButton(this, ID_WXBUTTON11, wxT("Personality"), wxPoint(312, 264), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton11"));

	WxButton10 = new wxButton(this, ID_WXBUTTON10, wxT("Options"), wxPoint(224, 264), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton10"));

	WxButton9 = new wxButton(this, ID_WXBUTTON9, wxT("Personality"), wxPoint(312, 232), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton9"));

	WxButton8 = new wxButton(this, ID_WXBUTTON8, wxT("Options"), wxPoint(224, 232), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton8"));

	WxStaticText9 = new wxStaticText(this, ID_WXSTATICTEXT9, wxT("Engine 4"), wxPoint(160, 264), wxDefaultSize, 0, wxT("WxStaticText9"));

	wxArrayString arrayStringFor_WxChoice6;
	arrayStringFor_WxChoice6.Add(wxT("Fixed Depth"));
	arrayStringFor_WxChoice6.Add(wxT("Fixed Time"));
	arrayStringFor_WxChoice6.Add(wxT("Depth And Time"));
	arrayStringFor_WxChoice6.Add(wxT("Depth Or Time"));
	WxChoice6 = new wxChoice(this, ID_WXCHOICE6, wxPoint(16, 264), wxSize(129, 23), arrayStringFor_WxChoice6, 0, wxDefaultValidator, wxT("WxChoice6"));
	WxChoice6->SetSelection(-1);

	WxStaticBox5 = new wxStaticBox(this, ID_WXSTATICBOX5, wxT("Graphs"), wxPoint(264, 8), wxSize(145, 129));

	WxStaticText8 = new wxStaticText(this, ID_WXSTATICTEXT8, wxT("Engine 3"), wxPoint(160, 232), wxDefaultSize, 0, wxT("WxStaticText8"));

	wxArrayString arrayStringFor_WxChoice5;
	arrayStringFor_WxChoice5.Add(wxT("Fixed Depth"));
	arrayStringFor_WxChoice5.Add(wxT("Fixed Time"));
	arrayStringFor_WxChoice5.Add(wxT("Depth And Time"));
	arrayStringFor_WxChoice5.Add(wxT("Depth Or Time"));
	WxChoice5 = new wxChoice(this, ID_WXCHOICE5, wxPoint(16, 232), wxSize(129, 23), arrayStringFor_WxChoice5, 0, wxDefaultValidator, wxT("WxChoice5"));
	WxChoice5->SetSelection(-1);

	WxButton7 = new wxButton(this, ID_WXBUTTON7, wxT("Personality"), wxPoint(312, 200), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton7"));

	WxButton6 = new wxButton(this, ID_WXBUTTON6, wxT("Options"), wxPoint(224, 200), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton6"));

	WxButton5 = new wxButton(this, ID_WXBUTTON5, wxT("Personality"), wxPoint(312, 168), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton5"));

	WxButton4 = new wxButton(this, ID_WXBUTTON4, wxT("Options"), wxPoint(224, 168), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton4"));

	WxStaticText7 = new wxStaticText(this, ID_WXSTATICTEXT7, wxT("Engine 2"), wxPoint(160, 200), wxDefaultSize, 0, wxT("WxStaticText7"));

	wxArrayString arrayStringFor_WxChoice4;
	arrayStringFor_WxChoice4.Add(wxT("Fixed Depth"));
	arrayStringFor_WxChoice4.Add(wxT("Fixed Time"));
	arrayStringFor_WxChoice4.Add(wxT("Depth And Time"));
	arrayStringFor_WxChoice4.Add(wxT("Depth Or Time"));
	WxChoice4 = new wxChoice(this, ID_WXCHOICE4, wxPoint(16, 200), wxSize(129, 23), arrayStringFor_WxChoice4, 0, wxDefaultValidator, wxT("WxChoice4"));
	WxChoice4->SetSelection(-1);

	WxStaticText6 = new wxStaticText(this, ID_WXSTATICTEXT6, wxT("Engine 1"), wxPoint(160, 168), wxDefaultSize, 0, wxT("WxStaticText6"));

	wxArrayString arrayStringFor_WxChoice3;
	arrayStringFor_WxChoice3.Add(wxT("Fixed Depth"));
	arrayStringFor_WxChoice3.Add(wxT("Fixed Time"));
	arrayStringFor_WxChoice3.Add(wxT("Depth And Time"));
	arrayStringFor_WxChoice3.Add(wxT("Depth Or Time"));
	WxChoice3 = new wxChoice(this, ID_WXCHOICE3, wxPoint(16, 168), wxSize(129, 23), arrayStringFor_WxChoice3, 0, wxDefaultValidator, wxT("WxChoice3"));
	WxChoice3->SetSelection(-1);

	WxFilePickerCtrl1 = new wxFilePickerCtrl(this, ID_WXFILEPICKERCTRL1, wxT(""), wxT("Select a file"), wxT("*.*"), wxPoint(568, 224), wxSize(177, 25), wxFLP_DEFAULT_STYLE, wxDefaultValidator, wxT("WxFilePickerCtrl1"));

	WxCheckBox14 = new wxCheckBox(this, ID_WXCHECKBOX14, wxT("Find Zugzwangs"), wxPoint(816, 56), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxCheckBox14"));

	WxCheckBox13 = new wxCheckBox(this, ID_WXCHECKBOX13, wxT("  Mark Forced Moves"), wxPoint(672, 56), wxSize(129, 25), 0, wxDefaultValidator, wxT("WxCheckBox13"));

	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Load"), wxPoint(856, 88), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, wxT("Book"), wxPoint(808, 88), wxDefaultSize, 0, wxT("WxStaticText5"));

	wxArrayString arrayStringFor_WxChoice2;
	arrayStringFor_WxChoice2.Add(wxT("Fixed Depth"));
	arrayStringFor_WxChoice2.Add(wxT("Fixed Time"));
	arrayStringFor_WxChoice2.Add(wxT("Depth And Time"));
	arrayStringFor_WxChoice2.Add(wxT("Depth Or Time"));
	WxChoice2 = new wxChoice(this, ID_WXCHOICE2, wxPoint(672, 88), wxSize(129, 23), arrayStringFor_WxChoice2, 0, wxDefaultValidator, wxT("WxChoice2"));
	WxChoice2->SetSelection(-1);

	WxCheckBox12 = new wxCheckBox(this, ID_WXCHECKBOX12, wxT("  Mark Book Moves"), wxPoint(432, 88), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxCheckBox12"));

	WxCheckBox11 = new wxCheckBox(this, ID_WXCHECKBOX11, wxT("  Classify ECO"), wxPoint(560, 88), wxSize(105, 25), 0, wxDefaultValidator, wxT("WxCheckBox11"));

	WxCheckBox10 = new wxCheckBox(this, ID_WXCHECKBOX10, wxT("  Insert Text Analysis"), wxPoint(576, 160), wxSize(145, 25), 0, wxDefaultValidator, wxT("WxCheckBox10"));

	WxCheckBox9 = new wxCheckBox(this, ID_WXCHECKBOX9, wxT("  Find Killers"), wxPoint(560, 56), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxCheckBox9"));

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("Number of Lines (pv's)"), wxPoint(744, 24), wxDefaultSize, 0, wxT("WxStaticText4"));

	WxSpinCtrl3 = new wxSpinCtrl(this, ID_WXSPINCTRL3, wxT("0"), wxPoint(616, 24), wxSize(113, 24), wxSP_ARROW_KEYS, 0, 100, 0);

	WxCheckBox8 = new wxCheckBox(this, ID_WXCHECKBOX8, wxT("  Write to Log"), wxPoint(432, 224), wxSize(129, 25), 0, wxDefaultValidator, wxT("WxCheckBox8"));

	WxCheckBox7 = new wxCheckBox(this, ID_WXCHECKBOX7, wxT("  Write to Annotation"), wxPoint(432, 192), wxSize(129, 25), 0, wxDefaultValidator, wxT("WxCheckBox7"));

	WxCheckBox6 = new wxCheckBox(this, ID_WXCHECKBOX6, wxT("  Generate Alternative Lines"), wxPoint(432, 24), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxCheckBox6"));

	WxCheckBox3 = new wxCheckBox(this, ID_WXCHECKBOX3, wxT("  Write PV"), wxPoint(856, 192), wxSize(89, 25), 0, wxDefaultValidator, wxT("WxCheckBox3"));

	WxCheckBox5 = new wxCheckBox(this, ID_WXCHECKBOX5, wxT("  Write Depth / Time"), wxPoint(704, 192), wxSize(145, 25), 0, wxDefaultValidator, wxT("WxCheckBox5"));

	WxCheckBox4 = new wxCheckBox(this, ID_WXCHECKBOX4, wxT("  Write Scores"), wxPoint(576, 192), wxSize(145, 25), 0, wxDefaultValidator, wxT("WxCheckBox4"));

	WxCheckBox2 = new wxCheckBox(this, ID_WXCHECKBOX2, wxT("  Find Blunders"), wxPoint(432, 56), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxCheckBox2"));

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(856, 272), wxSize(113, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(728, 272), wxSize(113, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	WxCheckBox1 = new wxCheckBox(this, ID_WXCHECKBOX1, wxT("  Save all Analysis"), wxPoint(432, 160), wxSize(121, 25), 0, wxDefaultValidator, wxT("WxCheckBox1"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Search Depth"), wxPoint(168, 96), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxSpinCtrl2 = new wxSpinCtrl(this, ID_WXSPINCTRL2, wxT("1"), wxPoint(16, 96), wxSize(129, 24), wxSP_ARROW_KEYS, 1, 50, 1);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Search Time"), wxPoint(168, 64), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxSpinCtrl1 = new wxSpinCtrl(this, ID_WXSPINCTRL1, wxT("0"), wxPoint(16, 64), wxSize(129, 24), wxSP_ARROW_KEYS, 0, 2000000, 0);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Search Type"), wxPoint(168, 32), wxDefaultSize, 0, wxT("WxStaticText1"));

	wxArrayString arrayStringFor_WxChoice1;
	arrayStringFor_WxChoice1.Add(wxT("Fixed Depth"));
	arrayStringFor_WxChoice1.Add(wxT("Fixed Time"));
	arrayStringFor_WxChoice1.Add(wxT("Depth And Time"));
	arrayStringFor_WxChoice1.Add(wxT("Depth Or Time"));
	WxChoice1 = new wxChoice(this, ID_WXCHOICE1, wxPoint(16, 32), wxSize(129, 23), arrayStringFor_WxChoice1, 0, wxDefaultValidator, wxT("WxChoice1"));
	WxChoice1->SetSelection(-1);

	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT("Output"), wxPoint(424, 144), wxSize(553, 121));

	WxStaticBox4 = new wxStaticBox(this, ID_WXSTATICBOX4, wxT("Engines"), wxPoint(8, 144), wxSize(401, 153));

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT("Search Type"), wxPoint(8, 8), wxSize(241, 129));

	WxStaticBox3 = new wxStaticBox(this, ID_WXSTATICBOX3, wxT("Config"), wxPoint(424, 8), wxSize(553, 121));

	SetTitle(wxT("FullAnalysisDialog"));
	SetIcon(wxNullIcon);
	SetSize(8,8,1008,344);
	Center();

}

FullAnalysisDialog::~FullAnalysisDialog()
{
}

void FullAnalysisDialog::OnOk( wxCommandEvent & event )
{

	Close(true);
}




void FullAnalysisDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}