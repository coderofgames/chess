#include "UCIEngineOptionsDialog.h"
#include "MyPipePanel.h"
#include "PersonalityListDialog.h"


enum 
{
	ENGINE_OPTION = wxID_HIGHEST+400,
	LOAD_PERSONALITY,
	ENGINE_OPTION_CHECK_1,
	ENGINE_OPTION_CHECK_2,
	ENGINE_OPTION_CHECK_3,
	ENGINE_OPTION_CHECK_4,
	ENGINE_OPTION_CHECK_5,
	ENGINE_OPTION_CHECK_6,
	ENGINE_OPTION_CHECK_7,
	ENGINE_OPTION_CHECK_8,
	ENGINE_OPTION_CHECK_9,
	ENGINE_OPTION_CHECK_10,
	ENGINE_OPTION_CHECK_11,
	ENGINE_OPTION_CHECK_12,
	ENGINE_OPTION_SPIN_1,
	ENGINE_OPTION_SPIN_2,
	ENGINE_OPTION_SPIN_3,
	ENGINE_OPTION_SPIN_4,
	ENGINE_OPTION_SPIN_5,
	ENGINE_OPTION_SPIN_6,
	ENGINE_OPTION_SPIN_7,
	ENGINE_OPTION_SPIN_8,
	ENGINE_OPTION_SPIN_9,
	ENGINE_OPTION_SPIN_10,
	ENGINE_OPTION_SPIN_11,
	ENGINE_OPTION_SPIN_12,
	ENGINE_OPTION_COMBO_1,
	ENGINE_OPTION_COMBO_2,
	ENGINE_OPTION_COMBO_3,
	ENGINE_OPTION_COMBO_4,
	ENGINE_OPTION_COMBO_5,
	ENGINE_OPTION_COMBO_6,
	ENGINE_OPTION_COMBO_7,
	ENGINE_OPTION_COMBO_8,
	ENGINE_OPTION_COMBO_9,
	ENGINE_OPTION_COMBO_10,
	ENGINE_OPTION_COMBO_11,
	ENGINE_OPTION_COMBO_12,
	ENGINE_OPTION_BUTTON_1,
	ENGINE_OPTION_BUTTON_2,
	ENGINE_OPTION_BUTTON_3,
	ENGINE_OPTION_BUTTON_4,
	ENGINE_OPTION_STRING_1,
	ENGINE_OPTION_STRING_2,
	ENGINE_OPTION_STRING_3,
	ENGINE_OPTION_STRING_4,
	ENGINE_OPTION_STRING_5,
	ENGINE_OPTION_STRING_6,
	ENGINE_OPTION_STRING_7,
	ENGINE_OPTION_STRING_8,
	ENGINE_OPTION_STRING_9,
	ENGINE_OPTION_STRING_10,
	ENGINE_OPTION_STRING_11,
	ENGINE_OPTION_STRING_12,
	PROP_GRID,
	//PROP_GRID_CHANGING,
};

BEGIN_EVENT_TABLE(EngineOptionsModalDialog, wxDialog)
	EVT_PG_CHANGED( PROP_GRID, EngineOptionsModalDialog::OnPropertyGridChange )
    EVT_PG_CHANGING( PROP_GRID, EngineOptionsModalDialog::OnPropertyGridChanging )
	EVT_BUTTON(wxID_OK, EngineOptionsModalDialog::OnOk)
	EVT_BUTTON(LOAD_PERSONALITY, EngineOptionsModalDialog::OnLoadPersonality)
	
	//EVT_CLOSE(EngineOptionsModalDialog::OnClose)
END_EVENT_TABLE()



EngineOptionsModalDialog::EngineOptionsModalDialog(wxWindow * parent, MyPipePanel *pp, string & optionsString, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{
	m_pPipePanel = pp;

	SetSize(400,600);

	wxButton * b = new wxButton( this, wxID_OK, _("OK") );
	
	wxButton * c = new wxButton( this, wxID_CANCEL, _("CANCEL") );

	wxButton *Load = new wxButton( this, LOAD_PERSONALITY, _("Load Personality"));


	wxSizer *dlgSizer = new wxBoxSizer(wxVERTICAL);

	wxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(b, 0, wxEXPAND, 5);
	buttonSizer->Add(c, 0, wxEXPAND, 5);
	buttonSizer->Add(Load, 0, wxEXPAND, 5);



    m_pg = new wxPropertyGrid(this,PROP_GRID,wxDefaultPosition,wxSize(400,400),
                        wxPG_SPLITTER_AUTO_CENTER |
                        wxPG_BOLD_MODIFIED );	

	//m_pg->SetPropertyAttributeAll(
	m_pg->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,true);
	m_pg->SetPropertyAttributeAll(wxString("MotionSpin"),true);
	
	
	
	vector<string> tokens = *pp->GetTokenizedOptionsVector();
	
	int checkCount = 0;
	int spinCount = 0;
	int comboCount = 0;
	int buttonCount = 0;
	int stringCount = 0;

	for( int i = 0; i < tokens.size(); i++ )
	{
		if( tokens[i] == "option" )
		{
			int ppi = i+2;
			string name="";
			while( tokens[ppi] != "type" && ppi < tokens.size() )
			{
				if( ppi > i+2)
					name+=" ";
				name+=tokens[ppi];

				ppi++;
			}
			if( ppi+1 > tokens.size() )
				break;

			i = ppi+1;
			//string name = tokens[i+2];
			if( tokens[i] == "spin" )
			{
				int defaultVal=0; 
				int minVal=0;
				int maxVal=0;

				int j = i+1;
				if( j>= tokens.size() )
					break;

			
				while( j < tokens.size()-1 && tokens[j] != "option" )
				{
					if( tokens[j] == "min" )
					{
						minVal = atoi(tokens[j+1].c_str());
						//j++;
					}
					else if( tokens[j] == "max")
					{
						maxVal = atoi(tokens[j+1].c_str());
						//j++;
					}
					else if( tokens[j] == "default")
					{
						defaultVal = atoi(tokens[j+1].c_str());
						//j++;
					}
					j++;
				}
				

				spinCount++;
				wxIntProperty *intProp = new wxIntProperty(name.c_str(), wxPG_LABEL);
				m_pg->Append(intProp );
				m_pg->SetPropertyAttribute(name.c_str(), wxPG_ATTR_MIN, (long)minVal );
				m_pg->SetPropertyAttribute(name.c_str(), wxPG_ATTR_MAX, (long)maxVal );
				m_pg->SetPropertyValue(name.c_str(),(long)defaultVal);
				m_pg->SetPropertyEditor( name.c_str(), wxPGEditor_SpinCtrl );
				optionsMap.push_back( new SpinOption(name,(long)defaultVal));
				
			}
			if( tokens[i] == "check" )
			{
				wxBoolProperty *boolProp = new wxBoolProperty(name.c_str(), wxPG_LABEL);
				m_pg->Append(boolProp );
			
				if( tokens[i+2] == "true")
				{
					m_pg->SetPropertyValue(name.c_str(),true);
					optionsMap.push_back( new CheckOption(name, true ));
				}
				else
				{
					optionsMap.push_back( new CheckOption(name, false ));
				}
				checkCount++;

				i+=2;
			}
			if( tokens[i] == "combo" )
			{
				wxString defaultVal = "";
				string defVal2 = "";
				int j = i+1;
				wxArrayString arr;
				

				int comboVarCount = 0;
				wxPGChoices soc;


				ComboOption *comboOption = new ComboOption( );
				comboOption->SetName(name);

				while( j < tokens.size()-1 && tokens[j] != "option" )
				{
					if( tokens[j] == "var" )
					{

						soc.Add( tokens[j+1].c_str(), comboVarCount++ );
						comboOption->AddOption( tokens[j+1] );
						//j++;
					}
					else if( tokens[j] == "default" )
					{
						defaultVal = tokens[j+1].c_str();
						defVal2 = tokens[j+1];
						//j++;
					}
					j++;
				}
				i=j;

				comboOption->SetSelectonString(defVal2);

				wxEnumProperty *choiceProp = new wxEnumProperty(name.c_str(),wxPG_LABEL,soc); 

				optionsMap.push_back( comboOption );

				comboCount++;

				m_pg->Append(choiceProp );
				m_pg->SetPropertyValue(name.c_str(),defaultVal);
			}
			if( tokens[i] == "button")
			{
				wxButton *btn = new wxButton(this, ENGINE_OPTION_BUTTON_1 + buttonCount, name.c_str());
				buttonSizer->Add(btn, 0, wxEXPAND,0);
				optionsMap.push_back( new ButtonOption(name , ENGINE_OPTION_BUTTON_1 + buttonCount) );
				buttonCount++;
			}
			if( tokens[i] == "string")
			{
				stringCount++;

				if( (name.find("path")!=-1) || (name.find("Path")!=-1) )
				{
					wxDirProperty *path = new wxDirProperty(name.c_str(),"");
					//wxFileProperty *path = new wxFileProperty(name.c_str(),"");
					m_pg->Append(path );
				}
				else if( (name.find("File")!=-1) || (name.find("file")!=-1) )
				{
					//wxDirProperty *path = new wxDirProperty(name.c_str(),"");
					wxFileProperty *path = new wxFileProperty(name.c_str(),"");
					m_pg->Append(path );
				}
				else if( (name.find("Dir")!=-1) || (name.find("dir")!=-1) )
				{
					wxDirProperty *path = new wxDirProperty(name.c_str(),"");
					//wxFileProperty *path = new wxFileProperty(name.c_str(),"");
					m_pg->Append(path );
				}
				else
				{
					wxStringProperty *stringProp = new wxStringProperty(name.c_str(),"");
					m_pg->Append(stringProp );
				}

				optionsMap.push_back( new StringOption(name) );
			}
		}
	}

	dlgSizer->Add(m_pg, 0, wxEXPAND, 5);
	dlgSizer->Add(buttonSizer, 0, wxEXPAND, 5);

	this->SetSizer(dlgSizer);
    dlgSizer->Fit(this);

}

EngineOptionsModalDialog::~EngineOptionsModalDialog()
{
	if( m_pg )
	{
		m_pg->Clear();
		delete m_pg;
		m_pg = NULL;
	}

		for( int i = 0; i < optionsMap.size(); i++ )
		{
			if( optionsMap[i] )
			{
				delete optionsMap[i];
			}
		}	
}

void EngineOptionsModalDialog::OnPropertyGridChange(wxPropertyGridEvent& event)
{
    wxPGProperty* property = event.GetProperty();

    const wxString& name = property->GetName();

    // Properties store values internally as wxVariants, but it is preferred
    // to use the more modern wxAny at the interface level
    wxAny value = property->GetValue();

    // Don't handle 'unspecified' values
    if ( value.IsNull() )
        return;

	for( int i = 0; i < optionsMap.size(); i++ )
	{
		if( name == optionsMap[i]->GetName() )
		{
			switch( optionsMap[i]->GetType() )
			{
			case SPIN_TYPE: 
				{
					outPutString = "";
					outPutString = "setoption name ";
					outPutString += name.c_str().AsChar();
					outPutString += " value ";//
					int valToSet = value.As<int>();
					char buff[20];
					_itoa(valToSet, buff, 10);
					outPutString += buff;
					outPutString += "\n";

					if( name == "MultiPV" )
					{
						m_pPipePanel->OptionsSetNumKibitzLines(valToSet);
					}
					FindAndSetOptionValue(name, buff);
				}
				break;
			case COMBO_TYPE: 
				{
					outPutString = "";
					outPutString = "setoption name ";
					outPutString += name.c_str().AsChar();
					outPutString += " value ";//
					outPutString += value.As<wxString>().c_str().AsChar();
					outPutString += "\n";
					FindAndSetOptionValue(name, value.As<wxString>());
				}
				break;
			case CHECK_TYPE:
				{
					outPutString = "";
					outPutString = "setoption name ";
					outPutString += name.c_str().AsChar();
					outPutString += " value ";//
					if( value.As<bool>() )
					{
						outPutString += "true\n";
						FindAndSetOptionValue(name, "true");
					}
					else
					{
						outPutString += "false\n";
						FindAndSetOptionValue(name, "false");
					}
					
				}
				break;
			case BUTTON_TYPE: 
				{
					FindAndSetOptionValue(name, value.As<wxString>());
				}
				break;
			case STRING_TYPE: 
				{
					outPutString = "";
					outPutString = "setoption name ";
					outPutString += name.c_str().AsChar();
					outPutString += " value ";//
					outPutString += value.As<wxString>().c_str().AsChar();
					outPutString += "\n";
					FindAndSetOptionValue(name, value.As<wxString>());
				}
				break;
			};
			break;
		}
	}

	if( outPutString != "" )
	{
		m_pPipePanel->SendOptionsString(outPutString);
	}
	

	
}

void EngineOptionsModalDialog::FindAndSetOptionValue( wxString t_name, wxString t_value)
{
	vector<string> *tokens = m_pPipePanel->GetTokenizedOptionsVector();

	for( unsigned int i = 0; i < (*tokens).size(); i++ )
	{
		if( (*tokens)[i] == "name" )
		{
			int pp = i+1;
			string name="";
			while( (*tokens)[pp] != "type" && pp < (*tokens).size() )
			{
				if( pp > i+2)
					name+=" ";
				name+=(*tokens)[pp];

				pp++;
			}
			if( pp+1 > (*tokens).size() )
				break;

			i = pp+1;


			if( wxString(name.c_str())== t_name )
			{
				if( (*tokens)[i] == "spin" )
				{
					int j = i+1;
					if( j>= (*tokens).size() )
						break;

			
					while( j < (*tokens).size()-1 && (*tokens)[j] != "option" )
					{
						if( (*tokens)[j] == "default")
						{
							(*tokens)[j+1] = "";
							(*tokens)[j+1] = t_value.c_str().AsChar();
							return;
							//j++;
						}
						j++;
					}
				}
				if( (*tokens)[i] == "check" )
				{
					(*tokens)[i+2] = "";
					(*tokens)[i+2] = t_value.c_str().AsChar();
					return;
				}
				if( (*tokens)[i] == "combo" )
				{

					int j = i+1;

					while( j < (*tokens).size()-1 && (*tokens)[j] != "option" )
					{
						if( (*tokens)[j] == "default" )
						{
							(*tokens)[j+1] = "";
							(*tokens)[j+1] = t_value.c_str().AsChar();
							return;
							//j++;
						}
						j++;
					}
					i=j;
				}
				if( (*tokens)[i] == "button" )
				{
					return;
				}
				if( (*tokens)[i] == "string" )
				{
					(*tokens)[i+2] = "";
					(*tokens)[i+2] = t_value.c_str().AsChar();
					return;
				}   
			}
		}
	}

}

void EngineOptionsModalDialog::OnPropertyGridChanging(wxPropertyGridEvent& event)
{
}


void EngineOptionsModalDialog::OnCancel( wxCommandEvent & event )
{
	if( m_pg )
	{
		m_pg->Clear();
		delete m_pg;
		m_pg = NULL;
	}
	Close(true);
}

void EngineOptionsModalDialog::OnClose(wxCloseEvent& event)
{

	
	
}

void EngineOptionsModalDialog::OnOk(wxCommandEvent& event)
{
        int res = wxMessageBox(wxT("Would you like to Save these Options as an Engine Personality?"),
                               wxT("Question"),
                               wxYES_NO,
                               this);
        if (res != wxYES)
		{
			if( m_pg )
			{
				m_pg->Clear();
				delete m_pg;
				m_pg = NULL;
			}
			Close(true);

			return;
		}

		UCIEnginePersonality *engineP = new UCIEnginePersonality();

		for( int i = 0; i < this->optionsMap.size(); i++ )
		{
			engineP->optionsMap.push_back(optionsMap[i]);
		}

		EnginePersonalityDialog pd((wxWindow*)this, this->m_pPipePanel->GetMyFrame(),this->m_pPipePanel->GetEngineName(),engineP, NULL, wxID_ANY, "Engine Personality dialog");

		if( pd.ShowModal() == wxID_OK )
		{
			wxMessageBox("Engine Personality Saved");
		}

	if( m_pg )
	{
		m_pg->Clear();
		delete m_pg;
		m_pg = NULL;
	}
			
	Close(true);
	return;
}


void EngineOptionsModalDialog::OnLoadPersonality(wxCommandEvent& event)
{
	EnginePersonalityDialog pd((wxWindow*)this, this->m_pPipePanel->GetMyFrame(),this->m_pPipePanel->GetEngineName(), NULL, this->m_pPipePanel, wxID_ANY, "Engine Personality dialog");

	if( pd.ShowModal() == wxID_OK )
	{
		wxMessageBox("Engine Personality Loaded");
	}
}