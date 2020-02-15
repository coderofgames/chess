#include "EngineMatchDialog.h"



#include "mdi.h"
#include "MyPipePanel.h"
#include "EngineListDialog.h"
#include "UCIEngineOptionsDialog.h"
#include "BoardCanvas.h"
#include "GameClock.h"


enum
{
	ID_WXCHOICE1 = wxID_HIGHEST + 4000,
	ID_WXFILEPICKERCTRL2,
	ID_WXBUTTON1,
	ID_WXBUTTON2,
	ID_WXBUTTON3,
	ID_WXBUTTON4,
	ID_WXBUTTON5,
	ID_WXBUTTON6,
	ID_WXBUTTON7,
	ID_WXCHECKBOX1,
	ID_WXCHECKBOX2,
	ID_WXCHECKBOX3,
	ID_WXSTATICTEXT1,
	ID_WXSTATICTEXT2,
	ID_WXSTATICTEXT3,
	ID_WXRADIOBUTTON1,
	ID_WXRADIOBUTTON2,
	ID_WXRADIOBUTTON3,
	ID_WXSPINCTRL1,
	ID_WXSPINCTRL2,
	ID_WXSPINCTRL3,
	ID_WXCOMBOBOX1,
	ID_WXCOMBOBOX2,
	ID_WXSTATICBOX1,
	ID_WXSTATICBOX2,
	ID_WXSTATICBOX3,
	ID_WXSTATICTEXT4,
	ID_WXEDIT1,
	ID_WXFILEPICKERCTRL1,
	ID_WXSTATICTEXT5,
	ID_WXSTATICTEXT6,
	ID_WXSPINCTRL4,
	ID_WXCOMBOBOX3,
	ID_WXCHECKBOX4,

};

#include "TimeControlSettingsDialog.h"

/*
	// other events
	void OnEngine1Load( wxCommandEvent & event );
	void OnEngine1Configure( wxCommandEvent & event );

	void OnEngine2Load( wxCommandEvent & event );
	void OnEngine2Configure( wxCommandEvent & event );

	void OnRadio( wxCommandEvent &event );
	void OnRadioButton1( wxCommandEvent &event );
	void OnRadioButton2( wxCommandEvent &event );
*/
BEGIN_EVENT_TABLE(EngineMatchDialog, wxDialog)
	EVT_BUTTON(wxID_OK, EngineMatchDialog::OnOk)
	EVT_BUTTON(ID_WXBUTTON6, EngineMatchDialog::OnEngine1Load)
	EVT_BUTTON(ID_WXBUTTON3, EngineMatchDialog::OnEngine1Configure)
	EVT_BUTTON(ID_WXBUTTON5, EngineMatchDialog::OnEngine2Load)
	EVT_BUTTON(ID_WXBUTTON4, EngineMatchDialog::OnEngine2Configure)
	EVT_BUTTON(ID_WXBUTTON7, EngineMatchDialog::OnTimeControlConfigure)
	

	EVT_RADIOBUTTON  (ID_WXRADIOBUTTON1,             EngineMatchDialog::OnRadioButton1)
	EVT_RADIOBUTTON  (ID_WXRADIOBUTTON2,            EngineMatchDialog::OnRadioButton2)
	EVT_RADIOBUTTON  (ID_WXRADIOBUTTON3,            EngineMatchDialog::OnRadioButton3)

END_EVENT_TABLE()


EngineMatchDialog::EngineMatchDialog(MyFrame* pFrame,  wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{

	retID = 0;
	m_pFrame = pFrame;
	timeControlID = 0;
	timeControlType = 0;
	engine1 = "";
	engine2 = "";
	numGames = 1;
	swapSidesBetweenGames = false;
	playSamePositionForEachColor = false;
	logEngineOutput = false;

	timeInMilliSecs = 0;
	timeInSeconds = 0;
	timeInMinutes = 0;
	incrementMillis = 0;
	incrementSeconds = 0 ;
	incrementMinutes = 0;

	timeConfigured = false;

/*

	WxCheckBox3 = new wxCheckBox(this, ID_WXCHECKBOX3, wxT("Use Best Moves (random default)"), wxPoint(240, 264), wxSize(209, 25), 0, wxDefaultValidator, wxT("WxCheckBox3"));

	WxStaticText6 = new wxStaticText(this, ID_WXSTATICTEXT6, wxT("Max Ply"), wxPoint(120, 264), wxDefaultSize, 0, wxT("WxStaticText6"));

	wxArrayString arrayStringFor_WxComboBox3;
	arrayStringFor_WxComboBox3.Add(wxT("GUI Book"));
	arrayStringFor_WxComboBox3.Add(wxT("Own Book"));
	arrayStringFor_WxComboBox3.Add(wxT("PGN File"));
	WxComboBox3 = new wxComboBox(this, ID_WXCOMBOBOX3, wxEmptyString, wxPoint(24, 232), wxSize(177, 23), arrayStringFor_WxComboBox3, 0, wxDefaultValidator, wxT("WxComboBox3"));
	WxComboBox3->SetSelection(0);
	WxSpinCtrl4 = new wxSpinCtrl(this, ID_WXSPINCTRL4, wxT("10"), wxPoint(24, 264), wxSize(81, 24), wxSP_ARROW_KEYS, 0, 100, 0);

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, wxT("Book File (PGN)"), wxPoint(392, 232), wxDefaultSize, 0, wxT("WxStaticText5"));

	WxFilePickerCtrl1 = new wxFilePickerCtrl(this, ID_WXFILEPICKERCTRL1, wxT(""), wxT("Select a file"), wxT("*.*"), wxPoint(240, 232), wxSize(145, 25), wxFLP_DEFAULT_STYLE, wxDefaultValidator, wxT("WxFilePickerCtrl1"));

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("Match Name"), wxPoint(192, 8), wxDefaultSize, 0, wxT("WxStaticText4"));

	WxEdit1 = new wxTextCtrl(this, ID_WXEDIT1, wxT("Engine Match"), wxPoint(16, 8), wxSize(161, 25), 0, wxDefaultValidator, wxT("WxEdit1"));

	WxButton7 = new wxButton(this, ID_WXBUTTON7, wxT("Configure"), wxPoint(272, 344), wxSize(73, 25), 0, wxDefaultValidator, wxT("WxButton7"));

	wxArrayString arrayStringFor_WxChoice1;
	arrayStringFor_WxChoice1.Add(wxT("Hyper-Bullet"));
	arrayStringFor_WxChoice1.Add(wxT("Bullet"));
	arrayStringFor_WxChoice1.Add(wxT("Blitz"));
	arrayStringFor_WxChoice1.Add(wxT("Rapid"));
	arrayStringFor_WxChoice1.Add(wxT("Standard"));
	arrayStringFor_WxChoice1.Add(wxT("Long"));
	WxChoice1 = new wxChoice(this, ID_WXCHOICE1, wxPoint(160, 344), wxSize(97, 23), arrayStringFor_WxChoice1, 0, wxDefaultValidator, wxT("WxChoice1"));
	WxChoice1->SetSelection(2);

	WxButton6 = new wxButton(this, ID_WXBUTTON6, wxT("Load"), wxPoint(296, 96), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton6"));

	WxButton5 = new wxButton(this, ID_WXBUTTON5, wxT("Load"), wxPoint(296, 128), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton5"));

	WxButton4 = new wxButton(this, ID_WXBUTTON4, wxT("Configure"), wxPoint(392, 128), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton4"));

	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Configure"), wxPoint(392, 96), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxCheckBox2 = new wxCheckBox(this, ID_WXCHECKBOX2, wxT("Play Same Position For Each Colour"), wxPoint(240, 200), wxSize(209, 25), 0, wxDefaultValidator, wxT("WxCheckBox2"));

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(144, 464), wxSize(105, 33), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(16, 464), wxSize(105, 33), 0, wxDefaultValidator, wxT("WxButton1"));

	WxCheckBox1 = new wxCheckBox(this, ID_WXCHECKBOX1, wxT("Swap Sides Between Games"), wxPoint(24, 200), wxSize(177, 25), 0, wxDefaultValidator, wxT("WxCheckBox1"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Engine Two"), wxPoint(216, 128), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Engine One"), wxPoint(216, 96), wxDefaultSize, 0, wxT("WxStaticText1"));

	WxSpinCtrl3 = new wxSpinCtrl(this, ID_WXSPINCTRL3, wxT("1"), wxPoint(160, 408), wxSize(97, 24), wxSP_ARROW_KEYS, 1, 50, 1);

	WxSpinCtrl2 = new wxSpinCtrl(this, ID_WXSPINCTRL2, wxT("1"), wxPoint(160, 376), wxSize(97, 24), wxSP_ARROW_KEYS, 1, 100, 1);

	WxRadioButton3 = new wxRadioButton(this, ID_WXRADIOBUTTON3, wxT("Fixed Time"), wxPoint(24, 408), wxSize(137, 25), 0, wxDefaultValidator, wxT("WxRadioButton3"));

	WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, wxT("Fixed Depth"), wxPoint(24, 376), wxSize(129, 25), 0, wxDefaultValidator, wxT("WxRadioButton2"));

	WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, wxT("Timed"), wxPoint(24, 344), wxSize(129, 25), 0, wxDefaultValidator, wxT("WxRadioButton1"));
	WxRadioButton1->SetValue(true);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Number of Games"), wxPoint(192, 40), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxSpinCtrl1 = new wxSpinCtrl(this, ID_WXSPINCTRL1, wxT("0"), wxPoint(16, 40), wxSize(161, 24), wxSP_ARROW_KEYS, 0, 100, 0);

	wxArrayString arrayStringFor_WxComboBox2;
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_pFrame->m_pipePanel[jj] &&
			m_pFrame->m_pipePanel[jj]->IsActive() )
			if( m_pFrame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxComboBox2.Add(m_pFrame->m_pipePanel[jj]->GetEngineName());
	}
	WxComboBox2 = new wxComboBox(this, ID_WXCOMBOBOX2, wxT(""), wxPoint(24, 128), wxSize(177, 23), arrayStringFor_WxComboBox2, 0, wxDefaultValidator, wxT("WxComboBox2"));
	if( arrayStringFor_WxComboBox2.size() > 1 )
		WxComboBox2->SetSelection(1);

	wxArrayString arrayStringFor_WxComboBox1;
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_pFrame->m_pipePanel[jj] &&
			m_pFrame->m_pipePanel[jj]->IsActive() )
			if( m_pFrame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxComboBox1.Add(m_pFrame->m_pipePanel[jj]->GetEngineName());
	}
	WxComboBox1 = new wxComboBox(this, ID_WXCOMBOBOX1, wxT(""), wxPoint(24, 96), wxSize(177, 23), arrayStringFor_WxComboBox1, 0, wxDefaultValidator, wxT("WxComboBox1"));
	if( arrayStringFor_WxComboBox1.size() > 0 )
		WxComboBox1->SetSelection(0);
	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT("Time Control"), wxPoint(16, 320), wxSize(345, 129));

	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT("Configuration"), wxPoint(16, 176), wxSize(489, 129));

	WxStaticBox3 = new wxStaticBox(this, ID_WXSTATICBOX3, wxT("Engines"), wxPoint(16, 72), wxSize(489, 97));

	SetTitle(wxT("Engine Match"));
	SetIcon(wxNullIcon);
	SetSize(8,0,536,547);
	Center();
	*/
	WxCheckBox4 = new wxCheckBox(this, ID_WXCHECKBOX4, wxT("Write Games Into Tree"), wxPoint(24, 296), wxSize(209, 25), 0, wxDefaultValidator, wxT("WxCheckBox4"));

	WxCheckBox3 = new wxCheckBox(this, ID_WXCHECKBOX3, wxT("Use Best Moves (random default)"), wxPoint(240, 264), wxSize(209, 25), 0, wxDefaultValidator, wxT("WxCheckBox3"));

	WxStaticText6 = new wxStaticText(this, ID_WXSTATICTEXT6, wxT("Max Ply"), wxPoint(120, 264), wxDefaultSize, 0, wxT("WxStaticText6"));

	wxArrayString arrayStringFor_WxComboBox3;
	arrayStringFor_WxComboBox3.Add(wxT("GUI Book"));
	arrayStringFor_WxComboBox3.Add(wxT("Own Book"));
	arrayStringFor_WxComboBox3.Add(wxT("PGN File"));
	WxComboBox3 = new wxComboBox(this, ID_WXCOMBOBOX3, wxT(""), wxPoint(24, 232), wxSize(177, 23), arrayStringFor_WxComboBox3, 0, wxDefaultValidator, wxT("WxComboBox3"));

	WxSpinCtrl4 = new wxSpinCtrl(this, ID_WXSPINCTRL4, wxT("0"), wxPoint(24, 264), wxSize(81, 24), wxSP_ARROW_KEYS, 0, 100, 0);

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, wxT("Book File (PGN)"), wxPoint(392, 232), wxDefaultSize, 0, wxT("WxStaticText5"));

	//WxFilePickerCtrl1 = new wxFilePickerCtrl(this, ID_WXFILEPICKERCTRL1, wxT(""), wxT("Select a file"), wxT("*.*"), wxPoint(240, 232), wxSize(145, 25), wxFLP_DEFAULT_STYLE, wxDefaultValidator, wxT("WxFilePickerCtrl1"));

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("Match Name"), wxPoint(192, 8), wxDefaultSize, 0, wxT("WxStaticText4"));

	WxEdit1 = new wxTextCtrl(this, ID_WXEDIT1, wxT("Engine Match"), wxPoint(16, 8), wxSize(161, 25), 0, wxDefaultValidator, wxT("WxEdit1"));

	WxButton7 = new wxButton(this, ID_WXBUTTON7, wxT("Configure"), wxPoint(272, 384), wxSize(73, 25), 0, wxDefaultValidator, wxT("WxButton7"));

	wxArrayString arrayStringFor_WxChoice1;
	arrayStringFor_WxChoice1.Add(wxT("Hyper-Bullet"));
	arrayStringFor_WxChoice1.Add(wxT("Bullet"));
	arrayStringFor_WxChoice1.Add(wxT("Blitz"));
	arrayStringFor_WxChoice1.Add(wxT("Rapid"));
	arrayStringFor_WxChoice1.Add(wxT("Standard"));
	arrayStringFor_WxChoice1.Add(wxT("Long"));
	WxChoice1 = new wxChoice(this, ID_WXCHOICE1, wxPoint(160, 384), wxSize(97, 23), arrayStringFor_WxChoice1, 0, wxDefaultValidator, wxT("WxChoice1"));
	WxChoice1->SetSelection(1);

	WxButton6 = new wxButton(this, ID_WXBUTTON6, wxT("Load"), wxPoint(296, 96), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton6"));

	WxButton5 = new wxButton(this, ID_WXBUTTON5, wxT("Load"), wxPoint(296, 128), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton5"));

	WxButton4 = new wxButton(this, ID_WXBUTTON4, wxT("Configure"), wxPoint(392, 128), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton4"));

	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Configure"), wxPoint(392, 96), wxSize(81, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxCheckBox2 = new wxCheckBox(this, ID_WXCHECKBOX2, wxT("Play Same Position For Each Colour"), wxPoint(240, 200), wxSize(209, 25), 0, wxDefaultValidator, wxT("WxCheckBox2"));

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(144, 496), wxSize(105, 33), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(16, 496), wxSize(105, 33), 0, wxDefaultValidator, wxT("WxButton1"));

	WxCheckBox1 = new wxCheckBox(this, ID_WXCHECKBOX1, wxT("Swap Sides Between Games"), wxPoint(24, 200), wxSize(177, 25), 0, wxDefaultValidator, wxT("WxCheckBox1"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Engine Two"), wxPoint(216, 128), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Engine One"), wxPoint(216, 96), wxDefaultSize, 0, wxT("WxStaticText1"));

	WxSpinCtrl3 = new wxSpinCtrl(this, ID_WXSPINCTRL3, wxT("1"), wxPoint(160, 448), wxSize(97, 24), wxSP_ARROW_KEYS, 1, 50, 1);

	WxSpinCtrl2 = new wxSpinCtrl(this, ID_WXSPINCTRL2, wxT("1"), wxPoint(160, 416), wxSize(97, 24), wxSP_ARROW_KEYS, 1, 100, 1);

	WxRadioButton3 = new wxRadioButton(this, ID_WXRADIOBUTTON3, wxT("Fixed Time"), wxPoint(24, 448), wxSize(137, 25), 0, wxDefaultValidator, wxT("WxRadioButton3"));

	WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, wxT("Fixed Depth"), wxPoint(24, 416), wxSize(129, 25), 0, wxDefaultValidator, wxT("WxRadioButton2"));

	WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, wxT("Timed"), wxPoint(24, 384), wxSize(129, 25), 0, wxDefaultValidator, wxT("WxRadioButton1"));
	WxRadioButton1->SetValue(true);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Number of Games"), wxPoint(192, 40), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxSpinCtrl1 = new wxSpinCtrl(this, ID_WXSPINCTRL1, wxT("1"), wxPoint(16, 40), wxSize(161, 24), wxSP_ARROW_KEYS, 0, 100, 0);

	wxArrayString arrayStringFor_WxComboBox2;
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_pFrame->m_pipePanel[jj] &&
			m_pFrame->m_pipePanel[jj]->IsActive() )
			if( m_pFrame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxComboBox2.Add(m_pFrame->m_pipePanel[jj]->GetEngineName());
	}
	WxComboBox2 = new wxComboBox(this, ID_WXCOMBOBOX2, wxT(""), wxPoint(24, 128), wxSize(177, 23), arrayStringFor_WxComboBox2, 0, wxDefaultValidator, wxT("WxComboBox2"));
	if( arrayStringFor_WxComboBox2.size() > 1 )
		WxComboBox2->SetSelection(0);

	wxArrayString arrayStringFor_WxComboBox1;
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_pFrame->m_pipePanel[jj] &&
			m_pFrame->m_pipePanel[jj]->IsActive() )
			if( m_pFrame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxComboBox1.Add(m_pFrame->m_pipePanel[jj]->GetEngineName());
	}
	WxComboBox1 = new wxComboBox(this, ID_WXCOMBOBOX1, wxT(""), wxPoint(24, 96), wxSize(177, 23), arrayStringFor_WxComboBox1, 0, wxDefaultValidator, wxT("WxComboBox1"));
	if( arrayStringFor_WxComboBox1.size() > 0 )
		WxComboBox1->SetSelection(0);

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT("Time Control"), wxPoint(16, 360), wxSize(345, 129));

	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT("Configuration"), wxPoint(16, 176), wxSize(489, 161));

	WxStaticBox3 = new wxStaticBox(this, ID_WXSTATICBOX3, wxT("Engines"), wxPoint(16, 72), wxSize(489, 97));

	SetTitle(wxT("Engine Match"));
	SetIcon(wxNullIcon);
	SetSize(8,8,536,575);
	Center();

	WxButton7->Enable();
	WxChoice1->Enable();
	WxSpinCtrl2->Disable();
	WxSpinCtrl3->Disable();
}

EngineMatchDialog::~EngineMatchDialog()
{
}

void EngineMatchDialog::OnOk( wxCommandEvent & event )
{
	eventName = WxEdit1->GetValue();

	plyForBook = WxSpinCtrl4->GetValue();
	
	if( WxRadioButton1->GetValue() ==true )
	{
		timeControlType = 0;
	}
	else if( WxRadioButton2->GetValue() == true )
	{
		timeControlType = 1;
	}
	else if( WxRadioButton3->GetValue() == true )
	{
		timeControlType = 2;
	}

	writeIntoTree = WxCheckBox4->GetValue();

	/*
	timeInMilliSecs = 0;
	timeInSeconds = 0;
	timeInMinutes = 0;
	incrementMillis = 0;
	incrementSeconds = 0 ;
	incrementMinutes = 0;
	*/
	if( timeControlType == 0 )
	{
		int controlID = WxChoice1->GetSelection();
		timeControlID  = controlID;
		/*if( str == "Hyper-Bullet" )
		{
			timeControlID =  0;
		}
		else if( str == "Bullet" )
		{
			timeControlID =  1;
		}
		else if( str == "Blitz" )
		{
			timeControlID =  2;
		}
		else if( str == "Rapid" )
		{
			timeControlID =  3;
		}
		else if( str =="Standard")
		{
			timeControlID =  4;
		}
		else if( str == "Long")
		{
			timeControlID =  5;
		}*/
		

	}
	else if( timeControlType == 1 )
	{
		fixedDepth = WxSpinCtrl2->GetValue();
	}
	else if( timeControlType == 2 )
	{
		this->timeInSeconds = WxSpinCtrl2->GetValue();
		timeInMinutes = timeInSeconds / 60;
		timeInMilliSecs = timeInSeconds *1000;
	}

	engine1 = WxComboBox1->GetValue();
	engine2 = WxComboBox2->GetValue();
	numGames = WxSpinCtrl1->GetValue();
	
	swapSidesBetweenGames = WxCheckBox1->GetValue();
	playSamePositionForEachColor = WxCheckBox2->GetValue();
	logEngineOutput = WxCheckBox3->GetValue();

	if( timeConfigured )
	{
		/*
	int start_minutes;
	int start_seconds;
	int start_inc_seconds;
	int start_inc_millis;
		*/
		if( start_minutes < 1 )
		{
			if( start_seconds < 10 )
			{
				this->timeControlID = MyFrame::EngineVsEngine::HYPER_BULLET;
			}
			else 
			{
				this->timeControlID = MyFrame::EngineVsEngine::BULLET;
			}
		}
		else if( start_minutes < 5 )
		{
			this->timeControlID = MyFrame::EngineVsEngine::BLITZ;
		}
		else if( start_minutes < 15 )
		{
			this->timeControlID = MyFrame::EngineVsEngine::RAPID;
		}
		else if( start_minutes < 30 )
		{
			this->timeControlID = MyFrame::EngineVsEngine::STANDARD;
		}
		else this->timeControlID = MyFrame::EngineVsEngine::LONG_TIME;
	}
	
	retID = 1;
	Close(true);
}




void EngineMatchDialog::OnCancel( wxCommandEvent & event )
{
	retID = 0;
	Close(true);
}


void EngineMatchDialog::OnEngine1Load( wxCommandEvent & event )
{
	wxString cmd;
	wxString name;
	EngineListModalDialog dialog(this, m_pFrame->m_pipePanel[0], wxID_ANY, "Engine List", &cmd);
	dialog.ShowModal();

    if ( cmd.empty() )
    {
		//Close(true);
		//	subframe->m_mgr.GetPane("Engine_Panel").Show(true);
		//	subframe->m_mgr.GetPane("Engine_Panel_2").Show(true);
		 return;
	}

    wxProcess *process = wxProcess::Open(cmd);
	 //long pid = wxExecute(cmd, 1, process);
	 //process->Redirect();
	//MyPipedProcess2 *process = new MyPipedProcess2(this, cmd, input);
    if ( !process )
    {
        wxLogError(wxT("Failed to launch the command."));
        return;
    }

    wxLogVerbose(wxT("PID of the new process: %ld"), process->GetPid());

    wxOutputStream *out = process->GetOutputStream();
    if ( !out )
    {
        wxLogError(wxT("Failed to connect to child stdin"));
        return;
    }
	//out->

    wxInputStream *in = process->GetInputStream();
    if ( !in )
    {
        wxLogError(wxT("Failed to connect to child stdout"));
        return;
    }

	wxArrayString arrayStringFor_WxComboBox1;
	arrayStringFor_WxComboBox1.Add(cmd);
	//cmd.find_last_of("/")
	wxString toInsert = "";
	int last_back_slash = cmd.find_last_of("\\");
	if( last_back_slash != -1 )
	{
		toInsert=cmd.substr(last_back_slash+1, cmd.length()-last_back_slash+1);
		//kbitzing_by += panelName;
	}
	else
	{
		//panelName = cmd;
		toInsert += cmd;
	}	

	WxComboBox1->Insert(toInsert,WxComboBox1->GetCount());
	WxComboBox2->Insert(toInsert,WxComboBox2->GetCount());
	/*if( m_pFrame->m_pipePanel[0]->GetEngineName()!= "")
		arrayStringFor_WxComboBox1.Add(m_pFrame->m_pipePanel[0]->GetEngineName());
	if( m_pFrame->m_pipePanel[1]->GetEngineName()!= "")	
		arrayStringFor_WxComboBox1.Add(m_pFrame->m_pipePanel[1]->GetEngineName());
		*/
	
				

	//in->
	if( m_pFrame->m_pipePanel[0]->EngineIsLoaded() )
	{
		if( m_pFrame->m_pipePanel[1]->EngineIsLoaded () )
		{
			bool panelFound = false;
			for( int i = 0; i < MAX_NUM_PIPE_PANELS; i++ )
			{
				if( m_pFrame->m_pipePanel[i]!= NULL )
				{
					if( !m_pFrame->m_pipePanel[i]->EngineIsLoaded() )
					{
						// non null pipe panel without engine.
						m_pFrame->m_mgr.GetPane(m_pFrame->m_pipePanel[i]->GetPanelName()).Show(true);
						m_pFrame->m_pipePanel[i]->LoadEngine(cmd,process);

						m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[i]);
						m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[i]);					
						// for pipe_panel 0, the string doesn't exist
						panelFound = true;
						break;
					}
				}
			}

			if( !panelFound )
			{
				wxCommandEvent cmdEvt;
				m_pFrame->OnCreatePipePanel(cmdEvt);
				m_pFrame->m_pipePanel[m_pFrame->GetNumActivePipePanels()-1]->LoadEngine(cmd,process);
			
				m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[m_pFrame->GetNumActivePipePanels()-1]);
				m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[m_pFrame->GetNumActivePipePanels()-1]);	
			}
		}
		else
		{
			m_pFrame->m_mgr.GetPane("Engine_Panel_2").Show(true);
			m_pFrame->m_pipePanel[1]->LoadEngine(cmd,process);


			m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[1]);
			m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[1]);
		}
#ifdef USE_CANVAS_SET_FOCUS
		//subframe->my_canvas->SetFocus();
#endif
	}
	else 
	{
		m_pFrame->m_mgr.GetPane("Engine_Panel").Show(true);
		m_pFrame->m_pipePanel[0]->LoadEngine(cmd,process);


		m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[0]);
		m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[0]);

#ifdef USE_CANVAS_SET_FOCUS
		//m_pFrame->my_canvas->SetFocus();
#endif
	}
}
void EngineMatchDialog::OnEngine1Configure( wxCommandEvent & event )
{	
	wxString str = WxComboBox1->GetValue();
	if( str.IsEmpty() )
		return;
	MyPipePanel *ppPanel=0;
	for( int i = 0 ; i < m_pFrame->numActivePipePanels; i++ )
	{
		if( m_pFrame->m_pipePanel[i]->GetEngineName() == str )
		{
			ppPanel = m_pFrame->m_pipePanel[i];
			break;
		}
	}

	if(!ppPanel )
		return;

	string emptyString = "";
	EngineOptionsModalDialog *dlg = new EngineOptionsModalDialog(this, ppPanel,emptyString , wxID_ANY,"Engine Options");
	if( dlg->ShowModal() == wxID_OK)
	{
	}

}

void EngineMatchDialog::OnEngine2Load( wxCommandEvent & event )
{
	wxString cmd;
	wxString name;
	EngineListModalDialog dialog(this, m_pFrame->m_pipePanel[0], wxID_ANY, "Engine List", &cmd);
	dialog.ShowModal();

    if ( cmd.empty() )
    {
		//Close(true);
		//	subframe->m_mgr.GetPane("Engine_Panel").Show(true);
		//	subframe->m_mgr.GetPane("Engine_Panel_2").Show(true);
		 return;
	}

    wxProcess *process = wxProcess::Open(cmd);
	 //long pid = wxExecute(cmd, 1, process);
	 //process->Redirect();
	//MyPipedProcess2 *process = new MyPipedProcess2(this, cmd, input);
    if ( !process )
    {
        wxLogError(wxT("Failed to launch the command."));
        return;
    }

    wxLogVerbose(wxT("PID of the new process: %ld"), process->GetPid());

    wxOutputStream *out = process->GetOutputStream();
    if ( !out )
    {
        wxLogError(wxT("Failed to connect to child stdin"));
        return;
    }
	//out->

    wxInputStream *in = process->GetInputStream();
    if ( !in )
    {
        wxLogError(wxT("Failed to connect to child stdout"));
        return;
    }

	wxString toInsert = "";
	int last_back_slash = cmd.find_last_of("\\");
	if( last_back_slash != -1 )
	{
		toInsert=cmd.substr(last_back_slash+1, cmd.length()-last_back_slash+1);
		//kbitzing_by += panelName;
	}
	else
	{
		//panelName = cmd;
		toInsert += cmd;
	}	

	WxComboBox1->Insert(toInsert,WxComboBox1->GetCount());
	WxComboBox2->Insert(toInsert,WxComboBox2->GetCount());
	
				

	//in->
	if( m_pFrame->m_pipePanel[0]->EngineIsLoaded() )
	{
		if( m_pFrame->m_pipePanel[1]->EngineIsLoaded () )
		{
			bool panelFound = false;
			for( int i = 0; i < MAX_NUM_PIPE_PANELS; i++ )
			{
				if( m_pFrame->m_pipePanel[i]!= NULL )
				{
					if( !m_pFrame->m_pipePanel[i]->EngineIsLoaded() )
					{
						// non null pipe panel without engine.
						m_pFrame->m_mgr.GetPane(m_pFrame->m_pipePanel[i]->GetPanelName()).Show(true);
						m_pFrame->m_pipePanel[i]->LoadEngine(cmd,process);

						m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[i]);
						m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[i]);					
						// for pipe_panel 0, the string doesn't exist
						panelFound = true;
						break;
					}
				}
			}

			if( !panelFound )
			{
				wxCommandEvent cmdEvt;
				m_pFrame->OnCreatePipePanel(cmdEvt);
				m_pFrame->m_pipePanel[m_pFrame->GetNumActivePipePanels()-1]->LoadEngine(cmd,process);
			
				m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[m_pFrame->GetNumActivePipePanels()-1]);
				m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[m_pFrame->GetNumActivePipePanels()-1]);	
			}
		}
		else
		{
			m_pFrame->m_mgr.GetPane("Engine_Panel_2").Show(true);
			m_pFrame->m_pipePanel[1]->LoadEngine(cmd,process);


			m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[1]);
			m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[1]);
		}
#ifdef USE_CANVAS_SET_FOCUS
		//subframe->my_canvas->SetFocus();
#endif
	}
	else 
	{
		m_pFrame->m_mgr.GetPane("Engine_Panel").Show(true);
		m_pFrame->m_pipePanel[0]->LoadEngine(cmd,process);


		m_pFrame->board_canvas->AddActiveEngine(m_pFrame->m_pipePanel[0]);
		m_pFrame->my_canvas->AddActiveEngine(m_pFrame->m_pipePanel[0]);

#ifdef USE_CANVAS_SET_FOCUS
		//m_pFrame->my_canvas->SetFocus();
#endif
	}
}
void EngineMatchDialog::OnEngine2Configure( wxCommandEvent & event )
{
	wxString str = WxComboBox2->GetValue();
	if( str.IsEmpty() )
		return;

	MyPipePanel *ppPanel;
	for( int i = 0 ; i < m_pFrame->numActivePipePanels; i++ )
	{
		if( m_pFrame->m_pipePanel[i]->GetEngineName() == str )
		{
			ppPanel = m_pFrame->m_pipePanel[i];
			break;
		}
	}

	if( !ppPanel )
		return;

	string emptyString = "";
	EngineOptionsModalDialog *dlg = new EngineOptionsModalDialog(this, ppPanel,emptyString , wxID_ANY,"Engine Options");
	if( dlg->ShowModal() == wxID_OK)
	{
	}
}


void EngineMatchDialog::OnRadio( wxCommandEvent &event )
{
	
}
void EngineMatchDialog::OnRadioButton1( wxCommandEvent &event )
{
	bool eventVal = event.IsChecked();
	if( eventVal )
	{
		WxButton7->Enable();
		WxChoice1->Enable();
		WxSpinCtrl2->Disable();
		WxSpinCtrl3->Disable();
	}
}
void EngineMatchDialog::OnRadioButton2( wxCommandEvent &event )
{
	bool eventVal = event.IsChecked();
	if( eventVal )
	{
		WxButton7->Disable();
		WxChoice1->Disable();
		WxSpinCtrl2->Enable();
		WxSpinCtrl3->Disable();
	}
}

void EngineMatchDialog::OnRadioButton3( wxCommandEvent &event )
{
	bool eventVal = event.IsChecked();
	if( eventVal )
	{
		WxButton7->Disable();
		WxChoice1->Disable();
		WxSpinCtrl2->Disable();
		WxSpinCtrl3->Enable();
	}
}

void EngineMatchDialog::OnTimeControlConfigure( wxCommandEvent & event )
{

	TimeControlDialog dlg(m_pFrame, NULL, this, wxID_ANY,"Time Control Dialog");
	dlg.ShowModal();
	if(  dlg.res== wxID_OK )
	{
		this->start_minutes = dlg.start_minutes;
		this->start_seconds = dlg.start_seconds;
		this->start_inc_seconds = dlg.start_inc_seconds;
		this->start_inc_millis = dlg.start_inc_millis;
			
		this->time_control_one_num_moves = dlg.time_control_one_num_moves;
		
		this->time_control_one_minutes = dlg.time_control_one_minutes;
		this->time_control_one_seconds = dlg.time_control_one_seconds;
		this->time_control_one_inc_seconds = dlg.time_control_one_inc_seconds;
		this->time_control_one_inc_millis = dlg.time_control_one_inc_millis;

		this->bFischer = dlg.bFischer;
		
		timeConfigured = true;
		//m_pFrame->gameClock->SetTimeControl(0,dlg.start_seconds,dlg.start_inc_millis,1);

	}
}