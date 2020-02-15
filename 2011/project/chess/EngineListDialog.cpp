#include "EngineListDialog.h"
#include "MyPipePanel.h"
#include "headers.h"
#include <fstream>
#include <istream>
#include "mdi.h"
#include "UCIEngineOptionsDialog.h"


using namespace std;


BEGIN_EVENT_TABLE(EngineListModalDialog, wxDialog)
	EVT_BUTTON(wxID_OK, EngineListModalDialog::OnOk)
	//EVT_BUTTON(wxID_CANCEL, GameInfoModalDialog::OnCancel)
	

	EVT_BUTTON(ID_WXBUTTON2, EngineListModalDialog::OnShutdownEngine)
	EVT_BUTTON(ID_WXBUTTON3, EngineListModalDialog::OnEngineOptions)
	EVT_BUTTON(ID_WXBUTTON7, EngineListModalDialog::OnAddEngine)
	EVT_BUTTON(ID_WXBUTTON8, EngineListModalDialog::OnDeleteEngine)
	EVT_LISTBOX(ID_WXLISTBOX1,EngineListModalDialog::WxListBox1Selected)
	EVT_LISTBOX(ID_WXLISTBOX2,EngineListModalDialog::WxListBox2Selected)
END_EVENT_TABLE()

EngineListModalDialog::EngineListModalDialog(wxWindow * parent, MyPipePanel *pipePanel, wxWindowID id, const wxString & title,wxString* name,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{
	m_name = name;
	wxPoint p;
	wxSize  sz;

	selectedList = -1;
 
	m_pipePanel = pipePanel;
	m_pFrame = m_pipePanel->GetParent();
	m_engineManager = pipePanel->GetEngineManager();
//wxDataViewListCtrl* lc;


	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Select an Engine to Configure or Display"), wxPoint(232, 192), wxSize(95, 63), 0, wxT("WxStaticText2"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Select an Engine and Click Ok to Load and Display the Engine Panel"), wxPoint(592, 176), wxSize(103, 79), 0, wxT("WxStaticText1"));

	WxStaticBox4 = new wxStaticBox(this, ID_WXSTATICBOX4, wxT(""), wxPoint(224, 176), wxSize(105, 73));

	WxStaticBox3 = new wxStaticBox(this, ID_WXSTATICBOX3, wxT(""), wxPoint(584, 160), wxSize(113, 89));

	WxButton8 = new wxButton(this, ID_WXBUTTON8, wxT("Remove Engine"), wxPoint(585, 83), wxSize(100, 30), 0, wxDefaultValidator, wxT("WxButton8"));

	WxButton7 = new wxButton(this, ID_WXBUTTON7, wxT("Add Engine"), wxPoint(585, 38), wxSize(100, 30), 0, wxDefaultValidator, wxT("WxButton7"));

	wxArrayString arrayStringFor_WxListBox2;
	WxListBox2 = new wxListBox(this, ID_WXLISTBOX2, wxPoint(384, 38), wxSize(185, 211), arrayStringFor_WxListBox2, wxLB_SINGLE);

	WxButton6 = new wxButton(this, ID_WXBUTTON6, wxT("Personality"), wxPoint(225, 85), wxSize(100, 30), 0, wxDefaultValidator, wxT("WxButton6"));

	WxButton5 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(128, 279), wxSize(100, 30), 0, wxDefaultValidator, wxT("WxButton5"));

	WxButton4 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(16, 278), wxSize(100, 30), 0, wxDefaultValidator, wxT("WxButton4"));

	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("Options"), wxPoint(225, 40), wxSize(100, 30), 0, wxDefaultValidator, wxT("WxButton3"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("Shutdown"), wxPoint(225, 131), wxSize(100, 30), 0, wxDefaultValidator, wxT("WxButton2"));

	wxArrayString arrayStringFor_WxListBox1;
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_pFrame->m_pipePanel[jj] &&
			m_pFrame->m_pipePanel[jj]->IsActive() )
			if( m_pFrame->m_pipePanel[jj]->GetEngineName()!= "")	
				arrayStringFor_WxListBox1.Add(m_pFrame->m_pipePanel[jj]->GetEngineName());
	}
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(24, 38), wxSize(185, 211), arrayStringFor_WxListBox1, wxLB_SINGLE);

	WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, wxT("Engine List"), wxPoint(368, 10), wxSize(339, 256));

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT("Loaded Engines"), wxPoint(8, 10), wxSize(339, 256));

	SetTitle(wxT("Project2"));
	SetIcon(wxNullIcon);
	SetSize(8,8,730,357);
	Center();



	//wxVector<wxVariant> data;

	ifstream myfile (string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+"/settings/engine_list.txt");
	//int count = 0;
	while ( myfile.good() )
	{
		string line;
		getline (myfile,line);
		if( !line.size() == 0 && line[0] != ' ')
		{	
			paths.push_back(line);
			
			wxString engineName;
			int last_back_slash = line.find_last_of("\\");
			if( last_back_slash != -1 )
			{
				engineName=line.substr(last_back_slash+1, line.length()-last_back_slash+1);
			}
			else
			{
				engineName = line;
			}
			WxListBox2->Append(engineName);
		}
	}
	myfile.close();


}

EngineListModalDialog::~EngineListModalDialog()
{
}



void EngineListModalDialog::OnOk( wxCommandEvent & event )
{
	
	if( selectedList == -1 )
	{
		wxMessageBox( "Select an Engine First or Cancel to Exit", "");
		return;
	}

	if( selectedList == 1 )
	{

		*m_name = paths[WxListBox1->GetSelection()];
		for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
		{

			if( m_pFrame->m_pipePanel[jj] &&
				m_pFrame->m_pipePanel[jj]->IsActive() )
				if( m_pFrame->m_pipePanel[jj]->GetEngineName()== *m_name)	
					m_pFrame->m_mgr.GetPane(m_pFrame->m_pipePanel[jj]->GetPanelName()).Show(true);
				//	arrayStringFor_WxListBox1.Add(m_pFrame->m_pipePanel[jj]->GetEngineName());
		}
		m_pFrame->m_mgr.Update();
	}

	else
	{
		if( WxListBox2->GetCount() != 0  )
		{	
			int sel = WxListBox2->GetSelection();
			if( sel >= 0 && sel < paths.size() )
				*m_name = paths[sel];//WxListBox2->GetStringSelection();
		}
		else
		{
			selectedList = 1;
			return;
		}

			/*  wxDataViewItemArray items;
    int len = lc->GetSelections( items );
	*m_name = lc->GetTextValue (*(int*)items[0].GetID(), 1);
	*/
	//wxString cmd = d
		ofstream myfile (string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+"/settings/engine_list.txt");
	
	/*for( int i = 0; i < lc->GetStore()->GetCount(); i++ )
	{
		
		if( lc->IsSelected(lc->GetStore()->GetItem(i)) )
		{
			*m_name = lc->GetTextValue (i, 1);
		}
		wxString str = lc->GetTextValue (i, 1);
		if( str.Length() != 0 )
		{		
			myfile << str.c_str().AsChar();
			myfile << "\n";
		}
	}*/
	if ( myfile.good() )
	{
		string line;
		for( int i = 0; i < paths.size(); i++ )
		{
			//m_list->Append(paths[i]);
			myfile << paths[i] << endl;
		}
	}
	else
	{
		wxMessageBox("Error Saving Engines List, Please restore from Backup in Settings Directory by deleting the old engines_list.txt and renaming engines_list.txt.bkup to engines_list.txt, if the problem persists please contact user support","");
	}

	myfile.close();
	}

	Close(true);
}

void EngineListModalDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}

void EngineListModalDialog::OnAddEngine( wxCommandEvent & event )
{
    wxFileDialog dialog(this,
                        wxT("Select An Engine"),
                        wxEmptyString,
                        wxT(""),
                        wxT("Engine Executables (*.exe)|*.exe"),
                        wxFD_OPEN);

    dialog.SetFilterIndex(0);

    if (dialog.ShowModal() == wxID_OK)
    {
		wxString strOutput = dialog.GetPath();
		//m_list->Append((char*)strOutput.c_str().AsChar());	
	
		paths.push_back(strOutput.c_str().AsChar());
			wxString engineName;
			int last_back_slash = strOutput.find_last_of("\\");
			if( last_back_slash != -1 )
			{
				engineName=strOutput.substr(last_back_slash+1, strOutput.length()-last_back_slash+1);
			}
			else
			{
				engineName = strOutput;
			}
			WxListBox2->Append(engineName);
		//WxListBox2->Append(strOutput);
    }
	selectedList = -1;
}

void EngineListModalDialog::OnDeleteEngine( wxCommandEvent & event )
{
	int sel = WxListBox2->GetSelection();
	WxListBox2->Delete(sel);
	paths.erase(paths.begin() + sel);
	selectedList = -1;
}

void EngineListModalDialog::OnEditEngine( wxCommandEvent & event )
{
}


void EngineListModalDialog::WxListBox1Selected( wxCommandEvent & event )
{
	selectedList = 1;
}

void EngineListModalDialog::WxListBox2Selected( wxCommandEvent & event )
{
	selectedList = 2;
}

void EngineListModalDialog::OnShutdownEngine( wxCommandEvent & event )
{
	if( selectedList != 1 )
	{
		wxMessageBox("Select an Engine to Shutdown","Input Error");
		return;
	}
	*m_name = paths[WxListBox1->GetSelection()];

	WxListBox1->Delete(WxListBox1->GetSelection());

	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( m_pFrame->m_pipePanel[jj] &&
			m_pFrame->m_pipePanel[jj]->IsActive() )
			if( m_pFrame->m_pipePanel[jj]->GetEngineName()== *m_name)	
				m_pFrame->m_pipePanel[jj]->DoClose();
			//	arrayStringFor_WxListBox1.Add(m_pFrame->m_pipePanel[jj]->GetEngineName());
	}
	selectedList = -1;

}


void EngineListModalDialog::OnEngineOptions( wxCommandEvent & event )
{
	if( selectedList != 1 )
	{
		wxMessageBox("Select an Engine to Load First, or use Add Engine","Input Error");
		return;
	}
	*m_name = paths[WxListBox1->GetSelection()];

	MyPipePanel *ppPanel=0;
	for( int i = 0 ; i < m_pFrame->numActivePipePanels; i++ )
	{
		if( m_pFrame->m_pipePanel[i]->GetEngineName() == *m_name )
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
	selectedList = -1;
}