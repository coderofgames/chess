#include "PlayVsComputerDialog.h"
//#include "GameInfoDialog.h"
#include "mdi.h"
#include "MyPipePanel.h"
#include "EngineListDialog.h"
#include "UCIEngineOptionsDialog.h"
#include "TimeControlSettingsDialog.h"
#include "GameClock.h"

BEGIN_EVENT_TABLE(PlayVsComputerDialog, wxDialog)
	EVT_BUTTON(wxID_OK, PlayVsComputerDialog::OnOk)
	EVT_BUTTON(ID_WXBUTTON3, PlayVsComputerDialog::OnLoadEngine)
	EVT_BUTTON(ID_WXBUTTON4, PlayVsComputerDialog::OnConfigureEngine)
	EVT_BUTTON(ID_WXBUTTON5, PlayVsComputerDialog::OnSetTimeControl)
	//EVT_BUTTON(ADD_ENGINE, EngineListModalDialog::OnAddEngine)
	
END_EVENT_TABLE()

PlayVsComputerDialog::PlayVsComputerDialog(MyFrame * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog((wxWindow*)parent, id, title, pos, size, style)
{
	m_frame = parent;

	game.CreateEmptyHeader();
/*
	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Load Engine"), wxPoint(24, 120), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxCheckBox1 = new wxCheckBox(this, ID_WXCHECKBOX1, wxT("Play From Current Position"), wxPoint(24, 88), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxCheckBox1"));
	WxCheckBox1->SetValue(true);

	WxRadioButton3 = new wxRadioButton(this, ID_WXRADIOBUTTON3, wxT("Keep Current Game"), wxPoint(20, 248), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton3"));
	WxRadioButton3->SetValue(true);

	wxArrayString arrayStringFor_WxChoice2;
	arrayStringFor_WxChoice2.Add("Player");
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_frame->m_pipePanel[jj] &&
			m_frame->m_pipePanel[jj]->IsActive() )
			if( m_frame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxChoice2.Add(m_frame->m_pipePanel[jj]->GetEngineName());
	}
	
	WxCombo2 = new wxComboBox(this, ID_WXCOMBO2, wxT(""),wxPoint(96, 56), wxSize(145, 23), arrayStringFor_WxChoice2, 0, wxDefaultValidator, wxT("WxChoice2"));
	WxCombo2->SetSelection(0);

	wxArrayString arrayStringFor_WxChoice1;
	arrayStringFor_WxChoice1.Add("Player");
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_frame->m_pipePanel[jj] &&
			m_frame->m_pipePanel[jj]->IsActive() )
			if( m_frame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxChoice1.Add(m_frame->m_pipePanel[jj]->GetEngineName());
	}
	
	WxCombo1 = new wxComboBox(this, ID_WXCOMBO1, wxT(""),wxPoint(96, 24), wxSize(145, 23), arrayStringFor_WxChoice1, 0, wxDefaultValidator, wxT("WxChoice1"));
							//(this, ID_WXCOMBOBOX2, wxT(""), wxPoint(24, 128), wxSize(177, 23), arrayStringFor_WxComboBox2, 0, wxDefaultValidator, wxT("WxComboBox2"));
	WxCombo1->SetSelection(-1);

	WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, wxT("Clear Database"), wxPoint(20, 216), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton2"));

	WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, wxT("Add To Database"), wxPoint(20, 186), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton1"));

	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT(""), wxPoint(11, 171), wxSize(238, 117));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Black Player"), wxPoint(21, 61), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("White Player"), wxPoint(21, 29), wxDefaultSize, 0, wxT("WxStaticText1"));

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(115, 312), wxSize(87, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(15, 312), wxSize(87, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT(""), wxPoint(11, 9), wxSize(238, 152));

	*/
	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Number Of Games"), wxPoint(136, 200), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxSpinCtrl1 = new wxSpinCtrl(this, ID_WXSPINCTRL1, wxT("0"), wxPoint(24, 200), wxSize(97, 24), wxSP_ARROW_KEYS, 1, 100, 0);
	WxSpinCtrl1->SetValue(1);

	WxButton5 = new wxButton(this, ID_WXBUTTON5, wxT("Setup Time"), wxPoint(136, 160), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton5"));

	wxArrayString arrayStringFor_WxComboBox1;
	arrayStringFor_WxComboBox1.Add("Hyper Bullet");
	arrayStringFor_WxComboBox1.Add("Bullet");
	arrayStringFor_WxComboBox1.Add("Blitz");
	arrayStringFor_WxComboBox1.Add("Rapid");
	arrayStringFor_WxComboBox1.Add("Standard");
	arrayStringFor_WxComboBox1.Add("Long");
	WxComboBox1 = new wxComboBox(this, ID_WXCOMBOBOX1, wxT("WxComboBox1"), wxPoint(24, 160), wxSize(97, 23), arrayStringFor_WxComboBox1, 0, wxDefaultValidator, wxT("WxComboBox1"));
	WxComboBox1->SetValue("Blitz");

	WxButton4 = new wxButton(this, ID_WXBUTTON4, wxT("Configure"), wxPoint(136, 120), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton4"));

	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Load Engine"), wxPoint(24, 120), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	//WxCheckBox1 = new wxCheckBox(this, ID_WXCHECKBOX1, wxT("Play From Current Position"), wxPoint(24, 88), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxCheckBox1"));
	//WxCheckBox1->SetValue(true);

	WxRadioButton3 = new wxRadioButton(this, ID_WXRADIOBUTTON3, wxT("Keep Current Game"), wxPoint(20, 320), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton3"));
	WxRadioButton3->SetValue(true);

	wxArrayString arrayStringFor_WxChoice2;
	arrayStringFor_WxChoice2.Add("Player");
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_frame->m_pipePanel[jj] &&
			m_frame->m_pipePanel[jj]->IsActive() )
			if( m_frame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxChoice2.Add(m_frame->m_pipePanel[jj]->GetEngineName());
	}
	WxCombo2 = new wxComboBox(this, ID_WXCOMBO2, wxT(""),wxPoint(96, 56), wxSize(145, 23), arrayStringFor_WxChoice2, 0, wxDefaultValidator, wxT("WxChoice2"));
	WxCombo2->SetSelection(-1);

//	wxArrayString arrayStringFor_WxChoice1;
	wxArrayString arrayStringFor_WxChoice1;
	arrayStringFor_WxChoice1.Add("Player");
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_frame->m_pipePanel[jj] &&
			m_frame->m_pipePanel[jj]->IsActive() )
			if( m_frame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxChoice1.Add(m_frame->m_pipePanel[jj]->GetEngineName());
	}
	WxCombo1 = new wxComboBox(this, ID_WXCOMBO1, wxT(""),wxPoint(96, 24), wxSize(145, 23), arrayStringFor_WxChoice1, 0, wxDefaultValidator, wxT("WxChoice1"));
	WxCombo1->SetSelection(0);

	WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, wxT("Clear Database"), wxPoint(20, 288), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton2"));

	WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, wxT("Add To Database"), wxPoint(20, 258), wxSize(169, 25), 0, wxDefaultValidator, wxT("WxRadioButton1"));

	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT(""), wxPoint(11, 243), wxSize(238, 117));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Black Player"), wxPoint(21, 61), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("White Player"), wxPoint(21, 29), wxDefaultSize, 0, wxT("WxStaticText1"));

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(115, 376), wxSize(87, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(15, 376), wxSize(87, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT(""), wxPoint(11, 9), wxSize(238, 232));

	SetTitle(wxT("NewGame"));
	SetIcon(wxNullIcon);
	SetSize(8,8,275,447);
	Center();
	retCode = wxID_CANCEL;

	whiteText = "";
	blackText = "";
	eventText = "";

	
	editedHeader = false;
	timeConfigured = false;
}

PlayVsComputerDialog::~PlayVsComputerDialog()
{
}


void PlayVsComputerDialog::OnOk( wxCommandEvent & event )
{
	//if( this->editedHeader == false )
	{
		whiteText = WxCombo1->GetValue();// ((wxComboBox*)FindWindow(ID_WXCOMBO1))->GetSelection();
		blackText = WxCombo2->GetValue();//((wxComboBox*)FindWindow(ID_WXCOMBO2))->GetSelection();
		//eventText = ((wxTextCtrl*)FindWindow(ID_WXEDIT3))->GetValue();
	}

	numGames = WxSpinCtrl1->GetValue();

	if( !timeConfigured )
	{
		wxString strtime = WxComboBox1->GetValue();
		if( strtime == "Hyper Bullet")
		{
			m_frame->gameClock->SetTimeControl(0, 10, 200,1);
		}
		if( strtime == "Bullet" )
		{
			m_frame->gameClock->SetTimeControl(0, 120, 2000,1);
		}
		if( strtime == "Blitz" )
		{
			m_frame->gameClock->SetTimeControl(0, 300, 3000,1);
		}
		if( strtime == "Rapid" )
		{
			m_frame->gameClock->SetTimeControl(0, 900, 0,1);
		}
		if( strtime == "Standard" )
		{
			m_frame->gameClock->SetTimeControl(0, 1500, 0,1);
		}
		if( strtime == "Long" )
		{
			m_frame->gameClock->SetTimeControl(0, 3000, 0,1);
		}
	}

	bool bRadio1 = ((wxRadioButton*)FindWindow(ID_WXRADIOBUTTON1))->GetValue();
	bool bRadio2 = ((wxRadioButton*)FindWindow(ID_WXRADIOBUTTON2))->GetValue();
	bool bRadio3 = ((wxRadioButton*)FindWindow(ID_WXRADIOBUTTON3))->GetValue();

	if( bRadio1 )
	{
		retCode = ADD_GAME_TO_LIST;
	}
	else if( bRadio2 )
	{
		retCode = ADD_GAME_CLEAR_LIST;
	}
	else if( bRadio3 )
	{
		retCode = KEEP_CURRENT;
	}	
	Close(true);
}



void PlayVsComputerDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}

void PlayVsComputerDialog::OnConfigureEngine( wxCommandEvent & event )
{
	wxString str1 = WxCombo1->GetValue();
	wxString str2 = WxCombo2->GetValue();
	
	if( (str1 != "Player") && (str2 != "Player") )
	{
		wxMessageBox("2 Engines Selected, Use The Engine Vs Engine Dialog for an engine match", "Error");
		return;
	}

	MyPipePanel *ppPanel;
	for( int i = 0 ; i < m_frame->numActivePipePanels; i++ )
	{
		if( m_frame->m_pipePanel[i]->GetEngineName() == str1 )
		{
			ppPanel = m_frame->m_pipePanel[i];
			break;
		}
		if( m_frame->m_pipePanel[i]->GetEngineName() == str2 )
		{
			ppPanel = m_frame->m_pipePanel[i];
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

void PlayVsComputerDialog::OnSetTimeControl( wxCommandEvent & event )
{
	TimeControlDialog dlg(m_frame, NULL, this, wxID_ANY,"Time Control Dialog");
	dlg.ShowModal();
	if(  dlg.res== wxID_OK )
	{
		int start_minutes = dlg.start_minutes;
		int start_seconds = dlg.start_seconds;
		int start_inc_seconds = dlg.start_inc_seconds;
		int start_inc_millis = dlg.start_inc_millis;
			
		int time_control_one_num_moves = dlg.time_control_one_num_moves;
		
		int time_control_one_minutes = dlg.time_control_one_minutes;
		int time_control_one_seconds = dlg.time_control_one_seconds;
		int time_control_one_inc_seconds = dlg.time_control_one_inc_seconds;
		int time_control_one_inc_millis = dlg.time_control_one_inc_millis;

		int bFischer = dlg.bFischer;


		if( dlg.bFischer )
		{
			m_frame->gameClock->UseFideTimeControl();
			m_frame->gameClock->SetNumMovesBeforeTimeControl(dlg.time_control_one_num_moves);
			m_frame->gameClock->SetNumMillisAfterTimeControl( dlg.time_control_one_seconds*1000);
			m_frame->gameClock->SetIncrementAfterTimeControl( dlg.time_control_one_inc_millis );
		}
		else
		{
			m_frame->gameClock->SetTimeControl(0, start_seconds, start_inc_millis, 1);
		}
		
		timeConfigured = true;
		//m_pFrame->gameClock->SetTimeControl(0,dlg.start_seconds,dlg.start_inc_millis,1);
		
	}
}

void PlayVsComputerDialog::OnLoadEngine( wxCommandEvent & event )
{
	wxString cmd;
	wxString name;
	EngineListModalDialog dialog(this, m_frame->m_pipePanel[0], wxID_ANY, "Engine List", &cmd);
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

	WxCombo1->Insert(toInsert,WxCombo1->GetCount());
	WxCombo2->Insert(toInsert,WxCombo2->GetCount());
	/*if( m_pFrame->m_pipePanel[0]->GetEngineName()!= "")
		arrayStringFor_WxComboBox1.Add(m_pFrame->m_pipePanel[0]->GetEngineName());
	if( m_pFrame->m_pipePanel[1]->GetEngineName()!= "")	
		arrayStringFor_WxComboBox1.Add(m_pFrame->m_pipePanel[1]->GetEngineName());
		*/
	
				

	//in->
	if( m_frame->m_pipePanel[0]->EngineIsLoaded() )
	{
		if( m_frame->m_pipePanel[1]->EngineIsLoaded () )
		{
			bool panelFound = false;
			for( int i = 0; i < MAX_NUM_PIPE_PANELS; i++ )
			{
				if( m_frame->m_pipePanel[i]!= NULL )
				{
					if( !m_frame->m_pipePanel[i]->EngineIsLoaded() )
					{
						// non null pipe panel without engine.
						m_frame->m_mgr.GetPane(m_frame->m_pipePanel[i]->GetPanelName()).Show(true);
						m_frame->m_pipePanel[i]->LoadEngine(cmd,process);

						m_frame->board_canvas->AddActiveEngine(m_frame->m_pipePanel[i]);
						m_frame->my_canvas->AddActiveEngine(m_frame->m_pipePanel[i]);					
						// for pipe_panel 0, the string doesn't exist
						panelFound = true;
						break;
					}
				}
			}

			if( !panelFound )
			{
				wxCommandEvent cmdEvt;
				m_frame->OnCreatePipePanel(cmdEvt);
				m_frame->m_pipePanel[m_frame->GetNumActivePipePanels()-1]->LoadEngine(cmd,process);
			
				m_frame->board_canvas->AddActiveEngine(m_frame->m_pipePanel[m_frame->GetNumActivePipePanels()-1]);
				m_frame->my_canvas->AddActiveEngine(m_frame->m_pipePanel[m_frame->GetNumActivePipePanels()-1]);	
			}
		}
		else
		{
			m_frame->m_mgr.GetPane("Engine_Panel_2").Show(true);
			m_frame->m_pipePanel[1]->LoadEngine(cmd,process);


			m_frame->board_canvas->AddActiveEngine(m_frame->m_pipePanel[1]);
			m_frame->my_canvas->AddActiveEngine(m_frame->m_pipePanel[1]);
		}
#ifdef USE_CANVAS_SET_FOCUS
		//subframe->my_canvas->SetFocus();
#endif
	}
	else 
	{
		m_frame->m_mgr.GetPane("Engine_Panel").Show(true);
		m_frame->m_pipePanel[0]->LoadEngine(cmd,process);


		m_frame->board_canvas->AddActiveEngine(m_frame->m_pipePanel[0]);
		m_frame->my_canvas->AddActiveEngine(m_frame->m_pipePanel[0]);

#ifdef USE_CANVAS_SET_FOCUS
		//m_pFrame->my_canvas->SetFocus();
#endif
	}
}



