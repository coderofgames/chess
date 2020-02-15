#include "PersonalityListDialog.h"
#include "mdi.h"
#include "EnginePersonality.h"
#include "UCIEngineOptionsDialog.h"
#include "MyPipePanel.h"
#include "MiscUtils.h"




BEGIN_EVENT_TABLE(EnginePersonalityDialog, wxDialog)
	EVT_BUTTON(wxID_OK, EnginePersonalityDialog::OnOk)
	//EVT_BUTTON(ID_WXBUTTON3, NewGameDialog::EditHeader)
	//EVT_BUTTON(wxID_CANCEL, GameInfoModalDialog::OnCancel)

	EVT_BUTTON(ID_WXBUTTON3, EnginePersonalityDialog::OnEditName)
	
END_EVENT_TABLE()

EnginePersonalityDialog::EnginePersonalityDialog(wxWindow* parent, MyFrame * frame, wxString &engineName, UCIEnginePersonality* pEnginePersonality, MyPipePanel* pp, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog((wxWindow*)parent, id, title, pos, size, style)
{
	m_frame = frame;
	m_pipePanel = pp;
	this->m_engineName = engineName;
	this->m_enginePersonality = pEnginePersonality;

	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Edit Name"), wxPoint(152, 224), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Modify"), wxPoint(32, 224), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	WxButton8 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(152, 272), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton8"));

	WxButton7 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(32, 272), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton7"));

	WxButton4 = new wxButton(this, ID_WXBUTTON4, wxT("Create New"), wxPoint(32, 192), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton4"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("Delete Selected"), wxPoint(152, 192), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	wxArrayString arrayStringFor_WxListBox1;
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(16, 16), wxSize(249, 145), arrayStringFor_WxListBox1, wxLB_SINGLE);

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT(""), wxPoint(16, 168), wxSize(249, 97));

	SetTitle(wxT("ShowActiveEngines"));
	SetIcon(wxNullIcon);
	SetSize(8,8,298,349);
	Center();

	

	string personalityFileName;
	if( pEnginePersonality != NULL )
	{
		WxListBox1->Append(wxT("New Personality"));
		m_elems.push_back(string("New Personality"));
	}
		personalityFileName = m_engineName + "_personalities.txt";//engine_list.txt
	
	//wxVector<wxVariant> data;

	ifstream myfile (string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+"/settings/" + personalityFileName);
	//int count = 0;
	while ( myfile.good() )
	{
		string line;
		getline (myfile,line);
		if( !line.size() == 0 && line[0] != ' ')
		{	
			m_elems.push_back(line);
			
			wxString personalityName;
			int last_back_slash = line.find_last_of("\\");
			if( last_back_slash != -1 )
			{
				personalityName=line.substr(last_back_slash+1, line.length()-last_back_slash+1);
			}
			else
			{
				personalityName = line;
			}
			WxListBox1->Append(personalityName);
		}
	
	myfile.close();
	}
}

EnginePersonalityDialog::~EnginePersonalityDialog()
{
}


void EnginePersonalityDialog::OnOk( wxCommandEvent & event )
{
	/*if( this->editedHeader == false )
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
	}*/
	if( this->m_enginePersonality )
	{
		string personalityNameStr = m_elems[0];
		for( int v = 0; v < personalityNameStr.size(); v++ )
		{
			if( personalityNameStr[v] == ' ' )
			{
				personalityNameStr[v] = '_';
			}
		}
		newPersonalityFileName = m_engineName + "_" + personalityNameStr;
		ofstream of(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar()) + string("/settings/" + newPersonalityFileName + ".txt"));
		for( int i = 0; i < m_enginePersonality->optionsMap.size(); i++ )
		{
			switch( m_enginePersonality->optionsMap[i]->GetType() )
			{
			case SPIN_TYPE:
				{
					of << "SPIN ";
					of<< "\""<<m_enginePersonality->optionsMap[i]->GetName() <<"\" ";
					of<< ((SpinOption*)m_enginePersonality->optionsMap[i])->GetValue() << endl;
				}
				break;
			case COMBO_TYPE:
				{
					of << "COMBO ";
					of<< "\""<<m_enginePersonality->optionsMap[i]->GetName() <<"\" ";
					of<< ((ComboOption*)m_enginePersonality->optionsMap[i])->GetSelectedString() << endl;
				}
				break;
			case CHECK_TYPE:
				{
					of << "CHECK ";
					of<< "\""<<m_enginePersonality->optionsMap[i]->GetName() <<"\" ";
					of<< ((CheckOption*)m_enginePersonality->optionsMap[i])->GetState() << endl;
					
				}
				break;
			case STRING_TYPE:
				{
					of << "STRING ";
					of<< "\""<<m_enginePersonality->optionsMap[i]->GetName() <<"\" ";
					of<< ((StringOption*)m_enginePersonality->optionsMap[i])->GetString() << endl;
					
				}
				break;
			}
			//of << m_elems[0] <<endl;
		}
		of.close();

		string personalityFileName = m_engineName + "_personalities.txt";//engine_list.txt
	
	//wxVector<wxVariant> data;

	
		of.open(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+"/settings/" + personalityFileName);

		for( int i = 0; i < m_elems.size(); i++ )
		{
			of << m_elems[i] << endl;
		}
		of.close();
	}
	else
	{
		wxString selectionName = WxListBox1->GetString(WxListBox1->GetSelection());
		string personalityNameStr = selectionName;
		for( int v = 0; v < personalityNameStr.size(); v++ )
		{
			if( personalityNameStr[v] == ' ' )
			{
				personalityNameStr[v] = '_';
			}
		}
		newPersonalityFileName = m_engineName + "_" + personalityNameStr;
		ifstream ifs(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar()) + string("/settings/" + newPersonalityFileName + ".txt"));

		while( ifs.good() )
		{
			string line;
			getline(ifs, line);
			if( line == "" )
				break;
			int first_quotes = line.find("\"");
			int last_quotes = line.find_last_of("\"");
			string name = GetSubstringInQuotes(line);
			string type = line.substr(0, line.find(" "));
			string val = line.substr(last_quotes+2, line.length() - last_quotes - 2);

			string outPutString;
			if( type == "SPIN" )
			{
				outPutString = "";
				outPutString = "setoption name ";
				outPutString += name;
				outPutString += " value ";//
				outPutString += val;
				outPutString += "\n";
			}
			else if( type == "COMBO" )
			{
				outPutString = "";
				outPutString = "setoption name ";
				outPutString += name;
				outPutString += " value ";//
				outPutString += val;
				outPutString += "\n";
			}
			else if( type == "CHECK")
			{
				outPutString = "";
				outPutString = "setoption name ";
				outPutString += name;
				outPutString += " value ";//
				if( val == "1" )
					outPutString += "true";
				else
					outPutString += "false";
				outPutString += "\n";
			}
			else if( type == "STRING")
			{
				outPutString = "";
				outPutString = "setoption name ";
				outPutString += name;
				outPutString += " value ";//
				outPutString += val;
				outPutString += "\n";
			}

			if( outPutString != "" )
			{
				m_pipePanel->SendOptionsString(outPutString);
			}
	
		}
	
	}

	

	
	Close(true);
}

/*
void NewGameDialog::AddGameClearList( wxCommandEvent & event )
{
	//this->SetReturnCode(ADD_GAME_CLEAR_LIST);
	retCode = ADD_GAME_CLEAR_LIST;
	Close(true);
}
*/
void EnginePersonalityDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}

/*
void NewGameDialog::EditHeader(wxCommandEvent &event)
{
	GameInfoModalDialog dialog(this->m_frame, &game, this, wxID_ANY, "Edit Header");
	int ret = dialog.ShowModal();
	if( dialog.edited )
		this->editedHeader = true;
	else
		this->editedHeader = false;

}
*/

void EnginePersonalityDialog::OnEditName( wxCommandEvent & event )
{
	wxString toSet = "";
    wxTextEntryDialog dialog(this,
                             "Annotation Editor",
                             wxT("Please enter a string"),
                             toSet,
                             wxOK | wxCANCEL);

    if (dialog.ShowModal() == wxID_OK)
	{
		int selection = WxListBox1->GetSelection();
		m_elems[selection] = dialog.GetValue();

		//WxListBox1->Clear();
		wxArrayString currentItems;
		
		WxListBox1->SetString(selection, dialog.GetValue());
		//WxListBox1->Set( currentItems);
		//WxListBox1->SetSelection(selection);
	}
}