
#include <fstream>
#include <istream>
#include "mdi.h"
#include "BookListDialog.h"


using namespace std;


BEGIN_EVENT_TABLE(BookListDialog, wxDialog)
	EVT_BUTTON(wxID_OK, BookListDialog::OnOk)
	//EVT_BUTTON(wxID_CANCEL, GameInfoModalDialog::OnCancel)
	EVT_BUTTON(ID_WXBUTTON4, BookListDialog::OnAddBook)
	EVT_BUTTON(ID_WXBUTTON2, BookListDialog::OnRemoveBook)
	

END_EVENT_TABLE()

BookListDialog::BookListDialog(wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{
	//m_name = name;
	wxPoint p;
	wxSize  sz;
	m_frame = (MyFrame*)parent;
//	selectedList = -1;
 
/*	m_pipePanel = pipePanel;
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
	Center();*/

	WxButton8 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(152, 240), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton8"));

	WxButton7 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(32, 240), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton7"));

	WxButton4 = new wxButton(this, ID_WXBUTTON4, wxT("Add Book"), wxPoint(32, 176), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton4"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("Remove Book"), wxPoint(152, 176), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	wxArrayString arrayStringFor_WxListBox1;
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(16, 16), wxSize(249, 129), arrayStringFor_WxListBox1, wxLB_SINGLE);

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT(""), wxPoint(16, 152), wxSize(249, 73));

	SetTitle(wxT("Book Dialog"));
	SetIcon(wxNullIcon);
	SetSize(8,8,298,319);
	Center();

	//wxVector<wxVariant> data;

	ifstream myfile (string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+"/books/polyglot/BookList.txt");
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
			WxListBox1->Append(engineName);
		}
	}
	myfile.close();

}

BookListDialog::~BookListDialog()
{
}



void BookListDialog::OnOk( wxCommandEvent & event )
{
	
	wxString name = paths[WxListBox1->GetSelection()];

	if( name != "" )
	{
		fclose(m_frame->polyglotBook);
		string filename = wxStandardPaths::Get().GetResourcesDir().c_str()+"/books/polyglot/" +name;
		m_frame->polyglotBook = fopen(
			filename.c_str(), "rb");

		m_frame->m_mgr.GetPane("moveData").Caption("Book: "+name);
	}





			/*  wxDataViewItemArray items;
    int len = lc->GetSelections( items );
	*m_name = lc->GetTextValue (*(int*)items[0].GetID(), 1);
	*/
	//wxString cmd = d
	ofstream myfile (string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+"/books/polyglot/BookList.txt");
	
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
		wxMessageBox("Error Saving Book List, Please restore from Backup in Books Directory","");
	}

	myfile.close();
	

	Close(true);
}

void BookListDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}

void BookListDialog::OnAddBook( wxCommandEvent & event )
{
    wxFileDialog dialog(this,
                        wxT("Select An Engine"),
                        wxEmptyString,
                        wxT(""),
                        wxT("Book Files (*.bin)|*.bin|Document files (*.doc;*.ods)|*.doc;*.ods"),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

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
			WxListBox1->Append(engineName);
		//WxListBox2->Append(strOutput);
    }
	//selectedList = -1;
}


void BookListDialog::OnRemoveBook( wxCommandEvent & event )
{
	int sel = WxListBox1->GetSelection();
	WxListBox1->Delete(sel);
	paths.erase(paths.begin() + sel);
	//selectedList = -1;
}





